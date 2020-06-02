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

#include "gl_renderer.hpp"

#include <algorithm>
#include <sstream>

#include <GL/glew.h>

#include "rendering/drawables/draw_rect.hpp"
#include "logger.hpp"

namespace Rendering {

	GLRenderer::GLRenderer()
		: Renderer(RendererType::OPENGL) {
	}

	void GLRenderer::Prepare() {
		glewInit();
		/* Map */
		std::stringstream info;
		info << "WindowSize={ width=" << internalWindow->width
			 << " height=" << internalWindow->height << " }";
		Logger::Debug(__PRETTY_FUNCTION__, info.str());
		glOrtho(0, internalWindow->width, internalWindow->height, 0, 0, 1);
// 		glGenVertexArrays(1, &VertexArrayID);
// 		glBindVertexArray(VertexArrayID);
	}

	void GLRenderer::DrawFrame() {
		DrawRect *rect;

		glClear(GL_COLOR_BUFFER_BIT);

		glColor3f(1, 1, 1);
		for (const auto &object : renderObjects) {
			switch (object->type) {
				case RenderObjectType::RECT:
					rect = dynamic_cast<DrawRect *>(object);

					if (rect == nullptr) {
						Logger::Error(__PRETTY_FUNCTION__, "RenderObjectType not equal to type (i.e. DrawRect)");
						continue;
					}

					glColor4f(rect->color.components.r / 255.0,
							  rect->color.components.g / 255.0,
							  rect->color.components.b / 255.0,
							  rect->color.components.a / 255.0
					);

					glBegin(GL_QUADS);
					glVertex2f(rect->bounds.left,  rect->bounds.top);
					glVertex2f(rect->bounds.left,  rect->bounds.bottom);
					glVertex2f(rect->bounds.right, rect->bounds.bottom);
					glVertex2f(rect->bounds.right, rect->bounds.top);
					glEnd();
					break;
				default:
					break;
			}
		}

		internalWindow->SwapBuffers();
	}

	void GLRenderer::Enqueue(RenderObject *object) {
		renderObjects.push_back(object);
	}

	void GLRenderer::Dequeue(RenderObject *object) {
		auto it = std::find(renderObjects.begin(), renderObjects.end(), object);

		if (it == renderObjects.end()) {
			std::stringstream info;
			info << "Invalid RenderObject! Pointer: " << object
				 << " renderObjects.size: " << renderObjects.size();
			Logger::Warning(__PRETTY_FUNCTION__, info.str());
		} else {
			renderObjects.erase(it);
		}
	}

}
