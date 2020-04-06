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

/* TODO Implement vector max-lengths? */

#include "http2_connection.hpp" 

#include <iostream>
#include <sstream>
#include <vector>

#include <cstring>

#include "logger.hpp"

namespace Net {
	namespace HTTP {
		HTTP2Connection::HTTP2Connection(Net::ConnectionInfo *connectionInfo)
				: ConnectionInfo(connectionInfo) {
		}

		HTTP2Connection::~HTTP2Connection() {
			
		}

		HTTP2ConnectionError HTTP2Connection::Request(HTTPResponseInfo *response, std::string method, std::string path) {
			if (!ConnectionInfo->Connected ||
				(ConnectionInfo->Secure && !ConnectionInfo->IsAuthenticated)) {
				return HTTP2ConnectionError::NOT_CONNECTED;
			}

			return HTTP2ConnectionError::NO_ERROR;
		}

		HTTP2ConnectionError HTTP2Connection::RequestNavigation(HTTPResponseInfo *response, std::string path) {
			return Request(response, "GET", path);
		}
	}
}
