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
		// TODO A thing that's probably better to do is to make a new vector and
		// loop through the elements of that vector, and adding the character or
		// adding a LF if needed. This way we can reduce reallocations.
		for (std::size_t i = 0; i < string.length(); i++) {
			if (string[i] == Unicode::CARRIAGE_RETURN) {
				string[i] = Unicode::LINE_FEED;
				if (string.length() > i + 1 && string[i + 1] == Unicode::LINE_FEED) {
					string.RemoveCharacterAt(i + 1);
				}
			} else if (string[i] == Unicode::FORM_FEED) {
				string[i] = Unicode::LINE_FEED;
			} else if (string[i] == Unicode::NULL_CHARACTER || Unicode::IsSurrogate(string[i])) {
				string[i] = Unicode::REPLACEMENT_CHARACTER;
			}
		}
		return true;
	}

} // namespace CSS
