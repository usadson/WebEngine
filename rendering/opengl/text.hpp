#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <vector>

#include <GL/glew.h>
#include <pango/pangocairo.h>

#include "data/text/ustring.hpp"

namespace Rendering {

	class Text {
	public:
		Text(cairo_t *cairoLayoutContext, Unicode::UString contents, int x, int y);
		~Text() noexcept;

		void
		Prepare();

		void
		Draw();

	private:
		// Non-owning, received from e.g. gl_renderer.hpp
		cairo_t *cairoLayoutContext;

		Unicode::UString contents;
		int x;
		int y;
		int width;
		int height;
		int textureID;

		[[nodiscard]] std::vector<char>
		ConvertContentsToCString() const noexcept;

		void
		CalculateTextSize(PangoLayout *);
	};

} // namespace Rendering
