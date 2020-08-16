/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <random>
#include <variant>
#include <vector>

#include <cstring>

#include "data/text/encoding/utf8.hpp"
#include "data/text/ustring.hpp"

namespace CSS {

	class TokenizerNameToken : public ::testing::Test {
	public:
		Context context { &ParseErrorTester::ReporterEndpoint };
	};

	TEST_F(TokenizerNameToken, TestSimple) {
		const Unicode::UString input("test-string ");
		const std::vector<Unicode::CodePoint> expected { 't', 'e', 's', 't', '-', 's', 't', 'r', 'i', 'n', 'g' };
		Unicode::CodePoint codePoint;
		std::vector<Unicode::CodePoint> output;

		Tokenizer tokenizer(context, input);
		ASSERT_TRUE(tokenizer.ConsumeName(output));
		EXPECT_FALSE(ParseErrorTester::WasParseErrorFired());
		EXPECT_TRUE(tokenizer.tokens.empty());
		EXPECT_TRUE(tokenizer.stream.Next(&codePoint));
		EXPECT_EQ(codePoint, Unicode::SPACE);
		EXPECT_EQ(output, expected);
	}

	TEST_F(TokenizerNameToken, TestEscapes) {
		const Unicode::UString input("\\test ");
		const std::vector<Unicode::CodePoint> expected { 't', 'e', 's', 't' };
		Unicode::CodePoint codePoint;
		std::vector<Unicode::CodePoint> output;

		Tokenizer tokenizer(context, input);
		ASSERT_TRUE(tokenizer.ConsumeName(output));
		EXPECT_FALSE(ParseErrorTester::WasParseErrorFired());
		EXPECT_TRUE(tokenizer.tokens.empty());
		EXPECT_TRUE(tokenizer.stream.Next(&codePoint));
		EXPECT_EQ(codePoint, Unicode::SPACE);
		EXPECT_EQ(output, expected);
	}

} // namespace CSS
