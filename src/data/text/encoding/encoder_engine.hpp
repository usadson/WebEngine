#pragma once

#include <string>
#include <vector>

#include "data/text/unicode.hpp"

namespace TextEncoding {
	namespace EncoderEngine {
		std::vector<Unicode::CodePoint> DecodeData(const char *data, size_t size, std::string encoding);
	}
}
