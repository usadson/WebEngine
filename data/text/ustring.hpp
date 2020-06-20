#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

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

		explicit UString(std::vector<Unicode::CodePoint> characters) noexcept : data(std::move(characters)) {
		}

		// cppcheck-suppress[noExplicitConstructor]
		explicit UString(Unicode::CodePoint) noexcept;

		/* Only for ASCII characters: */
		explicit UString(const char *, std::size_t) noexcept;

		// cppcheck-suppress noExplicitConstructor
		inline explicit UString(const char *ascii) noexcept : UString(ascii, strlen(ascii)) {
		}

	  public: // Methods
		[[nodiscard]] inline const Unicode::CodePoint &
		operator[](std::size_t index) const noexcept {
			return data[index];
		}

		[[nodiscard]] inline std::size_t
		length() const noexcept {
			return data.size();
		}

		[[nodiscard]] bool
		operator==(const UString &other) const noexcept;

		[[nodiscard]] inline bool
		operator!=(const UString &other) const noexcept {
			return !operator==(other);
		}

		UString &
		operator+=(const Unicode::CodePoint) noexcept;

		[[nodiscard]] UString
		operator+(const Unicode::CodePoint &) const noexcept;

		[[nodiscard]] UString
		operator+(const UString &) const noexcept;

		/** Only for strictly-ASCII related strings. **/
		/* Append a strictly-ASCII string */
		UString &
		operator+=(const UString &) noexcept;

		/* Append a strictly-ASCII string */
		UString &
		operator+=(const char *) noexcept;

		/* Is the character at position <std::size_t> an ASCII character? */
		[[nodiscard]] bool
		IsASCIIAlpha(std::size_t) const noexcept;

		/* Equals Ignore-case ASCII at index */
		[[nodiscard]] bool
		EqualsIgnoreCaseA(std::size_t, const char *) const noexcept;

		/* Equals Ignore-case ASCII at index + length */
		[[nodiscard]] bool
		EqualsIgnoreCaseAL(std::size_t, const char *, std::size_t) const noexcept;

		/* Equals case-sensitivily ASCII at index + length */
		[[nodiscard]] bool
		EqualsAL(std::size_t, const char *, std::size_t) const noexcept;

		[[nodiscard]] bool
		EqualsA(const char *) const noexcept;

		[[nodiscard]] bool
		StartsWithA(const char *) const noexcept;

		[[nodiscard]] bool
		StartsWithIgnoreCaseAL(std::size_t pos, const char *ascii, size_t length) const noexcept;

		[[nodiscard]] bool
		EqualsIgnoreCase(const UString &other) const noexcept;
	};

	std::ostream &
	operator<<(std::ostream &stream, const UString &string);

	int
	CompareStatic(const UString &lhs, const UString &rhs) noexcept;

	bool
	operator<(const UString &lhs, const UString &rhs) noexcept;

} // namespace Unicode
