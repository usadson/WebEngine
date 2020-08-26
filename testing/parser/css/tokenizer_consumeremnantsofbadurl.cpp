/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

namespace CSS {

	class TokenizerConsumeRemnantsOfBadURL : public ::testing::Test {
	public:
		Context context {&ParseErrorTester::ReporterEndpoint};
	};

	TEST_F(TokenizerConsumeRemnantsOfBadURL, SimpleTest) {
		const Unicode::UString string("https://example.com)TEXT");
		Tokenizer tokenizer(context, string);
		tokenizer.ConsumeRemnantsOfBadURL();
		EXPECT_EQ(tokenizer.stream.CodePointsLeft(), 4);
	}

	TEST_F(TokenizerConsumeRemnantsOfBadURL, EscapeTest) {
		const Unicode::UString string("some text here oh! an escaped parenthesis: \\))");
		Tokenizer tokenizer(context, string);
		tokenizer.ConsumeRemnantsOfBadURL();
		EXPECT_EQ(tokenizer.stream.CodePointsLeft(), 0);
	}

} // namespace CSS
