#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

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
	public: // Public Properties
		uint32_t height;
		uint32_t width;
		std::string windowManagerName;
	public: // Con/destructors
		inline explicit
		WindowBase(const std::string &windowManagerName)
			: height(0), width(0), windowManagerName(windowManagerName) {
		}

		virtual ~
		WindowBase() = default;

	public: // Public Methods
		virtual std::vector<RendererType>
		GetSupportedRenderers() = 0;

		virtual bool
		PollClose() = 0;

		virtual std::pair<bool, std::optional<void *>>
		PrepareForRenderer(RendererType) = 0;

		virtual void
		SetTitle(Unicode::UString string) = 0;

		virtual void
		SwapBuffers() = 0;
	};

}
