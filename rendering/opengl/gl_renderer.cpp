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

namespace Rendering {

	/**
	 * Render a quad to the screen using the old OpenGL API.
	 *
	 * @param color an RGB color that should be used to draw the quad
	 * @param x a.k.a. the left bound of the quad
	 * @param y a.k.a. the top bound of the quad
	 * @param width combined with x will make the right bound of the quad
	 * @param height combined with y will make the bottom bound of the quad
	 */
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
		tabHeight = window()->height * 0.07;
		tabLineHeight = tabHeight * 0.05;
	}

	void
	GLRenderer::Prepare() {
		glewInit();
		//		glOrtho(0, window()->width, window()->height, 0, 0, 1);

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);
		glViewport(0, 0, window()->width, window()->height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, window()->width, window()->height, 0, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		CalculateDimensions();
		cairoLayoutContext = CreateLayoutContext();
		text.Prepare(cairoLayoutContext);
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
				DrawQuad(tabs[i].view.color, 0, tabHeight + tabLineHeight, windowWidth,
					windowHeight - tabHeight - tabLineHeight);
			}
		}

		text.Draw();

		window()->SwapBuffers();
	}

	void
	GLRenderer::OnMouseDown(MouseButton button, double x, double y) {
		if (button == MouseButton::LEFT && y < tabHeight) {
			CheckTabSwitch(x, y);
		}
	}

	std::size_t
	GLRenderer::GetTabIndex(double x, double y) {
		return static_cast<std::size_t>(x / (window()->width / tabs.size()));
	}

	void
	GLRenderer::CheckTabSwitch(double x, double y) {
		const auto tabIndex = GetTabIndex(x, y);
		if (tabIndex < tabs.size()) {
			focussedTab = &tabs[tabIndex];
		}
	}

	cairo_t *
	GLRenderer::CreateLayoutContext() {
		cairo_surface_t *temp_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 0, 0);
		cairo_t *context = cairo_create(temp_surface);
		cairo_surface_destroy(temp_surface);
		return context;
	}

} // namespace Rendering
