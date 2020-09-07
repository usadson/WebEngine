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
	GLRenderer::CalculateDimensions() noexcept {
		tabWidth = window()->width / tabs.size();
		tabHeight = window()->height * 0.1;
		tabLineHeight = 4;
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

		CalculateDimensions();
	}

	void
	GLRenderer::DrawFrame() {
		glClear(GL_COLOR_BUFFER_BIT);

		const float windowWidth = window()->width;
		const float windowHeight = window()->height;

		for (std::size_t i = 0; i < tabs.size(); i++) {
			const bool isTabFocussed = focussedTab == &tabs[i];
			DrawQuad(tabs[i].view.color, tabWidth * i, 0, tabWidth, tabHeight);
			DrawQuad(isTabFocussed ? 0xFFFFFFFF : 0xBBBBBBFF, tabWidth * i, tabHeight, tabWidth, tabLineHeight);

			if (isTabFocussed) {
				DrawQuad(tabs[i].view.color, 0, tabHeight + tabLineHeight, windowWidth, windowHeight - tabHeight - tabLineHeight);
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
		if (button != MouseButton::LEFT || y > tabHeight) {
			return;
		}

		const std::size_t tabIndex = static_cast<std::size_t>(x / (window()->width / tabs.size()));
		if (tabIndex < tabs.size()) {
			focussedTab = &tabs[tabIndex];
		}
	}

} // namespace Rendering
