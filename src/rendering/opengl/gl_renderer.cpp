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
		info << "WindowSize={ Width=" << InternalWindow->Width
			 << " Height=" << InternalWindow->Height << " }";
		Logger::Debug(__PRETTY_FUNCTION__, info.str());
		glOrtho(0, InternalWindow->Width, InternalWindow->Height, 0, 0, 1);
// 		glGenVertexArrays(1, &VertexArrayID);
// 		glBindVertexArray(VertexArrayID);
	}

	void GLRenderer::DrawFrame() {
		DrawRect *rect;

		glClear(GL_COLOR_BUFFER_BIT);

		glColor3f(1, 1, 1);
		for (const auto &object : RenderObjects) {
			switch (object->Type) {
				case RenderObjectType::RECT:
					rect = dynamic_cast<DrawRect *>(object);
					glColor4f(rect->Color.Components.R / 255.0,
							  rect->Color.Components.G / 255.0,
							  rect->Color.Components.B / 255.0,
							  rect->Color.Components.A / 255.0
					);

					glBegin(GL_QUADS);
					glVertex2f(rect->Bounds.Left,  rect->Bounds.Top);
					glVertex2f(rect->Bounds.Left,  rect->Bounds.Bottom);
					glVertex2f(rect->Bounds.Right, rect->Bounds.Bottom);
					glVertex2f(rect->Bounds.Right, rect->Bounds.Top);
					glEnd();
					break;
				default:
					break;
			}
		}

		InternalWindow->SwapBuffers();
	}

	void GLRenderer::Enqueue(RenderObject *object) {
		RenderObjects.push_back(object);
	}

	void GLRenderer::Dequeue(RenderObject *object) {
		auto it = std::find(RenderObjects.begin(), RenderObjects.end(), object);

		if (it == RenderObjects.end()) {
			std::stringstream info;
			info << "Invalid RenderObject! Pointer: " << object
				 << " RenderObjects.size: " << RenderObjects.size();
			Logger::Warning(__PRETTY_FUNCTION__, info.str());
		} else {
			RenderObjects.erase(it);
		}
	}

}
