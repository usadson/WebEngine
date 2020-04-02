/* Unicode utility class */
#pragma once

#include <cstdint>

namespace Unicode {
	typedef uint32_t CodePoint;

	const CodePoint NUMBER_SIGN				= 0x0023;
	const CodePoint AMPERSAND				= 0x0026;
	const CodePoint REPLACEMENT_CHARACTER	= 0xFFFD;

	inline bool IsASCIIAlpha(CodePoint character) {
		return (character >= 0x41 && character <= 0x5A) ||
			   (character >= 0x61 && character <= 0x7A);
	}

	inline bool IsASCIIAlphaNumeric(CodePoint character) {
		return (character >= 0x30 && character <= 0x39) ||
			   (character >= 0x41 && character <= 0x5A) ||
			   (character >= 0x61 && character <= 0x7A);
	}
}
