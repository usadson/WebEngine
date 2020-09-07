#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <optional>
#include <string>
#include <vector>

namespace Rendering {
	class WindowBase;
}

#include "data/text/ustring.hpp"
#include "rendering/renderer.hpp"

namespace Rendering {

	class WindowBase {
	public:
		Renderer *renderer;

	  public: // Public Properties
		uint32_t height;
		uint32_t width;
		std::string windowManagerName;

	  public: // Con/destructors
		inline explicit WindowBase(const std::string &windowManagerName)
			: height(0), width(0), windowManagerName(windowManagerName) {
		}

		virtual ~WindowBase() noexcept = default;

	  public: // Public Methods
		virtual std::vector<RendererType>
		GetSupportedRenderers() const noexcept = 0;

		virtual bool
		PollClose() noexcept = 0;

		virtual std::pair<bool, std::optional<void *> >
		RegisterRenderer(Renderer &) = 0;

		virtual void
		SetTitle(Unicode::UString string) noexcept = 0;

		virtual void
		SwapBuffers() noexcept = 0;
	};

} // namespace Rendering
