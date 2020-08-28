/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

namespace CSS {

	class TokenizerConsumeToken : public ::testing::Test {
	public:
		Context context {&ParseErrorTester::ReporterEndpoint};

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
		const Unicode::UString string {' '};
		Tokenizer tokenizer(context, string);
		EXPECT_TRUE(tokenizer.ConsumeToken(Unicode::NUMBER_SIGN));
		EXPECT_FALSE(ParseErrorTester::WasParseErrorFired());
		ASSERT_EQ(tokenizer.tokens.size(), 2);
		EXPECT_EQ(tokenizer.tokens[0].type, TokenType::DELIM);
		auto *data = std::get_if<Unicode::CodePoint>(&tokenizer.tokens[0].data);
		ASSERT_NE(data, nullptr);
		EXPECT_EQ(*data, Unicode::NUMBER_SIGN);
	}

	TEST_F(TokenizerConsumeToken, TestHashId) {
		const Unicode::UString input {'t', ' '};
		const std::vector<Unicode::CodePoint> expected {'t'};
		Tokenizer tokenizer(context, input);
		EXPECT_TRUE(tokenizer.ConsumeToken(Unicode::NUMBER_SIGN));
		EXPECT_FALSE(ParseErrorTester::WasParseErrorFired());
		ASSERT_EQ(tokenizer.tokens.size(), 2);
		EXPECT_EQ(tokenizer.tokens[0].type, TokenType::HASH);
		auto *data = std::get_if<TokenHashData>(&tokenizer.tokens[0].data);
		ASSERT_NE(data, nullptr);
		EXPECT_EQ(data->type, TokenHashType::ID);
		EXPECT_EQ(data->codePoints, expected);
	}

	TEST_F(TokenizerConsumeToken, TestHashHex) {
		const Unicode::UString input("123456 ");
		const std::vector<Unicode::CodePoint> expected {'1', '2', '3', '4', '5', '6'};
		Tokenizer tokenizer(context, input);
		EXPECT_TRUE(tokenizer.ConsumeToken(Unicode::NUMBER_SIGN));
		EXPECT_FALSE(ParseErrorTester::WasParseErrorFired());
		ASSERT_EQ(tokenizer.tokens.size(), 2);
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

	TEST_F(TokenizerConsumeToken, TestSquareBrackets) {
		const Unicode::UString input;
		Tokenizer tokenizer(context, input);
		EXPECT_TRUE(tokenizer.ConsumeToken(Unicode::LEFT_SQUARE_BRACKET));
		EXPECT_TRUE(tokenizer.ConsumeToken(Unicode::RIGHT_SQUARE_BRACKET));
		EXPECT_FALSE(ParseErrorTester::WasParseErrorFired());
		ASSERT_EQ(tokenizer.tokens.size(), 2);
		EXPECT_EQ(tokenizer.tokens[0].type, TokenType::SQUARE_OPEN);
		EXPECT_EQ(tokenizer.tokens[1].type, TokenType::SQUARE_CLOSE);
	}

	TEST_F(TokenizerConsumeToken, TestCurlyBrackets) {
		const Unicode::UString input;
		Tokenizer tokenizer(context, input);
		EXPECT_TRUE(tokenizer.ConsumeToken(Unicode::LEFT_CURLY_BRACKET));
		EXPECT_TRUE(tokenizer.ConsumeToken(Unicode::RIGHT_CURLY_BRACKET));
		EXPECT_FALSE(ParseErrorTester::WasParseErrorFired());
		ASSERT_EQ(tokenizer.tokens.size(), 2);
		EXPECT_EQ(tokenizer.tokens[0].type, TokenType::CURLY_OPEN);
		EXPECT_EQ(tokenizer.tokens[1].type, TokenType::CURLY_CLOSE);
	}

	TEST_F(TokenizerConsumeToken, TestIntegerSignsAsDelim) {
		const Unicode::UString input(" not a number");
		for (const auto codePoint : {'+', '-', '.'}) {
			Tokenizer tokenizer(context, input);
			EXPECT_TRUE(tokenizer.ConsumeToken(codePoint));
			EXPECT_FALSE(ParseErrorTester::WasParseErrorFired());
			ASSERT_EQ(tokenizer.tokens.size(), 1);
			EXPECT_EQ(tokenizer.tokens[0].type, TokenType::DELIM);
			auto result = std::get<Unicode::CodePoint>(tokenizer.tokens[0].data);
			EXPECT_EQ(result, codePoint);
		}
	}

	TEST_F(TokenizerConsumeToken, TestIntegerSignsAsNumber) {
		Unicode::UString input("?12 ");
		for (const auto codePoint : {Unicode::PLUS_SIGN, Unicode::HYPHEN_MINUS}) {
			input[0] = codePoint;
			Tokenizer tokenizer(context, input);
			tokenizer.stream.Skip();
			EXPECT_TRUE(tokenizer.ConsumeToken(codePoint));
			EXPECT_FALSE(ParseErrorTester::WasParseErrorFired());
			ASSERT_EQ(tokenizer.tokens.size(), 1);
			EXPECT_EQ(tokenizer.tokens[0].type, TokenType::NUMBER);
			auto numericData = std::get<TokenNumericData>(tokenizer.tokens[0].data);
			EXPECT_EQ(numericData.type, TokenNumericType::INTEGER);
			if (codePoint == Unicode::HYPHEN_MINUS)
				EXPECT_EQ(numericData.integer, -12);
			else if (codePoint == Unicode::PLUS_SIGN)
				EXPECT_EQ(numericData.integer, 12);
		}
	}

	TEST_F(TokenizerConsumeToken, TestIntegerSignsAsDimension) {
		Unicode::UString input("?12br ");
		for (const auto codePoint : {Unicode::PLUS_SIGN, Unicode::HYPHEN_MINUS}) {
			input[0] = codePoint;
			Tokenizer tokenizer(context, input);
			tokenizer.stream.Skip();
			EXPECT_TRUE(tokenizer.ConsumeToken(codePoint));
			EXPECT_FALSE(ParseErrorTester::WasParseErrorFired());
			ASSERT_EQ(tokenizer.tokens.size(), 1);
			EXPECT_EQ(tokenizer.tokens[0].type, TokenType::DIMENSION);

			auto dimensionData = std::get<TokenDimensionData>(tokenizer.tokens[0].data);
			EXPECT_EQ(dimensionData.type, TokenNumericType::INTEGER);
			EXPECT_EQ(dimensionData.integer, codePoint == Unicode::PLUS_SIGN ? 12 : -12);
			const std::vector<Unicode::CodePoint> br {'b', 'r'};
			EXPECT_EQ(dimensionData.codePoints, br);
		}
	}

	TEST_F(TokenizerConsumeToken, TestComma) {
		const Unicode::UString input;
		Tokenizer tokenizer(context, input);
		EXPECT_TRUE(tokenizer.ConsumeToken(Unicode::COMMA));
		EXPECT_FALSE(ParseErrorTester::WasParseErrorFired());
		ASSERT_EQ(tokenizer.tokens.size(), 1);
		EXPECT_EQ(tokenizer.tokens[0].type, TokenType::COMMA);
	}

} // namespace CSS
