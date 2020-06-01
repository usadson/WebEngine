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
#include "frame.hpp"

namespace Net {
	namespace HTTP {

		enum class HTTP2Error {
			FAILED_READ,
			FRAME_TOO_LARGE,
			NO_ERROR,
			NOT_CONNECTED,
			SETTINGS_ACK_FLAG_INVALID_SIZE,
			SETTINGS_INVALID_STREAM,
			SETTINGS_INVALID_SIZE,
		};

		namespace H2 {
			struct Exception {
				HTTP2Error error;
				std::string message;

				inline Exception(HTTP2Error error, std::string message)
					: error(error), message(message) {
				}
			};

			struct LocalSettings {
				uint32_t maxFrameSize = 0x4000;
			};

			struct RemoteSettings {
				uint32_t headerTableSize = 0x1000;
				bool enablePush = true;
				std::optional<uint32_t> maxConcurrentStreams;
				uint32_t initialWindowSize = 0xFFFF;
				uint32_t maxFrameSize = 0x4000;
				std::optional<uint32_t> maxHeaderListSize;
			};
		}

		static std::map<HTTP2Error, std::string> HTTP2ErrorNames = {
			{ HTTP2Error::FAILED_READ, "FAILED_READ" },
			{ HTTP2Error::FRAME_TOO_LARGE, "FRAME_TOO_LARGE" },
			{ HTTP2Error::NO_ERROR, "NO_ERROR" },
			{ HTTP2Error::NOT_CONNECTED, "NOT_CONNECTED" },
		};

		inline std::ostream &
		operator<<(std::ostream &stream, const HTTP2Error &type) {
			return stream << HTTP2ErrorNames[type];
		}

		class HTTP2Connection {
		public: // Properties
			Net::ConnectionInfo *ConnectionInfo;
			H2::LocalSettings localSettings;
			H2::RemoteSettings remoteSettings;
		public: // Methods
			// Constructors setup the connection using 'connectionInfo'.
			HTTP2Connection(Net::ConnectionInfo *);

			~HTTP2Connection();

			// Return value: "" on success, otherwise the error.
			HTTP2Error
			Request(HTTPResponseInfo *, std::string method, std::string path);

			HTTP2Error
			RequestNavigation(HTTPResponseInfo *, std::string path);
		private: // Private Methods
			void
			HandleFrameGoaway(H2::Frame);

			void
			HandleFrameSettings(H2::Frame);

			H2::Frame
			ReadFrame();

			bool
			SendFrame(H2::Frame);
		};
	}
}
