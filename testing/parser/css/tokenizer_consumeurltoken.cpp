/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

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

} // namespace CSS
