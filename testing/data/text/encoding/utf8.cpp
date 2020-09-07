/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "data/text/encoding/utf8.hpp"

#include <climits>

#include <gtest/gtest.h>

#include "logger.hpp"

namespace TextEncoding {

	class UTF8Test : public ::testing::Test {
	protected:
		std::array<std::string, 6> inputs = {{
			"OK",
			"Hello, World!",
			"😃 😭 😈",
			"بعض الأمثلة على النص العربي!",
			"W̸͓͇̝̱̤͆͂̑͑͌̅̕ͅͅḕ̴̯̯͕̼͉̌̄̉̄̒̈́i̷̧̻͍͚̥̦̍̅͝r̴̗̠̹̮͚̦̳̣̄̓̎̎̉̇̀̃̓͜d̵̛̰̗͍̏͊̊̏̾͘ ̷̣̪̊̃͐̾̔̽͊̑̚͘Ṭ̷̢̮̉̊̆̅̇ͅẹ̶̭̯͎̰̺̹̝̗̌͑̾̇̄̀̈ͅx̵̧̫̊̀̒̐̎̚̕t̷̢͌̅̆͝",
			"(¯`·.¸¸.·´¯`·.¸¸.-> ＷⒺ𝓲𝓻∂ ţ𝕖᙭т <-.¸¸.·´¯`·.¸¸.·´¯)",
		}};
		std::array<std::vector<Unicode::CodePoint>, 6> expectedOutputs = {{
			{0x4f, 0x4b},
			{0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x2c, 0x20, 0x57, 0x6f, 0x72, 0x6c, 0x64, 0x21},
			{0x1f603, 0x20, 0x1f62d, 0x20, 0x1f608},
			{0x628, 0x639, 0x636, 0x20, 0x627, 0x644, 0x623, 0x645, 0x62b, 0x644, 0x629, 0x20, 0x639, 0x644, 0x649,
				0x20, 0x627, 0x644, 0x646, 0x635, 0x20, 0x627, 0x644, 0x639, 0x631, 0x628, 0x64a, 0x21},
			{0x57, 0x338, 0x346, 0x342, 0x311, 0x351, 0x34c, 0x315, 0x305, 0x353, 0x347, 0x31d, 0x345, 0x345, 0x331,
				0x324, 0x65, 0x334, 0x304, 0x340, 0x30c, 0x304, 0x309, 0x304, 0x312, 0x344, 0x32f, 0x32f, 0x355, 0x33c,
				0x349, 0x69, 0x337, 0x35d, 0x30d, 0x305, 0x33b, 0x34d, 0x35a, 0x325, 0x327, 0x326, 0x72, 0x334, 0x304,
				0x313, 0x30e, 0x30e, 0x309, 0x307, 0x340, 0x303, 0x343, 0x35c, 0x317, 0x320, 0x339, 0x32e, 0x35a, 0x326,
				0x333, 0x323, 0x64, 0x335, 0x30f, 0x31b, 0x34a, 0x30a, 0x30f, 0x33e, 0x358, 0x330, 0x317, 0x34d, 0x20,
				0x337, 0x31a, 0x358, 0x30a, 0x303, 0x350, 0x33e, 0x314, 0x33d, 0x34a, 0x311, 0x323, 0x32a, 0x54, 0x337,
				0x309, 0x30a, 0x306, 0x305, 0x307, 0x323, 0x345, 0x322, 0x32e, 0x65, 0x336, 0x30c, 0x351, 0x33e, 0x307,
				0x304, 0x300, 0x308, 0x323, 0x32d, 0x32f, 0x34e, 0x330, 0x33a, 0x339, 0x345, 0x31d, 0x317, 0x78, 0x335,
				0x30a, 0x31a, 0x340, 0x312, 0x315, 0x310, 0x30e, 0x32b, 0x327, 0x74, 0x337, 0x34c, 0x35d, 0x305, 0x306,
				0x322},
			{0x28, 0xaf, 0x60, 0xb7, 0x2e, 0xb8, 0xb8, 0x2e, 0xb7, 0xb4, 0xaf, 0x60, 0xb7, 0x2e, 0xb8, 0xb8, 0x2e, 0x2d,
				0x3e, 0x20, 0xff37, 0x24ba, 0x1d4f2, 0x1d4fb, 0x2202, 0x20, 0x163, 0x1d556, 0x166d, 0x442, 0x20, 0x3c,
				0x2d, 0x2e, 0xb8, 0xb8, 0x2e, 0xb7, 0xb4, 0xaf, 0x60, 0xb7, 0x2e, 0xb8, 0xb8, 0x2e, 0xb7, 0xb4, 0xaf,
				0x29},
		}};
		std::array<UTF8, 6> encoders;
	};

	TEST_F(UTF8Test, ValidInputs) {
		ASSERT_EQ(inputs.size(), encoders.size());
		ASSERT_EQ(inputs.size(), expectedOutputs.size());
	}

