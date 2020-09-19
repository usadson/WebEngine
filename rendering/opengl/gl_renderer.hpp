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
#include "text.hpp"

namespace Rendering {

	class GLRenderer : public Renderer {
	private:
		float tabWidth{};
		float tabHeight{};
		float tabLineHeight;
		cairo_t *cairoLayoutContext;
		Text text;

	  public: // Public Properties
		std::vector<Tab> tabs{5};
		Tab *focussedTab{&tabs[0]};

	  public: // Con/destructors
		inline GLRenderer() : Renderer(RendererType::OPENGL), text(cairoLayoutContext, Unicode::UString("Test"), 0, 0) {
		}

		inline ~GLRenderer() override {
			cairo_destroy(cairoLayoutContext);
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

	};

} // namespace Rendering
