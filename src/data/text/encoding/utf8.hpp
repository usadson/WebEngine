#pragma once

#include "encoding.hpp"
#include "data/text/unicode.hpp"

namespace TextEncoding {
	class UTF8 : public Encoding {
	private:
		Unicode::CodePoint CodePoint;
		size_t BytesSeen;
		uint_fast8_t BytesNeeded;
		uint_fast8_t LowerBoundary;
		uint_fast8_t UpperBoundary;
	public: // Methods
		bool Decode(const char *data, size_t size);
		static std::vector<Unicode::CodePoint> ASCIIDecode(const char *data, size_t size);
	};
}
