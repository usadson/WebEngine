#pragma once

#include "window.hpp"

namespace Rendering {

	class X11Window : public Window {
	public:
		X11Window();

		void Create();
		void Destroy();
		std::vector<RendererType> GetSupportedRenderers();
		void SetTitle(Unicode::UString string);
	};

}
