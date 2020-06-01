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

		extern std::map<HTTPConnectionError, std::string> HTTPConnectionErrorNames;

		inline std::ostream &
		operator<<(std::ostream &stream, const HTTPConnectionError &type) {
			return stream << HTTPConnectionErrorNames[type];
		}

		class HTTPConnection {
		public: // Properties
			Net::ConnectionInfo ConnectionInfo;
		private: // Private Methods
			HTTPConnectionError ConsumeHeaderField(HTTPResponseInfo *, char firstCharacter);
			HTTPConnectionError ConsumeHTTPVersion(HTTPResponseInfo *);
			HTTPConnectionError ConsumeReasonPhrase(HTTPResponseInfo *);
			HTTPConnectionError ConsumeStatusCode(HTTPResponseInfo *);
		public: // Methods
			// Constructors setup the connection using 'connectionInfo'.
			explicit
			HTTPConnection(Net::ConnectionInfo connectionInfo);

			~HTTPConnection();

			// Return value: "" on success, otherwise the error.
			HTTPConnectionError
			Request(HTTPResponseInfo *, const std::string &method, const std::string &path);

			HTTPConnectionError
			RequestNavigation(HTTPResponseInfo *, const std::string &path);
		};
	}
}
