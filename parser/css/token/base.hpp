#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <vector>

#include "data/text/unicode.hpp"
#include "parser/css/token/flag.hpp"
#include "parser/css/token/type.hpp"

// CSS Parse Tokens
//
// Implemented by using templates. Use with care, and only use TokenFlags
// (second template parameter) with corresponding values. See token_flag.hpp
// for details, or visit the standard.
//
// Examples:
// CSS::Token<CSS::TokenType::FUNCTION> functionToken({ 'H', 'e', 'y', '!' });
// CSS::Token<CSS::TokenType::HASH, CSS::TokenFlag::ID> hashToken({ 'H', 'i' });
// CSS::Token<CSS::TokenType::DELIM> delimToken('\n');
// CSS::Token<CSS::TokenType::DIMENSION, CSS::TokenFlag::INTEGER> dim(100);
// CSS::Token<CSS::TokenType::NUMBER, CSS::TokenFlag::NUMBER> dim(10.0f);
//
// Defined normatively by:
// https://www.w3.org/TR/css-syntax-3/#tokenization

namespace CSS {

	template<TokenType type, TokenFlag flag = TokenFlag::NULL_FLAG, typename numericType = void *>
	class Token {
	  public:
		Token() {}
	};

#define DECL_MULTIPLE_CODEPOINTS(T)                     \
template<>                                              \
class Token<T> {                                        \
  public:                                               \
    using CharVector = std::vector<Unicode::CodePoint>; \
                                                        \
    explicit Token(const CharVector &vec) : vec(vec) {  \
    }                                                   \
                                                        \
    [[nodiscard]] const CharVector &                    \
    Data() const noexcept {                             \
        return vec;                                     \
    }                                                   \
                                                        \
  private:                                              \
    CharVector vec;                                     \
}

	// <ident-token>, <function-token>, <at-keyword-token>, <hash-token>,
	// <string-token>, and <url-token> all have zero or more CodePoint's
	// stored. (<hash-token> is declared differently, though).
	DECL_MULTIPLE_CODEPOINTS(TokenType::IDENT);
	DECL_MULTIPLE_CODEPOINTS(TokenType::FUNCTION);
	DECL_MULTIPLE_CODEPOINTS(TokenType::AT_KEYWORD);
	DECL_MULTIPLE_CODEPOINTS(TokenType::STRING);
	DECL_MULTIPLE_CODEPOINTS(TokenType::URL);

} // namespace CSS
