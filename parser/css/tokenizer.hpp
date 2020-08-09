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
		Tokenizer(const Unicode::UString &input) : stream(input) {
		}

		[[nodiscard]] bool
		Run() noexcept;

	  private:
		TokenizerStream stream;
	};
} // namespace CSS

