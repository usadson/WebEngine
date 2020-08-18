/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

namespace CSS {

	class TokenizerConvertStringToNumber : public ::testing::Test {
	public:
		Context context {&ParseErrorTester::ReporterEndpoint};
		const Unicode::UString simulatedInput;
		Tokenizer tokenizer{ context, simulatedInput };

		void
		TestInt(const std::vector<Unicode::CodePoint> &in, std::int64_t expected) {
		const auto result = tokenizer.ConvertStringToNumber(in);
		const auto *asInt = std::get_if<std::int64_t>(&result);
		EXPECT_NE(asInt, nullptr);
		EXPECT_EQ(*asInt, expected);
		}
	};

	TEST_F(TokenizerConvertStringToNumber, IntTest) {
		const std::vector<Unicode::CodePoint> vec{ '1' };
		TestInt(vec, 1);
	}

	TEST_F(TokenizerConvertStringToNumber, SignedIntTest) {
		const std::vector<Unicode::CodePoint> vec{ '-', '1' };
		TestInt(vec, -1);
	}

} // namespace CSS
