#pragma once

#include <vector>

#include <cstdint>

#include "data/text/unicode.hpp"

namespace TextEncoding {
	class Encoding {
	public: // Properties
		std::vector<Unicode::CodePoint> Output;
	public: // Methods
		virtual bool Decode(const char *data, size_t size) = 0;
	};
}
