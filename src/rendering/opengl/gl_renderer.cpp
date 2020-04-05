#include "gl_renderer.hpp"

namespace Rendering {

	GLRenderer::GLRenderer()
		: Renderer(RendererType::OPENGL) {
		
	}

	void GLRenderer::DrawFrame() {
		InternalWindow->SwapBuffers();
	}

	void GLRenderer::Enqueue(RenderObject *object) {
		
	}

}
