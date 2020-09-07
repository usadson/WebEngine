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
	  public: // Public Properties
		std::vector<RenderObject *> renderObjects;
		std::vector<Tab> tabs{5};
		Tab *focussedTab{&tabs[0]};

	  public: // Con/destructors
		inline GLRenderer() : Renderer(RendererType::OPENGL) {
		}

		inline ~GLRenderer() override {
		}

	  public: // Public Methods
		void
		Dequeue(RenderObject *) override;

		void
		DrawFrame() override;

		void
		Enqueue(RenderObject *) override;

		void
		Prepare() override;
	};

} // namespace Rendering
