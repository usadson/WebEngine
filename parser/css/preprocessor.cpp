/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "preprocessor.hpp"

namespace CSS {

	// Tasks:
	// - Replace CRLF, CR, FF with a single LF
	// - Replace NULL with 0xFFFD
	bool
	Preprocessor::Run(Unicode::UString &string) const noexcept {
		for (std::size_t i = 0; i < string.length(); i++) {
			if (string[i] == '\r') {
				if (string.length() > i + 1 && string[i] == '\n') {
					// 					string[i] = '\n';
					// 					string.
				} else {
					// 					string[i] = '\n';
				}
			}
		}

		return true;
	}

} // namespace CSS
