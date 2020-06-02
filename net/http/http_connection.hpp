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
			HTTPConnectionError ConsumeHeaderField(HTTPResponseInfo *, char);
			HTTPConnectionError ConsumeHTTPVersion(HTTPResponseInfo *);
			HTTPConnectionError ConsumeReasonPhrase(HTTPResponseInfo *);
			HTTPConnectionError ConsumeStatusCode(HTTPResponseInfo *);

		public: // Methods
			HTTPConnectionError
			Request(HTTPResponseInfo *, const std::string &method, const std::string &path);

			HTTPConnectionError
			RequestNavigation(HTTPResponseInfo *, const std::string &path);
		};
	}
}
