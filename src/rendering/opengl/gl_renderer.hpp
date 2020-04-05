#pragma once

#include "rendering/renderer.hpp"

namespace Rendering {
	class GLRenderer : public Renderer {
	public:
		GLRenderer();

		void DrawFrame();
		void Enqueue(RenderObject *object);
	};
}
