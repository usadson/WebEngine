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
