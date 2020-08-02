#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <vector>

#include "data/text/unicode.hpp"
#include "parser/css/token_base.hpp"
#include "parser/css/token_flag.hpp"
#include "parser/css/token_type.hpp"

// CSS Parse Tokens
//
// Specialization of <delim-token>
//
// Defined normatively by:
// https://www.w3.org/TR/css-syntax-3/#tokenization

namespace CSS {

	template<>
	class Token<TokenType::DELIM> {
	  public:
		Token(Unicode::CodePoint character) noexcept : character(character) {
		}

		[[nodiscard]] inline Unicode::CodePoint
		Character() const noexcept {
			return character;
		}
	  private:
		Unicode::CodePoint character;
	};

} // namespace CSS
