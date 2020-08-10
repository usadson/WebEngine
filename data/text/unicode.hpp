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
	const CodePoint NULL_CHARACTER = 0x0000;
	const CodePoint START_OF_HEADING = 0x0001;
	const CodePoint START_OF_TEXT = 0x0002;
	const CodePoint END_OF_TEXT = 0x0003;
	const CodePoint END_OF_TRANSMISSION = 0x0004;
	const CodePoint ENQUIRY = 0x0005;
	const CodePoint ACKNOWLEDGE = 0x0006;
	const CodePoint BELL = 0x0007;
	const CodePoint BACKSPACE = 0x0008;
	const CodePoint CHARACTER_TABULATION = 0x0009;
	const CodePoint LINE_FEED = 0x000A;
	const CodePoint LINE_TABULATION = 0x000B;
	const CodePoint FORM_FEED = 0x000C;
	const CodePoint CARRIAGE_RETURN = 0x000D;
	// 	--- //

	// ASCII punctuation and symbols
	const CodePoint SPACE = 0x0020;				// ' '
	const CodePoint EXCLAMATION_MARK = 0x0021;	// '!'
	const CodePoint QUOTATION_MARK = 0x0022;	// '"'
	const CodePoint NUMBER_SIGN = 0x0023;		// '#'
	const CodePoint DOLLAR_SIGN = 0x0024;		// '$'
	const CodePoint PERCENT_SIGN = 0x0025;		// '%'
	const CodePoint AMPERSAND = 0x0026;			// '&'
	const CodePoint APOSTROPHE = 0x0027;		// '''
	const CodePoint LEFT_PARENTHESIS = 0x0028;	// '('
	const CodePoint RIGHT_PARENTHESIS = 0x0029;	// ')'
	const CodePoint ASTERISK = 0x002A;			// '*'
	const CodePoint PLUS_SIGN = 0x002B;			// '+'
	const CodePoint COMMA = 0x002C;				// ','
	const CodePoint HYPHEN_MINUS = 0x002D;		// '-'
	const CodePoint FULL_STOP = 0x002E;			// '.'
	const CodePoint SOLIDUS = 0x002F;			// '/'

	// ASCII digits
	const CodePoint DIGIT_ZERO = 0x0030;		// '0'
	const CodePoint DIGIT_ONE = 0x0031;			// '1'
	const CodePoint DIGIT_TWO = 0x0032;			// '2'
	const CodePoint DIGIT_THREE = 0x0033;		// '3'
	const CodePoint DIGIT_FOUR = 0x0034;		// '4'
	const CodePoint DIGIT_FIVE = 0x0035;		// '5'
	const CodePoint DIGIT_SIX = 0x0036;			// '6'
	const CodePoint DIGIT_SEVEN = 0x0037;		// '7'
	const CodePoint DIGIT_EIGHT = 0x0038;		// '8'
	const CodePoint DIGIT_NINE = 0x0039;		// '9'

	// ASCII punctuation and symbols (#2)
	const CodePoint COLON = 0x003A;				// ':'
	const CodePoint SEMICOLON = 0x003B;			// ';'
	const CodePoint LESS_THAN_SIGN = 0x003C;	// '<'
	const CodePoint EQUALS_SIGN = 0x003D;		// '='
	const CodePoint GREATER_THAN_SIGN = 0x003E;	// '>'
	const CodePoint QUESTION_MARK = 0x003F;		// '?'
	const CodePoint COMMERCIAL_AT = 0x0040;		// '@'

	// Unicode Specials
	const CodePoint REPLACEMENT_CHARACTER = 0xFFFD;

	inline bool
	IsASCIIAlpha(CodePoint character) {
		return (character >= 0x41 && character <= 0x5A) || (character >= 0x61 && character <= 0x7A);
	}

	inline bool
	IsASCIIAlphaNumeric(CodePoint character) {
		return (character >= 0x30 && character <= 0x39) || (character >= 0x41 && character <= 0x5A)
			   || (character >= 0x61 && character <= 0x7A);
	}

	inline CodePoint
	ToLowerASCII(CodePoint character) {
		/* Could use a ternary operator; this is cleaner. */
		if (character >= 'A' && character <= 'Z')
			return character + 0x20;
		return character;
	}

	inline bool
	IsSurrogate(CodePoint character) {
		return character >= 0xD800 && character <= 0xDFFF;
	}

} // namespace Unicode
