#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <tuple>

#include "parser/css/stylesheet.hpp"
#include "parser/css/tokenizer.hpp"

namespace CSS::Parser {

	namespace Algorithms {

		// Consume a list of rules
		//
		// [Spec]
		// Title: CSS Syntax Module Level 3
		// Section: 5.4.1
		// URL: https://www.w3.org/TR/css-syntax-3/#consume-list-of-rules
		[[nodiscard]] bool
		ConsumeListOfRules(bool topFlag, std::vector<Rule> &output) noexcept;

	} // namespace Algorithms

	// Parse a stylesheet
	//
	// [Spec]
	// Title: CSS Syntax Module Level 3
	// Section: 5.3.2.
	// URL: https://www.w3.org/TR/css-syntax-3/#parse-stylesheet
	[[nodiscard]] std::tuple<bool, Stylesheet>
	ParseStylesheet(const Tokenizer &) noexcept;

} // namespace CSS::Parser
