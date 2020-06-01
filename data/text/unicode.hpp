/**
 * Copyright (C) 2020 Tristan <tristan@thewoosh.me>
 *
 * All Rights Reserved.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
 */

/* Unicode utility class */
#pragma once

#include <cstdint>

/*
 * It is good practice to use the characters defined in this namespace instead
 * of using hardcoded hexadecimals.
 */
namespace Unicode {
	typedef uint32_t CodePoint;

	/* We can't use NULL as defined by the "C0 Controls and Basic Latin" 
	 * Unicode block, since this is a special keyword. */
	const CodePoint NULL_CHARACTER			= 0x0000;
	const CodePoint START_OF_HEADING		= 0x0001;
	const CodePoint START_OF_TEXT			= 0x0002;
	const CodePoint END_OF_TEXT				= 0x0003;
	const CodePoint END_OF_TRANSMISSION		= 0x0004;
	const CodePoint ENQUIRY					= 0x0005;
	const CodePoint ACKNOWLEDGE				= 0x0006;
	const CodePoint BELL					= 0x0007;
	const CodePoint BACKSPACE				= 0x0008;
	const CodePoint CHARACTER_TABULATION	= 0x0009;
	const CodePoint LINE_FEED				= 0x000A;
	const CodePoint LINE_TABULATION			= 0x000B;
	const CodePoint FORM_FEED				= 0x000C;
	const CodePoint CARRIAGE_RETURN			= 0x000D;
	// 	--- //
	const CodePoint SPACE					= 0x0020;
	const CodePoint NUMBER_SIGN				= 0x0023;
	const CodePoint AMPERSAND				= 0x0026;
	const CodePoint SEMICOLON				= 0x003B;
	const CodePoint EQUALS_SIGN				= 0x003D;
	const CodePoint REPLACEMENT_CHARACTER	= 0xFFFD;

	inline bool
	IsASCIIAlpha(CodePoint character) {
		return (character >= 0x41 && character <= 0x5A) ||
			   (character >= 0x61 && character <= 0x7A);
	}

	inline bool
	IsASCIIAlphaNumeric(CodePoint character) {
		return (character >= 0x30 && character <= 0x39) ||
			   (character >= 0x41 && character <= 0x5A) ||
			   (character >= 0x61 && character <= 0x7A);
	}
}
