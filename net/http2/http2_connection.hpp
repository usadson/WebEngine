#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <map>
#include <utility>

#include "net/http/http_response_info.hpp"
#include "net/connection_info.hpp"
#include "frame.hpp"

namespace Net::HTTP {
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
