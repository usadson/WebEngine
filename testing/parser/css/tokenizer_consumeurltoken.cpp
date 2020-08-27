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
		TestAllowed(const Unicode::UString &string, const std::vector<Unicode::CodePoint> &expected) noexcept {
			Tokenizer tokenizer(context, string);
			EXPECT_TRUE(tokenizer.ConsumeURLToken());
			EXPECT_EQ(tokenizer.stream.CodePointsLeft(), 0);
			ASSERT_EQ(tokenizer.tokens.size(), 1);
			const auto &token = tokenizer.tokens[0];
			ASSERT_EQ(token.type, TokenType::URL);
			const auto &data = std::get<TokenCodePointsData>(token.data);
			EXPECT_EQ(data.codePoints, expected);
		}
	};

	TEST_F(TokenizerConsumeURLToken, SimpleTest) {
		const Unicode::UString input {"https://example.com)"};
		TestAllowed(input, {'h', 't', 't', 'p', 's', ':', '/', '/', 'e', 'x', 'a', 'm', 'p', 'l', 'e', '.', 'c', 'o', 'm'});
	}

	TEST_F(TokenizerConsumeURLToken, WhitespaceTest) {
		const Unicode::UString input1 {"  \t \n\t blah)"};
		TestAllowed(input1, {'b', 'l', 'a', 'h'});
		const Unicode::UString input2 {"  \t \n\t blah \t \n )"};
		TestAllowed(input2, {'b', 'l', 'a', 'h'});
		const Unicode::UString input3 {"\tblah\n)"};
		TestAllowed(input3, {'b', 'l', 'a', 'h'});
	}

} // namespace CSS
