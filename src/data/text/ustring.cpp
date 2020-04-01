#include "ustring.hpp"

#include <iostream>

#include "encoding/utf8.hpp"
#include "logger.hpp"

namespace Unicode {
	UString::UString()
		   : Data({}) {
	}

	UString::UString(std::vector<Unicode::CodePoint> characters)
		   : Data(characters) {
	}

	UString::UString(Unicode::CodePoint character)
		   : Data({ character }) {
	}


	UString::UString(const char *characters, size_t size)
		   : Data(TextEncoding::UTF8::ASCIIDecode(characters, size)) {
	}

	UString &UString::operator+=(const Unicode::CodePoint character) {
		Data.push_back(character);
		return *this;
	}

	UString UString::operator+(const UString &other) const {
		UString result;
		result.Data.reserve(Data.size() + other.Data.size());
		result.Data.insert(std::end(result.Data), std::begin(this->Data), std::end(this->Data));
		result.Data.insert(std::end(result.Data), std::begin(other.Data), std::end(other.Data));
		return result;
	}

	// Transformed from https://stackoverflow.com/a/12136398
	// See https://stackoverflow.com/help/licensing
	int CompareStatic(const UString &lhs, const UString &rhs) {
		const Unicode::CodePoint *p1 = lhs.Data.data();
		const Unicode::CodePoint *p2 = rhs.Data.data();

		size_t i = 0;
		while (i++ != lhs.Data.size()) {
			if (rhs.Data.size() == i - 1)	return  1;
			if (*p2 > *p1)					return -1;
			if (*p1 > *p2)					return  1;

			p1++;
			p2++;
		}

		return i-1 != rhs.Data.size() ? -1 : 0;
	}

	int UString::Compare(const UString &other) const {
		return CompareStatic(*this, other);
	}

	bool operator<(const UString &lhs, const UString &rhs) {
		return CompareStatic(lhs, rhs) < 0;
	}

	UString &UString::operator+=(const UString other) {
		Data.insert(std::end(Data), std::begin(other.Data), std::end(other.Data));
		return *this;
	}

	UString &UString::operator+=(const char *ascii) {
		std::vector<Unicode::CodePoint> chars = TextEncoding::UTF8::ASCIIDecode(ascii, strlen(ascii));
		Data.insert(std::end(Data), std::begin(chars), std::end(chars));
		return *this;
	}

	bool CheckCaseInsensitive(const char *a, const char *b, size_t length) {
		return strncasecmp(a, b, length) == 0;
	}

	bool UString::IsASCIIAlpha(size_t index) {
		Unicode::CodePoint character = Data[index];
		return (character >= 0x41 && character <= 0x5A) || (character >= 0x61 && character <= 0x7A);
	}

	bool UString::EqualsIgnoreCaseAL(size_t index, const char *ascii, size_t length) {
		if (index + length >= Data.size()) {
			return false;
		}

		size_t i;
		uint8_t ucharacter;
		uint8_t acharacter;

		for (i = 0; i < length; i++) {
			ucharacter = (uint8_t) Data[index + i];
			acharacter = (uint8_t) ascii[i];

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

	bool UString::EqualsAL(size_t index, const char *ascii, size_t length) {
		if (index + length >= Data.size())
			return false;

		size_t i;
		uint8_t character;

		for (i = 0; i < length; i++) {
			character = (uint8_t) Data[index + i];

			if (character != ascii[i]) {
				return false;
			}
		}

		return true;
	}

	bool UString::EqualsA(const char *ascii) {
		size_t length = strlen(ascii);
		if (length >= Data.size())
			return false;

		size_t i;

		for (i = 0; i < length; i++)
			if ((uint8_t) Data[i] != ascii[i])
				return false;

		return true;
	}

	std::ostream &operator<<(std::ostream &stream, const UString &string) {
		size_t i;
		Unicode::CodePoint character;

		for (i = 0; i < string.length(); i++) {
			character = string[i];
			if (character < 0x80)
				stream << (char)character;
			else // TODO Export non-ascii characters
				stream << '?';
		}
		return stream;
	}
}
