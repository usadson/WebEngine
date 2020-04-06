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

#include "net/http/http_response_info.hpp"
#include "net/connection_info.hpp"

namespace Net {
	namespace HTTP {

		enum class HTTP2ConnectionError {
			NO_ERROR,
			NOT_CONNECTED,
		};

		static std::map<HTTP2ConnectionError, std::string> HTTP2ConnectionErrorNames = {
			{ HTTP2ConnectionError::NO_ERROR, "NO_ERROR" },
			{ HTTP2ConnectionError::NOT_CONNECTED, "NOT_CONNECTED" },
		};

		inline std::ostream &operator<<(std::ostream &stream, const HTTP2ConnectionError &type) {
			return stream << HTTP2ConnectionErrorNames[type];
		}

		class HTTP2Connection {
		public: // Properties
			Net::ConnectionInfo *ConnectionInfo;
		public: // Methods
			// Constructors setup the connection using 'connectionInfo'.
			HTTP2Connection(Net::ConnectionInfo *connectionInfo);
			~HTTP2Connection();

			// Return value: "" on success, otherwise the error.
			HTTP2ConnectionError Request(HTTPResponseInfo *response, std::string method, std::string path);
			HTTP2ConnectionError RequestNavigation(HTTPResponseInfo *response, std::string path);
		};

	}
}
