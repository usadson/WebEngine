#pragma once

#include <string>
#include <vector>

#include "data/text/ustring.hpp"
#include "rendering/renderer.hpp"

namespace Rendering {

	class Window {
	public:
		std::string WindowManagerName;
	public:
		inline Window(std::string windowManagerName)
			: WindowManagerName(windowManagerName) {
		}
		virtual void Create() = 0;
		virtual void Destroy() = 0;
		virtual std::vector<RendererType> GetSupportedRenderers() = 0;
		virtual void SetTitle(Unicode::UString string) = 0;
	};

}
