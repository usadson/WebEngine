#pragma once

#include <map>
#include <utility>

#include "http_response_info.hpp"
#include "net/connection_info.hpp"

namespace Net {
	namespace HTTP {
		enum class HTTPConnectionError {
			FAILED_READ_GENERIC,
			FAILED_READ_HEADER_FIELD_GENERIC,
			FAILED_READ_HEADER_FIELD_NAME,
			FAILED_READ_HEADER_FIELD_VALUE,
			FAILED_READ_HTTP_VERSION,
			FAILED_READ_MESSAGE_BODY,
			FAILED_READ_REASON_PHRASE,
			FAILED_READ_STATUS_CODE,
			FAILED_WRITE_REQUEST,
			INCORRECT_HEADER_FIELD_GENERIC,
			INCORRECT_HEADER_FIELD_NAME,
			INCORRECT_HEADER_FIELD_VALUE,
			INCORRECT_PROTOCOL,
			INCORRECT_REASON_PHRASE,
			INCORRECT_START_LINE,
			NO_ERROR,
		};

		static std::map<HTTPConnectionError, std::string> HTTPConnectionErrorNames = {
			{ HTTPConnectionError::FAILED_READ_GENERIC, "FAILED_READ_GENERIC" },
			{ HTTPConnectionError::FAILED_READ_HEADER_FIELD_GENERIC, "FAILED_READ_HEADER_FIELD_GENERIC" },
			{ HTTPConnectionError::FAILED_READ_HEADER_FIELD_NAME, "FAILED_READ_HEADER_FIELD_NAME" },
			{ HTTPConnectionError::FAILED_READ_HTTP_VERSION, "FAILED_READ_HTTP_VERSION" },
			{ HTTPConnectionError::FAILED_READ_HEADER_FIELD_VALUE, "FAILED_READ_HEADER_FIELD_VALUE" },
			{ HTTPConnectionError::FAILED_READ_REASON_PHRASE, "FAILED_READ_REASON_PHRASE" },
			{ HTTPConnectionError::FAILED_READ_STATUS_CODE, "FAILED_READ_STATUS_CODE" },
			{ HTTPConnectionError::FAILED_READ_MESSAGE_BODY, "FAILED_READ_MESSAGE_BODY" },
			{ HTTPConnectionError::FAILED_WRITE_REQUEST, "FAILED_WRITE_REQUEST" },
			{ HTTPConnectionError::INCORRECT_HEADER_FIELD_GENERIC, "INCORRECT_HEADER_FIELD_GENERIC" },
			{ HTTPConnectionError::INCORRECT_HEADER_FIELD_NAME, "INCORRECT_HEADER_FIELD_NAME" },
			{ HTTPConnectionError::INCORRECT_HEADER_FIELD_VALUE, "INCORRECT_HEADER_FIELD_VALUE" },
			{ HTTPConnectionError::INCORRECT_REASON_PHRASE, "INCORRECT_REASON_PHRASE" },
			{ HTTPConnectionError::INCORRECT_START_LINE, "INCORRECT_START_LINE" },
			{ HTTPConnectionError::NO_ERROR, "NO_ERROR" },
		};

		inline std::ostream &operator<<(std::ostream &stream, const HTTPConnectionError &type) {
			return stream << HTTPConnectionErrorNames[type];
		}

		class HTTPConnection {
		public: // Properties
			Net::ConnectionInfo ConnectionInfo;
		private: // Private Methods
			HTTPConnectionError ConsumeHeaderField(HTTPResponseInfo *response, char firstCharacter);
			HTTPConnectionError ConsumeHTTPVersion(HTTPResponseInfo *response);
			HTTPConnectionError ConsumeReasonPhrase(HTTPResponseInfo *response);
			HTTPConnectionError ConsumeStatusCode(HTTPResponseInfo *response);
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
