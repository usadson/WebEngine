/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

namespace CSS {

	class TokenizerFunctionTests : public ::testing::Test {
	public:
		Context context {&ParseErrorTester::ReporterEndpoint};

		void
		TestURL(const Token &token, const std::vector<Unicode::CodePoint> &expected) {
			ASSERT_EQ(token.type, TokenType::URL);
			const auto *data = std::get_if<TokenCodePointsData>(&token.data);
			ASSERT_NE(data, nullptr);
			EXPECT_EQ(data->codePoints, expected);
		}

		void
		TestFunction(const Token &token, const std::vector<Unicode::CodePoint> &expected) {
			ASSERT_EQ(token.type, TokenType::FUNCTION);
			const auto *data = std::get_if<TokenCodePointsData>(&token.data);
			ASSERT_NE(data, nullptr);
			EXPECT_EQ(data->codePoints, expected);
		}
	};

} // namespace CSS
