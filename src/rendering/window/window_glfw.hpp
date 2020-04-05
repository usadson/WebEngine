#pragma once

#include <GLFW/glfw3.h>

#include "window.hpp"

namespace Rendering {

	class WindowGLFW : public WindowBase {
	private:
		GLFWwindow *InternalWindow;
	public:
		WindowGLFW();
		~WindowGLFW();

		std::vector<RendererType> GetSupportedRenderers();
		std::pair<bool, std::optional<void *>> PrepareForRenderer(RendererType);
		bool PollClose();
		void SetTitle(Unicode::UString string);
		void SwapBuffers();
	private:
		bool InternalPrepareGL();
	};

}
