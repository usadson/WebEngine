/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

namespace CSS {

	class TokenizerConsumeNumericToken : public ::testing::Test {
	public:
		Context context {&ParseErrorTester::ReporterEndpoint};
		Unicode::UString streamContents {};
		Tokenizer tokenizer {context, streamContents};
		const Unicode::UString initialString {};

		void
		Tokenize(Unicode::UString &&string) {
			streamContents = std::move(string);
			tokenizer.stream.SetString(&streamContents);
			ASSERT_TRUE(tokenizer.ConsumeNumericToken());
		}
	};

	TEST_F(TokenizerConsumeNumericToken, NumberTest) {
		Tokenize({'1', '.', '0', ' '});
		EXPECT_EQ(tokenizer.stream.CodePointsLeft(), 1);
		ASSERT_EQ(tokenizer.tokens.size(), 1);
		const CSS::Token &token = tokenizer.tokens[0];
		EXPECT_EQ(token.type, Token::Type::NUMBER);

		// TokenNumericData is the base/alternative of all other types in this
		// case.
		const auto &tokenData = std::get<TokenNumericData>(token.data);
		EXPECT_EQ(tokenData.type, TokenNumericType::NUMBER);
		EXPECT_EQ(tokenData.number, 1.0);
	}

	TEST_F(TokenizerConsumeNumericToken, PercentageTest) {
		Tokenize({'1', '0', '0', '%'});
		EXPECT_EQ(tokenizer.stream.CodePointsLeft(), 0);
		ASSERT_EQ(tokenizer.tokens.size(), 1);
		const CSS::Token &token = tokenizer.tokens[0];
		EXPECT_EQ(token.type, Token::Type::PERCENTAGE);

		const auto &tokenData = std::get<TokenNumericData>(token.data);
		EXPECT_EQ(tokenData.type, TokenNumericType::INTEGER);
		EXPECT_EQ(tokenData.integer, 100);
	}

	TEST_F(TokenizerConsumeNumericToken, DimensionTest) {
		Unicode::CodePoint codePoint;
		Tokenize({'3', '5', '0', 'p', 'x', ' '});
		EXPECT_EQ(tokenizer.stream.CodePointsLeft(), 1);
		EXPECT_TRUE(tokenizer.stream.Peek(&codePoint));
		EXPECT_EQ(codePoint, ' ');
		ASSERT_EQ(tokenizer.tokens.size(), 1);
		const CSS::Token &token = tokenizer.tokens[0];
		ASSERT_EQ(token.type, Token::Type::DIMENSION);

		const auto &tokenData = std::get<TokenDimensionData>(token.data);
		EXPECT_EQ(tokenData.type, TokenNumericType::INTEGER);
		EXPECT_EQ(tokenData.integer, 350);
		const std::vector<Unicode::CodePoint> expected = {'p', 'x'};
		EXPECT_EQ(tokenData.codePoints, expected);
	}

} // namespace CSS
