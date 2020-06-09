#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

namespace Rendering {

	struct RGBAColor {
		// FIXME This macro isn't standard and this hack is pretty shitty
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
#else
		uint8_t a;
		uint8_t b;
		uint8_t g;
		uint8_t r;
#endif
	};

	union DrawColor {
		RGBAColor components;
		uint32_t value;
	};

} // namespace Rendering
