/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "texture.hpp"

GLTexture::GLTexture(GLuint width, GLuint height, GLenum format, const GLvoid *data) {
	glGenTextures(1, &textureID);
	bind();
	setParameters();
	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 GL_RGBA,
				 width,
				 height,
				 0,
				 format,
				 GL_UNSIGNED_BYTE,
				 data);
}

GLTexture::~GLTexture() {
	glDeleteTextures(1, &textureID);
}

void
GLTexture::setParameters() noexcept {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
