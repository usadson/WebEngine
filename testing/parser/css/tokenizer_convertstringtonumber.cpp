/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <limits>

namespace CSS {

	class TokenizerConvertStringToNumber : public ::testing::Test {
	public:
		Context context {&ParseErrorTester::ReporterEndpoint};
		const Unicode::UString simulatedInput;
		Tokenizer tokenizer {context, simulatedInput};

		void
		TestInt(const std::vector<Unicode::CodePoint> &in, std::int64_t expected) {
			const auto result = tokenizer.ConvertStringToNumber(in);
			const auto *asInt = std::get_if<std::int64_t>(&result);
			EXPECT_NE(asInt, nullptr);
			EXPECT_EQ(*asInt, expected);
		}

		void
		TestDouble(const std::vector<Unicode::CodePoint> &in, double expected) {
			const auto result = tokenizer.ConvertStringToNumber(in);
			const auto *asDouble = std::get_if<double>(&result);
			EXPECT_NE(asDouble, nullptr);
			EXPECT_EQ(*asDouble, expected);
		}
	};

	TEST_F(TokenizerConvertStringToNumber, IntTest) {
		const std::vector<Unicode::CodePoint> vec {'1'};
		TestInt(vec, 1);
	}

	TEST_F(TokenizerConvertStringToNumber, SignedIntTest) {
		const std::vector<Unicode::CodePoint> vec {'-', '1'};
		TestInt(vec, -1);
	}

	TEST_F(TokenizerConvertStringToNumber, MaxIntegerTest) {
		const auto max = std::numeric_limits<std::int64_t>::max();
		const std::string stdString = std::to_string(max);
		const Unicode::UString testString(stdString.c_str());
		const std::vector<Unicode::CodePoint> vec(std::cbegin(testString), std::cend(testString));
		TestInt(vec, max);
	}

	TEST_F(TokenizerConvertStringToNumber, MinIntegerTest) {
		const auto min = std::numeric_limits<std::int64_t>::min();
		const std::string stdString = std::to_string(min);
		const Unicode::UString testString(stdString.c_str());
		const std::vector<Unicode::CodePoint> vec(std::cbegin(testString), std::cend(testString));
		TestInt(vec, min);
	}

	TEST_F(TokenizerConvertStringToNumber, DoubleFractionalTest) {
		const std::vector<Unicode::CodePoint> vec {'1', '.', '2'};
		TestDouble(vec, 1.2);
	}

	TEST_F(TokenizerConvertStringToNumber, ExponentTest) {
		const std::vector<Unicode::CodePoint> vec {'1', 'e', '2'};
		TestDouble(vec, 1e2);
	}

	TEST_F(TokenizerConvertStringToNumber, BigExponentTest) {
		const std::vector<Unicode::CodePoint> vec {'1', '2', '3', 'e', '4', '5'};
		TestDouble(vec, 123e45);
	}

} // namespace CSS
