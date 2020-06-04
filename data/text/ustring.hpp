/**
 * Copyright (C) 2020 Tristan
 *
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <ostream>
#include <vector>

#include <cstring>

#include "unicode.hpp"

namespace Unicode {
	class UString {
	/**
	 * The following property should be private but since the functions outside
	 * this class (inside namespace Unicode) need this data.
	 */
	public: // Private Property
		std::vector<Unicode::CodePoint> data;

	public: // Constructor Methods
		UString() noexcept;

		explicit
		UString(std::vector<Unicode::CodePoint>) noexcept;

		// cppcheck-suppress[noExplicitConstructor]
		explicit
		UString(Unicode::CodePoint) noexcept;

		/* Only for ASCII characters: */
		explicit
		UString(const char *, size_t) noexcept;

		// cppcheck-suppress noExplicitConstructor
		inline explicit
		UString(const char *ascii) noexcept
			: UString(ascii, strlen(ascii)) {}

	public: // Methods
		inline const Unicode::CodePoint &
		operator[](size_t index) const noexcept {
			return data[index];
		}

		inline size_t
		length() const noexcept {
			return data.size();
		}

		UString &
		operator+=(const Unicode::CodePoint) noexcept;

		UString
		operator+(const Unicode::CodePoint &) const noexcept;

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
