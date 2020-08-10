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
		(void) tokenizer.ConsumeEscapedCodePoint();
		ASSERT_TRUE(tokenizer.tokens.empty());
		ASSERT_TRUE(ParseErrorTester::WasParseErrorFired());
	}

	TEST_F(TokenizerConsumeEscapedCodePointTest, TestHexNull) {
		const Unicode::UString string("000000");
		Tokenizer tokenizer(context, string);
		ASSERT_EQ(tokenizer.ConsumeEscapedCodePoint(), Unicode::REPLACEMENT_CHARACTER);
		ASSERT_TRUE(tokenizer.tokens.empty());
		ASSERT_FALSE(ParseErrorTester::WasParseErrorFired());
	}

} // namespace CSS
