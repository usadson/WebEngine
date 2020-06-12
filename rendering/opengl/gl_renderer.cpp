/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "gl_renderer.hpp"

#include <algorithm>
#include <sstream>

#include <GL/glew.h>

#include "logger.hpp"
#include "rendering/drawables/draw_rect.hpp"

namespace Rendering {

	void
	GLRenderer::Prepare() {
		glewInit();
		/* Map */
		std::stringstream info;
		info << "WindowSize={ width=" << window()->width << " height=" << window()->height << " }";
		Logger::Debug(__PRETTY_FUNCTION__, info.str());
		glOrtho(0, window()->width, window()->height, 0, 0, 1);
		// 		glGenVertexArrays(1, &VertexArrayID);
		// 		glBindVertexArray(VertexArrayID);
	}

	void
	GLRenderer::DrawFrame() {
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

					glColor4f(rect->color.components.r / 255.0, rect->color.components.g / 255.0,
							  rect->color.components.b / 255.0, rect->color.components.a / 255.0);

					glBegin(GL_QUADS);
					glVertex2f(rect->bounds.left, rect->bounds.top);
					glVertex2f(rect->bounds.left, rect->bounds.bottom);
					glVertex2f(rect->bounds.right, rect->bounds.bottom);
					glVertex2f(rect->bounds.right, rect->bounds.top);
					glEnd();
					break;
				default:
					break;
			}
		}

		window()->SwapBuffers();
	}

	void
	GLRenderer::Enqueue(RenderObject *object) {
		renderObjects.push_back(object);
	}

	void
	GLRenderer::Dequeue(RenderObject *object) {
		auto it = std::find(renderObjects.begin(), renderObjects.end(), object);

		if (it == renderObjects.end()) {
			std::stringstream info;
			info << "Invalid RenderObject! Pointer: " << object << " renderObjects.size: " << renderObjects.size();
			Logger::Warning(__PRETTY_FUNCTION__, info.str());
		} else {
			renderObjects.erase(it);
		}
	}

} // namespace Rendering
