#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <iosfwd>

#include "data/text/unicode.hpp"
#include "data/text/ustring.hpp"

namespace CSS {
	class TokenizerStream {
	  public:
		explicit
		TokenizerStream(const Unicode::UString *string) noexcept;

		// Returns how many code points there are left to be read.
		// Assures that you can call Next this many times succesfully, or Peek
		// with offset of `CodePointsLeft() - 1`.
		[[nodiscard]] inline std::size_t
		CodePointsLeft() const noexcept {
			return string->length() - position;
		}

		// Consumes the character if possible.
		// If destination is nullptr, it won't fail.
		//
		// Returns true if not at the end, otherwise false.
		// If false is returned, destination and position are untouched.
		[[nodiscard]] bool
		Next(Unicode::CodePoint *destination) noexcept;

		// Get a character without consuming it.
		// If destination is nullptr, it won't fail.
		//
		// Returns true if not at the end, otherwise false.
		// If false is returned, destination is untouched.
		[[nodiscard]] bool
		Peek(Unicode::CodePoint *destination, std::size_t offset = 0) const noexcept;

		// Reconsume the previous code point. The next time Next/Peek is called,
		// the previous code point will be returned (if the stream isn't at the
		// end).
		void
		Reconsume() noexcept;

		// Uses this string instead of the one passed by the constructor.
		// Will set position to 0, of course.
		void
		SetString(const Unicode::UString *string) noexcept;

		// Discards/skips the current character if possible.
		// Effectively the same as: `Next(nullptr)`.
		//
		// Returns true if not at the end, otherwise false.
		// If false is returned, destination and position are untouched.
		/* discardable */ bool
		Skip() noexcept;

	  private:
		const Unicode::UString *string;
		std::size_t position{ 0 };
	};
} // namespace CSS

