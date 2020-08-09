#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "data/text/ustring.hpp"
#include "parser/css/tokenizer_stream.hpp"

#ifndef PRIVATE_VISIBILITY
#define PRIVATE_VISIBILITY private
#endif

namespace CSS {
	class Tokenizer {
	  public:
		explicit inline
		Tokenizer(const Unicode::UString &input) : inputString(input), stream(&inputString) {
		}

		[[nodiscard]] bool
		Run() noexcept;

	  PRIVATE_VISIBILITY:
		const Unicode::UString &inputString;
		TokenizerStream stream;
		Unicode::UString string;

		// The 'consume comments' tokenizer algorithm.
		//
		// [Spec]
		// Title: CSS Syntax Module Level 3
		// Section: 4.3.2.
		// URL: https://www.w3.org/TR/css-syntax-3/#consume-comment
		[[nodiscard]] bool
		ConsumeComments() noexcept;
	};
} // namespace CSS

