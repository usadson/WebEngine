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
		const RendererType Type;
		std::shared_ptr<Rendering::WindowBase> InternalWindow;
	public: // Methods
		inline Renderer(RendererType type)
			: Type(type) {
		}

		/* The name may be misleading, but this function will be called every
		 * 'loop' and this function should calculate which or if it should (re)
		 * draw some parts of the queue. */
		virtual void DrawFrame() = 0;
		virtual void Enqueue(RenderObject *object) = 0;

		virtual inline void SetWindow(std::shared_ptr<Rendering::WindowBase> window) {
			InternalWindow = window;
		}
	};

}
