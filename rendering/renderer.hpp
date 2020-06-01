/**
 * Copyright (C) 2020 Tristan <tristan@thewoosh.me>
 *
 * All Rights Reserved.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
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
