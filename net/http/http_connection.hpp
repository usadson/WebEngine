#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

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
			NOT_CONNECTED,
		};

		extern std::map<HTTPConnectionError, std::string> httpConnectionErrorNames;

		inline std::ostream &
		operator<<(std::ostream &stream, const HTTPConnectionError &type) {
			return stream << httpConnectionErrorNames[type];
		}

		class HTTPConnection {
		public: // Public Properties
			Net::ConnectionInfo connectionInfo;

		public: // Con/destructors
			// Constructors setup the connection using 'connectionInfo'.
			explicit
			HTTPConnection(const Net::ConnectionInfo &);

			~HTTPConnection();

		private: // Private Methods
			HTTPConnectionError
			ConsumeHeaderField(HTTPResponseInfo *, char);

			HTTPConnectionError
			ConsumeHTTPVersion(HTTPResponseInfo *);

			HTTPConnectionError
			ConsumeReasonPhrase(HTTPResponseInfo *);

			HTTPConnectionError
			ConsumeStatusCode(HTTPResponseInfo *);

			HTTPConnectionError
			ConsumeHeaderFieldName(std::vector<char> *);

			HTTPConnectionError
			ConsumeHeaderFieldValue(std::vector<char> *);

			HTTPConnectionError
			ConsumeWhiteSpace();

			HTTPConnectionError
			ConsumeHeaders(HTTPResponseInfo *);

			HTTPConnectionError
			ConsumeMessageBody(HTTPResponseInfo *);

		public: // Methods
			HTTPConnectionError
			Request(HTTPResponseInfo *, const std::string &method, const std::string &path);

			HTTPConnectionError
			RequestNavigation(HTTPResponseInfo *, const std::string &path);
		};
	}
}
