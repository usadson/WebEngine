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
		TokenizerStream(const Unicode::UString *string) noexcept;

		// Consumes the character if possible.
		// If destination is nullptr, it won't fail.
		//
		// Returns true if not at the end, otherwise false.
		// If false is returned, destination and position are untouched.
		[[nodiscard]] bool
		Next(char *destination) noexcept;

		// Get a character without consuming it.
		// If destination is nullptr, it won't fail.
		//
		// Returns true if not at the end, otherwise false.
		// If false is returned, destination is untouched.
		[[nodiscard]] bool
		Peek(char *destination, std::size_t offset = 0) noexcept;

		// Uses this string instead of the one passed by the constructor.
		// Will set position to 0, of course.
		void
		SetString(const Unicode::UString *string) noexcept;

	  private:
		const Unicode::UString *string;
		std::size_t position{ 0 };
	};
} // namespace CSS

