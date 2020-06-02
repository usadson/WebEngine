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

		extern std::map<HTTP2Error, std::string> HTTP2ErrorNames;

		namespace H2 {
			struct Exception {
				HTTP2Error error;
				std::string message;

				inline Exception(HTTP2Error error, const std::string &message)
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

		inline std::ostream &
		operator<<(std::ostream &stream, const HTTP2Error &type) {
			return stream << HTTP2ErrorNames[type];
		}

		class HTTP2Connection {
		public: // Properties
			Net::ConnectionInfo *connectionInfo;
			H2::LocalSettings localSettings;
			H2::RemoteSettings remoteSettings;
		public: // Con/destructors
			// Constructors setup the connection using 'connectionInfo'.
			explicit
			HTTP2Connection(Net::ConnectionInfo *);

			~HTTP2Connection();
		public: // Methods
			// Return value: "" on success, otherwise the error.
			HTTP2Error
			Request(HTTPResponseInfo *, std::string method, std::string path);

			HTTP2Error
			RequestNavigation(HTTPResponseInfo *, const std::string &path);
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
