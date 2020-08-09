#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "parser/css/parse_error.hpp"

namespace CSS {

	class Context {
	  public:
		inline void
		ReportParseError(ParseError error) const noexcept {
			if (errorReporter != nullptr) {
				(*errorReporter)(error);
			}
		}

		void (*errorReporter)(ParseError){ nullptr };
	};

} // namespace CSS
