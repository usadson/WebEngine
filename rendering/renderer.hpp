/**
 * Copyright (C) 2020 Tristan
 *
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

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
