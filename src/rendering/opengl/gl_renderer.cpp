#include "gl_renderer.hpp"

#include <GL/glew.h>

namespace Rendering {

	GLRenderer::GLRenderer()
		: Renderer(RendererType::OPENGL) {
	}

	void GLRenderer::Prepare() {
		glewInit();
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);
	}

	void GLRenderer::DrawFrame() {
		glColor3f(1,1,1);

		glBegin(GL_QUADS);
		glVertex2f(-0.5, -0.5);
		glVertex2f(-0.5,  0.5);
		glVertex2f( 0.5,  0.5);
		glVertex2f( 0.5, -0.5);
		glEnd();

		InternalWindow->SwapBuffers();
	}

	void GLRenderer::Enqueue(RenderObject *object) {
		
	}

}
