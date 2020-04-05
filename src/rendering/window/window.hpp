#pragma once

#include <string>
#include <optional>
#include <vector>

namespace Rendering {
	class WindowBase;
}

#include "data/text/ustring.hpp"
#include "rendering/renderer.hpp"

namespace Rendering {

	class WindowBase {
	public:
		std::string WindowManagerName;
	public:
		inline WindowBase(std::string windowManagerName)
			: WindowManagerName(windowManagerName) {
		}

		virtual ~WindowBase() = default;

		virtual std::vector<RendererType> GetSupportedRenderers() = 0;
		virtual bool PollClose() = 0;
		virtual std::pair<bool, std::optional<void *>> PrepareForRenderer(RendererType) = 0;
		virtual void SetTitle(Unicode::UString string) = 0;
		virtual void SwapBuffers() = 0;
	};

}
