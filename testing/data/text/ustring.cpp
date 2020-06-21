/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "data/text/ustring.hpp"
#include "data/text/unicode.hpp"

#include <climits>

#include <gtest/gtest.h>

namespace Unicode {

	class UStringTest : public ::testing::Test {
	protected:
		UString normalString { "Hello world" };
	};

	TEST_F(UStringTest, LengthAssertion) {
		ASSERT_EQ(normalString.length(), 11) << "Invalid size detected";
	}

	TEST_F(UStringTest, OperatorEquals) {
		ASSERT_EQ(normalString, Unicode::UString("Hello world"));
		ASSERT_TRUE(normalString == Unicode::UString("Hello world"));

		ASSERT_FALSE(normalString == Unicode::UString("Hella world"));
		ASSERT_FALSE(normalString == Unicode::UString("something else"));
		ASSERT_FALSE(normalString == Unicode::UString(""));

		ASSERT_TRUE(Unicode::UString() == Unicode::UString());
		ASSERT_TRUE(Unicode::UString() == Unicode::UString(""));
		ASSERT_TRUE(Unicode::UString("") == Unicode::UString(""));
		ASSERT_TRUE(Unicode::UString("Hello") == Unicode::UString("Hello"));
	}

	TEST_F(UStringTest, OperatorNotEquals) {
		ASSERT_TRUE(normalString != Unicode::UString());
		ASSERT_TRUE(normalString != Unicode::UString(""));
		ASSERT_TRUE(normalString != Unicode::UString("Hello worl"));
		ASSERT_TRUE(normalString != Unicode::UString("ello world"));

		ASSERT_TRUE(Unicode::UString() != Unicode::UString("a string with contents"));
		ASSERT_TRUE(Unicode::UString("") != Unicode::UString("a string with contents"));
		ASSERT_TRUE(Unicode::UString("equal length") != Unicode::UString("other string"));
	}

	TEST_F(UStringTest, OperatorPlus) {
		auto str = normalString + Unicode::UString("!");
		ASSERT_EQ(str, Unicode::UString("Hello world!"));
		str = normalString + Unicode::UString("!!");
		ASSERT_EQ(str, Unicode::UString("Hello world!!"));

		ASSERT_EQ(Unicode::UString(), Unicode::UString() + Unicode::UString());
		ASSERT_EQ(Unicode::UString("a"), Unicode::UString("a") + Unicode::UString("") + Unicode::UString(""));
		ASSERT_EQ(Unicode::UString("ab"), Unicode::UString("a") + Unicode::UString("b") + Unicode::UString(""));
		ASSERT_EQ(Unicode::UString("abc"), Unicode::UString("a") + Unicode::UString("b") + Unicode::UString("c"));
	}

	TEST_F(UStringTest, OperatorPlusEquals) {
		Unicode::UString str;
		str += Unicode::UString();
		ASSERT_EQ(str, Unicode::UString());

		str += Unicode::LINE_FEED;
		ASSERT_EQ(str, Unicode::UString("\n"));

		str += Unicode::UString();
		ASSERT_EQ(str, Unicode::UString("\n"));

		str += Unicode::UString("text");
		ASSERT_EQ(str, Unicode::UString("\ntext"));

		str += "\nanother line";
		ASSERT_EQ(str, Unicode::UString("\ntext\nanother line"));

		str += "";
		ASSERT_EQ(str, Unicode::UString("\ntext\nanother line"));
	}

	TEST_F(UStringTest, CopyTo) {
		Unicode::UString tmp;
		normalString.CopyTo(tmp);
		ASSERT_EQ(normalString, tmp);
	}

	TEST_F(UStringTest, EqualsIgnoreCaseA) {
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
	}

	TEST_F(UStringTest, EqualsIgnoreCase) {
		ASSERT_FALSE(normalString.EqualsIgnoreCase(Unicode::UString("")));
		ASSERT_FALSE(normalString.EqualsIgnoreCase(Unicode::UString("another string with another length")));
		ASSERT_FALSE(normalString.EqualsIgnoreCase(Unicode::UString("sameLength")));

		ASSERT_TRUE(normalString.EqualsIgnoreCase(Unicode::UString("Hello world")));
		ASSERT_TRUE(normalString.EqualsIgnoreCase(Unicode::UString("hello world")));
		ASSERT_TRUE(normalString.EqualsIgnoreCase(Unicode::UString("HELLO WORLD")));
		ASSERT_TRUE(normalString.EqualsIgnoreCase(Unicode::UString("hElLo WoRlD")));
	}

	TEST_F(UStringTest, EqualsA) {
		ASSERT_TRUE(normalString.EqualsA("Hello world"));
		ASSERT_FALSE(normalString.EqualsA("Helo world"));
		ASSERT_FALSE(normalString.EqualsA(""));
	}

	TEST_F(UStringTest, EqualsAL) {
		ASSERT_TRUE(normalString.EqualsAL(0, "Hello world", 11));
		ASSERT_FALSE(normalString.EqualsAL(0, "nope", 4));
		ASSERT_FALSE(normalString.EqualsAL(0, "", 0));
	}

	TEST_F(UStringTest, StartsWithA) {
		ASSERT_TRUE(normalString.StartsWithA(""));
		ASSERT_TRUE(normalString.StartsWithA("Hello"));
		ASSERT_TRUE(normalString.StartsWithA("Hello world"));

		ASSERT_FALSE(normalString.StartsWithA("Hello warld"));
		ASSERT_FALSE(normalString.StartsWithA("Helo"));
		ASSERT_FALSE(normalString.StartsWithA("something else"));
	}

	TEST_F(UStringTest, StartsWithIgnoreCaseAL) {
		ASSERT_TRUE(normalString.StartsWithIgnoreCaseAL(0, "", 0));
		ASSERT_TRUE(normalString.StartsWithIgnoreCaseAL(0, "hello world", 11));
		ASSERT_TRUE(normalString.StartsWithIgnoreCaseAL(0, "hello world", 5));
		ASSERT_TRUE(normalString.StartsWithIgnoreCaseAL(0, "HELLO", 5));
		ASSERT_TRUE(normalString.StartsWithIgnoreCaseAL(0, "hElLO wORl garbage text", 10));

		ASSERT_TRUE(normalString.StartsWithIgnoreCaseAL(6, "wOrlD", 5));
		ASSERT_TRUE(normalString.StartsWithIgnoreCaseAL(3, "lo wOrlD", 8));

		ASSERT_FALSE(normalString.StartsWithIgnoreCaseAL(0, "HEllA WORLD", 11));
		ASSERT_FALSE(normalString.StartsWithIgnoreCaseAL(0, "something else", 14));
		ASSERT_FALSE(normalString.StartsWithIgnoreCaseAL(3, "something else", 11));
	}

} // namespace Unicode

int
main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
