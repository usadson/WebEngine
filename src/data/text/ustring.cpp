#include "ustring.hpp"

#include "encoding/utf8.hpp"

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

	const bool UString::operator<(const UString &other) const {
		size_t i = 0;

		while (i < Data.size() && i < other.Data.size()) {
			
		}

		/*
		int i = 0;
		while ((str[i] != 0) && (rhs.str[i] != 0) && (str[i] == rhs.str[i])) {
		++i;
		}
		if ((str[i] == 0) && (rhs.str[i] == 0)) return false;
		if (str[i] == 0) return true;
		if (str[i] < rhs.str[i]) return true;
		*/
		return false;
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
		if (index + length >= Data.size())
			return false;

		size_t i;
		uint8_t character;

		for (i = index; i < index + length; i++) {
			character = (uint8_t) Data[i];

			if (character >= 0x41 && character <= 0x5A)
				character -= 0x20;

			if (character != ascii[i])
				return false;
		}

		return true;
	}

	bool UString::EqualsAL(size_t index, const char *ascii, size_t length) {
		if (index + length >= Data.size())
			return false;

		size_t i;
		uint8_t character;

		for (i = index; i < index + length; i++) {
			character = (uint8_t) Data[i];

			if (character != ascii[i])
				return false;
		}

		return true;
	}

	std::ostream &operator<<(std::ostream &stream, const UString &string) {
		return stream;// << error.Name;
	}
}
