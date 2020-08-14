#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "data/text/unicode.hpp"

#include <type_traits>
#include <variant>

namespace CSS {

	enum class TokenType {
		IDENT,        // <ident-token>
		FUNCTION,     // <function-token>
		AT_KEYWORD,   // <at-keyword-token>
		HASH,         // <hash-token>
		STRING,       // <string-token>
		BAD_STRING,   //<bad-string-token>
		URL,          // <url-token>
		BAD_URL,      // <bad-url-token>
		DELIM,        // <delim-token>
		NUMBER,       // <number-token>
		PERCENTAGE,   //<percentage-token>
		DIMENSION,    // <dimension-token>
		WHITESPACE,   // <whitespace-token>
		CDO,          // <CDO-token>
		CDC,          // <CDC-token>
		COLON,        // <colon-token>
		SEMICOLON,    // <semicolon-token>
		COMMA,        // <comma-token>
		SQUARE_OPEN,  // <[-token>
		SQUARE_CLOSE, // <]-token>
		PAREN_OPEN,   // <(-token>
		PAREN_CLOSE,  // <)-token>
		CURLY_OPEN,   // <{-token>
		CURLY_CLOSE   // <}-token>
	};

	enum class TokenHashType {
		ID,
		UNRESTRICTED
	};

	enum class TokenNumericType {
		INTEGER,
		NUMBER
	};

	struct TokenCodePointsData {
		std::vector<Unicode::CodePoint> codePoints;
	};

	struct TokenHashData : public TokenCodePointsData {
		TokenHashType type { TokenHashType::UNRESTRICTED };
	};

	struct TokenNumericData {
		TokenNumericType type { TokenNumericType::INTEGER };
		union {
			uint64_t integer;
			double number;
		};
	};

	struct TokenDimensionData : public TokenNumericData, public TokenCodePointsData {
	};

	struct Token {
		TokenType type;

		std::variant<
			// For any other type:
			std::nullptr_t,

			// For:
			//  - <ident-token>
			//  - <function-token>
			//  - <at-keyword-token>
			//  - <string-token>
			//  - <url-token>
			TokenCodePointsData,

			// For <hash-token>:
			TokenHashData,

			// For <delim-token>:
			Unicode::CodePoint,

			// For:
			//  - <number-token>
			//  - <percentage-token>
			TokenNumericData,

			// For <dimension-token>:
			TokenDimensionData> data;

		template<typename T>
		inline Token(TokenType type, T data) noexcept
			: type(type), data(data) {
		}

		inline Token(Unicode::CodePoint codePoint) noexcept
			: type(TokenType::DELIM), data(codePoint) {
		}
	};

	template<TokenType type> Token
	MakeToken() noexcept {
		switch (type) {
			case TokenType::IDENT:
			case TokenType::FUNCTION:
			case TokenType::AT_KEYWORD:
			case TokenType::STRING:
			case TokenType::URL:
				return Token { type, TokenCodePointsData() };
			case TokenType::HASH:
				return Token { type, TokenHashData() };
			case TokenType::DELIM:
				return Token { type, Unicode::REPLACEMENT_CHARACTER };
			case TokenType::NUMBER:
			case TokenType::PERCENTAGE:
				return Token { type, TokenNumericData() };
			case TokenType::DIMENSION:
				return Token { type, TokenDimensionData() };
			default:
				return Token { type, nullptr };
		}
	}

} // namespace CSS

