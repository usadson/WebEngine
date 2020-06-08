#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <vector>

#include <cstdint>

namespace Net::HTTP::H2 {

	namespace FrameFlags {
		const uint8_t ACK = 0x1;
	}

	enum FrameType {
		DATA = 0x0,
		HEADERS = 0x1,
		PRIORITY = 0x2,
		RST_STREAM = 0x3,
		SETTINGS = 0x4,
		PUSH_PROMISE = 0x5,
		PING = 0x6,
		GOAWAY = 0x7,
		WINDOW_UPDATE = 0x8,
		CONTINUATION = 0x9,
		ALTSVC = 0xA,
		ORIGIN = 0xB
	};

	struct Frame {
		uint32_t length : 24;
		uint8_t type;
		uint8_t flags;
		uint32_t stream;
		std::vector<char> payload;
	};
}
