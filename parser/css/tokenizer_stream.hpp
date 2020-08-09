#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <iosfwd>
namespace Unicode { class UString; }

namespace CSS {
	class TokenizerStream {
	  public:
		explicit
		TokenizerStream(const Unicode::UString &string) noexcept;

		// Consumes the character if possible.
		//
		// Returns true if not at the end, otherwise false.
		// If false is returned, destination and position are untouched.
		[[nodiscard]] bool
		Next(char *destination) noexcept;

		// Get a character without consuming it.
		//
		// Returns true if not at the end, otherwise false.
		// If false is returned, destination is untouched.
		[[nodiscard]] bool
		Peek(char *destination, std::size_t offset = 0) noexcept;

	  private:
		const Unicode::UString &string;
		std::size_t position{ 0 };
	};
} // namespace CSS

