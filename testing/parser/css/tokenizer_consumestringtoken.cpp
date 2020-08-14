/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <random>
#include <variant>
#include <vector>

#include "data/text/ustring.hpp"

namespace CSS {

	class TokenizerConsumeStringToken : public ::testing::Test {
	public:
		Context context { &ParseErrorTester::ReporterEndpoint };

		std::random_device randomDevice;
		std::mt19937 randomGenerator{ randomDevice() };
		std::uniform_int_distribution<Unicode::CodePoint> integerDistributor{ 0, Unicode::LAST_ALLOWED_CODE_POINT };

		[[nodiscard]] Unicode::CodePoint
		GetRandomCodePoint() noexcept {
			return integerDistributor(randomGenerator);
		}

		void
		TestLegal(const Unicode::UString &string, Unicode::CodePoint ending, const std::vector<Unicode::CodePoint> &result) {
			Tokenizer tokenizer(context, string);
			ASSERT_TRUE(tokenizer.ConsumeStringToken(ending));
			ASSERT_FALSE(ParseErrorTester::WasParseErrorFired());
			ASSERT_EQ(tokenizer.tokens.size(), 1);
			ASSERT_EQ(tokenizer.tokens[0].type, TokenType::STRING);
			const auto *data = std::get_if<TokenCodePointsData>(&tokenizer.tokens[0].data);
			ASSERT_NE(data, nullptr);
			ASSERT_EQ(data->codePoints, result);
		}
	};

	TEST_F(TokenizerConsumeStringToken, TestEmptyQuotationMarkEnding) {
		const Unicode::UString string("\"");
		const Unicode::CodePoint ending = Unicode::QUOTATION_MARK;
		const std::vector<Unicode::CodePoint> result = {};
		TestLegal(string, ending, result);
	}

} // namespace CSS
