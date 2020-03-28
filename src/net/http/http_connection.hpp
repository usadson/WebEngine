#pragma once

#include <utility>

#include "http_response_info.hpp"
#include "net/connection_info.hpp"

namespace Net {
	namespace HTTP {
		enum class HTTPConnectionError {
			FAILED_READ_PATH,
			FAILED_READ_STATUS,
			FAILED_READ_VERSION,
			FAILED_WRITE_REQUEST,
			INCORRECT_PROTOCOL,
			NO_ERROR,
		};

		class HTTPConnection {
		public: // Properties
			Net::ConnectionInfo ConnectionInfo;
		public: // Methods
			// Constructors setup the connection using 'connectionInfo'.
			HTTPConnection(Net::ConnectionInfo connectionInfo);
			~HTTPConnection();

			// Return value: "" on success, otherwise the error.
			HTTPConnectionError Request(HTTPResponseInfo *response, std::string method, std::string path);
			HTTPConnectionError RequestNavigation(HTTPResponseInfo *response, std::string path);
		};
	}
}
