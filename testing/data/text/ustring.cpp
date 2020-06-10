/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "data/text/ustring.hpp"

#include <climits>

#include <gtest/gtest.h>

namespace Unicode {

	class UStringTest : public ::testing::Test {
	protected:
		UString normalString { "Hello world" };
	};

	TEST_F(UStringTest, ValidInputs) {
		ASSERT_EQ(normalString.length(), 11)
			<< "Invalid size detected";

		ASSERT_TRUE(normalString.EqualsIgnoreCaseA(0, "HELLO WORLD"));
		ASSERT_TRUE(normalString.EqualsIgnoreCaseA(0, "hello world"));
		ASSERT_TRUE(normalString.EqualsIgnoreCaseA(0, "hElLo wOrLd"));
		ASSERT_TRUE(normalString.EqualsIgnoreCaseA(0, "hELLo wORLd"));

		ASSERT_TRUE(normalString.EqualsIgnoreCaseA(6, "WORLD"));
		ASSERT_TRUE(normalString.EqualsIgnoreCaseA(6, "world"));
		ASSERT_TRUE(normalString.EqualsIgnoreCaseA(6, "World"));
		ASSERT_TRUE(normalString.EqualsIgnoreCaseA(6, "wOrLD"));

		ASSERT_FALSE(normalString.EqualsIgnoreCaseA(0, "another string with another length"));
		ASSERT_FALSE(normalString.EqualsIgnoreCaseA(0, "sameLength"));
		ASSERT_FALSE(normalString.EqualsIgnoreCaseA(0, ""));

		ASSERT_TRUE(normalString.EqualsA("Hello world"));
		ASSERT_TRUE(normalString.EqualsAL(0, "Hello world", 11));

		ASSERT_FALSE(normalString.EqualsA("Helo world"));
		ASSERT_FALSE(normalString.EqualsA(""));

		ASSERT_FALSE(normalString.EqualsAL(0, "nope", 4));
		ASSERT_FALSE(normalString.EqualsAL(0, "", 0));
	}

} // namespace TextEncoding

int
main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

