#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

namespace Rendering {

	struct DrawColor {
		uint8_t r { 0x00 };
		uint8_t g { 0x00 };
		uint8_t b { 0x00 };
		uint8_t a { 0x00 };

		inline
		DrawColor() = default;

		inline
		DrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept :
			r(r), g(g), b(b), a(a) {
		}

		inline
		DrawColor(uint32_t rgba) noexcept
			: r((rgba & 0xFF000000) >> 24), g((rgba & 0x00FF0000) >> 16), b((rgba & 0x0000FF00) >> 8), a(rgba & 0x000000FF) {
		}
	};

} // namespace Rendering
