/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <climits>
#include <cstdlib>

#include <gtest/gtest.h>

#include "data/text/ustring.hpp"
#include "parser/css/tokenizer_stream.hpp"

namespace CSS {

	class TokenizerStreamTest : public ::testing::Test {
	  public:
		char destination = 'A';
	};

	TEST_F(TokenizerStreamTest, TestNextEmpty) {
		Unicode::UString test;
		CSS::TokenizerStream stream(test);

		ASSERT_FALSE(stream.Next(nullptr));
		ASSERT_FALSE(stream.Next(&destination));
	}

	TEST_F(TokenizerStreamTest, TestPeekEmpty) {
		Unicode::UString test;
		CSS::TokenizerStream stream(test);

		ASSERT_FALSE(stream.Peek(nullptr));
		ASSERT_FALSE(stream.Peek(&destination));

		ASSERT_FALSE(stream.Peek(nullptr, 6));
		ASSERT_FALSE(stream.Peek(&destination, 6));

		ASSERT_FALSE(stream.Peek(nullptr, std::rand()));
		ASSERT_FALSE(stream.Peek(&destination, std::rand()));
	}

} // namespace CSS

int
main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
