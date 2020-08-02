#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <optional>
#include <string>
#include <vector>

#include <cstdint>

#include "http_header_field.hpp"

namespace Net::HTTP {
	class HTTPResponseInfo {
	  public: // Properties
		std::string httpVersion;
		uint16_t statusCode;
		std::string reasonPhrase;

		std::vector<HTTPHeaderField> headers;
		std::vector<char> messageBody;

	  public: // Methods
		[[nodiscard]] std::optional<const char *>
		GetHeader(const char *) const;

		[[nodiscard]] std::optional<std::size_t>
		GetHeaderUnsigned(const char *) const;
	};
} // namespace Net::HTTP
