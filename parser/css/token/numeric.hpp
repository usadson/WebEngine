#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <vector>

#include "data/text/unicode.hpp"
#include "parser/css/token/base.hpp"
#include "parser/css/token/flag.hpp"
#include "parser/css/token/type.hpp"

/**
 * CSS Parse Tokens
 *
 * Specialization of <hash-token>
 *
 * Defined normatively by:
 * https://www.w3.org/TR/css-syntax-3/#tokenization
 */

namespace CSS {

#define DECL_NUMBERIC_TOKEN(T, F, N)                    \
template<>                                              \
class Token<T, F, N> {                                  \
  public:                                               \
    explicit Token(N value) : value(value) {            \
    }                                                   \
                                                        \
    [[nodiscard]] N                                     \
    Data() const noexcept {                             \
        return value;                                   \
    }                                                   \
                                                        \
  private:                                              \
    N value;                                            \
}

	/**
	 * <number-token>, <percentage-token>, and <dimension-token> have a numeric
	 * value. <number-token> and <dimension-token> additionally have a type flag
	 * set to either "integer" or "number". The type flag defaults to "integer"
	 * if not otherwise set. <dimension-token> additionally have a unit composed
	 * of one or more code points.
	 */

	DECL_NUMBERIC_TOKEN(TokenType::NUMBER, TokenFlag::NUMBER, double);
	DECL_NUMBERIC_TOKEN(TokenType::NUMBER, TokenFlag::INTEGER, int);
	DECL_NUMBERIC_TOKEN(TokenType::PERCENTAGE, TokenFlag::INTEGER, int);

	template<>
	class Token<TokenType::DIMENSION, TokenFlag::INTEGER, int> {
	  public:
		using CharVector = std::vector<Unicode::CodePoint>;

		Token(int number, const CharVector &vec)
			: number(number), vec(vec) {
		}

		[[nodiscard]] int
		Number() const noexcept {
			return number;
		}

		[[nodiscard]] const CharVector &
		Data() const noexcept {
			return vec;
		}

	  private:
		int number;
		CharVector vec;
	};

	template<>
	class Token<TokenType::DIMENSION, TokenFlag::NUMBER, double> {
	  public:
		using CharVector = std::vector<Unicode::CodePoint>;

		Token(double number, const CharVector &vec)
			: number(number), vec(vec) {
		}

		[[nodiscard]] double
		Number() const noexcept {
			return number;
		}

		[[nodiscard]] const CharVector &
		Data() const noexcept {
			return vec;
		}

	  private:
		double number;
		CharVector vec;
	};

} // namespace CSS
