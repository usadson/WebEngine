/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "data/text/unicode.hpp"

#include <gtest/gtest.h>

namespace Unicode {

	TEST(Unicode, IsASCIIAlpha) {
		EXPECT_TRUE(IsASCIIAlpha('a'));
		EXPECT_TRUE(IsASCIIAlpha('q'));
		EXPECT_TRUE(IsASCIIAlpha('z'));
		EXPECT_TRUE(IsASCIIAlpha(Unicode::LATIN_SMALL_LETTER_A));
		EXPECT_TRUE(IsASCIIAlpha(Unicode::LATIN_SMALL_LETTER_Q));
		EXPECT_TRUE(IsASCIIAlpha(Unicode::LATIN_SMALL_LETTER_Z));

		EXPECT_TRUE(IsASCIIAlpha('A'));
		EXPECT_TRUE(IsASCIIAlpha('Q'));
		EXPECT_TRUE(IsASCIIAlpha('Z'));
		EXPECT_TRUE(IsASCIIAlpha(Unicode::LATIN_CAPITAL_LETTER_A));
		EXPECT_TRUE(IsASCIIAlpha(Unicode::LATIN_CAPITAL_LETTER_Q));
		EXPECT_TRUE(IsASCIIAlpha(Unicode::LATIN_CAPITAL_LETTER_Z));

		EXPECT_FALSE(IsASCIIAlpha('\f'));
		EXPECT_FALSE(IsASCIIAlpha(Unicode::REPLACEMENT_CHARACTER));
		EXPECT_FALSE(IsASCIIAlpha(Unicode::COLON));
	}

	TEST(Unicode, IsDigit) {
		EXPECT_TRUE(IsDigit('0'));
		EXPECT_TRUE(IsDigit('3'));
		EXPECT_TRUE(IsDigit('9'));

		EXPECT_TRUE(IsDigit(Unicode::DIGIT_ZERO));
		EXPECT_TRUE(IsDigit(Unicode::DIGIT_THREE));
		EXPECT_TRUE(IsDigit(Unicode::DIGIT_NINE));

		EXPECT_FALSE(IsDigit(Unicode::NULL_CHARACTER));
		EXPECT_FALSE(IsDigit(Unicode::COLON));
		EXPECT_FALSE(IsDigit(Unicode::REPLACEMENT_CHARACTER));
		EXPECT_FALSE(IsDigit(Unicode::LAST_ALLOWED_CODE_POINT));
	}

	TEST(Unicode, IsASCIIAlphaNumeric) {
		EXPECT_TRUE(IsASCIIAlphaNumeric('0'));
		EXPECT_TRUE(IsASCIIAlphaNumeric('3'));
		EXPECT_TRUE(IsASCIIAlphaNumeric('9'));
		EXPECT_TRUE(IsASCIIAlphaNumeric(Unicode::DIGIT_ZERO));
		EXPECT_TRUE(IsASCIIAlphaNumeric(Unicode::DIGIT_THREE));
		EXPECT_TRUE(IsASCIIAlphaNumeric(Unicode::DIGIT_NINE));

		EXPECT_TRUE(IsASCIIAlphaNumeric('a'));
		EXPECT_TRUE(IsASCIIAlphaNumeric('z'));
		EXPECT_TRUE(IsASCIIAlphaNumeric('q'));
		EXPECT_TRUE(IsASCIIAlphaNumeric('f'));
		EXPECT_TRUE(IsASCIIAlphaNumeric(Unicode::LATIN_SMALL_LETTER_A));
		EXPECT_TRUE(IsASCIIAlphaNumeric(Unicode::LATIN_SMALL_LETTER_Z));
		EXPECT_TRUE(IsASCIIAlphaNumeric(Unicode::LATIN_SMALL_LETTER_Y));
		EXPECT_TRUE(IsASCIIAlphaNumeric(Unicode::LATIN_SMALL_LETTER_H));

		EXPECT_TRUE(IsASCIIAlphaNumeric('A'));
		EXPECT_TRUE(IsASCIIAlphaNumeric('Z'));
		EXPECT_TRUE(IsASCIIAlphaNumeric('U'));
		EXPECT_TRUE(IsASCIIAlphaNumeric('S'));
		EXPECT_TRUE(IsASCIIAlphaNumeric(Unicode::LATIN_CAPITAL_LETTER_A));
		EXPECT_TRUE(IsASCIIAlphaNumeric(Unicode::LATIN_CAPITAL_LETTER_Z));
		EXPECT_TRUE(IsASCIIAlphaNumeric(Unicode::LATIN_CAPITAL_LETTER_X));
		EXPECT_TRUE(IsASCIIAlphaNumeric(Unicode::LATIN_CAPITAL_LETTER_V));

		for (std::size_t i = 0; i < 10; i++) {
			EXPECT_FALSE(IsASCIIAlphaNumeric(std::rand() + 0x007B));
		}

		EXPECT_FALSE(IsASCIIAlphaNumeric(Unicode::START_OF_TEXT));
		EXPECT_FALSE(IsASCIIAlphaNumeric(Unicode::SOLIDUS));
		EXPECT_FALSE(IsASCIIAlphaNumeric(Unicode::COLON));
		EXPECT_FALSE(IsASCIIAlphaNumeric(Unicode::COMMERCIAL_AT));
		EXPECT_FALSE(IsASCIIAlphaNumeric(Unicode::LEFT_SQUARE_BRACKET));
		EXPECT_FALSE(IsASCIIAlphaNumeric(Unicode::GRAVE_ACCENT));
		EXPECT_FALSE(IsASCIIAlphaNumeric(Unicode::LEFT_CURLY_BRACKET));
		EXPECT_FALSE(IsASCIIAlphaNumeric(Unicode::REPLACEMENT_CHARACTER));
		EXPECT_FALSE(IsASCIIAlphaNumeric(Unicode::LAST_ALLOWED_CODE_POINT));
	}

	TEST(Unicode, ToLowerASCII) {
		ASSERT_EQ(ToLowerASCII('a'), 'a');
		ASSERT_EQ(ToLowerASCII('q'), 'q');
		ASSERT_EQ(ToLowerASCII('z'), 'z');

		ASSERT_EQ(ToLowerASCII('A'), 'a');
		ASSERT_EQ(ToLowerASCII('Q'), 'q');
		ASSERT_EQ(ToLowerASCII('Z'), 'z');

		ASSERT_EQ(ToLowerASCII('0'), '0');
		ASSERT_EQ(ToLowerASCII(0xDEADBEEF), 0xDEADBEEF);
	}

	TEST(Unicode, IsSurrogate) {
		for (std::size_t i = 0; i < 15; i++) {
			EXPECT_TRUE(IsSurrogate(0xD800 + std::rand() % 0x7FF));
		}
		for (std::size_t i = 0; i < 15; i++) {
			EXPECT_FALSE(IsSurrogate(std::rand() % 0xD800));
		}
		for (std::size_t i = 0; i < 15; i++) {
			EXPECT_FALSE(IsSurrogate(0xE000 + std::rand()));
		}

		// Constraints
		EXPECT_FALSE(IsSurrogate(0xD7FF));
		EXPECT_TRUE(IsSurrogate(0xD800));

		EXPECT_TRUE(IsSurrogate(0xDFFF));
		EXPECT_FALSE(IsSurrogate(0xE000));

		EXPECT_FALSE(IsSurrogate(Unicode::LATIN_CAPITAL_LETTER_V));
		EXPECT_FALSE(IsSurrogate(Unicode::LAST_ALLOWED_CODE_POINT));
	}

} // namespace Unicode

int
main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
