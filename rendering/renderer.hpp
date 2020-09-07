#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <memory>

namespace Rendering {

	enum class RendererType { OPENGL };

	class Renderer;
} // namespace Rendering

#include "window/window.hpp"
#include "rendering/input.hpp"

namespace Rendering {

	class Renderer {
	  private: // Properties
		const RendererType rendererType;
		std::shared_ptr<Rendering::WindowBase> internalWindow{ nullptr };

	  protected:
		inline explicit Renderer(RendererType type) : rendererType(type) {
		}

		[[nodiscard]] inline std::shared_ptr<Rendering::WindowBase>
		window() const noexcept {
			return internalWindow;
		}

	  public: // Methods
		virtual ~Renderer() = default;

		/* The name may be misleading, but this function will be called every
		 * 'loop' and this function should calculate which or if it should (re)
		 * draw some parts of the queue. */
		virtual void
		DrawFrame()
			= 0;

		virtual void
		Prepare()
			= 0;

		virtual void
		OnMouseDown(MouseButton, double x, double y)
			= 0;

		virtual inline void
		SetWindow(const std::shared_ptr<Rendering::WindowBase> &window) noexcept {
			internalWindow = window;
		}

		[[nodiscard]] inline RendererType
		type() const noexcept {
			return rendererType;
		}
	};

} // namespace Rendering