	TEST_F(UTF8Test, NormalDecoding) {
		for (std::size_t i = 0; i < inputs.size(); i++) {
			ASSERT_TRUE(encoders[i].Decode(inputs[i].c_str(), inputs[i].length()))
				<< "Decode() failed on string: \"" << inputs[i] << '"';

			ASSERT_EQ(encoders[i].Output.size(), expectedOutputs[i].size())
				<< "Incorrect output size for string: \"" << inputs[i] << '"';

			for (std::size_t j = 0; j < encoders[i].Output.size(); j++) {
				ASSERT_EQ(encoders[i].Output[j], expectedOutputs[i][j])
					<< "Incorrect decoding of string: \"" << inputs[i] << "\", the " << j
					<< "th character is incorrect.";
			}
		}
	}

	class UTF8BoundaryTests : public ::testing::Test {
	protected:
		std::array<std::array<std::vector<uint8_t>, 6>, 3> boundaryInputs = {{
			{{{0x00}, {0xc2, 0x80}, {0xe0, 0xa0, 0x80}, {0xf0, 0x90, 0x80, 0x80},
				{0xef, 0xbf, 0xbd, 0xef, 0xbf, 0xbd, 0xef, 0xbf, 0xbd, 0xef, 0xbf, 0xbd, 0xef, 0xbf, 0xbd},
				{0xef, 0xbf, 0xbd, 0xef, 0xbf, 0xbd, 0xef, 0xbf, 0xbd, 0xef, 0xbf, 0xbd, 0xef, 0xbf, 0xbd, 0xef, 0xbf,
					0xbd}}},
			{{
				{0x7f},
				{0xdf, 0xbf},
				{0xef, 0xbf, 0xbf},
				{0xef, 0xbf, 0xbd, 0xef, 0xbf, 0xbd, 0xef, 0xbf, 0xbd, 0xef, 0xbf, 0xbd},
				{0xef, 0xbf, 0xbd, 0xef, 0xbf, 0xbd, 0xef, 0xbf, 0xbd, 0xef, 0xbf, 0xbd, 0xef, 0xbf, 0xbd},
				{0xef, 0xbf, 0xbd, 0xef, 0xbf, 0xbd, 0xef, 0xbf, 0xbd, 0xef, 0xbf, 0xbd, 0xef, 0xbf, 0xbd, 0xef, 0xbf,
					0xbd},
			}},
			{{
				{0xed, 0x9f, 0xbf},
				{0xee, 0x80, 0x80},
				{0xef, 0xbf, 0xbd},
				{0xf4, 0x8f, 0xbf, 0xbf},
			}},
		}};

		std::array<std::array<UTF8, 6>, 3> encoders;

		// U+110000 is outside of the Unicode Standard
		std::vector<uint8_t> outsideBounds = {
			0xf4,
			0x90,
			0x80,
			0x80,
		};
		UTF8 encoderOutsideBounds;
	};

	TEST_F(UTF8BoundaryTests, BoundaryTests) {
		for (std::size_t i = 0; i < boundaryInputs.size(); i++) {
			for (std::size_t j = 0; j < boundaryInputs[i].size(); j++) {
				ASSERT_TRUE(encoders[i][j].Decode(
					reinterpret_cast<const char *>(boundaryInputs[i][j].data()), boundaryInputs[i][j].size()))
					<< "Boundary Test failed for Test " << i << '.' << j;
			}
		}
	}

	TEST_F(UTF8BoundaryTests, OutsideBoundaries) {
		ASSERT_FALSE(
			encoderOutsideBounds.Decode(reinterpret_cast<const char *>(outsideBounds.data()), outsideBounds.size()))
			<< "U+110000 is an invalid character, but the decoder didn't return an error";
	}

	class UTF8FuzzTest : public ::testing::Test {
	protected:
		UTF8 encoder;
		std::array<char, 4> data;
	};

	TEST_F(UTF8FuzzTest, ASCIITest) {
		for (unsigned char i = 0; i < 128; i++) {
			ASSERT_TRUE(encoder.Decode(reinterpret_cast<const char *>(&i), 1))
				<< "ASCII character decode failed: character: 0x" << std::hex << (static_cast<uint16_t>(i) & 0x00ff)
				<< std::dec;
		}
	}

	TEST_F(UTF8FuzzTest, TwoOctetsTest) {
		for (unsigned char i = 0xC2; i <= 0xDF; i++) {
			data[0] = static_cast<char>(i);
			for (std::size_t j = 0; j <= 255; j++) {
				data[1] = static_cast<char>(j);

				if (j >= 0x80 && j <= 0xBF) {
					ASSERT_TRUE(encoder.Decode(data.data(), 2))
						<< "Character is inside the bounds, but failed: 0x" << std::hex << static_cast<uint16_t>(i)
						<< static_cast<uint16_t>(j) << std::dec;
				} else {
					ASSERT_FALSE(encoder.Decode(data.data(), 2))
						<< "Character is outside the bounds, but passed: 0x" << std::hex << static_cast<uint16_t>(i)
						<< static_cast<uint16_t>(j) << std::dec;
				}
			}
		}
	}

} // namespace TextEncoding

int
main(int argc, char **argv) {
	Logger::SetOutputState(false);
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
