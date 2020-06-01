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
		UString();

		explicit
		UString(const std::vector<Unicode::CodePoint> &);

		// cppcheck-suppress[noExplicitConstructor]
		UString(Unicode::CodePoint character);

		/* Only for ASCII characters: */
		explicit
		UString(const char *, size_t);

		// cppcheck-suppress noExplicitConstructor
		inline
		UString(const char *ascii) : UString(ascii, strlen(ascii)) {}
	public: // Methods
		inline const Unicode::CodePoint &
		operator[](size_t index) const {
			return Data[index];
		}

		inline size_t
		length() const {
			return Data.size();
		}

		UString &
		operator+=(const Unicode::CodePoint);

		UString
		operator+(const UString &) const;

		/** Only for strictly-ASCII related strings. **/
		/* Append a strictly-ASCII string */
		UString &
		operator+=(const UString &);

		/* Append a strictly-ASCII string */
		UString &
		operator+=(const char *);

		/* Is the character at position <size_t> an ASCII character? */
		bool
		IsASCIIAlpha(size_t);

		/* Equals Ignore-case ASCII at index */
		bool
		EqualsIgnoreCaseA(size_t, const char *);

		/* Equals Ignore-case ASCII at index + length */
		bool
		EqualsIgnoreCaseAL(size_t, const char *, size_t );

		/* Equals case-sensitivily ASCII at index + length */
		bool
		EqualsAL(size_t, const char *, size_t);

		bool
		EqualsA(const char *);

		int
		Compare(const UString &) const;
	};

	std::ostream
	&operator<<(std::ostream &stream, const UString &string);

	int
	CompareStatic(const UString &lhs, const UString &rhs);

	bool
	operator<(const UString &lhs, const UString &rhs);

}
