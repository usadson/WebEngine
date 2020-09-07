#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

namespace Rendering { class View; }

#include <cstdlib>

#include "tab.hpp"

namespace Rendering {

	// The view is the manager of contents in a tab.
	class View {
	public:
		std::uint32_t color{0xFFFFFFFFu / RAND_MAX * static_cast<std::uint32_t>(std::rand())};

		inline View(Tab &tab) noexcept : tab(tab) {
		}

	private:
		const Tab &tab;
	};

} // namespace Rendering
