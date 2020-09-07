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

	inline static void
	DrawQuad(
		std::uint32_t color, std::uint32_t x, std::uint32_t y, std::uint32_t width, std::uint32_t height) noexcept {
		glColor3ub((color & 0xFF000000) >> 24, (color & 0x00FF0000) >> 16, (color & 0x0000FF00) >> 8);
		glBegin(GL_QUADS);
		glVertex2f(x, y);
		glVertex2f(x, y + height);
		glVertex2f(x + width, y + height);
		glVertex2f(x + width, y);
		glEnd();
	}

	void
	GLRenderer::Prepare() {
		std::cout << "Tabs: " << tabs.size() << '\n';
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
		glClear(GL_COLOR_BUFFER_BIT);

		const float wWidth = window()->width;
		const float wHeight = window()->height;
		const float width = wWidth / tabs.size();
		const float height = wHeight * 0.1;

		for (std::size_t i = 0; i < tabs.size(); i++) {
			const bool isTabFocussed = focussedTab == &tabs[i];
			DrawQuad(tabs[i].view.color, width * i, 0, width, height);
			DrawQuad(isTabFocussed ? 0xFFFFFFFF : 0xBBBBBBFF, width * i, height, width, 4);

			if (isTabFocussed) {
				DrawQuad(tabs[i].view.color, 0, height + 4, wWidth, wHeight - height - 4);
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

	void
	GLRenderer::OnMouseDown(MouseButton button, double x, double y) {
		std::cout << "[MouseDown] Btn=" << static_cast<int>(button) << " pos(" << x << ", " << y << ")\n";
	}

} // namespace Rendering
