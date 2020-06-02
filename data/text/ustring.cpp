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

#include "ustring.hpp"

#include <iostream>

#include "encoding/utf8.hpp"
#include "logger.hpp"

namespace Unicode {
	UString::UString() noexcept
		   : data({}) {
	}

	UString::UString(const std::vector<Unicode::CodePoint> &characters) noexcept
		   : data(characters) {
	}

	UString::UString(Unicode::CodePoint character) noexcept
		   : data({ character }) {
	}


	UString::UString(const char *characters, size_t size) noexcept
		   : data(TextEncoding::UTF8::ASCIIDecode(characters, size)) {
	}

	UString &
	UString::operator+=(const Unicode::CodePoint character) noexcept {
		data.push_back(character);
		return *this;
	}

	UString
	UString::operator+(const UString &other) const noexcept {
		UString result;
		result.data.reserve(data.size() + other.data.size());
		result.data.insert(std::end(result.data), std::begin(this->data), std::end(this->data));
		result.data.insert(std::end(result.data), std::begin(other.data), std::end(other.data));
		return result;
	}

	UString
	UString::operator+(const Unicode::CodePoint &character) const noexcept {
		UString result;
		result.data.reserve(data.size() + 1);
		result.data.insert(std::end(result.data), std::begin(this->data), std::end(this->data));
		result.data.push_back(character);
		return result;
	}

	// Transformed from https://stackoverflow.com/a/12136398
	// See https://stackoverflow.com/help/licensing
	int
	CompareStatic(const UString &lhs, const UString &rhs) noexcept {
		const Unicode::CodePoint *p1 = lhs.data.data();
		const Unicode::CodePoint *p2 = rhs.data.data();

		size_t i = 0;
		while (i++ != lhs.data.size()) {
			if (rhs.data.size() == i - 1)	return  1;
			if (*p2 > *p1)					return -1;
			if (*p1 > *p2)					return  1;

			p1++;
			p2++;
		}

		return i-1 != rhs.data.size() ? -1 : 0;
	}

	int 
	UString::Compare(const UString &other) const noexcept {
		return CompareStatic(*this, other);
	}

	bool
	operator<(const UString &lhs, const UString &rhs) noexcept {
		return CompareStatic(lhs, rhs) < 0;
	}

	UString &
	UString::operator+=(const UString &other) noexcept {
		data.insert(std::end(data), std::begin(other.data), std::end(other.data));
		return *this;
	}

	UString &
	UString::operator+=(const char *ascii) noexcept {
		std::vector<Unicode::CodePoint> chars = TextEncoding::UTF8::ASCIIDecode(ascii, strlen(ascii));
		data.insert(std::end(data), std::begin(chars), std::end(chars));
		return *this;
	}

	bool
	CheckCaseInsensitive(const char *a, const char *b, size_t length) {
		return strncasecmp(a, b, length) == 0;
	}

	bool
	UString::IsASCIIAlpha(size_t index) const noexcept {
		Unicode::CodePoint character = data[index];
		return (character >= 0x41 && character <= 0x5A) || (character >= 0x61 && character <= 0x7A);
	}

	bool
	UString::EqualsIgnoreCaseA(size_t index, const char *ascii) const noexcept {
		return UString::EqualsIgnoreCaseAL(index, ascii, strlen(ascii));
	}

	bool
	UString::EqualsIgnoreCaseAL(size_t index, const char *ascii, size_t length) const noexcept {
		if (index + length >= data.size()) {
			return false;
		}

		size_t i;

		for (i = 0; i < length; i++) {
			uint8_t ucharacter = (uint8_t) data[index + i];
			uint8_t acharacter = (uint8_t) ascii[i];

			if (ucharacter >= 0x41 && ucharacter <= 0x5A)
				ucharacter += 0x20;

			if (acharacter >= 0x41 && acharacter <= 0x5A)
				acharacter += 0x20;

			if (acharacter != ucharacter) {
				return false;
			}
		}

		return true;
	}

	bool
	UString::EqualsAL(size_t index, const char *ascii, size_t length) const noexcept {
		if (index + length >= data.size())
			return false;

		size_t i;

		for (i = 0; i < length; i++) {
			uint8_t character = (uint8_t) data[index + i];

			if (character != ascii[i])
				return false;
		}

		return true;
	}

	bool
	UString::EqualsA(const char *ascii) const noexcept {
		size_t length = strlen(ascii);

		if (length > data.size())
			return false;

		size_t i;
		for (i = 0; i < length; i++)
			if ((uint8_t) data[i] != ascii[i])
				return false;

		return true;
	}

	std::ostream &
	operator<<(std::ostream &stream, const UString &string) {
		size_t i;

		for (i = 0; i < string.length(); i++) {
			Unicode::CodePoint character = string[i];

			if (character < 0x80)
				stream << (char)character;
			else // TODO Export non-ascii characters
				stream << '?';
		}
		return stream;
	}
}
