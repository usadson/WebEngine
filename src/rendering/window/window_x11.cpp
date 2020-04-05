#include "window_x11.hpp"

namespace Rendering {

	X11Window::X11Window()
		: Window("X11") {
	}

	void X11Window::Create() {
		
	}

	void X11Window::Destroy() {
		
	}

	std::vector<RendererType> X11Window::GetSupportedRenderers() {
		return { RendererType::OPENGL };
	}

	void X11Window::SetTitle(Unicode::UString title) {
		
	}

}
