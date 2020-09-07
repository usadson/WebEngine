/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "window_glfw.hpp"

#include <iostream>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

#include "logger.hpp"
#include "rendering/input.hpp"

namespace GLFWSuite {

	[[nodiscard]] inline constexpr Rendering::MouseButton
	ResolveMouseButton(int button) noexcept {
		switch (button) {
			case GLFW_MOUSE_BUTTON_LEFT:
				return Rendering::MouseButton::LEFT;
			case GLFW_MOUSE_BUTTON_MIDDLE:
				return Rendering::MouseButton::MIDDLE;
			case GLFW_MOUSE_BUTTON_RIGHT:
				return Rendering::MouseButton::RIGHT;
			default:
				// Unknown mouse button that we don't care abt
				return Rendering::MouseButton::ILLEGAL;
		}
	}

	void
	MouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
		static_cast<void>(mods);
		auto *renderer = reinterpret_cast<Rendering::WindowGLFW *>(glfwGetWindowUserPointer(window))->renderer;

		const auto mouseButton = ResolveMouseButton(button);
		if (mouseButton == Rendering::MouseButton::ILLEGAL) {
			return;
		}

		double x, y;
		glfwGetCursorPos(window, &x, &y);

		switch (action) {
			case GLFW_PRESS:
				renderer->OnMouseDown(mouseButton, x, y);
				break;
			default:
				break;
		}
	}

} // namespace GLFWSuite

namespace Rendering {

	/* Since the window is the most important thing of the application, there
	 * isn't really need for graceful error handling (I think). Therefore I can
	 * call std::runtime_error() */
	WindowGLFW::WindowGLFW() : WindowBase("GLFW"), internalWindow(nullptr) {
		if (glfwInit() == GLFW_FALSE) {
			throw std::runtime_error("GLFW initialization failed.");
		}
	}

	WindowGLFW::~WindowGLFW() noexcept {
		if (internalWindow != nullptr)
			glfwDestroyWindow(internalWindow);

		glfwTerminate();
	}

	std::vector<RendererType>
	WindowGLFW::GetSupportedRenderers() const noexcept {
		return {RendererType::OPENGL};
	}

	std::pair<bool, std::optional<void *>>
	WindowGLFW::RegisterRenderer(Renderer &renderer) {
		this->renderer = &renderer;

		switch (renderer.type()) {
			case RendererType::OPENGL:
				return {InternalPrepareGL(), {}};
			default:
				Logger::Error("GLFW", "Renderer not recognised!");
				return {false, {}};
		}
	}

	void
	GLFWErrorHandler(int error, const char *message) noexcept {
		(void)error;
		Logger::Error("GLFW", message);
	}

	bool
	WindowGLFW::InternalPrepareGL() {
		glfwSetErrorCallback(GLFWErrorHandler);

		float sizeFactor = 0.8;

		if (internalWindow != nullptr) {
			Logger::Severe("GLFW", "InternalPrepareGL() called twice! Quitting...");
			throw std::runtime_error("InternalPrepareGL called twice!");
		}

		GLFWmonitor *monitor = glfwGetPrimaryMonitor();
		if (monitor == nullptr) {
			Logger::Error("GLFW", "Failed to retrieve monitor information.");
			return false;
		}

		const GLFWvidmode *videoMode = glfwGetVideoMode(monitor);
		if (videoMode == nullptr) {
			Logger::Error("GLFW", "Failed to retrieve monitor information.");
			return false;
		}

		std::cout << "Width: " << videoMode->width << " Height: " << videoMode->height << std::endl;

		width = static_cast<uint32_t>(static_cast<float>(videoMode->width) * sizeFactor);
		height = static_cast<uint32_t>(static_cast<float>(videoMode->height) * sizeFactor);

		std::cout << "Width: " << this->width << " Height: " << this->height << std::endl;

		internalWindow = glfwCreateWindow(width, height, "WebEngine", nullptr, nullptr);
		if (internalWindow == nullptr) {
			Logger::Error("GLFW", "Failed to create window!");
			return false;
		}

		/* Center the window */
		glfwSetWindowPos(internalWindow, static_cast<uint32_t>((videoMode->width - this->width) / 2),
			static_cast<uint32_t>((videoMode->height - this->height) / 2));

		glfwSetWindowUserPointer(internalWindow, this);
		glfwSetMouseButtonCallback(internalWindow, &GLFWSuite::MouseButtonCallback);

		glfwMakeContextCurrent(internalWindow);
		return true;
	}

	bool
	WindowGLFW::PollClose() noexcept {
		glfwPollEvents();
		return glfwWindowShouldClose(internalWindow) == GLFW_TRUE;
	}

	void
	WindowGLFW::SetTitle(Unicode::UString title) noexcept {
		(void)title; // BUG
	}

	void
	WindowGLFW::SwapBuffers() noexcept {
		glfwSwapBuffers(internalWindow);
	}

} // namespace Rendering
