#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <GLFW/glfw3.h>

#include "window.hpp"

namespace Rendering {

	class WindowGLFW : public WindowBase {
	  private: // Private Properties
		GLFWwindow *internalWindow;

	  public: // Con/destructor
		WindowGLFW() /* throwable */;
		~WindowGLFW() noexcept;

	  public: // Public Methods
		[[nodiscard]] std::vector<RendererType>
		GetSupportedRenderers() const noexcept override;

		[[nodiscard]] std::pair<bool, std::optional<void *> >
		RegisterRenderer(Renderer &) override;

		bool
		PollClose() noexcept override;

		void SetTitle(Unicode::UString) noexcept override;

		void
		SwapBuffers() noexcept override;

	  private: // Private Methods
		bool
		InternalPrepareGL() /* throwable */;
	};

} // namespace Rendering
