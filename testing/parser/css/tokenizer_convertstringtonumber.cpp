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
	};

	TEST_F(TokenizerConvertStringToNumber, IntTest) {
		const std::vector<Unicode::CodePoint> vec{ '1' };
		const std::variant<std::monostate, std::int64_t, double> result = tokenizer.ConvertStringToNumber(vec);
		const auto *asInt = std::get_if<std::int64_t>(&result);
		EXPECT_NE(asInt, nullptr);
		EXPECT_EQ(*asInt, 1);
	}

	TEST_F(TokenizerConvertStringToNumber, SignedIntTest) {
		const std::vector<Unicode::CodePoint> vec{ '-', '1' };
		const auto result = tokenizer.ConvertStringToNumber(vec);
		const auto *asInt = std::get_if<std::int64_t>(&result);
		EXPECT_NE(asInt, nullptr);
		EXPECT_EQ(*asInt, -1);
	}

} // namespace CSS
