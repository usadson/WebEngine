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
		WindowGLFW();
		~WindowGLFW();

	  public: // Public Methods
		[[nodiscard]] std::vector<RendererType>
		GetSupportedRenderers() override;

		[[nodiscard]] std::pair<bool, std::optional<void *> >
		PrepareForRenderer(RendererType) override;

		bool
		PollClose() override;

		void SetTitle(Unicode::UString) override;

		void
		SwapBuffers() override;

	  private: // Private Methods
		bool
		InternalPrepareGL();
	};

} // namespace Rendering
