/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <gtest/gtest.h>

#include "data/text/unicode.hpp"
#include "data/text/ustring.hpp"
#include "parser/css/tokenizer_algorithms.hpp"
#include "parser/css/tokenizer_stream.hpp"

namespace CSS {

	class TokenizerAlgorithmsTest : public ::testing::Test {
	public:
		Unicode::CodePoint destination = 'A';
	};

	TEST_F(TokenizerAlgorithmsTest, IsWhitespace) {
		EXPECT_TRUE(IsWhitespace(Unicode::CHARACTER_TABULATION));
		EXPECT_TRUE(IsWhitespace(Unicode::LINE_FEED));
		EXPECT_TRUE(IsWhitespace(Unicode::SPACE));
		EXPECT_FALSE(IsWhitespace(Unicode::CARRIAGE_RETURN));
		EXPECT_FALSE(IsWhitespace(Unicode::FORM_FEED));
		EXPECT_FALSE(IsWhitespace(Unicode::NULL_CHARACTER));
	}

	TEST_F(TokenizerAlgorithmsTest, IsHexCharacterBounds) {
		EXPECT_TRUE(IsHexCharacter('a'));
		EXPECT_TRUE(IsHexCharacter('f'));
		EXPECT_TRUE(IsHexCharacter('A'));
		EXPECT_TRUE(IsHexCharacter('F'));
		EXPECT_TRUE(IsHexCharacter('0'));
		EXPECT_TRUE(IsHexCharacter('9'));

		EXPECT_FALSE(IsHexCharacter('a' - 1));
		EXPECT_FALSE(IsHexCharacter('g'));
		EXPECT_FALSE(IsHexCharacter('A' - 1));
		EXPECT_FALSE(IsHexCharacter('G'));
		EXPECT_FALSE(IsHexCharacter('0' - 1));
		EXPECT_FALSE(IsHexCharacter('9' + 1));
	}

	TEST_F(TokenizerAlgorithmsTest, IsHexCharacterValid) {
		for (Unicode::CodePoint codePoint = 'a'; codePoint < 'g'; codePoint++) {
			EXPECT_TRUE(IsHexCharacter(codePoint));
		}
		for (Unicode::CodePoint codePoint = 'A'; codePoint < 'G'; codePoint++) {
			EXPECT_TRUE(IsHexCharacter(codePoint));
		}
		for (Unicode::CodePoint codePoint = '0'; codePoint <= '9'; codePoint++) {
			EXPECT_TRUE(IsHexCharacter(codePoint));
		}
	}

	TEST_F(TokenizerAlgorithmsTest, IsHexCharacterInvalidASCII) {
		for (Unicode::CodePoint codePoint = 0; codePoint < '0'; codePoint++) {
			EXPECT_FALSE(IsHexCharacter(codePoint));
		}
		for (Unicode::CodePoint codePoint = '9' + 1; codePoint < 'A'; codePoint++) {
			EXPECT_FALSE(IsHexCharacter(codePoint));
		}
		for (Unicode::CodePoint codePoint = 'Z' + 1; codePoint < 'a'; codePoint++) {
			EXPECT_FALSE(IsHexCharacter(codePoint));
		}
		for (Unicode::CodePoint codePoint = 'z' + 1; codePoint < 255; codePoint++) {
			EXPECT_FALSE(IsHexCharacter(codePoint));
		}
	}

	TEST_F(TokenizerAlgorithmsTest, IsHexCharacterInvalidNonASCII) {
		for (std::size_t i = 0; i < 15; i++) {
			EXPECT_FALSE(IsHexCharacter(0xFF + std::rand()));
		}
	}

	TEST_F(TokenizerAlgorithmsTest, IsNonASCIICodePoint) {
		for (std::size_t i = 0; i < 15; i++) {
			EXPECT_FALSE(IsNonASCIICodePoint(std::rand() % 0x80));
		}
		for (std::size_t i = 0; i < 15; i++) {
			EXPECT_TRUE(IsNonASCIICodePoint(0x80 + std::rand()));
		}
	}

