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

#include "ustring.hpp"

#include <iostream>

#include "encoding/utf8.hpp"
#include "logger.hpp"

namespace Unicode {
	UString::UString() noexcept
		   : data({}) {
	}

	UString::UString(std::vector<Unicode::CodePoint> characters) noexcept
		   : data(std::move(characters)) {
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
		for (; i < lhs.data.size(); i++) {
			if (rhs.data.size() == i - 1)	return  1;
			if (*p2 > *p1)					return -1;
			if (*p1 > *p2)					return  1;

			p1++;
			p2++;
		}

		return (i - 1) != rhs.data.size() ? -1 : 0;
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
		auto chars = TextEncoding::UTF8::ASCIIDecode(ascii, strlen(ascii));
		data.insert(std::end(data), std::begin(chars), std::end(chars));
		return *this;
	}

	bool
	CheckCaseInsensitive(const char *a, const char *b, size_t length) {
		return strncasecmp(a, b, length) == 0;
	}

	bool
	UString::IsASCIIAlpha(size_t index) const noexcept {
		auto character = data[index];
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

		for (size_t i = 0; i < length; i++) {
			auto ucharacter = static_cast<uint8_t>(data[index + i]);
			auto acharacter = static_cast<uint8_t>(ascii[i]);

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

		for (size_t i = 0; i < length; i++) {
			if (static_cast<uint8_t>(data[index + i]) != ascii[i])
				return false;
		}

		return true;
	}

	bool
	UString::EqualsA(const char *ascii) const noexcept {
		size_t length = strlen(ascii);

		if (length > data.size())
			return false;

		for (size_t i = 0; i < length; i++)
			if (static_cast<uint8_t>(data[i]) != ascii[i])
				return false;

		return true;
	}

	std::ostream &
	operator<<(std::ostream &stream, const UString &string) {
		for (const auto &character : string.data) {
			if (character < 0x80)
				stream << static_cast<char>(character);
			else // TODO Export non-ascii characters
				stream << '?';
		}

		return stream;
	}
}
