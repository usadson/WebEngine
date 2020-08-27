/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "parser/css/tokenizer_algorithms.hpp"

namespace CSS {

	class TokenizerConsumeURLToken : public ::testing::Test {
	public:
		Context context {&ParseErrorTester::ReporterEndpoint};

		void
		RunTest(bool isLegal, const Unicode::UString &string, const std::vector<Unicode::CodePoint> &expected, bool isBad=false) noexcept {
			Tokenizer tokenizer(context, string);
			EXPECT_EQ(tokenizer.ConsumeURLToken(), isLegal);

			EXPECT_EQ(tokenizer.stream.CodePointsLeft(), 0);
			ASSERT_EQ(tokenizer.tokens.size(), 1);

			const auto &token = tokenizer.tokens[0];
			if (isBad) {
				EXPECT_EQ(token.type, TokenType::BAD_URL);
			} else {
				ASSERT_EQ(token.type, TokenType::URL);

				const auto &data = std::get<TokenCodePointsData>(token.data);
				EXPECT_EQ(data.codePoints, expected);
				if (data.codePoints != expected) {
					const auto mismatch = std::mismatch(std::begin(expected), std::end(expected), std::begin(data.codePoints), std::end(data.codePoints));
					std::cout << "First mismatch is: " << *mismatch.first << ' ' << *mismatch.second << '\n';
				}
			}
		}
	};

	TEST_F(TokenizerConsumeURLToken, SimpleTest) {
		const Unicode::UString input {"https://example.com)"};
		RunTest(true, input, {'h', 't', 't', 'p', 's', ':', '/', '/', 'e', 'x', 'a', 'm', 'p', 'l', 'e', '.', 'c', 'o', 'm'});
	}

	TEST_F(TokenizerConsumeURLToken, WhitespaceTest) {
		const Unicode::UString input1 {"  \t \n\t blah)"};
		RunTest(true, input1, {'b', 'l', 'a', 'h'});

		const Unicode::UString input2 {"  \t \n\t blah \t \n )"};
		RunTest(true, input2, {'b', 'l', 'a', 'h'});

		const Unicode::UString input3 {"\tblah\n)"};
		RunTest(true, input3, {'b', 'l', 'a', 'h'});
	}

	TEST_F(TokenizerConsumeURLToken, EOFTest) {
		const Unicode::UString input {"hi"};
		RunTest(false, input, {'h', 'i'});
		EXPECT_TRUE(ParseErrorTester::WasParseErrorFired(ParseError::EOF_IN_CONSUMING_URL));
	}

	TEST_F(TokenizerConsumeURLToken, EmptyTest) {
		const Unicode::UString input;
		RunTest(false, input, {});
		EXPECT_TRUE(ParseErrorTester::WasParseErrorFired(ParseError::EOF_IN_CONSUMING_URL));
	}

	TEST_F(TokenizerConsumeURLToken, InvalidCharactersTest) {
		// Illegal characters in this case: " ' ( \n DELETE
		Unicode::UString input {'t', 'e', 's', 't', '?', ')'};
		for (const auto &character : {Unicode::QUOTATION_MARK, Unicode::APOSTROPHE, Unicode::LEFT_PARENTHESIS, Unicode::LINE_TABULATION, Unicode::DELETE}) {
			input[4] = character;
			RunTest(false, input, {}, true);
			EXPECT_TRUE(ParseErrorTester::WasParseErrorFired(ParseError::UNEXPECTED_CHARACTER_IN_URL));
		}
		// Illegal characters in this case: U+0000 to U+0008 inclusive
		for (Unicode::CodePoint character = Unicode::NULL_CHARACTER; character <= Unicode::BACKSPACE; character++) {
			input[4] = character;
			RunTest(false, input, {}, true);
			EXPECT_TRUE(ParseErrorTester::WasParseErrorFired(ParseError::UNEXPECTED_CHARACTER_IN_URL));
		}
		// Illegal characters in this case: U+000E to U+0001F inclusive
		for (Unicode::CodePoint character = Unicode::SHIFT_OUT; character <= Unicode::INFORMATION_SEPARATOR_ONE; character++) {
			input[4] = character;
			RunTest(false, input, {}, true);
			EXPECT_TRUE(ParseErrorTester::WasParseErrorFired(ParseError::UNEXPECTED_CHARACTER_IN_URL));
		}
	}

	TEST_F(TokenizerConsumeURLToken, ValidCharactersTest) {
		std::vector<Unicode::CodePoint> v;
		v.reserve(0x100);
		for (Unicode::CodePoint cp = 0; cp < 0x100; cp++) {
			bool isVisible = cp > Unicode::BACKSPACE && cp != Unicode::LINE_TABULATION && (cp < 0xE || cp > 0x1F) && cp != Unicode::DELETE;
			bool isNonExplicitlyForbidden = cp != Unicode::APOSTROPHE && cp != Unicode::QUOTATION_MARK && cp != Unicode::LEFT_PARENTHESIS;
			bool isNonImplicitlyForbidden = cp != Unicode::RIGHT_PARENTHESIS && !IsWhitespace(cp) && cp != Unicode::REVERSE_SOLIDUS;
			if (isVisible && isNonExplicitlyForbidden && isNonImplicitlyForbidden) {
				v.push_back(cp);
			}
		}
		for (std::size_t i = v.size(); i < v.capacity(); i++) {
			v.push_back(std::rand() + 0x100);
		}
		Unicode::UString input(v);
		input += Unicode::RIGHT_PARENTHESIS;
		RunTest(true, input, v);
		EXPECT_FALSE(ParseErrorTester::WasParseErrorFired());
	}

} // namespace CSS
