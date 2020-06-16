/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "http_response_info.hpp"

#include <algorithm>
#include <sstream>

#include <cstring>

namespace Net::HTTP {
	std::optional<const char *>
	HTTPResponseInfo::GetHeader(const char *name) const {
		auto result
			= std::find_if(std::begin(headers), std::end(headers), [name](const HTTPHeaderField &headerField) -> bool {
				  return strcasecmp(headerField.fieldName.c_str(), name) == 0;
			  });

		if (result == std::end(headers))
			return std::optional<const char *>();

		return std::optional<const char *>(result->fieldValue.c_str());
	}

	std::optional<std::size_t>
	HTTPResponseInfo::GetHeaderUnsigned(const char *name) const {
		std::optional<const char *> value;

		value = GetHeader(name);
		if (value.has_value()) {
			try {
				return { std::stoull(value.value()) };
			} catch (const std::exception &) {
				return {};
			}
		}

		return {};
	}
} // namespace Net::HTTP
