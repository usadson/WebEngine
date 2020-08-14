#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 *
 * Unicode utility class
 */

#include <cstdint>

/*
 * It is good practice to use the characters defined in this namespace instead
 * of using hardcoded hexadecimals.
 */
namespace Unicode {
	typedef uint32_t CodePoint;

	/* We can't use NULL as defined by the "C0 Controls and Basic Latin"
	 * Unicode block, since this is a special keyword. */
	constexpr CodePoint NULL_CHARACTER = 0x0000;
	constexpr CodePoint START_OF_HEADING = 0x0001;
	constexpr CodePoint START_OF_TEXT = 0x0002;
	constexpr CodePoint END_OF_TEXT = 0x0003;
	constexpr CodePoint END_OF_TRANSMISSION = 0x0004;
	constexpr CodePoint ENQUIRY = 0x0005;
	constexpr CodePoint ACKNOWLEDGE = 0x0006;
	constexpr CodePoint BELL = 0x0007;
	constexpr CodePoint BACKSPACE = 0x0008;
	constexpr CodePoint CHARACTER_TABULATION = 0x0009;
	constexpr CodePoint LINE_FEED = 0x000A;
	constexpr CodePoint LINE_TABULATION = 0x000B;
	constexpr CodePoint FORM_FEED = 0x000C;
	constexpr CodePoint CARRIAGE_RETURN = 0x000D;
	// 	--- //

	// ASCII punctuation and symbols
	constexpr CodePoint SPACE = 0x0020;						// ' '
	constexpr CodePoint EXCLAMATION_MARK = 0x0021;			// '!'
	constexpr CodePoint QUOTATION_MARK = 0x0022;			// '"'
	constexpr CodePoint NUMBER_SIGN = 0x0023;				// '#'
	constexpr CodePoint DOLLAR_SIGN = 0x0024;				// '$'
	constexpr CodePoint PERCENT_SIGN = 0x0025;				// '%'
	constexpr CodePoint AMPERSAND = 0x0026;					// '&'
	constexpr CodePoint APOSTROPHE = 0x0027;				// '''
	constexpr CodePoint LEFT_PARENTHESIS = 0x0028;			// '('
	constexpr CodePoint RIGHT_PARENTHESIS = 0x0029;			// ')'
	constexpr CodePoint ASTERISK = 0x002A;					// '*'
	constexpr CodePoint PLUS_SIGN = 0x002B;					// '+'
	constexpr CodePoint COMMA = 0x002C;						// ','
	constexpr CodePoint HYPHEN_MINUS = 0x002D;				// '-'
	constexpr CodePoint FULL_STOP = 0x002E;					// '.'
	constexpr CodePoint SOLIDUS = 0x002F;					// '/'

	// ASCII digits
	constexpr CodePoint DIGIT_ZERO = 0x0030;				// '0'
	constexpr CodePoint DIGIT_ONE = 0x0031;					// '1'
	constexpr CodePoint DIGIT_TWO = 0x0032;					// '2'
	constexpr CodePoint DIGIT_THREE = 0x0033;				// '3'
	constexpr CodePoint DIGIT_FOUR = 0x0034;				// '4'
	constexpr CodePoint DIGIT_FIVE = 0x0035;				// '5'
	constexpr CodePoint DIGIT_SIX = 0x0036;					// '6'
	constexpr CodePoint DIGIT_SEVEN = 0x0037;				// '7'
	constexpr CodePoint DIGIT_EIGHT = 0x0038;				// '8'
	constexpr CodePoint DIGIT_NINE = 0x0039;				// '9'

	// ASCII punctuation and symbols (#2)
	constexpr CodePoint COLON = 0x003A;						// ':'
	constexpr CodePoint SEMICOLON = 0x003B;					// ';'
	constexpr CodePoint LESS_THAN_SIGN = 0x003C;			// '<'
	constexpr CodePoint EQUALS_SIGN = 0x003D;				// '='
	constexpr CodePoint GREATER_THAN_SIGN = 0x003E;			// '>'
	constexpr CodePoint QUESTION_MARK = 0x003F;				// '?'
	constexpr CodePoint COMMERCIAL_AT = 0x0040;				// '@'

