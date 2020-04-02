#include "single_byte_encoding.hpp"

namespace TextEncoding {
	bool AbstractSBEncoding::Decode(const char *data, size_t size) {
		uint8_t value;
		size_t i;
		Unicode::CodePoint character;

		// We could call v.clear(); and v.resize(); or v.clear(); and
		// v.push_back() * size, but this is a waste of resources since the
		// vector may already have enough space.

		Output.reserve(size);

		for (i = 0; i < size; i++) {
			value = (uint8_t) data[i];
			character = value < 0x80 ? value : Indices[value - 0x80];

			if (Output.size() > i)
				Output[i] = character;
			else
				Output.push_back(character);
		}

		Output.shrink_to_fit();
		return true;
	}
}
