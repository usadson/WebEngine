#pragma once

#include <GL/glew.h>

#include "rendering/renderer.hpp"

namespace Rendering {
	class GLRenderer : public Renderer {
	public: // Properties
		GLuint VertexArrayID;
	public: // Methods
		GLRenderer();

		void DrawFrame();
		void Enqueue(RenderObject *object);
		void Prepare();
	};
}
