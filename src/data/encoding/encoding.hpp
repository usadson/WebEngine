#pragma once

#include <vector>

#include <cstdint>

namespace TextEncoding {
	typedef uint32_t UnicodeCodePoint;

	class Encoding {
	public: // Methods
		virtual bool Decode(const char *data, size_t size) = 0;
	};
}
