/**
 * Copyright (C) 2020 Tristan
 *
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "window_glfw.hpp"

#include <iostream>

#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

#include "logger.hpp"

namespace Rendering {

	/* Since the window is the most important thing of the application, there
	 * isn't really need for graceful error handling (I think). Therefore I can
	 * call std::runtime_error() */
	WindowGLFW::WindowGLFW()
		: WindowBase("GLFW"), internalWindow(nullptr) {
		if (!glfwInit()) {
			throw std::runtime_error("GLFW initialization failed.");
		}
	}

	WindowGLFW::~WindowGLFW() {
		if (internalWindow != nullptr)
			glfwDestroyWindow(internalWindow);

		glfwTerminate();
	}

	std::vector<RendererType>
	WindowGLFW::GetSupportedRenderers() {
		return { RendererType::OPENGL };
	}

	std::pair<bool, std::optional<void *>>
	WindowGLFW::PrepareForRenderer(RendererType type) {
		switch (type) {
			case RendererType::OPENGL:
				return { InternalPrepareGL(), {} };
			default:
				Logger::Error("GLFW", "Renderer not recognised!");
				return { false, {} };
		}
	}

	void
	GLFWErrorHandler(int error, const char *message) {
		(void) error;
		Logger::Error("GLFW", message);
	}

	bool
	WindowGLFW::InternalPrepareGL() {
		glfwSetErrorCallback(GLFWErrorHandler);

		GLFWmonitor *monitor;
		float sizeFactor;
		const GLFWvidmode *videoMode;

		sizeFactor = 0.8;

		if (internalWindow != nullptr) {
			Logger::Severe("GLFW", "InternalPrepareGL() called twice! Quitting...");
			throw std::runtime_error("InternalPrepareGL called twice!");
		}

		monitor = glfwGetPrimaryMonitor();
		if (!monitor) {
			Logger::Error("GLFW", "Failed to retrieve monitor information.");
			return false;
		}

		videoMode = glfwGetVideoMode(monitor);
		if (!videoMode) {
			Logger::Error("GLFW", "Failed to retrieve monitor information.");
			return false;
		}

		std::cout << "Width: " << videoMode->width << " Height: " << videoMode->height << std::endl;

		width = videoMode->width * sizeFactor;
		height = videoMode->height * sizeFactor;

		std::cout << "Width: " << this->width << " Height: " << this->height << std::endl;

		internalWindow = glfwCreateWindow(width, height, "WebEngine", nullptr, nullptr);
		if (internalWindow == nullptr) {
			Logger::Error("GLFW", "Failed to create window!");
			return false;
		}

		/* Center the window */
		glfwSetWindowPos(internalWindow, (videoMode->width - this->width) / 2, (videoMode->height - this->height) / 2);

		glfwMakeContextCurrent(internalWindow);
		return true;
	}

	bool
	WindowGLFW::PollClose() {
		glfwPollEvents();
		return glfwWindowShouldClose(internalWindow);
	}

	void
	WindowGLFW::SetTitle(Unicode::UString title) {
		(void) title; // BUG
	}

	void
	WindowGLFW::SwapBuffers() {
		glfwSwapBuffers(internalWindow);
	}

}
