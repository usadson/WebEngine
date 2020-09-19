#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <vector>

#include <GL/glew.h>
#include <pango/pangocairo.h>

#include "rendering/renderer.hpp"

#include "tab.hpp"

namespace Rendering {

	class GLRenderer : public Renderer {
	private:
		float tabWidth{};
		float tabHeight{};
		float tabLineHeight;
		cairo_t *cairoRenderContext;
		cairo_t *cairoLayoutContext;
		int textWidth = 0, textHeight = 0, textureID = 0;

	  public: // Public Properties
		std::vector<Tab> tabs{5};
		Tab *focussedTab{&tabs[0]};

	  public: // Con/destructors
		inline GLRenderer() : Renderer(RendererType::OPENGL) {
		}

		inline ~GLRenderer() override {
			cairo_destroy(cairoLayoutContext);
			cairo_destroy(cairoRenderContext);
		}

	  public: // Public Methods
		void
		CalculateDimensions() noexcept;

		void
		DrawFrame() override;

		void
		OnMouseDown(MouseButton, double x, double y) override;

		void
		Prepare() override;

	private:
		void
		CheckTabSwitch(double x, double y);

		[[nodiscard]] std::size_t
		GetTabIndex(double x, double y);

		[[nodiscard]] cairo_t *
		CreateLayoutContext();

		void
		CreateTestTexture();

		void
		CalculateTextSize(PangoLayout *layout, int *width, int *height);

		void
		DrawTestTexture();

	};

} // namespace Rendering
