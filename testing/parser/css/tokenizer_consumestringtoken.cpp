/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <random>
#include <variant>
#include <vector>

#include <cstring>

#include "data/text/encoding/utf8.hpp"
#include "data/text/ustring.hpp"

namespace CSS {

	class TokenizerConsumeStringToken : public ::testing::Test {
	public:
		Context context {&ParseErrorTester::ReporterEndpoint};

		std::random_device randomDevice;
		std::mt19937 randomGenerator {randomDevice()};
		std::uniform_int_distribution<Unicode::CodePoint> integerDistributor {0, Unicode::LAST_ALLOWED_CODE_POINT};

		[[nodiscard]] Unicode::CodePoint
		GetRandomCodePoint() noexcept {
			return integerDistributor(randomGenerator);
		}

		void
		CompareCodePointVectors(const std::vector<Unicode::CodePoint> &tokenizer,
			const std::vector<Unicode::CodePoint> &expected) const noexcept {
			ASSERT_EQ(tokenizer, expected) << "Comparison failed: tokenizer=\"" << Unicode::UString(tokenizer)
										   << "\" expected=\"" << Unicode::UString(expected) << '"';
		}

		void
		TestLegal(
			const Unicode::UString &string, Unicode::CodePoint ending, const std::vector<Unicode::CodePoint> &result) {
			Tokenizer tokenizer(context, string);
			ASSERT_TRUE(tokenizer.ConsumeStringToken(ending));
			ASSERT_FALSE(ParseErrorTester::WasParseErrorFired());
			ASSERT_EQ(tokenizer.tokens.size(), 1);
			ASSERT_EQ(tokenizer.tokens[0].type, Token::Type::STRING);
			const auto *data = std::get_if<TokenCodePointsData>(&tokenizer.tokens[0].data);
			ASSERT_NE(data, nullptr);
			CompareCodePointVectors(data->codePoints, result);
		}

		// An parse error will be fired and no token will be emitted.
		void
		TestIllegalFatal(const Unicode::UString &string, Unicode::CodePoint ending, ParseError expectedError) {
			Tokenizer tokenizer(context, string);
			ASSERT_TRUE(tokenizer.ConsumeStringToken(ending));
			ASSERT_TRUE(ParseErrorTester::WasParseErrorFired(expectedError));
			ASSERT_TRUE(tokenizer.tokens.empty());
		}

		// An parse error will be fired and no token will be emitted.
		void
		TestIllegalBadString(const Unicode::UString &string, Unicode::CodePoint ending, ParseError expectedError) {
			Tokenizer tokenizer(context, string);
			ASSERT_TRUE(tokenizer.ConsumeStringToken(ending));
			ASSERT_TRUE(ParseErrorTester::WasParseErrorFired(expectedError));
			ASSERT_EQ(tokenizer.tokens.size(), 1);
			ASSERT_EQ(tokenizer.tokens[0].type, Token::Type::BAD_STRING);
			ASSERT_TRUE(std::holds_alternative<std::nullptr_t>(tokenizer.tokens[0].data));
			ASSERT_EQ(std::get<std::nullptr_t>(tokenizer.tokens[0].data), nullptr);
		}
	};

	TEST_F(TokenizerConsumeStringToken, TestEmptyQuotationMarkEnding) {
		const Unicode::UString string("\"");
		const Unicode::CodePoint ending = Unicode::QUOTATION_MARK;
		const std::vector<Unicode::CodePoint> result = {};
		TestLegal(string, ending, result);
	}

	TEST_F(TokenizerConsumeStringToken, TestEmptyApostropheEnding) {
		const Unicode::UString string("'");
		const Unicode::CodePoint ending = Unicode::APOSTROPHE;
		const std::vector<Unicode::CodePoint> result = {};
		TestLegal(string, ending, result);
	}

