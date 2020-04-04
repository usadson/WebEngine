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
		UString(std::vector<Unicode::CodePoint> data);
		UString(Unicode::CodePoint character);

		/* Only for ASCII characters: */
		UString(const char *ascii, size_t length);
		inline UString(const char *ascii) : UString(ascii, strlen(ascii)) {}
	public: // Methods
		inline const Unicode::CodePoint &operator[](size_t index) const {
			return Data[index];
		}

		inline size_t length() const {
			return Data.size();
		}

		UString &operator+=(const Unicode::CodePoint character);
		UString operator+(const UString &other) const;

		/* Only for ASCII characters: */
		UString &operator+=(const UString other);
		UString &operator+=(const char *ascii);
		bool IsASCIIAlpha(size_t index);
		/* Equals Ignore-case ASCII */
		bool EqualsIgnoreCaseA(size_t index, const char *ascii);
		/* Equals Ignore-case ASCII + length */
		bool EqualsIgnoreCaseAL(size_t index, const char *ascii, size_t length);
		/* Equals case-sensitivily ASCII + length */
		bool EqualsAL(size_t index, const char *ascii, size_t length);
		bool EqualsA(const char *ascii);
		int Compare(const UString &other) const;
	};

	std::ostream &operator<<(std::ostream &stream, const UString &string);
	int CompareStatic(const UString &lhs, const UString &rhs);
	bool operator<(const UString &lhs, const UString &rhs);
}
