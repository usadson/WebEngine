/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

// This file includes all the tokenizer subfunctions

#include <gtest/gtest.h>

// Make sure to also add them to the ingredients in the Makefile!
#include "tokenizer_consumecomments.cpp"
#include "tokenizer_consumeescapedcodepoint.cpp"

int
main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
