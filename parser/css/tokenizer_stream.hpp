#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "data/text/ustring.hpp"

namespace CSS {
	class TokenizerStream {
	  public:
		explicit
		TokenizerStream(const Unicode::UString &string) noexcept;

		// Gets the next value if possible.
		// Will consume the character.
		[[nodiscard]] bool
		Next(char *destination) noexcept;

		[[nodiscard]] bool
		Peek(char *destination, std::size_t offset = 0) noexcept;

	  private:
		const Unicode::UString &string;
		std::size_t position{ 0 };
	};
} // namespace CSS

