/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <random>
#include <variant>
#include <vector>

#include "data/text/ustring.hpp"

namespace CSS {

	class TokenizerConsumeStringToken : public ::testing::Test {
	public:
		Context context { &ParseErrorTester::ReporterEndpoint };

		std::random_device randomDevice;
		std::mt19937 randomGenerator { randomDevice() };
		std::uniform_int_distribution<Unicode::CodePoint> integerDistributor { 0, Unicode::LAST_ALLOWED_CODE_POINT };

		[[nodiscard]] Unicode::CodePoint
		GetRandomCodePoint() noexcept {
			return integerDistributor(randomGenerator);
		}

		void
		TestLegal(
			const Unicode::UString &string, Unicode::CodePoint ending, const std::vector<Unicode::CodePoint> &result) {
			Tokenizer tokenizer(context, string);
			ASSERT_TRUE(tokenizer.ConsumeStringToken(ending));
			ASSERT_FALSE(ParseErrorTester::WasParseErrorFired());
			ASSERT_EQ(tokenizer.tokens.size(), 1);
			ASSERT_EQ(tokenizer.tokens[0].type, TokenType::STRING);
			const auto *data = std::get_if<TokenCodePointsData>(&tokenizer.tokens[0].data);
			ASSERT_NE(data, nullptr);
			ASSERT_EQ(data->codePoints, result);
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
			ASSERT_EQ(tokenizer.tokens[0].type, TokenType::BAD_STRING);
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
		Unicode::UString string;
		TestIllegalFatal(string, ending, ParseError::EOF_IN_CONSUMING_STRING);
	}

	TEST_F(TokenizerConsumeStringToken, TestNewLine) {
		const Unicode::CodePoint ending = Unicode::APOSTROPHE;
		Unicode::UString string(Unicode::LINE_FEED);
		TestIllegalBadString(string, ending, ParseError::NEWLINE_IN_CONSUMING_STRING);
	}

} // namespace CSS
