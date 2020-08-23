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
		Unicode::CodePoint destination = 'A';
	};

	TEST_F(TokenizerStreamTest, TestCodePointsLeft) {
		Unicode::UString emptyString;
		const CSS::TokenizerStream emptyStream(&emptyString);
		EXPECT_EQ(emptyStream.CodePointsLeft(), 0);

		const auto count = std::rand() % 0x100;
		std::vector<Unicode::CodePoint> vec(count, Unicode::LATIN_CAPITAL_LETTER_A);
		Unicode::UString randomSizeString = Unicode::UString(vec);
		const CSS::TokenizerStream randomSizeStream(&randomSizeString);
		EXPECT_EQ(randomSizeStream.CodePointsLeft(), count);
	}

	TEST_F(TokenizerStreamTest, TestNextEmpty) {
		Unicode::UString test;
		CSS::TokenizerStream stream(&test);

		ASSERT_FALSE(stream.Next(nullptr));
		ASSERT_FALSE(stream.Next(&destination));
	}

	TEST_F(TokenizerStreamTest, TestNextSemantics) {
		Unicode::UString test("ABC");
		CSS::TokenizerStream stream(&test);

		ASSERT_TRUE(stream.Next(&destination));
		ASSERT_EQ(destination, test[0]);
		ASSERT_TRUE(stream.Next(&destination));
		ASSERT_EQ(destination, test[1]);
		ASSERT_TRUE(stream.Next(&destination));
		ASSERT_EQ(destination, test[2]);
		ASSERT_FALSE(stream.Next(&destination));
		ASSERT_EQ(destination, test[2]);
	}

	TEST_F(TokenizerStreamTest, TestPeekSame) {
		Unicode::UString test("   ");
		std::generate(std::begin(test), std::end(test), []() { return 'A' + (random() % 26); });
		CSS::TokenizerStream stream(&test);

		Unicode::CodePoint peek = '\0';
		Unicode::CodePoint next = '\0';
		for (std::size_t i = 0; i < test.length(); i++) {
			ASSERT_TRUE(stream.Peek(&peek));
			ASSERT_TRUE(stream.Next(&next));
			ASSERT_EQ(peek, next);
		}

		ASSERT_FALSE(stream.Peek(&peek));
		ASSERT_FALSE(stream.Next(&next));
		ASSERT_EQ(peek, next);
	}

	TEST_F(TokenizerStreamTest, TestPeekEmpty) {
		Unicode::UString test;
		CSS::TokenizerStream stream(&test);

		ASSERT_FALSE(stream.Peek(nullptr));
		ASSERT_FALSE(stream.Peek(&destination));

		ASSERT_FALSE(stream.Peek(nullptr, 6));
		ASSERT_FALSE(stream.Peek(&destination, 6));

		ASSERT_FALSE(stream.Peek(nullptr, std::rand()));
		ASSERT_FALSE(stream.Peek(&destination, std::rand()));
	}

	TEST_F(TokenizerStreamTest, TestPeekFirstSemantics) {
		Unicode::UString test("ABC");
		CSS::TokenizerStream stream(&test);

		Unicode::CodePoint local = 'Q';
		destination = 'O';
		ASSERT_TRUE(stream.Peek(&local));
		ASSERT_TRUE(stream.Peek(&destination, 0));
		ASSERT_EQ(local, destination);
	}

	TEST_F(TokenizerStreamTest, TestPeek) {
		Unicode::UString test("ABC");
		CSS::TokenizerStream stream(&test);

		ASSERT_TRUE(stream.Peek(&destination, 0));
		ASSERT_EQ(destination, 'A');
		ASSERT_TRUE(stream.Peek(&destination, 1));
		ASSERT_EQ(destination, 'B');
		ASSERT_TRUE(stream.Peek(&destination, 2));
		ASSERT_EQ(destination, 'C');
		ASSERT_FALSE(stream.Peek(&destination, 3));
	}

	TEST_F(TokenizerStreamTest, TestPeekEndSemantics) {
		Unicode::UString test("G");
		CSS::TokenizerStream stream(&test);

		ASSERT_TRUE(stream.Peek(&destination, 0));
		ASSERT_EQ(destination, 'G');
		ASSERT_FALSE(stream.Peek(&destination, 5));
		ASSERT_EQ(destination, 'G');
	}

	TEST_F(TokenizerStreamTest, TestSkip) {
		Unicode::UString test {'G'};
		CSS::TokenizerStream stream(&test);

		ASSERT_TRUE(stream.Skip());
		ASSERT_FALSE(stream.Skip());
		ASSERT_FALSE(stream.Skip());
		ASSERT_FALSE(stream.Skip());
	}

} // namespace CSS

int
main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
