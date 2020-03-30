#pragma once

#include "encoding.hpp"

namespace TextEncoding {
	class UTF8 : public Encoding {
	private:
		TextEncoding::UnicodeCodePoint CodePoint;
		size_t BytesSeen;
		uint_fast8_t BytesNeeded;
		uint_fast8_t LowerBoundary;
		uint_fast8_t UpperBoundary;
	public: // Properties
		std::vector<UnicodeCodePoint> Output;
	public: // Methods
		bool Decode(const char *data, size_t size);
	};
}
