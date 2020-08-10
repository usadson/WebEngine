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
		ASSERT_EQ(tokenizer.ConsumeEscapedCodePoint(), Unicode::REPLACEMENT_CHARACTER);
		ASSERT_TRUE(tokenizer.tokens.empty());
		ASSERT_TRUE(ParseErrorTester::WasParseErrorFired(CSS::ParseError::EOF_IN_CONSUMING_ESCAPED_CODE_POINT));
	}

	TEST_F(TokenizerConsumeEscapedCodePointTest, TestHexNull) {
		const Unicode::UString string("000000");
		Tokenizer tokenizer(context, string);
		ASSERT_EQ(tokenizer.ConsumeEscapedCodePoint(), Unicode::REPLACEMENT_CHARACTER);
		ASSERT_TRUE(tokenizer.tokens.empty());
		ASSERT_FALSE(ParseErrorTester::WasParseErrorFired());
	}

	TEST_F(TokenizerConsumeEscapedCodePointTest, TestHexSurrogate) {
		std::stringstream stream;
		stream << std::hex << 0xD800 + std::rand() % 0x800;
		const Unicode::UString string(stream.str().c_str());
		Tokenizer tokenizer(context, string);
		ASSERT_EQ(tokenizer.ConsumeEscapedCodePoint(), Unicode::REPLACEMENT_CHARACTER);
		ASSERT_TRUE(tokenizer.tokens.empty());
		ASSERT_FALSE(ParseErrorTester::WasParseErrorFired());
	}

	TEST_F(TokenizerConsumeEscapedCodePointTest, TestHexFurtherThanMaxUnicode) {
		std::stringstream stream;
		stream << std::hex << Unicode::LAST_ALLOWED_CODE_POINT + std::rand();
		const Unicode::UString string(stream.str().c_str());
		Tokenizer tokenizer(context, string);
		ASSERT_EQ(tokenizer.ConsumeEscapedCodePoint(), Unicode::REPLACEMENT_CHARACTER);
		ASSERT_TRUE(tokenizer.tokens.empty());
		ASSERT_FALSE(ParseErrorTester::WasParseErrorFired());
	}

} // namespace CSS
