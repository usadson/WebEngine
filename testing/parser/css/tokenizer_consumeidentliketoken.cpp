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
		ASSERT_EQ(tokenizer.tokens[0].type, TokenType::IDENT);
		const auto *data = std::get_if<TokenCodePointsData>(&tokenizer.tokens[0].data);
		ASSERT_NE(data, nullptr);
		EXPECT_EQ(data->codePoints, expected);
	}

	TEST_F(TokenizerConsumeIdentLikeToken, FunctionTest) {
		const Unicode::UString input("rgb(255, 255, 255)");
		const std::vector<Unicode::CodePoint> expected {'r', 'g', 'b'};
		Tokenizer tokenizer(context, input);
		tokenizer.stream.Skip();
		EXPECT_TRUE(tokenizer.ConsumeToken(input[0]));
		EXPECT_EQ(tokenizer.tokens.size(), 1);
		ASSERT_EQ(tokenizer.tokens[0].type, TokenType::FUNCTION);
		const auto *data = std::get_if<TokenCodePointsData>(&tokenizer.tokens[0].data);
		ASSERT_NE(data, nullptr);
		EXPECT_EQ(data->codePoints, expected);
	}

	TEST_F(TokenizerConsumeIdentLikeToken, URLTest) {
		const Unicode::UString input("url(https://test.com/)");
		const std::vector<Unicode::CodePoint> expected {'h', 't', 't', 'p', 's', ':', '/', '/', 't', 'e', 's', 't', '.', 'c', 'o', 'm', '/'};
		Tokenizer tokenizer(context, input);
		tokenizer.stream.Skip();
		EXPECT_TRUE(tokenizer.ConsumeToken(input[0]));
		EXPECT_EQ(tokenizer.tokens.size(), 1);
		ASSERT_EQ(tokenizer.tokens[0].type, TokenType::URL);
		const auto *data = std::get_if<TokenCodePointsData>(&tokenizer.tokens[0].data);
		ASSERT_NE(data, nullptr);
		EXPECT_EQ(data->codePoints, expected);
	}

} // namespace CSS
