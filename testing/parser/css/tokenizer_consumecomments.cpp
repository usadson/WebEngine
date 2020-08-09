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

	class TokenizerConsumeCommentsTest : public ::testing::Test {};

	TEST_F(TokenizerConsumeCommentsTest, TestEmpty) {
		const Unicode::UString string;
		Tokenizer tokenizer(string);
		ASSERT_TRUE(tokenizer.ConsumeComments());
		ASSERT_TRUE(tokenizer.string.Empty());
	}

	TEST_F(TokenizerConsumeCommentsTest, TestNoCommentCharacters) {
		const Unicode::UString string("No comment characters are in here");
		const std::size_t length = string.length();
		Tokenizer tokenizer(string);
		ASSERT_TRUE(tokenizer.ConsumeComments());
		ASSERT_EQ(tokenizer.string.length(), length);
	}

	TEST_F(TokenizerConsumeCommentsTest, TestNoComments) {
		const Unicode::UString string("/N*o comments are in her*e/");
		const std::size_t length = string.length();
		Tokenizer tokenizer(string);
		ASSERT_TRUE(tokenizer.ConsumeComments());
		ASSERT_EQ(tokenizer.string.length(), length);
	}

	TEST_F(TokenizerConsumeCommentsTest, TestNoCommentsSlashStarSlash) {
		const Unicode::UString string("/*/");
		Tokenizer tokenizer(string);
		ASSERT_TRUE(tokenizer.ConsumeComments());
		ASSERT_EQ(tokenizer.string.length(), 3);
	}

	TEST_F(TokenizerConsumeCommentsTest, TestEmptyComment) {
		const Unicode::UString string("/**/");
		Tokenizer tokenizer(string);
		ASSERT_TRUE(tokenizer.ConsumeComments());
		for (auto c : tokenizer.string) {
			std::cout << static_cast<char>(c) << " (" << c << ")\n";
		}
		ASSERT_EQ(tokenizer.string.length(), 0);
	}

} // namespace CSS

int
main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
