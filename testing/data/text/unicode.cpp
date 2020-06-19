/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "data/text/unicode.hpp"

#include <gtest/gtest.h>

namespace Unicode {
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
} // namespace Unicode

int
main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
