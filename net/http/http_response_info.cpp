/**
 * Copyright (C) 2020 Tristan
 *
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "http_response_info.hpp"

#include <algorithm>
#include <sstream>

#include <cstring>

namespace Net {
	namespace HTTP {
		std::optional<const char *>
		HTTPResponseInfo::GetHeader(const char *name) const {
			auto result = std::find_if(std::begin(headers), std::end(headers),
				[name](const HTTPHeaderField &headerField) -> bool {
					return strcasecmp(headerField.fieldName.c_str(), name) == 0;
				}
			);

			if (result == std::end(headers))
				return std::optional<const char *>();
			else
				return std::optional<const char *>(result->fieldValue.c_str());
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
