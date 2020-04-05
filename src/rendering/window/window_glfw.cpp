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
		: WindowBase("GLFW"), InternalWindow(nullptr) {
		if (!glfwInit()) {
			throw std::runtime_error("GLFW initialization failed.");
		}
	}

	WindowGLFW::~WindowGLFW() {
		if (InternalWindow != nullptr)
			glfwDestroyWindow(InternalWindow);

		glfwTerminate();
	}

	std::vector<RendererType> WindowGLFW::GetSupportedRenderers() {
		return { RendererType::OPENGL };
	}

	std::pair<bool, std::optional<void *>> WindowGLFW::PrepareForRenderer(RendererType type) {
		switch (type) {
			case RendererType::OPENGL:
				return { InternalPrepareGL(), {} };
			default:
				Logger::Error("GLFW", "Renderer not recognised!");
				return { false, {} };
		}
	}

	bool WindowGLFW::InternalPrepareGL() {
		GLFWmonitor *monitor;
		float sizeFactor;
		const GLFWvidmode *videoMode;

		sizeFactor = 0.8;

		if (InternalWindow != nullptr) {
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

		Width = videoMode->width * sizeFactor;
		Height = videoMode->height * sizeFactor;

		InternalWindow = glfwCreateWindow(Width, Height, "WebEngine", nullptr, nullptr);
		if (InternalWindow == nullptr) {
			Logger::Error("GLFW", "Failed to create window!");
			return false;
		}

		/* Center the window */
		glfwSetWindowPos(InternalWindow, (videoMode->width - Width) / 2, (videoMode->height - Height) / 2);

		glfwMakeContextCurrent(InternalWindow);
		return true;
	}

	bool WindowGLFW::PollClose() {
		glfwPollEvents();
		return glfwWindowShouldClose(InternalWindow);
	}

	void WindowGLFW::SetTitle(Unicode::UString title) {
		
	}

	void WindowGLFW::SwapBuffers() {
		glfwSwapBuffers(InternalWindow);
	}

}
