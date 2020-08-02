/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "preprocessor.hpp"

#include <cstdio>

namespace CSS {

	bool
	Preprocessor::Run(Unicode::UString &string) const noexcept {
		for (std::size_t i = 0; i < string.length(); i++) {
			if (string[i] == '\r') {
				if (string.length() > i + 1 && string[i + 1] == '\n') {
					string[i] = '\n';
					string.RemoveCharacterAt(i + 1);
				} else {
					string[i] = '\n';
				}
			}
		}
		return true;
	}

} // namespace CSS
