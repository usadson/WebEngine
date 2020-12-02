/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "text.hpp"

#include <vector>

namespace Rendering {

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

	cairo_t *
	create_cairo_context(int width, int height, int channels, cairo_surface_t **surf, unsigned char **buffer) {
		*buffer = static_cast<unsigned char *>(calloc(channels * width * height, sizeof(unsigned char)));
		*surf = cairo_image_surface_create_for_data(*buffer, CAIRO_FORMAT_ARGB32, width, height, channels * width);
		return cairo_create(*surf);
	}

	Text::Text(Unicode::UString contents, int x, int y) : contents(contents), x(x), y(y) {
	}

	Text::~Text() {
	}

	void
	Text::Prepare(cairo_t *cairoLayoutContext) {

		const auto text = ConvertContentsToCString();
		const char *font = "Sans Bold 18";

		cairo_surface_t *temp_surface;
		cairo_surface_t *surface;
		unsigned char *surface_data = NULL;
		PangoFontDescription *desc;

		/* Create a PangoLayout, set the font and text */
		PangoLayout *layout = pango_cairo_create_layout(cairoLayoutContext);
		pango_layout_set_text(layout, text.data(), -1);

		/* Load the font */
		desc = pango_font_description_from_string(font);
		pango_layout_set_font_description(layout, desc);
		pango_font_description_free(desc);

		/* Get text dimensions and create a context to render to */
		CalculateTextSize(layout);
		auto *cairoRenderContext = create_cairo_context(width, height, 4, &surface, &surface_data);

		/* Render */
		cairo_set_source_rgba(cairoRenderContext, 1, 1, 1, 1);
		pango_cairo_show_layout(cairoRenderContext, layout);
		textureID = create_texture(width, height, surface_data);

		/* Clean up */
		free(surface_data);
		g_object_unref(layout);
	}

	std::vector<char>
	Text::ConvertContentsToCString() const noexcept {
		std::vector<char> result;

		for (const auto &codePoint : contents) {
			if (codePoint < 0x100) {
				result.push_back(codePoint);
			} else {
				// TODO:
				// https://encoding.spec.whatwg.org/#utf-8-encoder
				result.push_back('?');
			}
		}

		return result;
	}

	void
	Text::CalculateTextSize(PangoLayout *layout) {
		pango_layout_get_size(layout, &width, &height);
		/* Divide by pango scale to get dimensions in pixels. */
		width /= PANGO_SCALE;
		height /= PANGO_SCALE;
	}

	void
	Text::Draw() {
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, textureID);
		glColor3f(1.f, 1.0f, 1.0f);

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(width, 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(width, height);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(0.0f, height);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);
		glPopMatrix();
	}

} // namespace Rendering
