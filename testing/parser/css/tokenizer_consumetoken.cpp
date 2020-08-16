/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

namespace CSS {

	class TokenizerConsumeToken : public ::testing::Test {
	public:
		Context context { &ParseErrorTester::ReporterEndpoint };

		void
		TestWhitespace(const Unicode::UString &string) {
			Tokenizer tokenizer(context, string);
			tokenizer.stream.Skip();
			ASSERT_TRUE(tokenizer.ConsumeToken(string[0]));
			ASSERT_FALSE(ParseErrorTester::WasParseErrorFired());
			ASSERT_EQ(tokenizer.tokens.size(), 1);
			ASSERT_EQ(tokenizer.tokens[0].type, TokenType::WHITESPACE);
			ASSERT_TRUE(std::holds_alternative<std::nullptr_t>(tokenizer.tokens[0].data));
			ASSERT_EQ(std::get<std::nullptr_t>(tokenizer.tokens[0].data), nullptr);
		}
	};

	TEST_F(TokenizerConsumeToken, TestWhitespaceSpace) {
		const Unicode::UString string("     ");
		TestWhitespace(string);
	}

	TEST_F(TokenizerConsumeToken, TestWhitespaceNewline) {
		const Unicode::UString string("\n\n\n\n\n\n\n\n\n");
		TestWhitespace(string);
	}

	TEST_F(TokenizerConsumeToken, TestWhitespaceCharacterTabulation) {
		const Unicode::UString string("\t\t\t\t\t\t\t\t\t");
		TestWhitespace(string);
	}

	TEST_F(TokenizerConsumeToken, TestWhitespaceCombined) {
		const Unicode::UString string("  \n\n\t\t");
		TestWhitespace(string);
		const Unicode::UString string2("\n \t \n\t \t\n \t \n");
		TestWhitespace(string);
		const Unicode::UString string3(" \t \t \n\n \t\n \t \t ");
		TestWhitespace(string);
	}

	TEST_F(TokenizerConsumeToken, TestHashDelim) {
		const Unicode::UString string { ' ' };
		Tokenizer tokenizer(context, string);
		EXPECT_TRUE(tokenizer.ConsumeToken(Unicode::NUMBER_SIGN));
		EXPECT_FALSE(ParseErrorTester::WasParseErrorFired());
		ASSERT_EQ(tokenizer.tokens.size(), 1);
		EXPECT_EQ(tokenizer.tokens[0].type, TokenType::DELIM);
		auto *data = std::get_if<Unicode::CodePoint>(&tokenizer.tokens[0].data);
		ASSERT_NE(data, nullptr);
		EXPECT_EQ(*data, Unicode::NUMBER_SIGN);
	}

	TEST_F(TokenizerConsumeToken, TestHashId) {
		const Unicode::UString input { 't', ' ' };
		const std::vector<Unicode::CodePoint> expected { 't' };
		Tokenizer tokenizer(context, input);
		EXPECT_TRUE(tokenizer.ConsumeToken(Unicode::NUMBER_SIGN));
		EXPECT_FALSE(ParseErrorTester::WasParseErrorFired());
		ASSERT_EQ(tokenizer.tokens.size(), 1);
		EXPECT_EQ(tokenizer.tokens[0].type, TokenType::HASH);
		auto *data = std::get_if<TokenHashData>(&tokenizer.tokens[0].data);
		ASSERT_NE(data, nullptr);
		EXPECT_EQ(data->type, TokenHashType::ID);
		EXPECT_EQ(data->codePoints, expected);
	}

	TEST_F(TokenizerConsumeToken, TestHashHex) {
		const Unicode::UString input("123456 ");
		const std::vector<Unicode::CodePoint> expected { '1', '2', '3', '4', '5', '6' };
		Tokenizer tokenizer(context, input);
		EXPECT_TRUE(tokenizer.ConsumeToken(Unicode::NUMBER_SIGN));
		EXPECT_FALSE(ParseErrorTester::WasParseErrorFired());
		ASSERT_EQ(tokenizer.tokens.size(), 1);
		EXPECT_EQ(tokenizer.tokens[0].type, TokenType::HASH);
		auto *data = std::get_if<TokenHashData>(&tokenizer.tokens[0].data);
		ASSERT_NE(data, nullptr);
		EXPECT_EQ(data->type, TokenHashType::UNRESTRICTED);
		EXPECT_EQ(data->codePoints, expected);
	}

	TEST_F(TokenizerConsumeToken, TestParenthesis) {
		const Unicode::UString input;
		Tokenizer tokenizer(context, input);
		EXPECT_TRUE(tokenizer.ConsumeToken(Unicode::LEFT_PARENTHESIS));
		EXPECT_TRUE(tokenizer.ConsumeToken(Unicode::RIGHT_PARENTHESIS));
		EXPECT_FALSE(ParseErrorTester::WasParseErrorFired());
		ASSERT_EQ(tokenizer.tokens.size(), 2);
		EXPECT_EQ(tokenizer.tokens[0].type, TokenType::PAREN_OPEN);
		EXPECT_EQ(tokenizer.tokens[1].type, TokenType::PAREN_CLOSE);
	}

} // namespace CSS
