/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <climits>

#include <gtest/gtest.h>

#include "data/text/ustring.hpp"
#include "parser/css/preprocessor.hpp"

namespace CSS {

	class PreprocessorTest : public ::testing::Test {
	public:
		Preprocessor instance;
	};

	TEST_F(PreprocessorTest, RunSimple) {
		Unicode::UString string {};
		ASSERT_TRUE(instance.Run(string));
	}

} // namespace CSS

int
main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
