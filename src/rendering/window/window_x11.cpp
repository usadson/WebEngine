#include "window_x11.hpp"

#include <iostream>

#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

namespace Rendering {

	X11Window::X11Window()
		: Window("X11") {
		int screenIndex;
		int defaultDisplay;
		xcb_screen_iterator_t screenIterator;

		XCBScreen = NULL;

		XDisplay = XOpenDisplay(NULL);
		if (XDisplay == NULL)
			throw std::runtime_error("failed to create X11 window");

		defaultDisplay = DefaultScreen(XDisplay);

		XCBConnection = XGetXCBConnection(XDisplay);
		if (XCBConnection == NULL) {
			XCloseDisplay(XDisplay);
			XDisplay = nullptr;
			throw std::runtime_error("failed to get XCB connection");
		}

		XSetEventQueueOwner(XDisplay, XCBOwnsEventQueue);

		screenIterator = xcb_setup_roots_iterator(xcb_get_setup(XCBConnection));

		for(screenIndex = defaultDisplay;
			screenIterator.rem && screenIndex > 0;
			--screenIndex, xcb_screen_next(&screenIterator));

		XCBScreen = screenIterator.data;

		
	}

	X11Window::~X11Window() {
		XCloseDisplay(XDisplay);
	}

	std::vector<RendererType> X11Window::GetSupportedRenderers() {
		return { RendererType::OPENGL };
	}

	std::pair<bool, std::optional<void *>> X11Window::PrepareForRenderer(RendererType type) {
		switch (type) {
			case RendererType::OPENGL:
				
				return { true, {} };
			default:
				std::cout << "X11Window: Renderer not recognised!" << std::endl;
				return { false, {} };
		}
	}

	void X11Window::SetTitle(Unicode::UString title) {
		
	}

}
