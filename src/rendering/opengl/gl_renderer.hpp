#pragma once

#include "rendering/renderer.hpp"

namespace Rendering {
	class GLRenderer : public Renderer {
	public:
		GLRenderer();

		void Enqueue(RenderObject *object);
	};
}
