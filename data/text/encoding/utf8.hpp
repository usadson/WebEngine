/**
 * Copyright (C) 2020 Tristan <tristan@thewoosh.me>
 *
 * All Rights Reserved.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
 */

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
		bool
		Decode(const char *data, size_t size) override;

		static std::vector<Unicode::CodePoint>
		ASCIIDecode(const char *data, size_t size);
	};
}
