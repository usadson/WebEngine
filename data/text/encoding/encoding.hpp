#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <vector>

#include <cstdint>

#include "data/text/unicode.hpp"

namespace TextEncoding {
	class Encoding {
	  public: // Properties
		std::vector<Unicode::CodePoint> Output;

	  public: // Methods
		inline virtual ~Encoding() {
		}

		virtual bool
		Decode(const char *data, size_t size)
			= 0;
	};
} // namespace TextEncoding