	// Uppercase Latin alphabet
	constexpr CodePoint LATIN_CAPITAL_LETTER_A = 0x0041;	// 'A'
	constexpr CodePoint LATIN_CAPITAL_LETTER_B = 0x0042;	// 'B'
	constexpr CodePoint LATIN_CAPITAL_LETTER_C = 0x0043;	// 'C'
	constexpr CodePoint LATIN_CAPITAL_LETTER_D = 0x0044;	// 'D'
	constexpr CodePoint LATIN_CAPITAL_LETTER_E = 0x0045;	// 'E'
	constexpr CodePoint LATIN_CAPITAL_LETTER_F = 0x0046;	// 'F'
	constexpr CodePoint LATIN_CAPITAL_LETTER_G = 0x0047;	// 'G'
	constexpr CodePoint LATIN_CAPITAL_LETTER_H = 0x0048;	// 'H'
	constexpr CodePoint LATIN_CAPITAL_LETTER_I = 0x0049;	// 'I'
	constexpr CodePoint LATIN_CAPITAL_LETTER_J = 0x004A;	// 'J'
	constexpr CodePoint LATIN_CAPITAL_LETTER_K = 0x004B;	// 'K'
	constexpr CodePoint LATIN_CAPITAL_LETTER_L = 0x004C;	// 'L'
	constexpr CodePoint LATIN_CAPITAL_LETTER_M = 0x004D;	// 'M'
	constexpr CodePoint LATIN_CAPITAL_LETTER_N = 0x004E;	// 'N'
	constexpr CodePoint LATIN_CAPITAL_LETTER_O = 0x004F;	// 'O'
	constexpr CodePoint LATIN_CAPITAL_LETTER_P = 0x0050;	// 'P'
	constexpr CodePoint LATIN_CAPITAL_LETTER_Q = 0x0051;	// 'Q'
	constexpr CodePoint LATIN_CAPITAL_LETTER_R = 0x0052;	// 'R'
	constexpr CodePoint LATIN_CAPITAL_LETTER_S = 0x0053;	// 'S'
	constexpr CodePoint LATIN_CAPITAL_LETTER_T = 0x0054;	// 'T'
	constexpr CodePoint LATIN_CAPITAL_LETTER_U = 0x0055;	// 'U'
	constexpr CodePoint LATIN_CAPITAL_LETTER_V = 0x0056;	// 'V'
	constexpr CodePoint LATIN_CAPITAL_LETTER_W = 0x0057;	// 'W'
	constexpr CodePoint LATIN_CAPITAL_LETTER_X = 0x0058;	// 'X'
	constexpr CodePoint LATIN_CAPITAL_LETTER_Y = 0x0059;	// 'Y'
	constexpr CodePoint LATIN_CAPITAL_LETTER_Z = 0x005A;	// 'Z'

	// ASCII punctuation and symbols (#3)
	constexpr CodePoint LEFT_SQUARE_BRACKET = 0x005B;		// '['
	constexpr CodePoint REVERSE_SOLIDUS = 0x005C;			// '\'
	constexpr CodePoint RIGHT_SQUARE_BRACKET = 0x005D;		// ']'
	constexpr CodePoint CIRCUMFLEX_ACCENT = 0x005E;			// '^'
	constexpr CodePoint LOW_LINE = 0x005F;					// '_'
	constexpr CodePoint GRAVE_ACCENT = 0x0060;				// '`'

