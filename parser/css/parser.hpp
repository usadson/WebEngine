#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <tuple>

#include "parser/css/stylesheet.hpp"
#include "parser/css/tokenizer.hpp"

#ifndef PRIVATE_VISIBILITY
#define PRIVATE_VISIBILITY private
#endif

namespace CSS {

	class Parser {
	  public:
		inline explicit Parser(const Tokenizer &tokenizer) : tokenizer(tokenizer) {}

		// Parse a stylesheet
		//
		// [Spec]
		// Title: CSS Syntax Module Level 3
		// Section: 5.3.2.
		// URL: https://www.w3.org/TR/css-syntax-3/#parse-stylesheet
		[[nodiscard]] Stylesheet
		ParseStylesheet() noexcept;
		
	  PRIVATE_VISIBILITY:
		const Tokenizer &tokenizer;
		bool errorOccurred = false;
		bool topLevelFlag = false;

		// Consume a list of rules
		//
		// [Spec]
		// Title: CSS Syntax Module Level 3
		// Section: 5.4.1
		// URL: https://www.w3.org/TR/css-syntax-3/#consume-list-of-rules
		void
		ConsumeListOfRules(std::vector<Rule> &) noexcept;

		// Consume a list of rules
		//
		// The bool return type indicates whether the iterator was manually
		// modified or not, i.e. if modified some tokens were consumed.
		//
		// [Spec]
		// Title: CSS Syntax Module Level 3
		// Section: 5.4.3
		// URL: https://www.w3.org/TR/css-syntax-3/#consume-a-qualified-rule
		[[nodiscard]] bool
		ConsumeQualifiedRule(std::vector<Token>::const_iterator &, std::vector<Rule> &) noexcept;
	};

} // namespace CSS
