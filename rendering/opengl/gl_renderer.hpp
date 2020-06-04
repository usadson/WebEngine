#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <vector>

#include <GL/glew.h>

#include "rendering/renderer.hpp"

namespace Rendering {

	class GLRenderer : public Renderer {
	public: // Public Properties
		std::vector<RenderObject *> renderObjects;

	public: // Con/destructors
		GLRenderer();

		inline
		~GLRenderer() override {
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

}
