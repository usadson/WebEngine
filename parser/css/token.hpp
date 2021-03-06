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

	using IntegerType = std::int64_t;
	using NumberType = double;

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
			IntegerType integer;
			NumberType number;
		};
	};

	struct TokenDimensionData : public TokenNumericData, public TokenCodePointsData {
	};

	struct Token {
		enum class Type {
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

		Type type;

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
		inline Token(Type type, T data) noexcept
			: type(type), data(data) {
		}

		inline explicit Token(Type type) noexcept
			: type(type), data(nullptr) {
		}

		inline explicit Token(Unicode::CodePoint codePoint) noexcept
			: type(Type::DELIM), data(codePoint) {
		}
	};

	template<Token::Type type> Token
	MakeToken() noexcept {
		switch (type) {
			case Token::Type::IDENT:
			case Token::Type::FUNCTION:
			case Token::Type::AT_KEYWORD:
			case Token::Type::STRING:
			case Token::Type::URL:
				return Token { type, TokenCodePointsData() };
			case Token::Type::HASH:
				return Token { type, TokenHashData() };
			case Token::Type::DELIM:
				return Token { type, Unicode::REPLACEMENT_CHARACTER };
			case Token::Type::NUMBER:
			case Token::Type::PERCENTAGE:
				return Token { type, TokenNumericData() };
			case Token::Type::DIMENSION:
				return Token { type, TokenDimensionData() };
			default:
				return Token { type, nullptr };
		}
	}

} // namespace CSS

