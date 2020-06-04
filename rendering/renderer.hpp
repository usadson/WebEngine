#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <memory>

namespace Rendering {

	enum class RendererType {
		OPENGL
	};

	class Renderer;
}

#include "window/window.hpp"
#include "render_object.hpp"

namespace Rendering {

	class Renderer {
	public: // Properties
		const RendererType type;
		std::shared_ptr<Rendering::WindowBase> internalWindow;
	public: // Methods
		inline explicit
		Renderer(RendererType type)
			: type(type) {
		}

		virtual
		~Renderer() {
		}

		virtual void
		Dequeue(RenderObject *) = 0;

		/* The name may be misleading, but this function will be called every
		 * 'loop' and this function should calculate which or if it should (re)
		 * draw some parts of the queue. */
		virtual void
		DrawFrame() = 0;

		virtual void
		Enqueue(RenderObject *) = 0;

		virtual void
		Prepare() = 0;

		virtual inline void
		SetWindow(std::shared_ptr<Rendering::WindowBase> window) {
			internalWindow = window;
		}
	};

}
