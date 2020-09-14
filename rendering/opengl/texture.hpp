#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <GL/glew.h>

class GLTexture {
public:
	GLTexture(GLuint width, GLuint height, GLenum format, const GLvoid *data);
	~GLTexture();

	[[nodiscard]] inline constexpr GLint
	GetTexture() const noexcept {
		return textureID;
	}

	inline void
	bind() const noexcept {
		glBindTexture(GL_TEXTURE_2D, textureID);
	}

private:
	GLuint textureID;

	void
	setParameters() noexcept;
};

