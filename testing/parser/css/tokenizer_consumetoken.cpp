/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

namespace CSS {

	class TokenizerConsumeToken : public ::testing::Test {
	public:
		Context context { &ParseErrorTester::ReporterEndpoint };

		void
		TestWhitespace(const Unicode::UString &string) {
			Tokenizer tokenizer(context, string);
			tokenizer.stream.Skip();
			ASSERT_TRUE(tokenizer.ConsumeToken(string[0]));
			ASSERT_FALSE(ParseErrorTester::WasParseErrorFired());
			ASSERT_EQ(tokenizer.tokens.size(), 1);
			ASSERT_EQ(tokenizer.tokens[0].type, TokenType::WHITESPACE);
			ASSERT_TRUE(std::holds_alternative<std::nullptr_t>(tokenizer.tokens[0].data));
			ASSERT_EQ(std::get<std::nullptr_t>(tokenizer.tokens[0].data), nullptr);
		}
	};

	TEST_F(TokenizerConsumeToken, TestWhitespaceSpace) {
		const Unicode::UString string("     ");
		TestWhitespace(string);
	}

	TEST_F(TokenizerConsumeToken, TestWhitespaceNewline) {
		const Unicode::UString string("\n\n\n\n\n\n\n\n\n");
		TestWhitespace(string);
	}

} // namespace CSS

