/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#pragma once

#include <string>
#include <vector>

#include "data/text/unicode.hpp"

namespace TextEncoding {
	namespace EncoderEngine {
		std::vector<Unicode::CodePoint>
		DecodeData(const char *data, std::size_t size, std::string encoding);
	}
} // namespace TextEncoding
