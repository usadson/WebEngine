/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <limits>

#include <cmath>

namespace CSS {

	class TokenizerConvertStringToNumber : public ::testing::Test {
	public:
		Context context {&ParseErrorTester::ReporterEndpoint};
		const Unicode::UString simulatedInput;
		Tokenizer tokenizer {context, simulatedInput};

		void
		TestInteger(const std::vector<char> &in, CSS::IntegerType expected) {
			const auto result = tokenizer.ConvertStringToNumber(in);
			const auto *asInteger = std::get_if<CSS::IntegerType>(&result);
			EXPECT_NE(asInteger, nullptr);
			EXPECT_EQ(*asInteger, expected);
		}

		void
		TestNumber(const std::vector<char> &in, CSS::NumberType expected) {
			const auto result = tokenizer.ConvertStringToNumber(in);
			const auto *asNumber = std::get_if<CSS::NumberType>(&result);
			EXPECT_NE(asNumber, nullptr);
			EXPECT_LT(std::abs(*asNumber - expected), 1e-6) << " output=" << *asNumber << " != expected=" << expected;
		}
	};

	TEST_F(TokenizerConvertStringToNumber, IntTest) {
		TestInteger({'1'}, 1);
	}

	TEST_F(TokenizerConvertStringToNumber, SignPrefixTest) {
		TestInteger({'2'}, 2);
		TestInteger({'-', '2'}, -2);
		TestInteger({'+', '2'}, 2);
		TestNumber({'2', '.', '0'}, 2);
		TestNumber({'+', '2', '.', '0'}, 2.0);
		TestNumber({'-', '2', '.', '0'}, -2.0);
	}

	TEST_F(TokenizerConvertStringToNumber, MaxIntegerTest) {
		const auto max = std::numeric_limits<CSS::IntegerType>::max();
		const std::string stdString = std::to_string(max);
		const Unicode::UString testString(stdString.c_str());
		const std::vector<char> vec(std::cbegin(testString), std::cend(testString));
		TestInteger(vec, max);
	}

	TEST_F(TokenizerConvertStringToNumber, MinIntegerTest) {
		const auto min = std::numeric_limits<CSS::IntegerType>::min();
		const std::string stdString = std::to_string(min);
		const Unicode::UString testString(stdString.c_str());
		const std::vector<char> vec(std::cbegin(testString), std::cend(testString));
		TestInteger(vec, min);
	}

	TEST_F(TokenizerConvertStringToNumber, NumberFractionalTest) {
		TestNumber({'1', '.', '2'}, 1.2);
	}

	TEST_F(TokenizerConvertStringToNumber, ExponentTest) {
		TestNumber({'1', 'e', '2'}, 1e2);
		TestNumber({'2', 'E', '1'}, 2E1);
	}

	TEST_F(TokenizerConvertStringToNumber, BigExponentTest) {
		TestNumber({'1', '2', '3', 'e', '4', '5'}, 123e45);
		TestNumber({'9', '8', '7', 'e', '6', '5'}, 987E65);
	}

	TEST_F(TokenizerConvertStringToNumber, ExponentFractionalTest) {
		TestNumber({'1', '.', '2', 'e', '3'}, 1.2e3);
		TestNumber({'9', '.', '8', 'E', '7'}, 9.8E7);
	}

	TEST_F(TokenizerConvertStringToNumber, ExponentSignPrefixTest) {
		TestNumber({'1', 'e', '+', '2'}, 1e2);
		TestNumber({'2', 'e', '-', '4'}, 2e-4);
		TestNumber({'5', 'E', '+', '6'}, 5E6);
		TestNumber({'7', 'E', '-', '8'}, 7E-8);
	}

	TEST_F(TokenizerConvertStringToNumber, ExponentFractionalSignPrefixTest) {
		TestNumber({'1', '.', '2', 'e', '+', '3'}, 1.2e3);
		TestNumber({'1', '.', '2', 'e', '-', '3'}, 1.2e-3);
		TestNumber({'4', '.', '5', 'e', '+', '6'}, 4.5e6);
		TestNumber({'7', '.', '8', 'e', '-', '9'}, 7.8e-9);
	}

	TEST_F(TokenizerConvertStringToNumber, SignPrefixIntExponentFractionalSignPrefixTest) {
		TestNumber({'+', '1', '.', '2', 'e', '+', '3'}, +1.2e3);
		TestNumber({'-', '4', '.', '5', 'e', '-', '6'}, -4.5e-6);
	}

	TEST_F(TokenizerConvertStringToNumber, RandomIntegerTest) {
		for (std::size_t i = 0; i < 15; i++) {
			const auto value = std::rand();
			const std::string string = std::to_string(value);
			const std::vector<char> vec(std::cbegin(string), std::cend(string));
			TestInteger(vec, value);
		}
	}

	TEST_F(TokenizerConvertStringToNumber, RandomNumberTest) {
		std::uniform_real_distribution<CSS::NumberType> distributor(-1e16, 1e16);
		std::default_random_engine engine;

		for (std::size_t i = 0; i < 15; i++) {
			const auto value = distributor(engine);
			const std::string string = std::to_string(value);
			const std::vector<char> vec(std::cbegin(string), std::cend(string));
			TestNumber(vec, value);
		}
	}

	TEST_F(TokenizerConvertStringToNumber, FractionalWithoutIntegerTest) {
		TestNumber({'.', '3'}, 0.3);
		TestNumber({'.', '4', '4'}, 0.44);
		TestNumber({'.', '4', '4', 'e', '6'}, 0.44e6);
	}

} // namespace CSS