	TEST_F(TokenizerAlgorithmsTest, IsNameStartCodePoint) {
		for (Unicode::CodePoint codePoint = 0; codePoint < 'A'; codePoint++) {
			EXPECT_FALSE(IsNameStartCodePoint(codePoint));
		}
		for (Unicode::CodePoint codePoint = 'A'; codePoint <= 'Z'; codePoint++) {
			EXPECT_TRUE(IsNameStartCodePoint(codePoint));
		}
		for (Unicode::CodePoint codePoint = 'Z' + 1; codePoint < '_'; codePoint++) {
			EXPECT_FALSE(IsNameStartCodePoint(codePoint));
		}
		EXPECT_TRUE(IsNameStartCodePoint('_'));
		for (Unicode::CodePoint codePoint = '_' + 1; codePoint < 'a'; codePoint++) {
			EXPECT_FALSE(IsNameStartCodePoint(codePoint));
		}
		for (Unicode::CodePoint codePoint = 'a'; codePoint <= 'z'; codePoint++) {
			EXPECT_TRUE(IsNameStartCodePoint(codePoint));
		}
		for (std::size_t i = 0; i < 15; i++) {
			EXPECT_TRUE(IsNameStartCodePoint('z' + 1 + std::rand()));
		}
	}

	TEST_F(TokenizerAlgorithmsTest, IsNameCodePoint) {
		for (Unicode::CodePoint codePoint = 0; codePoint < '-'; codePoint++) {
			EXPECT_FALSE(IsNameCodePoint(codePoint));
		}
		EXPECT_TRUE(IsNameCodePoint('-'));
		for (Unicode::CodePoint codePoint = '-' + 1; codePoint < '0'; codePoint++) {
			EXPECT_FALSE(IsNameCodePoint(codePoint));
		}
		for (Unicode::CodePoint codePoint = '0'; codePoint <= '9'; codePoint++) {
			EXPECT_TRUE(IsNameCodePoint(codePoint));
		}
		for (Unicode::CodePoint codePoint = '9' + 1; codePoint < 'A'; codePoint++) {
			EXPECT_FALSE(IsNameCodePoint(codePoint));
		}
		for (Unicode::CodePoint codePoint = 'A'; codePoint <= 'Z'; codePoint++) {
			EXPECT_TRUE(IsNameCodePoint(codePoint));
		}
		for (Unicode::CodePoint codePoint = 'Z' + 1; codePoint < '_'; codePoint++) {
			EXPECT_FALSE(IsNameCodePoint(codePoint));
		}
		EXPECT_TRUE(IsNameCodePoint('_'));
		for (Unicode::CodePoint codePoint = '_' + 1; codePoint < 'a'; codePoint++) {
			EXPECT_FALSE(IsNameCodePoint(codePoint));
		}
		for (Unicode::CodePoint codePoint = 'a'; codePoint <= 'z'; codePoint++) {
			EXPECT_TRUE(IsNameCodePoint(codePoint));
		}
		for (std::size_t i = 0; i < 15; i++) {
			EXPECT_TRUE(IsNameCodePoint('z' + 1 + std::rand()));
		}
	}

	TEST_F(TokenizerAlgorithmsTest, IsValidEscape_Illegal) {
		Unicode::UString string;
		TokenizerStream stream(&string);
		EXPECT_FALSE(IsValidEscape(stream));

		string = {'\\'};
		stream = TokenizerStream(&string);
		EXPECT_FALSE(IsValidEscape(stream));

		string = {'\\', '\n'};
		stream = TokenizerStream(&string);
		EXPECT_FALSE(IsValidEscape(stream));

		string = {std::rand() % Unicode::REVERSE_SOLIDUS, ' '};
		stream = TokenizerStream(&string);
		EXPECT_FALSE(IsValidEscape(stream));

		string = {Unicode::REVERSE_SOLIDUS + 1 + std::rand(), ' '};
		stream = TokenizerStream(&string);
		EXPECT_FALSE(IsValidEscape(stream));
	}

