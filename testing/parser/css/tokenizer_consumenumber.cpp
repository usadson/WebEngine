/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

namespace CSS {

	class TokenizerConsumeNumber : public ::testing::Test {
	public:
		Context context {&ParseErrorTester::ReporterEndpoint};
	};

	TEST_F(TokenizerConsumeNumber, TestInt) {
		const Unicode::UString input{'1'};
		Tokenizer tokenizer(context, input);
		static_cast<void>(tokenizer.ConsumeNumber());
	}

} // namespace CSS
