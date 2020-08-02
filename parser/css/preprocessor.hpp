#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "data/text/unicode.hpp"
#include "data/text/ustring.hpp"

namespace CSS {
	class Preprocessor {
	  public:
		  [[nodiscard]] bool
		  Run(Unicode::UString &string) const noexcept;
	};
} // namespace CSS