	TEST_F(TokenizerAlgorithmsTest, IsValidEscape_Legal) {
		Unicode::UString string {'\\', '\0'};
		TokenizerStream stream(&string);

		for (std::size_t i = 0; i < 15; i++) {
			string[1] = std::rand() % Unicode::LAST_ALLOWED_CODE_POINT;
			stream.SetString(&string);
			EXPECT_TRUE(IsValidEscape(stream)) << "string=" << std::hex << string[0] << ' ' << string[1] << std::dec;
		}
	}

	TEST_F(TokenizerAlgorithmsTest, WillStartIdentifierHyphenMinus) {
		Unicode::UString string {'-', '-'};
		TokenizerStream stream(&string);
		EXPECT_TRUE(WillStartIdentifier(stream));

		for (std::size_t i = 0; i < 15; i++) {
			string[1] = std::rand();
			if (string[1] == Unicode::HYPHEN_MINUS) {
				continue;
			}
			stream.SetString(&string);
			EXPECT_FALSE(WillStartIdentifier(stream));
		}

		string = {'-', '\\', '\n'};
		stream.SetString(&string);
		EXPECT_FALSE(WillStartIdentifier(stream));

		for (std::size_t i = 0; i < 15; i++) {
			string[2] = std::rand();
			if (string[2] == Unicode::LINE_FEED) {
				continue;
			}
			stream.SetString(&string);
			EXPECT_TRUE(WillStartIdentifier(stream));
		}
	}

	TEST_F(TokenizerAlgorithmsTest, WillStartIdentifierNameStartCodePoint) {
		Unicode::UString string {'0'};
		TokenizerStream stream(&string);
		Unicode::CodePoint &codePoint = string[0];

		for (codePoint = 0; codePoint < 'A'; codePoint++) {
			stream.SetString(&string);
			EXPECT_FALSE(WillStartIdentifier(stream));
		}
		for (codePoint = 'A'; codePoint <= 'Z'; codePoint++) {
			stream.SetString(&string);
			EXPECT_TRUE(WillStartIdentifier(stream));
		}
		for (codePoint = 'Z' + 1; codePoint < '_'; codePoint++) {
			stream.SetString(&string);
			EXPECT_FALSE(WillStartIdentifier(stream));
		}
		string[0] = '_';
		stream.SetString(&string);
		EXPECT_TRUE(WillStartIdentifier(stream));
		for (codePoint = '_' + 1; codePoint < 'a'; codePoint++) {
			stream.SetString(&string);
			EXPECT_FALSE(WillStartIdentifier(stream));
		}
		for (codePoint = 'a'; codePoint <= 'z'; codePoint++) {
			stream.SetString(&string);
			EXPECT_TRUE(WillStartIdentifier(stream));
		}
		for (std::size_t i = 0; i < 15; i++) {
			string[0] = 'z' + 1 + std::rand();
			stream.SetString(&string);
			EXPECT_TRUE(WillStartIdentifier(stream));
		}
	}

	TEST_F(TokenizerAlgorithmsTest, WillStartIdentifierReverseSolidus) {
		Unicode::UString string {'\\', '0'};
		TokenizerStream stream(&string);

		string = {'\\'};
		stream.SetString(&string);
		EXPECT_FALSE(WillStartIdentifier(stream));

		string = {'\\', '\n'};
		stream.SetString(&string);
		EXPECT_FALSE(WillStartIdentifier(stream));

		for (std::size_t i = 0; i < 15; i++) {
			string[1] = std::rand() % Unicode::LAST_ALLOWED_CODE_POINT;
			stream.SetString(&string);
			EXPECT_TRUE(WillStartIdentifier(stream))
				<< "string=" << std::hex << string[0] << ' ' << string[1] << std::dec;
		}
	}

} // namespace CSS

int
main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
