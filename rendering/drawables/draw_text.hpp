#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "rendering/render_object.hpp"
#include "draw_color.hpp"

namespace Rendering {
	class DrawText : public RenderObject {
	public:
		/* RGBA */
		DrawColor color;
		const char *text;
	public:
		inline
		DrawText()
			: RenderObject(RenderObjectType::TEXT), text("undefined text") {
		}
	};
}
