/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "ustring.hpp"

#include <algorithm>
#include <iterator>

#include <stdint.h>

#include "data/text/unicode.hpp"
#include "encoding/utf8.hpp"

namespace Unicode {
	UString::UString() noexcept : data({}) {
	}

	UString::UString(Unicode::CodePoint character) noexcept : data({character}) {
	}

	UString::UString(const char *characters, std::size_t size) noexcept
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
	UString::Compare(const UString &other) const noexcept {
		const Unicode::CodePoint *p1 = data.data();
		const Unicode::CodePoint *p2 = other.data.data();

		std::size_t i = 0;
		for (; i < data.size(); i++) {
			if (other.data.size() == i - 1)
				return 1;
			if (*p2 > *p1)
				return -1;
			if (*p1 > *p2)
				return 1;

			p1++;
			p2++;
		}

		return (i - 1) != other.data.size() ? -1 : 0;
	}

	void
	UString::CopyTo(UString &dest) const noexcept {
		dest.data = this->data;
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

	std::ostream &
	UString::operator<<(std::ostream &stream) const {
		for (const auto &character : data) {
			if (character < 0x80)
				stream << static_cast<char>(character);
			else // TODO Export non-ascii characters
				stream << '?';
		}

		return stream;
	}

	bool
	UString::IsASCIIAlpha(std::size_t index) const noexcept {
		auto character = data[index];
		return (character >= 0x41 && character <= 0x5A) || (character >= 0x61 && character <= 0x7A);
	}

	bool
	UString::EqualsIgnoreCaseA(std::size_t index, const char *ascii) const noexcept {
		return UString::EqualsIgnoreCaseAL(index, ascii, strlen(ascii));
	}

	bool
	UString::EqualsIgnoreCaseAL(std::size_t index, const char *ascii, std::size_t length) const noexcept {
		if (index + length != data.size()) {
			return false;
		}

		for (std::size_t i = 0; i < length; i++) {
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
	UString::EqualsIgnoreCase(const UString &other) const noexcept {
		if (data.size() != other.data.size())
			return false;
		auto pair = std::mismatch(std::cbegin(data),
			std::cend(data),
			std::cbegin(other.data),
			[](Unicode::CodePoint a, Unicode::CodePoint b) {
				return Unicode::ToLowerASCII(a) == Unicode::ToLowerASCII(b);
			});

		/* See:
		 * https://en.cppreference.com/w/cpp/algorithm/mismatch
		 */
		return pair.first == std::cend(data) && pair.second == std::cend(other.data);
	}

	bool
	UString::operator==(const UString &other) const noexcept {
		if (data.size() != other.data.size())
			return false;
		auto pair = std::mismatch(std::cbegin(data), std::cend(data), std::cbegin(other.data));

		/* See:
		 * https://en.cppreference.com/w/cpp/algorithm/mismatch
		 */
		return pair.first == std::cend(data) && pair.second == std::cend(other.data);
	}

	bool
	UString::EqualsAL(std::size_t index, const char *ascii, std::size_t length) const noexcept {
		if (index + length != data.size())
			return false;

		for (std::size_t i = 0; i < length; i++) {
			if (static_cast<uint8_t>(data[index + i]) != ascii[i])
				return false;
		}

		return true;
	}

	bool
	UString::EqualsA(const char *ascii) const noexcept {
		std::size_t length = strlen(ascii);

		if (length != data.size())
			return false;

		for (std::size_t i = 0; i < length; i++)
			if (static_cast<uint8_t>(data[i]) != ascii[i])
				return false;

		return true;
	}

	bool
	UString::RemoveCharacterAt(std::size_t i) noexcept {
		if (data.size() < i + 1) {
			return false;
		}

		data.erase(std::begin(data) + i);
		return true;
	}

	bool
	UString::StartsWithA(const char *ascii) const noexcept {
		std::size_t length = strlen(ascii);

		if (length > data.size())
			return false;

		for (std::size_t i = 0; i < length; i++)
			if (static_cast<uint8_t>(data[i]) != ascii[i])
				return false;

		return true;
	}

	bool
	UString::StartsWithIgnoreCaseAL(std::size_t pos, const char *ascii, std::size_t length) const noexcept {
		if (pos + length > data.size())
			return false;

		for (std::size_t i = pos; i < length; i++) {
			auto ucharacter = static_cast<uint8_t>(data[pos + i]);
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
} // namespace Unicode
