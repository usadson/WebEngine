/**
 * Copyright (C) 2020 Tristan <tristan@thewoosh.me>
 *
 * All Rights Reserved.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
 */

#pragma once

#include <ostream>
#include <vector>

#include <cstring>

#include "unicode.hpp"

namespace Unicode {
	class UString {
	public: // Private Properties (don't change)
		std::vector<Unicode::CodePoint> Data;
	public: // Constructor Methods
		UString() noexcept;

		explicit
		UString(const std::vector<Unicode::CodePoint> &) noexcept;

		// cppcheck-suppress[noExplicitConstructor]
		UString(Unicode::CodePoint character) noexcept;

		/* Only for ASCII characters: */
		explicit
		UString(const char *, size_t) noexcept;

		// cppcheck-suppress noExplicitConstructor
		inline
		UString(const char *ascii) noexcept
			: UString(ascii, strlen(ascii)) {}
	public: // Methods
		inline const Unicode::CodePoint &
		operator[](size_t index) const noexcept {
			return Data[index];
		}

		inline size_t
		length() const noexcept {
			return Data.size();
		}

		UString &
		operator+=(const Unicode::CodePoint) noexcept;

		UString
		operator+(const UString &) const noexcept;

		/** Only for strictly-ASCII related strings. **/
		/* Append a strictly-ASCII string */
		UString &
		operator+=(const UString &) noexcept;

		/* Append a strictly-ASCII string */
		UString &
		operator+=(const char *) noexcept;

		/* Is the character at position <size_t> an ASCII character? */
		bool
		IsASCIIAlpha(size_t) const noexcept;

		/* Equals Ignore-case ASCII at index */
		bool
		EqualsIgnoreCaseA(size_t, const char *) const noexcept;

		/* Equals Ignore-case ASCII at index + length */
		bool
		EqualsIgnoreCaseAL(size_t, const char *, size_t) const noexcept;

		/* Equals case-sensitivily ASCII at index + length */
		bool
		EqualsAL(size_t, const char *, size_t) const noexcept;

		bool
		EqualsA(const char *) const noexcept;

		int
		Compare(const UString &) const noexcept;
	};

	std::ostream
	&operator<<(std::ostream &stream, const UString &string);

	int
	CompareStatic(const UString &lhs, const UString &rhs) noexcept;

	bool
	operator<(const UString &lhs, const UString &rhs) noexcept;

}
