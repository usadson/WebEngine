#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "draw_color.hpp"
#include "rendering/render_object.hpp"

namespace Rendering {
	class DrawRect : public RenderObject {
	  public:
		/* RGBA */
		DrawColor color;

	  public:
		inline DrawRect() : RenderObject(RenderObjectType::RECT) {
		}
	};
} // namespace Rendering
