#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <ostream>
#include <utility>
#include <vector>

#include <cstring>

#include "unicode.hpp"

namespace Unicode {
	/**
	 *                                [Warning]
	 * Using case-insensitive functions only works with ASCII characters. These
	 * are characters from the ISO basic Latin alphabet*. These functions are
	 * useful for example for parsing languages with English keywords, like
	 * HTML. Do not use these anywhere except where standards explicitly
	 * permits use. This is to avoid internationalization bugs.
	 *
	 *                                [Warning]
	 * Using `const char *` inputs is only permitted on ASCII strings. If you
	 * want to use non-ASCII strings or characters inside a UString, use a
	 * TextEncoding::Encoding** for the former, and an Unicode::CodePoint***
	 * for thelatter.
	 *
	 * *   Wikipedia article:
	 *     https://en.wikipedia.org/wiki/ISO_basic_Latin_alphabet
	 * **  See the declaration in: data/text/encoder/encoding.hpp
	 * *** See the declaration in: data/text/unicode.hpp
	 */
	class UString {
		/**
		 * The following property should be private but since the functions outside
		 * this class (inside namespace Unicode) need this data.
		 */
	private:
		std::vector<Unicode::CodePoint> data;

	public:
		/*** Constructors ***/
		UString() noexcept;

		explicit UString(std::vector<Unicode::CodePoint> const &characters) noexcept : data(characters) {
		}

		explicit UString(std::vector<Unicode::CodePoint> &&characters) noexcept : data(std::move(characters)) {
		}

		explicit UString(Unicode::CodePoint) noexcept;

		explicit UString(const char *, std::size_t) noexcept;

		inline explicit UString(const char *ascii) noexcept : UString(ascii, strlen(ascii)) {
		}

		/*** Methods ****/
		[[nodiscard]] inline const Unicode::CodePoint &
		operator[](std::size_t index) const noexcept {
			return data[index];
		}

		[[nodiscard]] inline Unicode::CodePoint &
		operator[](std::size_t index) noexcept {
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

		[[nodiscard]] UString
		operator+(const Unicode::CodePoint &) const noexcept;

		[[nodiscard]] UString
		operator+(const UString &) const noexcept;

		UString &
		operator+=(Unicode::CodePoint) noexcept;

		/* Append a strictly-ASCII string */
		UString &
		operator+=(const UString &) noexcept;

		/* Append a strictly-ASCII string */
		UString &
		operator+=(const char *) noexcept;

		std::ostream &
		operator<<(std::ostream &stream) const;

		[[nodiscard]] int
		Compare(const UString &) const noexcept;

		void
		CopyTo(UString &) const noexcept;

		[[nodiscard]] bool
		EqualsA(const char *) const noexcept;

		/* Equals case-sensitivily ASCII at index + length */
		[[nodiscard]] bool
		EqualsAL(std::size_t, const char *, std::size_t) const noexcept;

		[[nodiscard]] bool
		EqualsIgnoreCase(const UString &other) const noexcept;

		/* Equals Ignore-case ASCII at index */
		[[nodiscard]] bool
		EqualsIgnoreCaseA(std::size_t, const char *) const noexcept;

		/* Equals Ignore-case ASCII at index + length */
		[[nodiscard]] bool
		EqualsIgnoreCaseAL(std::size_t, const char *, std::size_t) const noexcept;

		/* Is the character at position <std::size_t> an ASCII character? */
		[[nodiscard]] bool
		IsASCIIAlpha(std::size_t) const noexcept;

		bool
		RemoveCharacterAt(std::size_t) noexcept;

		[[nodiscard]] bool
		StartsWithA(const char *) const noexcept;

		[[nodiscard]] bool
		StartsWithIgnoreCaseAL(std::size_t pos, const char *ascii, size_t length) const noexcept;

		[[nodiscard]] inline constexpr auto
		begin() noexcept {
			return std::begin(data);
		}

		[[nodiscard]] inline constexpr auto
		end() noexcept {
			return std::end(data);
		}

		[[nodiscard]] inline constexpr auto
		cbegin() noexcept {
			return std::cbegin(data);
		}

		[[nodiscard]] inline constexpr auto
		cend() noexcept {
			return std::cend(data);
		}

		[[nodiscard]] inline constexpr auto
		rbegin() noexcept {
			return std::rbegin(data);
		}

		[[nodiscard]] inline constexpr auto
		rend() noexcept {
			return std::rend(data);
		}

		[[nodiscard]] inline constexpr auto
		crbegin() const noexcept {
			return std::crbegin(data);
		}

		[[nodiscard]] inline constexpr auto
		crend() const noexcept {
			return std::crend(data);
		}
	};

	inline std::ostream &
	operator<<(std::ostream &stream, const UString &string) {
		return string.operator<<(stream);
	}

	[[nodiscard]] inline bool
	operator<(const UString &lhs, const UString &rhs) noexcept {
		return lhs.Compare(rhs) < 0;
	}

} // namespace Unicode
