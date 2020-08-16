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

} // namespace CSS

int
main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
