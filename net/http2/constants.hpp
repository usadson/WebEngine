#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <vector>

#include <cstdint>

namespace Net::HTTP::H2 {
	enum Settings {
		HEADER_TABLE_SIZE = 0x1,
		ENABLE_PUSH = 0x2,
		MAX_CONCURRENT_STREAMS = 0x3,
		INITIAL_WINDOW_SIZE = 0x4,
		MAX_FRAME_SIZE = 0x5,
		MAX_HEADER_LIST_SIZE = 0x6
	};
}
