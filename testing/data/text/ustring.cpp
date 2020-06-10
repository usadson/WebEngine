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
		ASSERT_EQ(normalString.length(), 11);
	}

} // namespace TextEncoding

int
main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

