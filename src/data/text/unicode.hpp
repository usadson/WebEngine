/* Unicode utility class */
#pragma once

#include <cstdint>

namespace Unicode {
	typedef uint32_t CodePoint;

	const CodePoint REPLACEMENT_CHARACTER = 0xFFFD;
}
