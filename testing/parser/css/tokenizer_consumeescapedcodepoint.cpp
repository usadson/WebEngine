/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

namespace CSS {

	class TokenizerConsumeEscapedCodePointTest : public ::testing::Test {
	public:
		Context context { &ParseErrorTester::ReporterEndpoint };
	};

	TEST_F(TokenizerConsumeEscapedCodePointTest, TestEmpty) {
		const Unicode::UString string("");
		Tokenizer tokenizer(context, string);
		ASSERT_TRUE(tokenizer.ConsumeEscapedCodePoint());
		ASSERT_TRUE(tokenizer.tokens.empty());
		ASSERT_TRUE(ParseErrorTester::WasParseErrorFired());
	}

} // namespace CSS
