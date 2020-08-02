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

// CSS Parse Tokens
//
// Specialization of <hash-token>
//
// Defined normatively by:
// https://www.w3.org/TR/css-syntax-3/#tokenization

namespace CSS {

	template<TokenFlag flag>
	class Token<TokenType::HASH, flag> {
	  public:
		using CharVector = std::vector<Unicode::CodePoint>;

		Token(const CharVector &vec) : vec(vec) {
		}

		[[nodiscard]] const CharVector &
		Data() const noexcept {
			return vec;
		}

	  private:
		CharVector vec;
	};

} // namespace CSS
