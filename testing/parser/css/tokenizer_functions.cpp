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
		EXPECT_EQ(tokenizer.tokens[2].type, Token::Type::PAREN_CLOSE);
		EXPECT_EQ(tokenizer.tokens[3].type, Token::Type::SEMICOLON);
	}

	TEST_F(TokenizerFunctionTests, QuotationMarkStringTest) {
		const Unicode::UString input("greet(\"hi\");");
		Tokenizer tokenizer(context, input);
		EXPECT_TRUE(tokenizer.Run());
		ASSERT_EQ(tokenizer.tokens.size(), 4);
		TestFunction(tokenizer.tokens[0], {'g', 'r', 'e', 'e', 't'});
		TestString(tokenizer.tokens[1], {'h', 'i'});
		EXPECT_EQ(tokenizer.tokens[2].type, Token::Type::PAREN_CLOSE);
		EXPECT_EQ(tokenizer.tokens[3].type, Token::Type::SEMICOLON);
	}

	TEST_F(TokenizerFunctionTests, ApostropheStringTest) {
		const Unicode::UString input("bye(\'see ya later!\');");
		Tokenizer tokenizer(context, input);
		EXPECT_TRUE(tokenizer.Run());
		ASSERT_EQ(tokenizer.tokens.size(), 4);
		TestFunction(tokenizer.tokens[0], {'b', 'y', 'e'});
		TestString(tokenizer.tokens[1], {'s', 'e', 'e', ' ', 'y', 'a', ' ', 'l', 'a', 't', 'e', 'r', '!'});
		EXPECT_EQ(tokenizer.tokens[2].type, Token::Type::PAREN_CLOSE);
		EXPECT_EQ(tokenizer.tokens[3].type, Token::Type::SEMICOLON);
	}

	TEST_F(TokenizerFunctionTests, StringifiedURLTest) {
		const Unicode::UString input("url('style.css');");
		Tokenizer tokenizer(context, input);
		EXPECT_TRUE(tokenizer.Run());
		ASSERT_EQ(tokenizer.tokens.size(), 4);
		TestFunction(tokenizer.tokens[0], {'u', 'r', 'l'});
		TestString(tokenizer.tokens[1], {'s', 't', 'y', 'l', 'e', '.', 'c', 's', 's'});
		EXPECT_EQ(tokenizer.tokens[2].type, Token::Type::PAREN_CLOSE);
		EXPECT_EQ(tokenizer.tokens[3].type, Token::Type::SEMICOLON);
	}

	TEST_F(TokenizerFunctionTests, UntrimmedFunctionWithStringTest) {
		const Unicode::UString input("greet( \t \t \"hi\"\t  \t\n);");
		Tokenizer tokenizer(context, input);
		EXPECT_TRUE(tokenizer.Run());
		ASSERT_EQ(tokenizer.tokens.size(), 6);
		TestFunction(tokenizer.tokens[0], {'g', 'r', 'e', 'e', 't'});
		EXPECT_EQ(tokenizer.tokens[1].type, Token::Type::WHITESPACE);
		TestString(tokenizer.tokens[2], {'h', 'i'});
		EXPECT_EQ(tokenizer.tokens[3].type, Token::Type::WHITESPACE);
		EXPECT_EQ(tokenizer.tokens[4].type, Token::Type::PAREN_CLOSE);
		EXPECT_EQ(tokenizer.tokens[5].type, Token::Type::SEMICOLON);
	}

	TEST_F(TokenizerFunctionTests, UntrimmedURLFunctionTest) {
		const Unicode::UString input("url( \t \t \"style\"\t  \t\n);");
		Tokenizer tokenizer(context, input);
		EXPECT_TRUE(tokenizer.Run());
		ASSERT_EQ(tokenizer.tokens.size(), 5);
		TestFunction(tokenizer.tokens[0], {'u', 'r', 'l'});
		TestString(tokenizer.tokens[1], {'s', 't', 'y', 'l', 'e'});
		EXPECT_EQ(tokenizer.tokens[2].type, Token::Type::WHITESPACE);
		EXPECT_EQ(tokenizer.tokens[3].type, Token::Type::PAREN_CLOSE);
		EXPECT_EQ(tokenizer.tokens[4].type, Token::Type::SEMICOLON);
	}

} // namespace CSS
