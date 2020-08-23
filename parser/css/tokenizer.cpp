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

#include <cmath>

#include "parser/css/tokenizer_algorithms.hpp"

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
	Tokenizer::ConsumeName(std::vector<Unicode::CodePoint> &result) noexcept {
		Unicode::CodePoint codePoint;
		while (stream.Next(&codePoint)) {
			if (IsNameCodePoint(codePoint)) {
				result.push_back(codePoint);
				continue;
			}
			stream.Reconsume();

			if (IsValidEscape(stream)) {
				static_cast<void>(stream.Skip());
				result.push_back(ConsumeEscapedCodePoint());
			} else {
				return true;
			}
		}
		return false;
	}

	std::variant<std::monostate, std::int64_t, double>
	Tokenizer::ConsumeNumber() noexcept {
		std::vector<char> repr;
		Unicode::CodePoint codePoint;
		Unicode::CodePoint codePointNext;
		bool isInteger = true;

		static_cast<void>(stream.Next(&codePoint));
		if (codePoint == Unicode::PLUS_SIGN || codePoint == Unicode::HYPHEN_MINUS) {
			repr.push_back(codePoint);
		} else {
			stream.Reconsume();
		}

		while (true) {
			if (!stream.Next(&codePoint)) {
				break;
			}

			if (!Unicode::IsDigit(codePoint)) {
				stream.Reconsume();
				break;
			}

			repr.push_back(codePoint);
		}

		if (!stream.Peek(nullptr, 1)) {
			// NOTE Convert
			return {};
		}

		static_cast<void>(stream.Next(&codePoint));
		static_cast<void>(stream.Next(&codePointNext));

		if (codePoint == Unicode::FULL_STOP && Unicode::IsDigit(codePointNext)) {
			repr.push_back(codePoint);
			repr.push_back(codePointNext);
			isInteger = true;
			if (!stream.Peek(&codePoint, 1)) {
				// NOTE Convert
				return {};
			}
			static_cast<void>(stream.Next(&codePoint));
			static_cast<void>(stream.Next(&codePointNext));
		}

		if (codePoint == Unicode::LATIN_SMALL_LETTER_E || codePoint == Unicode::LATIN_CAPITAL_LETTER_E) {
			isInteger = true;
			repr.push_back(codePoint);
			if (codePointNext == Unicode::PLUS_SIGN || codePointNext == Unicode::HYPHEN_MINUS) {
				repr.push_back(codePointNext);
			} else {
				stream.Reconsume();
			}

			while (true) {
				if (!stream.Next(&codePoint)) {
					std::cerr << "ErrorReturn #7390\n";
					// NOTE check if we can return from here or that we need to
					// convert the integer now.
					return {};
				} else if (Unicode::IsDigit(codePoint)) {
					repr.push_back(codePoint);
				} else {
					stream.Reconsume();
					break;
				}
			}
		}
		repr.push_back('\0');
		std::cout << "repr=" << repr.data() << '\n';
		return {};
	}

	bool
	Tokenizer::ConsumeNumericToken() noexcept {
		return true;
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
			case Unicode::LEFT_PARENTHESIS:
				tokens.push_back(CSS::MakeToken<CSS::TokenType::PAREN_OPEN>());
				return true;
			case Unicode::RIGHT_PARENTHESIS:
				tokens.push_back(CSS::MakeToken<CSS::TokenType::PAREN_CLOSE>());
				return true;
			case Unicode::PLUS_SIGN:
				if (stream.Peek(&character) && Unicode::IsDigit(character)) {
					stream.Reconsume();
					return ConsumeNumericToken();
				}
				tokens.emplace_back(TokenType::DELIM, Unicode::PLUS_SIGN);
				return true;
		}
		return true;
	}

	std::variant<std::monostate, std::int64_t, double>
	Tokenizer::ConvertStringToNumber(const std::vector<Unicode::CodePoint> &string) noexcept {
		int s = 1;
		std::string iAsStr;
		std::uint64_t f = 0;
		std::size_t d = 0;
		int t = 1;
		std::int64_t e = 0;

		auto it = std::cbegin(string);
		if (*it == Unicode::HYPHEN_MINUS) {
			s = -1;
			++it;
		} else if (*it == Unicode::PLUS_SIGN) {
			++it;
		}

		auto itBegin = it;
		while (it != std::end(string) && Unicode::IsDigit(*it)) {
			it++;
		}
		iAsStr = std::string(itBegin, it);

		if (it != std::end(string) && *it == Unicode::FULL_STOP) {
			itBegin = ++it;
			while (Unicode::IsDigit(*it++)) {
			}
			--it;
			f = std::stol(std::string(itBegin, it));
			d = std::distance(itBegin, it);
		}

		if (it + 1 < std::end(string)
			&& (*it == Unicode::LATIN_CAPITAL_LETTER_E || *it == Unicode::LATIN_SMALL_LETTER_E)) {
			++it;
			if (*it == Unicode::HYPHEN_MINUS) {
				t = -1;
				++it;
			} else if (*it == Unicode::PLUS_SIGN) {
				++it;
			}
			itBegin = it;
			while (it != std::end(string) && Unicode::IsDigit(*it)) {
				it++;
			}
			e = std::stol(std::string(itBegin, it));
		}

		if (f == 0 && d == 0 && t == 1 && e == 0) {
			if (s == 1) {
				return std::stol(iAsStr);
			}
			return std::stol('-' + iAsStr);
		}

		const std::int64_t i = std::stol(iAsStr);
		return static_cast<double>(s) * (i + f / std::pow(10.0, d)) * std::pow(10.0, t * e);
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
		Unicode::CodePoint next;
		if (!stream.Peek(&next) || (!IsNameCodePoint(next) && !IsValidEscape(stream))) {
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
