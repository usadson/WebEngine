/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

namespace CSS {

	class TokenizerConsumeCommentsTest : public ::testing::Test {
	public:
		Context context { &ParseErrorTester::ReporterEndpoint };
	};

	TEST_F(TokenizerConsumeCommentsTest, TestEmpty) {
		const Unicode::UString string;
		Tokenizer tokenizer(context, string);
		ASSERT_TRUE(tokenizer.ConsumeComments());
		ASSERT_TRUE(tokenizer.string.Empty());
		ASSERT_FALSE(ParseErrorTester::WasParseErrorFired());
	}

	TEST_F(TokenizerConsumeCommentsTest, TestNoCommentCharacters) {
		const Unicode::UString string("No comment characters are in here");
		const std::size_t length = string.length();
		Tokenizer tokenizer(context, string);
		ASSERT_TRUE(tokenizer.ConsumeComments());
		ASSERT_EQ(tokenizer.string.length(), length);
		ASSERT_FALSE(ParseErrorTester::WasParseErrorFired());
	}

	TEST_F(TokenizerConsumeCommentsTest, TestNoComments) {
		const Unicode::UString string("/N*o comments are in her*e/");
		const std::size_t length = string.length();
		Tokenizer tokenizer(context, string);
		ASSERT_TRUE(tokenizer.ConsumeComments());
		ASSERT_EQ(tokenizer.string.length(), length);
		ASSERT_FALSE(ParseErrorTester::WasParseErrorFired());
	}

	TEST_F(TokenizerConsumeCommentsTest, TestNoCommentsSlashStarSlash) {
		const Unicode::UString string("/*/");
		const Unicode::UString testString("Test string $iaf259");
		Tokenizer tokenizer(context, string);
		tokenizer.string = testString;
		ASSERT_FALSE(tokenizer.ConsumeComments());
		ASSERT_EQ(tokenizer.string, testString);
		ASSERT_TRUE(ParseErrorTester::WasParseErrorFired(CSS::ParseError::EOF_IN_CONSUMING_COMMENT));
	}

	TEST_F(TokenizerConsumeCommentsTest, TestEmptyComment) {
		const Unicode::UString string("/**/");
		Tokenizer tokenizer(context, string);
		ASSERT_TRUE(tokenizer.ConsumeComments());
		ASSERT_EQ(tokenizer.string.length(), 0);
		ASSERT_FALSE(ParseErrorTester::WasParseErrorFired());
	}

	TEST_F(TokenizerConsumeCommentsTest, TestCommentWithText) {
		const Unicode::UString string("/* Hello this is a comment*/");
		Tokenizer tokenizer(context, string);
		ASSERT_TRUE(tokenizer.ConsumeComments());
		ASSERT_EQ(tokenizer.string.length(), 0);
		ASSERT_FALSE(ParseErrorTester::WasParseErrorFired());
	}

	TEST_F(TokenizerConsumeCommentsTest, TestCommentStart) {
		const Unicode::UString input("/* The following is a thing with leaves */Tree");
		const Unicode::UString output("Tree");
		Tokenizer tokenizer(context, input);
		ASSERT_TRUE(tokenizer.ConsumeComments());
		ASSERT_EQ(tokenizer.string, output);
		ASSERT_FALSE(ParseErrorTester::WasParseErrorFired());
	}

	TEST_F(TokenizerConsumeCommentsTest, TestCommentMiddle) {
		const Unicode::UString input("Tr/* unk? */ee");
		const Unicode::UString output("Tree");
		Tokenizer tokenizer(context, input);
		ASSERT_TRUE(tokenizer.ConsumeComments());
		ASSERT_EQ(tokenizer.string, output);
		ASSERT_FALSE(ParseErrorTester::WasParseErrorFired());
	}

	TEST_F(TokenizerConsumeCommentsTest, TestCommentEnd) {
		const Unicode::UString input("Tree/*s are beautiful*/");
		const Unicode::UString output("Tree");
		Tokenizer tokenizer(context, input);
		ASSERT_TRUE(tokenizer.ConsumeComments());
		ASSERT_EQ(tokenizer.string, output);
		ASSERT_FALSE(ParseErrorTester::WasParseErrorFired());
	}

	TEST_F(TokenizerConsumeCommentsTest, TestCommentMultipleEnds) {
		const Unicode::UString input("/*Comment*/*/");
		const Unicode::UString output("*/");
		Tokenizer tokenizer(context, input);
		ASSERT_TRUE(tokenizer.ConsumeComments());
		ASSERT_EQ(tokenizer.string, output);
		ASSERT_FALSE(ParseErrorTester::WasParseErrorFired());
	}

	TEST_F(TokenizerConsumeCommentsTest, TestCommentMultipleEnds2) {
		const Unicode::UString input("/*Comment*/ Test*/");
		const Unicode::UString output(" Test*/");
		Tokenizer tokenizer(context, input);
		ASSERT_TRUE(tokenizer.ConsumeComments());
		ASSERT_EQ(tokenizer.string, output);
		ASSERT_FALSE(ParseErrorTester::WasParseErrorFired());
	}

	TEST_F(TokenizerConsumeCommentsTest, TestCommentMultipleBeginnings) {
		const Unicode::UString input("/*/**/");
		const Unicode::UString output("");
		Tokenizer tokenizer(context, input);
		ASSERT_TRUE(tokenizer.ConsumeComments());
		ASSERT_EQ(tokenizer.string, output);
		ASSERT_FALSE(ParseErrorTester::WasParseErrorFired());
	}

	TEST_F(TokenizerConsumeCommentsTest, TestCommentNested) {
		const Unicode::UString input("/*/**/*/");
		const Unicode::UString output("*/");
		Tokenizer tokenizer(context, input);
		ASSERT_TRUE(tokenizer.ConsumeComments());
		ASSERT_EQ(tokenizer.string, output);
		ASSERT_FALSE(ParseErrorTester::WasParseErrorFired());
	}

	TEST_F(TokenizerConsumeCommentsTest, TestCommentEOF) {
		const Unicode::UString input("The following/* is faulty!");
		const Unicode::UString testString("Random string #2gjs");
		Tokenizer tokenizer(context, input);
		tokenizer.string = testString;
		ASSERT_FALSE(tokenizer.ConsumeComments());
		ASSERT_EQ(tokenizer.string, testString);
		ASSERT_TRUE(ParseErrorTester::WasParseErrorFired(CSS::ParseError::EOF_IN_CONSUMING_COMMENT));
	}

} // namespace CSS
