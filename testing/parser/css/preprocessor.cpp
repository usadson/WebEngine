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

	TEST_F(PreprocessorTest, RunNoActions) {
		Unicode::UString string("Test", 4);
		ASSERT_TRUE(instance.Run(string));
		ASSERT_EQ(string.length(), 4);
		ASSERT_EQ(string[0], 'T');
		ASSERT_EQ(string[1], 'e');
		ASSERT_EQ(string[2], 's');
		ASSERT_EQ(string[3], 't');
	}

	TEST_F(PreprocessorTest, RunCR) {
		Unicode::UString string("T\re\rs\rt", 7);
		ASSERT_TRUE(instance.Run(string));
		ASSERT_EQ(string.length(), 7);
		ASSERT_EQ(string[0], 'T');
		ASSERT_EQ(string[1], '\n');
		ASSERT_EQ(string[2], 'e');
		ASSERT_EQ(string[3], '\n');
		ASSERT_EQ(string[4], 's');
		ASSERT_EQ(string[5], '\n');
		ASSERT_EQ(string[6], 't');
	}

	TEST_F(PreprocessorTest, RunCRLF) {
		Unicode::UString string("Te\r\nst", 6);
		ASSERT_TRUE(instance.Run(string));
		ASSERT_EQ(string.length(), 5);
		ASSERT_EQ(string[0], 'T');
		ASSERT_EQ(string[1], 'e');
		ASSERT_EQ(string[2], '\n');
		ASSERT_EQ(string[3], 's');
		ASSERT_EQ(string[4], 't');
	}

	TEST_F(PreprocessorTest, RunNullCharacter) {
		Unicode::UString string;
		string += 'T';
		string += 'e';
		string += Unicode::NULL_CHARACTER;
		string += 's';
		string += 't';
		ASSERT_TRUE(instance.Run(string));
		ASSERT_EQ(string.length(), 5);
		ASSERT_EQ(string[0], 'T');
		ASSERT_EQ(string[1], 'e');
		ASSERT_EQ(string[2], Unicode::REPLACEMENT_CHARACTER);
		ASSERT_EQ(string[3], 's');
		ASSERT_EQ(string[4], 't');
	}

	TEST_F(PreprocessorTest, RunNullCharacterEnd) {
		Unicode::UString string;
		string += 'T';
		string += 'e';
		string += 's';
		string += 't';
		string += Unicode::NULL_CHARACTER;
		ASSERT_TRUE(instance.Run(string));
		ASSERT_EQ(string.length(), 5);
		ASSERT_EQ(string[0], 'T');
		ASSERT_EQ(string[1], 'e');
		ASSERT_EQ(string[2], 's');
		ASSERT_EQ(string[3], 't');
		ASSERT_EQ(string[4], Unicode::REPLACEMENT_CHARACTER);
	}

} // namespace CSS

int
main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
