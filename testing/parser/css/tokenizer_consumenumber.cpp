/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

namespace CSS {

	class TokenizerConsumeNumber : public ::testing::Test {
	public:
		Context context {&ParseErrorTester::ReporterEndpoint};
		const Unicode::UString initialString{};
		Tokenizer tokenizer{ context, initialString };

		void
		TestInt(Unicode::UString string, std::int64_t expected) {
			tokenizer.stream.SetString(&string);
			const auto result = tokenizer.ConsumeNumber();
			const auto *asInt = std::get_if<std::int64_t>(&result);
			EXPECT_NE(asInt, nullptr);
			EXPECT_EQ(*asInt, expected);
		}

		void
		TestDouble(Unicode::UString string, double expected) {
			tokenizer.stream.SetString(&string);
			const auto result = tokenizer.ConsumeNumber();
			const auto *asDouble = std::get_if<double>(&result);
			EXPECT_NE(asDouble, nullptr);
			EXPECT_LT(std::abs(*asDouble - expected), 1e-6) << " output=" << *asDouble << " != expected=" << expected;
		}
	};

	TEST_F(TokenizerConsumeNumber, IntTest) {
		TestInt({'1'}, 1);
		TestInt({'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'}, 1234567890);
		TestInt({'-', '2', '0', '4', '8'}, -2048);
		TestInt({'+', '4', '0', '9', '6'}, 4096);
	}

} // namespace CSS
