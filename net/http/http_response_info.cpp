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

#include "http_response_info.hpp"

#include <sstream>

#include <cstring>

namespace Net {
	namespace HTTP {
		std::optional<const char *>
		HTTPResponseInfo::GetHeader(const char *name) const {
			for (const HTTPHeaderField &headerField : Headers) {
				if (strcasecmp(headerField.FieldName.c_str(), name) == 0)
					return std::optional<const char *>(headerField.FieldValue.c_str());
			}

			return std::optional<const char *>();
		}

		std::optional<size_t>
		HTTPResponseInfo::GetHeaderUnsigned(const char *name) const {
			size_t res;
			std::optional<const char *> value;

			value = GetHeader(name);
			if (value.has_value() && sscanf(value.value(), "%zu", &res) == 1)
				return res;
			
			return std::optional<size_t>();
		}
	}
}
