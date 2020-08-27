/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

namespace CSS {

	class TokenizerConsumeURLToken : public ::testing::Test {
	public:
		Context context {&ParseErrorTester::ReporterEndpoint};
	};

	TEST_F(TokenizerConsumeURLToken, SimpleTest) {
		const Unicode::UString string("https://example.com)");
		Tokenizer tokenizer(context, string);
		EXPECT_TRUE(tokenizer.ConsumeURLToken());
		EXPECT_EQ(tokenizer.stream.CodePointsLeft(), 0);
		ASSERT_EQ(tokenizer.tokens.size(), 1);
		const auto &token = tokenizer.tokens[0];
		ASSERT_EQ(token.type, TokenType::URL);
		const auto &data = std::get<TokenCodePointsData>(token.data);
		const std::vector<Unicode::CodePoint> expected = {'h', 't', 't', 'p', 's', ':', '/', '/', 'e', 'x', 'a', 'm', 'p', 'l', 'e', '.', 'c', 'o', 'm'};
		EXPECT_EQ(data.codePoints, expected);
	}

} // namespace CSS
