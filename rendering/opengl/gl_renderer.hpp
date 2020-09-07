#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <vector>

#include <GL/glew.h>

#include "rendering/renderer.hpp"

#include "tab.hpp"

namespace Rendering {

	class GLRenderer : public Renderer {
	private:
		float tabWidth{};
		float tabHeight{};
		float tabLineHeight;

	  public: // Public Properties
		std::vector<Tab> tabs{5};
		Tab *focussedTab{&tabs[0]};

	  public: // Con/destructors
		inline GLRenderer() : Renderer(RendererType::OPENGL) {
		}

		inline ~GLRenderer() override {
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
	};

} // namespace Rendering
