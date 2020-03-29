#pragma once

#include <string>
#include <vector>

#include <cstdint>

#include "http_header_field.hpp"

namespace Net {
	namespace HTTP {
		class HTTPResponseInfo {
		public: // Properties
			std::string HTTPVersion;
			uint16_t StatusCode;
			std::string ReasonPhrase;

			std::vector<HTTPHeaderField> Headers;
		};
	}
}
