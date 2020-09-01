#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "parser/css/tokenizer.hpp"

namespace CSS {

	// [Spec]
	// Title: CSS Syntax Module Level 3
	// Section: 5.1.1.
	// URL: https://www.w3.org/TR/css-syntax-3/#stylesheet-diagram
	[[nodiscard]] bool
	ParseStylesheet(const Tokenizer &) noexcept;
	struct Stylesheet {
		using Element = std::variant<Token::Type/* qualified-rule, at-rule*/>;
		// TokenType can be WHITESPACE, CDC and CDO
		
		std::vector<Element> elements;
	};

} // namespace CSS::Parser
