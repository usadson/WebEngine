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

	TEST_F(TokenizerConsumeToken, TestSimpleTypes) {
		const std::map<Unicode::CodePoint, TokenType> map {{Unicode::COLON, TokenType::COLON},
			{Unicode::COMMA, TokenType::COMMA},
			{Unicode::SEMICOLON, TokenType::SEMICOLON},
			{Unicode::LEFT_CURLY_BRACKET, TokenType::CURLY_OPEN},
			{Unicode::RIGHT_CURLY_BRACKET, TokenType::CURLY_CLOSE},
			{Unicode::LEFT_SQUARE_BRACKET, TokenType::SQUARE_OPEN},
			{Unicode::RIGHT_SQUARE_BRACKET, TokenType::SQUARE_CLOSE},
			{Unicode::LEFT_PARENTHESIS, TokenType::PAREN_OPEN},
			{Unicode::RIGHT_PARENTHESIS, TokenType::PAREN_CLOSE}};
		const Unicode::UString input;
		for (const auto &pair : map) {
			Tokenizer tokenizer(context, input);
			EXPECT_TRUE(tokenizer.ConsumeToken(pair.first));
			EXPECT_FALSE(ParseErrorTester::WasParseErrorFired());
			ASSERT_EQ(tokenizer.tokens.size(), 1);
			EXPECT_EQ(tokenizer.tokens[0].type, pair.second);
		}
	}

	TEST_F(TokenizerConsumeToken, TestFullStopAsNumber) {
		Unicode::UString input(".34 ");
		Tokenizer tokenizer(context, input);
		tokenizer.stream.Skip();
		EXPECT_TRUE(tokenizer.ConsumeToken(Unicode::FULL_STOP));
		EXPECT_FALSE(ParseErrorTester::WasParseErrorFired());
		ASSERT_EQ(tokenizer.tokens.size(), 1);
		EXPECT_EQ(tokenizer.tokens[0].type, TokenType::NUMBER);
		auto numericData = std::get<TokenNumericData>(tokenizer.tokens[0].data);
		EXPECT_EQ(numericData.type, TokenNumericType::NUMBER);
		EXPECT_EQ(numericData.number, 0.34);
	}

	TEST_F(TokenizerConsumeToken, TestLessThanSignAsCDO) {
		const Unicode::UString input("<!--");
		Tokenizer tokenizer(context, input);
		tokenizer.stream.Skip();
		EXPECT_TRUE(tokenizer.ConsumeToken(Unicode::LESS_THAN_SIGN));
		EXPECT_FALSE(ParseErrorTester::WasParseErrorFired());
		ASSERT_EQ(tokenizer.tokens.size(), 1);
		EXPECT_EQ(tokenizer.tokens[0].type, TokenType::CDO);
	}

	TEST_F(TokenizerConsumeToken, TestLessThanSignAsDelim) {
		const std::array inputs = {
			Unicode::UString("<-!-"),
			Unicode::UString("<--!"),
			Unicode::UString("<!- "),
			Unicode::UString("<! --"),
			Unicode::UString("< !--"),
			Unicode::UString("< !- -"),
			Unicode::UString("< ! - -")
		};
		for (const auto &input : inputs) {
			Tokenizer tokenizer(context, input);
			tokenizer.stream.Skip();
			EXPECT_TRUE(tokenizer.ConsumeToken(Unicode::LESS_THAN_SIGN));
			EXPECT_FALSE(ParseErrorTester::WasParseErrorFired());
			ASSERT_EQ(tokenizer.tokens.size(), 1);
			EXPECT_EQ(tokenizer.tokens[0].type, TokenType::DELIM);
			auto *data = std::get_if<Unicode::CodePoint>(&tokenizer.tokens[0].data);
			ASSERT_NE(data, nullptr);
			EXPECT_EQ(*data, Unicode::LESS_THAN_SIGN);
		}
	}

	TEST_F(TokenizerConsumeToken, TestCommercialAtAsAtKeyword) {
		const std::array<std::pair<Unicode::UString, std::vector<Unicode::CodePoint>>, 2> inputs = {{
			{Unicode::UString("@hi "), {'h', 'i'}},
			{Unicode::UString("@A "), {'A'}}
		}};
		for (const auto &input : inputs) {
			Tokenizer tokenizer(context, input.first);
			tokenizer.stream.Skip();
			EXPECT_TRUE(tokenizer.ConsumeToken(Unicode::COMMERCIAL_AT));
			EXPECT_FALSE(ParseErrorTester::WasParseErrorFired());
			ASSERT_EQ(tokenizer.tokens.size(), 1);
			EXPECT_EQ(tokenizer.tokens[0].type, TokenType::AT_KEYWORD);
			const auto *data = std::get_if<TokenCodePointsData>(&tokenizer.tokens[0].data);
			ASSERT_NE(data, nullptr);
			EXPECT_EQ(data->codePoints, input.second);
		}
	}

} // namespace CSS
