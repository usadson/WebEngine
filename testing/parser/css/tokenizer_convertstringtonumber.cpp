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
		const auto *asDouble = std::get_if<double>(&result);
		EXPECT_NE(asDouble, nullptr);
		EXPECT_EQ(*asDouble, 1);
	}

} // namespace CSS
