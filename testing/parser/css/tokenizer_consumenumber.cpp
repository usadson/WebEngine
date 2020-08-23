/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

struct OverConsumeInputs {
	bool isInt;
	bool appendRandom;
	std::uint64_t intValue;
	double numberValue;
	Unicode::UString string;
	Unicode::CodePoint customEnding {};
};

namespace CSS {

	class TokenizerConsumeNumber : public ::testing::Test {
	public:
		Context context {&ParseErrorTester::ReporterEndpoint};
		Unicode::UString streamContents {};
		Tokenizer tokenizer {context, streamContents};
		const Unicode::UString initialString {};

		void
		TestInt(Unicode::UString &&string, std::int64_t expected) {
			streamContents = std::move(string);
			tokenizer.stream.SetString(&streamContents);
			const auto result = tokenizer.ConsumeNumber();
			const auto *asInt = std::get_if<std::int64_t>(&result);
			EXPECT_NE(asInt, nullptr);
			EXPECT_EQ(*asInt, expected);
		}

		void
		TestDouble(Unicode::UString &&string, double expected) {
			streamContents = std::move(string);
			tokenizer.stream.SetString(&streamContents);
			const auto result = tokenizer.ConsumeNumber();
			const auto *asDouble = std::get_if<double>(&result);
			EXPECT_NE(asDouble, nullptr);
			EXPECT_LT(std::abs(*asDouble - expected), 1e-6) << " output=" << *asDouble << " != expected=" << expected;
		}

		[[nodiscard]] Unicode::CodePoint
		GetRandomNonNumericCodePoint() const noexcept {
			Unicode::CodePoint val;
			while (true) {
				switch (val = std::rand()) {
					case Unicode::HYPHEN_MINUS:
					case Unicode::PLUS_SIGN:
					case Unicode::LATIN_CAPITAL_LETTER_E:
					case Unicode::LATIN_SMALL_LETTER_E:
						break;
					default:
						if (!Unicode::IsDigit(val)) {
							return val;
						}
						break;
				}
			}
		}
	};

	TEST_F(TokenizerConsumeNumber, IntTest) {
		TestInt({'1'}, 1);
		TestInt({'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'}, 1234567890);
		TestInt({'-', '2', '0', '4', '8'}, -2048);
		TestInt({'+', '4', '0', '9', '6'}, 4096);
	}

	TEST_F(TokenizerConsumeNumber, DoubleTest) {
		TestDouble({'1', '.', '0'}, 1.0);
		TestDouble(
			{
				'2',
				'e',
				'3',
			},
			2000.0);
		TestDouble({'4', 'e', '-', '5'}, 4e-5);
		TestDouble({'-', '6', '.', '7', 'e', '8'}, -6.7e8);
		TestDouble({'+', '9', '.', '0'}, 9.0);
	}

	TEST_F(TokenizerConsumeNumber, OverConsumeTest) {
		;
		const std::array<OverConsumeInputs, 7> inputs = {{
			{true, true, 1, 0.0, {'1'}},
			{false, true, 0, 1.0, {'1', '.', '0'}},
			{false, true, 0, 2e1, {'2', 'e', '1'}},
			{false, true, 0, 3.5e6, {'3', '.', '5', 'e', '6'}},
			{false, true, 0, -2.7e9, {'-', '2', '.', '7', 'e', '9'}},
			{false, false, 0, -2.7e9, {'-', '2', '.', '7', 'e', '9'}, 'e'},
			{false, false, 0, -2.7e9, {'-', '2', '.', '7', 'e', '9'}, 'E'},
		}};

		Unicode::CodePoint codePoint {};
		for (const auto &input : inputs) {
			auto ending = input.customEnding != 0 ? input.customEnding : GetRandomNonNumericCodePoint();
			auto string = input.string;
			string += ending;
			if (input.isInt) {
				TestInt(std::move(string), input.intValue);
			} else {
				TestDouble(std::move(string), input.numberValue);
			}
			EXPECT_TRUE(tokenizer.stream.Next(&codePoint));
			EXPECT_EQ(codePoint, ending);
		}
	}

} // namespace CSS
