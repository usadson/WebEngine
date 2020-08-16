/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <gtest/gtest.h>

#include "data/text/unicode.hpp"
#include "parser/css/tokenizer_algorithms.hpp"
#include "parser/css/tokenizer_stream.hpp"

namespace CSS {

	class TokenizerAlgorithmsTest : public ::testing::Test {
	public:
		Unicode::CodePoint destination = 'A';
	};

	TEST_F(TokenizerAlgorithmsTest, IsWhitespace) {
		EXPECT_TRUE(IsWhitespace(Unicode::CHARACTER_TABULATION));
		EXPECT_TRUE(IsWhitespace(Unicode::LINE_FEED));
		EXPECT_TRUE(IsWhitespace(Unicode::SPACE));
		EXPECT_FALSE(IsWhitespace(Unicode::CARRIAGE_RETURN));
		EXPECT_FALSE(IsWhitespace(Unicode::FORM_FEED));
		EXPECT_FALSE(IsWhitespace(Unicode::NULL_CHARACTER));
	}

	TEST_F(TokenizerAlgorithmsTest, IsHexCharacterBounds) {
		EXPECT_TRUE(IsHexCharacter('a'));
		EXPECT_TRUE(IsHexCharacter('f'));
		EXPECT_TRUE(IsHexCharacter('A'));
		EXPECT_TRUE(IsHexCharacter('F'));
		EXPECT_TRUE(IsHexCharacter('0'));
		EXPECT_TRUE(IsHexCharacter('9'));

		EXPECT_FALSE(IsHexCharacter('a' - 1));
		EXPECT_FALSE(IsHexCharacter('g'));
		EXPECT_FALSE(IsHexCharacter('A' - 1));
		EXPECT_FALSE(IsHexCharacter('G'));
		EXPECT_FALSE(IsHexCharacter('0' - 1));
		EXPECT_FALSE(IsHexCharacter('9' + 1));
	}

	TEST_F(TokenizerAlgorithmsTest, IsHexCharacterValid) {
		for (Unicode::CodePoint codePoint = 'a'; codePoint < 'g'; codePoint++) {
			EXPECT_TRUE(IsHexCharacter(codePoint));
		}
		for (Unicode::CodePoint codePoint = 'A'; codePoint < 'G'; codePoint++) {
			EXPECT_TRUE(IsHexCharacter(codePoint));
		}
		for (Unicode::CodePoint codePoint = '0'; codePoint <= '9'; codePoint++) {
			EXPECT_TRUE(IsHexCharacter(codePoint));
		}
	}

} // namespace CSS

int
main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
