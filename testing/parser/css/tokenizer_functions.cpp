/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

namespace CSS {

	class TokenizerFunctionTests : public ::testing::Test {
	public:
		Context context {&ParseErrorTester::ReporterEndpoint};
	};

	TEST_F(TokenizerFunctionTests, NumberTest) {
		const Unicode::UString input("twelve(12);");
		Tokenizer tokenizer(context, input);
		EXPECT_TRUE(tokenizer.Run());
		ASSERT_EQ(tokenizer.tokens.size(), 4);
		TestFunction(tokenizer.tokens[0], {'t', 'w', 'e', 'l', 'v', 'e'});
		TestNumber<CSS::IntegerType>(tokenizer.tokens[1], 12);
		EXPECT_EQ(tokenizer.tokens[2].type, TokenType::PAREN_CLOSE);
		EXPECT_EQ(tokenizer.tokens[3].type, TokenType::SEMICOLON);
	}

} // namespace CSS
