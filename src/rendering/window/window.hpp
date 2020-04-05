#pragma once

#include <string>
#include <optional>
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

		virtual ~Window() = default;

		virtual std::vector<RendererType> GetSupportedRenderers() = 0;
		virtual std::pair<bool, std::optional<void *>> PrepareForRenderer(RendererType) = 0;
		virtual void SetTitle(Unicode::UString string) = 0;
	};

}
