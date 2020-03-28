#pragma once

#include <string>

#include <cstdint>

namespace Net {
	namespace HTTP {
		class HTTPResponseInfo {
		public: // Properties
			std::string HTTPVersion;
			uint16_t StatusCode;
			std::string ReasonPhrase;
		};
	}
}
