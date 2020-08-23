/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

struct OverConsumeInputs {
	bool isInteger;
	bool appendRandom;
	CSS::IntegerType intValue;
	CSS::NumberType numberValue;
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
		TestInteger(Unicode::UString &&string, CSS::IntegerType expected) {
			streamContents = std::move(string);
			tokenizer.stream.SetString(&streamContents);
			const auto result = tokenizer.ConsumeNumber();
			const auto *asInteger = std::get_if<CSS::IntegerType>(&result);
			EXPECT_NE(asInteger, nullptr);
			EXPECT_EQ(*asInteger, expected);
		}

		void
		TestNumber(Unicode::UString &&string, CSS::NumberType expected) {
			streamContents = std::move(string);
			tokenizer.stream.SetString(&streamContents);
			const auto result = tokenizer.ConsumeNumber();
			const auto *asNumber = std::get_if<CSS::NumberType>(&result);
			EXPECT_NE(asNumber, nullptr);
			EXPECT_LT(std::abs(*asNumber - expected), 1e-6) << " output=" << *asNumber << " != expected=" << expected;
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

	TEST_F(TokenizerConsumeNumber, IntegerTest) {
		TestInteger({'1'}, 1);
		TestInteger({'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'}, 1234567890);
		TestInteger({'-', '2', '0', '4', '8'}, -2048);
		TestInteger({'+', '4', '0', '9', '6'}, 4096);
	}

	TEST_F(TokenizerConsumeNumber, NumberTest) {
		TestNumber({'1', '.', '0'}, 1.0);
		TestNumber(
			{
				'2',
				'e',
				'3',
			},
			2000.0);
		TestNumber({'4', 'e', '-', '5'}, 4e-5);
		TestNumber({'-', '6', '.', '7', 'e', '8'}, -6.7e8);
		TestNumber({'+', '9', '.', '0'}, 9.0);
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
			if (input.isInteger) {
				TestInteger(std::move(string), input.intValue);
			} else {
				TestNumber(std::move(string), input.numberValue);
			}
			EXPECT_TRUE(tokenizer.stream.Next(&codePoint));
			EXPECT_EQ(codePoint, ending);
		}
	}

} // namespace CSS