	TEST_F(TokenizerConsumeStringToken, TestEmptyRandomEnding) {
		Unicode::CodePoint ending;
		while ((ending = GetRandomCodePoint()) == Unicode::LINE_FEED) {
		}

		Unicode::UString string(ending);
		const std::vector<Unicode::CodePoint> result = {};
		TestLegal(string, ending, result);
	}

	TEST_F(TokenizerConsumeStringToken, TestEOF) {
		const Unicode::CodePoint ending = Unicode::APOSTROPHE;
		const Unicode::UString string;
		TestIllegalFatal(string, ending, ParseError::EOF_IN_CONSUMING_STRING);
	}

	TEST_F(TokenizerConsumeStringToken, TestNewLine) {
		const Unicode::CodePoint ending = Unicode::APOSTROPHE;
		const Unicode::UString string(Unicode::LINE_FEED);
		TestIllegalBadString(string, ending, ParseError::NEWLINE_IN_CONSUMING_STRING);
	}

	TEST_F(TokenizerConsumeStringToken, TestEscapedNewLine) {
		Unicode::UString string {'\\', '\n', '"'};
		const Unicode::CodePoint ending = Unicode::QUOTATION_MARK;
		const std::vector<Unicode::CodePoint> result;
		TestLegal(string, ending, result);
	}

	TEST_F(TokenizerConsumeStringToken, TestEscapedNewLineInText) {
		Unicode::UString string("Hello this is long thus \\\nwe begin on a new line.'");
		const Unicode::CodePoint ending = Unicode::APOSTROPHE;
		const char *text = "Hello this is long thus we begin on a new line.";
		const auto result = TextEncoding::UTF8::ASCIIDecode(text, strlen(text));
		TestLegal(string, ending, result);
	}

	TEST_F(TokenizerConsumeStringToken, TestEscapedCodePoint) {
		Unicode::UString string {'H', 'e', '\\', '0', '0', '0', '0', '6', 'C', 'l', 'o', '"'};
		const Unicode::CodePoint ending = Unicode::QUOTATION_MARK;
		const std::vector<Unicode::CodePoint> result {'H', 'e', 'l', 'l', 'o'};
		TestLegal(string, ending, result);
	}

	TEST_F(TokenizerConsumeStringToken, TestCalledByConsumeTokenQuotationMark) {
		const Unicode::UString input {'"', 'T', 'h', 'i', 's', '"'};
		const std::vector<Unicode::CodePoint> expected {'T', 'h', 'i', 's'};
		Unicode::CodePoint character;
		Tokenizer tokenizer(context, input);
		ASSERT_TRUE(tokenizer.stream.Next(&character));
		ASSERT_TRUE(tokenizer.ConsumeToken(character));
		ASSERT_FALSE(ParseErrorTester::WasParseErrorFired());
		ASSERT_EQ(tokenizer.tokens.size(), 1);
		ASSERT_EQ(tokenizer.tokens[0].type, Token::Type::STRING);
		const auto *data = std::get_if<TokenCodePointsData>(&tokenizer.tokens[0].data);
		ASSERT_NE(data, nullptr);
		CompareCodePointVectors(data->codePoints, expected);
	}

	TEST_F(TokenizerConsumeStringToken, TestCalledByConsumeTokenApostrophe) {
		const Unicode::UString input {'\'', 'T', 'h', 'i', 's', '\''};
		const std::vector<Unicode::CodePoint> expected {'T', 'h', 'i', 's'};
		Unicode::CodePoint character;
		Tokenizer tokenizer(context, input);
		ASSERT_TRUE(tokenizer.stream.Next(&character));
		ASSERT_TRUE(tokenizer.ConsumeToken(character));
		ASSERT_FALSE(ParseErrorTester::WasParseErrorFired());
		ASSERT_EQ(tokenizer.tokens.size(), 1);
		ASSERT_EQ(tokenizer.tokens[0].type, Token::Type::STRING);
		const auto *data = std::get_if<TokenCodePointsData>(&tokenizer.tokens[0].data);
		ASSERT_NE(data, nullptr);
		CompareCodePointVectors(data->codePoints, expected);
	}

} // namespace CSS
