/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

namespace CSS {

	class TokenizerConsumeIdentLikeToken : public ::testing::Test {
	public:
		Context context {&ParseErrorTester::ReporterEndpoint};
	};

	TEST_F(TokenizerConsumeIdentLikeToken, IdentTest) {
		const Unicode::UString input("string");
		const std::vector<Unicode::CodePoint> expected {'s', 't', 'r', 'i', 'n', 'g'};
		Tokenizer tokenizer(context, input);
		tokenizer.stream.Skip();
		EXPECT_TRUE(tokenizer.ConsumeToken(input[0]));
		EXPECT_EQ(tokenizer.tokens.size(), 1);
		ASSERT_EQ(tokenizer.tokens[0].type, Token::Type::IDENT);
		const auto *data = std::get_if<TokenCodePointsData>(&tokenizer.tokens[0].data);
		ASSERT_NE(data, nullptr);
		EXPECT_EQ(data->codePoints, expected);
		EXPECT_EQ(tokenizer.stream.CodePointsLeft(), 0);
	}

	TEST_F(TokenizerConsumeIdentLikeToken, FunctionTest) {
		const Unicode::UString input("rgb(36)");
		const std::vector<Unicode::CodePoint> expected {'r', 'g', 'b'};
		const std::vector<Unicode::CodePoint> next {'3', '6', ')'};
		Tokenizer tokenizer(context, input);
		tokenizer.stream.Skip();
		EXPECT_TRUE(tokenizer.ConsumeToken(input[0]));
		EXPECT_EQ(tokenizer.tokens.size(), 1);
		ASSERT_EQ(tokenizer.tokens[0].type, Token::Type::FUNCTION);
		const auto *data = std::get_if<TokenCodePointsData>(&tokenizer.tokens[0].data);
		ASSERT_NE(data, nullptr);
		EXPECT_EQ(data->codePoints, expected);
		EXPECT_EQ(tokenizer.stream.CodePointsLeft(), 3);
		for (const auto &nextCP : next) {
			Unicode::CodePoint cp;
			EXPECT_TRUE(tokenizer.stream.Next(&cp));
			EXPECT_EQ(nextCP, cp) << static_cast<char>(nextCP) << " != " << static_cast<char>(cp);
		}
	}

	TEST_F(TokenizerConsumeIdentLikeToken, URLTest) {
		const Unicode::UString input("url(https://test.com/)");
		const std::vector<Unicode::CodePoint> expected {
			'h', 't', 't', 'p', 's', ':', '/', '/', 't', 'e', 's', 't', '.', 'c', 'o', 'm', '/'};
		Tokenizer tokenizer(context, input);
		tokenizer.stream.Skip();
		EXPECT_TRUE(tokenizer.ConsumeToken(input[0]));
		EXPECT_EQ(tokenizer.tokens.size(), 1);
		ASSERT_EQ(tokenizer.tokens[0].type, Token::Type::URL);
		const auto *data = std::get_if<TokenCodePointsData>(&tokenizer.tokens[0].data);
		ASSERT_NE(data, nullptr);
		EXPECT_EQ(data->codePoints, expected);
	}

} // namespace CSS
