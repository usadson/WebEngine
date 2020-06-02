/**
 * Copyright (C) 2020 Tristan
 *
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
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
		inline explicit
		WindowBase(const std::string &windowManagerName)
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
