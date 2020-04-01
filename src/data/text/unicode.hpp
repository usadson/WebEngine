/* Unicode utility class */
#pragma once

#include <cstdint>

namespace Unicode {
	typedef uint32_t CodePoint;

	const CodePoint REPLACEMENT_CHARACTER = 0xFFFD;

	inline bool IsASCIIAlpha(CodePoint character) {
		return (character >= 0x41 && character <= 0x5A) ||
			   (character >= 0x61 && character <= 0x7A);
	}
}
