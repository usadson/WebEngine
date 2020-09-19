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

		glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
		glDisable (GL_DEPTH_TEST);
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable (GL_TEXTURE_2D);
		glViewport (0, 0, window()->width, window()->height);
		glMatrixMode (GL_PROJECTION);
		glLoadIdentity ();
		glOrtho(0, window()->width, window()->height, 0, -1, 1);
		glMatrixMode (GL_MODELVIEW);
		glLoadIdentity ();

		CalculateDimensions();
		cairoLayoutContext = CreateLayoutContext();
		CreateTestTexture();
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

		DrawTestTexture();

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

	void
	GLRenderer::CalculateTextSize(PangoLayout *layout, int *width, int *height) {
		pango_layout_get_size(layout, width, height);
		/* Divide by pango scale to get dimensions in pixels. */
		*width /= PANGO_SCALE;
		*height /= PANGO_SCALE;
	}

	cairo_t *
	create_cairo_context(int width, int height, int channels, cairo_surface_t **surf, unsigned char **buffer) {
		*buffer = static_cast<unsigned char *>(calloc(channels * width * height, sizeof(unsigned char)));
		*surf = cairo_image_surface_create_for_data(*buffer, CAIRO_FORMAT_ARGB32, width, height, channels * width);
		return cairo_create(*surf);
	}

	void
	GLRenderer::DrawTestTexture() {
		/* Render a texture in immediate mode. */
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glClear(GL_COLOR_BUFFER_BIT);
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, textureID);
		glColor3f(1.f, 1.0f, 1.0f);

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(textWidth, 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(textWidth, textHeight);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(0.0f, textHeight);
		glEnd();

		glPopMatrix();
	}

	unsigned int
	create_texture(unsigned int width, unsigned int height, unsigned char *pixels) {
		unsigned int texture_id;

		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, pixels);

		return texture_id;
	}

	void
	GLRenderer::CreateTestTexture() {
		const char *text = "Test";
		const char *font = "Sans Bold 18";

		cairo_surface_t *temp_surface;
		cairo_surface_t *surface;
		unsigned char *surface_data = NULL;
		PangoFontDescription *desc;

		/* Create a PangoLayout, set the font and text */
		PangoLayout *layout = pango_cairo_create_layout(cairoLayoutContext);
		pango_layout_set_text(layout, text, -1);

		/* Load the font */
		desc = pango_font_description_from_string(font);
		pango_layout_set_font_description(layout, desc);
		pango_font_description_free(desc);

		/* Get text dimensions and create a context to render to */
		CalculateTextSize(layout, &textWidth, &textHeight);
		cairoRenderContext = create_cairo_context(textWidth, textHeight, 4, &surface, &surface_data);

		/* Render */
		cairo_set_source_rgba(cairoRenderContext, 1, 1, 1, 1);
		pango_cairo_show_layout(cairoRenderContext, layout);
		textureID = create_texture(textWidth, textHeight, surface_data);

		/* Clean up */
		free(surface_data);
		g_object_unref(layout);
	}

} // namespace Rendering