	// Lowercase Latin alphabet
	constexpr CodePoint LATIN_SMALL_LETTER_A = 0x0061;		// 'a'
	constexpr CodePoint LATIN_SMALL_LETTER_B = 0x0062;		// 'b'
	constexpr CodePoint LATIN_SMALL_LETTER_C = 0x0063;		// 'c'
	constexpr CodePoint LATIN_SMALL_LETTER_D = 0x0064;		// 'd'
	constexpr CodePoint LATIN_SMALL_LETTER_E = 0x0065;		// 'e'
	constexpr CodePoint LATIN_SMALL_LETTER_F = 0x0066;		// 'f'
	constexpr CodePoint LATIN_SMALL_LETTER_G = 0x0067;		// 'g'
	constexpr CodePoint LATIN_SMALL_LETTER_H = 0x0068;		// 'h'
	constexpr CodePoint LATIN_SMALL_LETTER_I = 0x0069;		// 'i'
	constexpr CodePoint LATIN_SMALL_LETTER_J = 0x006A;		// 'j'
	constexpr CodePoint LATIN_SMALL_LETTER_K = 0x006B;		// 'k'
	constexpr CodePoint LATIN_SMALL_LETTER_L = 0x006C;		// 'l'
	constexpr CodePoint LATIN_SMALL_LETTER_M = 0x006D;		// 'm'
	constexpr CodePoint LATIN_SMALL_LETTER_N = 0x006E;		// 'n'
	constexpr CodePoint LATIN_SMALL_LETTER_O = 0x006F;		// 'o'
	constexpr CodePoint LATIN_SMALL_LETTER_P = 0x0070;		// 'p'
	constexpr CodePoint LATIN_SMALL_LETTER_Q = 0x0071;		// 'q'
	constexpr CodePoint LATIN_SMALL_LETTER_R = 0x0072;		// 'r'
	constexpr CodePoint LATIN_SMALL_LETTER_S = 0x0073;		// 's'
	constexpr CodePoint LATIN_SMALL_LETTER_T = 0x0074;		// 't'
	constexpr CodePoint LATIN_SMALL_LETTER_U = 0x0075;		// 'u'
	constexpr CodePoint LATIN_SMALL_LETTER_V = 0x0076;		// 'v'
	constexpr CodePoint LATIN_SMALL_LETTER_W = 0x0077;		// 'w'
	constexpr CodePoint LATIN_SMALL_LETTER_X = 0x0078;		// 'x'
	constexpr CodePoint LATIN_SMALL_LETTER_Y = 0x0079;		// 'y'
	constexpr CodePoint LATIN_SMALL_LETTER_Z = 0x007A;		// 'z'

	// ASCII punctuation and symbols (#4)
	constexpr CodePoint LEFT_CURLY_BRACKET = 0x007B;		// '{'
	constexpr CodePoint VERTICAL_LINE = 0x007C;				// '|'
	constexpr CodePoint RIGHT_CURLY_BRACKET = 0x007D;		// '}'
	constexpr CodePoint TILDE = 0x007E;						// '~'

	// Control character
	constexpr CodePoint DELETE = 0x007F;

	// -- C1 Controls and Latin-1 Supplement -- //
	// URL: https://www.unicode.org/charts/PDF/U0080.pdf

	// C1 controls
	constexpr CodePoint PADDING_CHARACTER = 0x0080;

	// Unicode Specials
	constexpr CodePoint REPLACEMENT_CHARACTER = 0xFFFD;

	constexpr CodePoint LAST_ALLOWED_CODE_POINT = 0x10FFFF;

	[[nodiscard]] inline constexpr bool
	IsASCIIAlpha(CodePoint character) noexcept {
		return (character >= LATIN_CAPITAL_LETTER_A && character <= LATIN_CAPITAL_LETTER_Z) || (character >= LATIN_SMALL_LETTER_A && character <= LATIN_SMALL_LETTER_Z);
	}

	[[nodiscard]] inline constexpr bool
	IsDigit(CodePoint character) noexcept {
		return character >= Unicode::DIGIT_ZERO && character <= Unicode::DIGIT_NINE;
	}

	[[nodiscard]] inline constexpr bool
	IsASCIIAlphaNumeric(CodePoint character) noexcept {
		return IsDigit(character) || IsASCIIAlpha(character);
	}

	[[nodiscard]] inline constexpr CodePoint
	ToLowerASCII(CodePoint character) noexcept {
		/* Could use a ternary operator; this is cleaner. */
		if (character >= LATIN_CAPITAL_LETTER_A && character <= LATIN_CAPITAL_LETTER_Z)
			return character + 0x20;
		return character;
	}

	[[nodiscard]] inline constexpr bool
	IsSurrogate(CodePoint character) noexcept {
		return character >= 0xD800 && character <= 0xDFFF;
	}

} // namespace Unicode
