#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "data/text/ustring.hpp"
#include "parser/css/tokenizer_stream.hpp"

namespace CSS {
	class Tokenizer {
	  public:
		explicit inline
		Tokenizer(const Unicode::UString &input) : stream(input), inputString(input) {
		}

		[[nodiscard]] bool
		Run() noexcept;

	  private:
		TokenizerStream stream;
		const Unicode::UString &inputString;

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

