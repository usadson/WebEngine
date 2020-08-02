#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "data/text/unicode.hpp"
#include "encoding.hpp"

namespace TextEncoding {
	class UTF8 : public Encoding {
	  private:
		Unicode::CodePoint CodePoint;
		std::size_t BytesSeen;
		uint_fast8_t BytesNeeded;
		uint_fast8_t LowerBoundary;
		uint_fast8_t UpperBoundary;

	  public: // Methods
		inline UTF8() noexcept : CodePoint(0), BytesSeen(0), BytesNeeded(0), LowerBoundary(0x80), UpperBoundary(0xBF) {
		}

		[[nodiscard]] bool
		Decode(const char *data, std::size_t size) override;

		[[nodiscard]] static std::vector<Unicode::CodePoint>
		ASCIIDecode(const char *data, std::size_t size);
	};
} // namespace TextEncoding
