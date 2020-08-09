/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <climits>
#include <cstdlib>

#define PRIVATE_VISIBILITY public

#include "data/text/ustring.hpp"
#include "parser/css/tokenizer.hpp"

namespace CSS {

	class TokenizerConsumeCommentsTest : public ::testing::Test {
	};

	TEST_F(TokenizerConsumeCommentsTest, TestEmpty) {
		const Unicode::UString string;
		Tokenizer tokenizer(string);
		ASSERT_TRUE(tokenizer.ConsumeComments());
		ASSERT_TRUE(string.Empty());
	}
} // namespace CSS

int
main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
