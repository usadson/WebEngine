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
		TestInt({'1'}, 1);
	}

	TEST_F(TokenizerConvertStringToNumber, SignPrefixTest) {
		TestInt({ '2' }, 2);
		TestInt({ '-', '2' }, -2);
		TestInt({ '+', '2' }, 2);
		TestDouble({ '2', '.', '0' }, 2);
		TestDouble({ '+', '2', '.', '0' }, 2.0);
		TestDouble({ '-', '2', '.', '0' }, -2.0);
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
		TestDouble({'1', '.', '2'}, 1.2);
	}

	TEST_F(TokenizerConvertStringToNumber, ExponentTest) {
		TestDouble({'1', 'e', '2'}, 1e2);
		TestDouble({'2', 'E', '1'}, 2E1);
	}

	TEST_F(TokenizerConvertStringToNumber, BigExponentTest) {
		TestDouble({'1', '2', '3', 'e', '4', '5'}, 123e45);
		TestDouble({'9', '8', '7', 'e', '6', '5'}, 987E65);
	}

	TEST_F(TokenizerConvertStringToNumber, ExponentFractionalTest) {
		TestDouble({'1', '.', '2', 'e', '3'}, 1.2e3);
		TestDouble({'9', '.', '8', 'E', '7'}, 9.8E7);
	}

	TEST_F(TokenizerConvertStringToNumber, ExponentSignPrefixTest) {
		TestDouble({'1', 'e', '+', '2'}, 1e2);
		TestDouble({'2', 'e', '-', '4'}, 2e-4);
		TestDouble({'5', 'E', '+', '6'}, 5E6);
		TestDouble({'7', 'E', '-', '8'}, 7E-8);
	}

	TEST_F(TokenizerConvertStringToNumber, ExponentFractionalSignPrefixTest) {
		TestDouble({'1', '.', '2', 'e', '+', '3'}, 1.2e3);
		TestDouble({'1', '.', '2', 'e', '-', '3'}, 1.2e-3);
		TestDouble({'4', '.', '5', 'e', '+', '6'}, 4.5e6);
		TestDouble({'7', '.', '8', 'e', '-', '9'}, 7.8e-9);
	}

	TEST_F(TokenizerConvertStringToNumber, SignPrefixIntExponentFractionalSignPrefixTest) {
		TestDouble({'+', '1', '.', '2', 'e', '+', '3'}, +1.2e3);
		TestDouble({'-', '4', '.', '5', 'e', '-', '6'}, -4.5e-6);
	}

	TEST_F(TokenizerConvertStringToNumber, RandomIntegerTest) {
		for (std::size_t i = 0; i < 15; i++) {
			const auto value = std::rand();
			const std::string string = std::to_string(value);
			const Unicode::UString ustring(string.c_str());
			const std::vector<Unicode::CodePoint> vec(std::cbegin(ustring), std::cend(ustring));
			TestInt(vec, value);
		}
	}

} // namespace CSS
