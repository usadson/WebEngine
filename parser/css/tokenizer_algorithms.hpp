#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "data/text/unicode.hpp"
#include "parser/css/tokenizer_stream.hpp"

namespace CSS {

	[[nodiscard]] inline constexpr bool
	IsWhitespace(Unicode::CodePoint character) noexcept {
		return character == Unicode::LINE_FEED || character == Unicode::CHARACTER_TABULATION || character == Unicode::SPACE;
	}

	[[nodiscard]] inline constexpr bool
	IsHexCharacter(Unicode::CodePoint character) noexcept {
		return (character >= Unicode::LATIN_CAPITAL_LETTER_A && character <= Unicode::LATIN_CAPITAL_LETTER_F)
			|| (character >= Unicode::LATIN_SMALL_LETTER_A && character <= Unicode::LATIN_SMALL_LETTER_F)
			|| (character >= Unicode::DIGIT_ZERO && character <= Unicode::DIGIT_NINE);
	}

	[[nodiscard]] inline constexpr bool
	IsNonASCIICodePoint(Unicode::CodePoint codePoint) noexcept {
		return codePoint >= Unicode::PADDING_CHARACTER;
	}

	[[nodiscard]] inline constexpr bool
	IsNameStartCodePoint(Unicode::CodePoint codePoint) noexcept {
		return Unicode::IsASCIIAlpha(codePoint) || IsNonASCIICodePoint(codePoint) || codePoint == Unicode::LOW_LINE;
	}

	[[nodiscard]] inline constexpr bool
	IsNameCodePoint(Unicode::CodePoint codePoint) noexcept {
		return IsNameStartCodePoint(codePoint) || Unicode::IsDigit(codePoint) || codePoint == Unicode::HYPHEN_MINUS;
	}

	[[nodiscard]] inline bool
	IsValidEscape(const CSS::TokenizerStream &stream, std::size_t offset = 0) noexcept {
		Unicode::CodePoint next, next2;
		return stream.Peek(&next, offset) && stream.Peek(&next2, offset + 1) && next == Unicode::REVERSE_SOLIDUS
			&& next2 != Unicode::LINE_FEED;
	}

	[[nodiscard]] inline bool
	WillStartIdentifier(const CSS::TokenizerStream &stream) noexcept {
		Unicode::CodePoint first, second;
		if (!stream.Peek(&first)) {
			return false;
		}

		if (IsNameStartCodePoint(first)) {
			return true;
		}

		if (!stream.Peek(&second, 1)) {
			return false;
		}

		if (first == Unicode::SOLIDUS && second != Unicode::LINE_FEED) {
			return true;
		}

		if (first == Unicode::HYPHEN_MINUS && (second == Unicode::HYPHEN_MINUS || IsValidEscape(stream, 1))) {
			return true;
		}

		return false;
	}

} // namespace CSS
