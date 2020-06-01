/**
 * Copyright (C) 2020 Tristan <tristan@thewoosh.me>
 *
 * All Rights Reserved.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
 */

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
		uint32_t height;
		uint32_t width;
		std::string windowManagerName;
	public:
		inline WindowBase(std::string windowManagerName)
			: height(0), width(0), windowManagerName(windowManagerName) {
		}

		virtual ~
		WindowBase() = default;

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
