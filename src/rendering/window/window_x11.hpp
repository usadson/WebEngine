#pragma once

#include "window.hpp"

#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h>
#include <xcb/xcb.h>

namespace Rendering {

	class X11Window : public Window {
	private:
		Display *XDisplay;
		xcb_connection_t *XCBConnection;
		xcb_screen_t *XCBScreen;
		xcb_drawable_t XCBWindow;
	public:
		X11Window();
		~X11Window();

		std::vector<RendererType> GetSupportedRenderers();
		std::pair<bool, std::optional<void *>> PrepareForRenderer(RendererType);
		void SetTitle(Unicode::UString string);
	};

}
