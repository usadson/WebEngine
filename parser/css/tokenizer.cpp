/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "tokenizer.hpp"

#include <array>
#include <iostream>
#include <iterator>
#include <vector>

inline bool
IsWhitespace(Unicode::CodePoint character) noexcept {
	return character == Unicode::LINE_FEED || character == Unicode::CHARACTER_TABULATION || character == Unicode::SPACE;
}

inline bool
IsHexCharacter(Unicode::CodePoint character) {
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
IsValidEscape(const CSS::TokenizerStream &stream, std::size_t offset=0) noexcept {
	Unicode::CodePoint next, next2;
	return stream.Peek(&next, offset) && stream.Peek(&next2, offset + 1) && next == Unicode::SOLIDUS && next2 != Unicode::LINE_FEED;
}

[[nodiscard]] inline bool
WillStartIdentifier(const CSS::TokenizerStream &stream) noexcept {
	Unicode::CodePoint first, second, third;
	if (!stream.Peek(&first) || !stream.Peek(&second, 1) || !stream.Peek(&third, 2)) {
		return false;
	}

	switch (first) {
		case Unicode::HYPHEN_MINUS:
			return IsNameStartCodePoint(second) || second == Unicode::HYPHEN_MINUS || IsValidEscape(stream, 1);
		case Unicode::SOLIDUS:
			return second != Unicode::LINE_FEED;
		default:
			return IsNameStartCodePoint(first);
	}
}

namespace CSS {

	bool
	Tokenizer::ConsumeComments() noexcept {
		std::vector<Unicode::CodePoint> data(std::cbegin(inputString), std::cend(inputString));
		std::vector<Unicode::CodePoint>::iterator commentStart = std::end(data);
		bool inComment = false;

		for (auto iterator = std::begin(data); iterator < std::end(data); ++iterator) {
			if (inComment) {
				if (iterator + 1 != std::end(data) && *iterator == Unicode::ASTERISK
					&& *(iterator + 1) == Unicode::SOLIDUS) {
					inComment = false;
					iterator++;								// Consume '/' aswell
					data.erase(commentStart, iterator + 1); // +1 because std::vector<T>::erase is exclusive
					iterator = commentStart;
				}
			} else if (iterator + 1 != std::end(data) && *iterator == Unicode::SOLIDUS
					   && *(iterator + 1) == Unicode::ASTERISK) {
				inComment = true;
				commentStart = iterator;
				iterator++; // Consume '*' aswell
			}
		}

		if (inComment) {
			context.ReportParseError(CSS::ParseError::EOF_IN_CONSUMING_COMMENT);
			return false;
		}

		string = Unicode::UString(std::move(data));
		stream.SetString(&string);
		return true;
	}

	Unicode::CodePoint
	Tokenizer::ConsumeEscapedCodePoint() noexcept {
		Unicode::CodePoint character;
		if (!stream.Next(&character)) {
			context.ReportParseError(CSS::ParseError::EOF_IN_CONSUMING_ESCAPED_CODE_POINT);
			return Unicode::REPLACEMENT_CHARACTER;
		}

		if (!IsHexCharacter(character)) {
			return character;
		}

		std::string s;
		s += static_cast<char>(character);

		for (std::size_t i = 1; i < 6; i++) {
			if (!stream.Next(&character)) {
				break;
			}
			if (IsHexCharacter(character)) {
				s += static_cast<char>(character);
			} else if (!IsWhitespace(character)) {
				stream.Reconsume();
			}
		}

		unsigned long int value = std::stoul(s, nullptr, 16);

		if (value == 0 || Unicode::IsSurrogate(value) || value > Unicode::LAST_ALLOWED_CODE_POINT) {
			return Unicode::REPLACEMENT_CHARACTER;
		}

		return value;
	}

	bool
	Tokenizer::ConsumeStringToken(Unicode::CodePoint endingCodePoint) noexcept {
		Token token = CSS::MakeToken<TokenType::STRING>();
		std::vector<Unicode::CodePoint> &characters = std::get_if<TokenCodePointsData>(&token.data)->codePoints;

		Unicode::CodePoint character;
		while (stream.Next(&character)) {
			if (character == endingCodePoint) {
				tokens.push_back(token);
				return true;
			}

			switch (character) {
				case Unicode::LINE_FEED:
					context.ReportParseError(CSS::ParseError::NEWLINE_IN_CONSUMING_STRING);
					tokens.push_back(CSS::MakeToken<CSS::TokenType::BAD_STRING>());
					stream.Reconsume();
					return true;
				case Unicode::REVERSE_SOLIDUS:
					if (stream.Peek(&character)) {
						if (character != Unicode::LINE_FEED) {
							characters.push_back(ConsumeEscapedCodePoint());
						} else {
							stream.Skip();
						}
					}
					break;
				default:
					characters.push_back(character);
					break;
			}
		}

		context.ReportParseError(CSS::ParseError::EOF_IN_CONSUMING_STRING);
		return true;
	}

	bool
	Tokenizer::ConsumeToken(Unicode::CodePoint character) noexcept {
		switch (character) {
			case Unicode::LINE_FEED:
			case Unicode::CHARACTER_TABULATION:
			case Unicode::SPACE:
				while (stream.Next(&character) && IsWhitespace(character)) {
					tokens.push_back(CSS::MakeToken<CSS::TokenType::WHITESPACE>());
					return true;
				}
				break;
			case Unicode::QUOTATION_MARK:
				return ConsumeStringToken(character);
			case Unicode::NUMBER_SIGN:
				if (!TryParseHashTokenName()) {
					tokens.emplace_back(TokenType::DELIM, character);
				}
				break;
			case Unicode::APOSTROPHE:
				return ConsumeStringToken(character);
		}
		return true;
	}

	bool
	Tokenizer::Run() noexcept {
		if (!ConsumeComments()) {
			return false;
		}

		Unicode::CodePoint character;
		while (stream.Next(&character)) {
			if (!ConsumeToken(character)) {
				return false;
			}
		}

		return true;
	}

	bool
	Tokenizer::TryParseHashTokenName() noexcept {
		stream.Skip();
		Unicode::CodePoint next;
		if (!stream.Peek(&next) || !IsNameCodePoint(next) || !IsValidEscape(stream)) {
			return false;
		}

		auto token = MakeToken<TokenType::HASH>();
		auto *data = std::get_if<TokenHashData>(&token.data);
		if (WillStartIdentifier(stream)) {
			data->type = TokenHashType::ID;
		}

		ConsumeName(data->codePoints);

		tokens.push_back(token);
		return true;
	}

} // namespace CSS
