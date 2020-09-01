/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "parser/css/parser.hpp"

namespace CSS {
	
	Stylesheet
	Parser::ParseStylesheet() noexcept {
		Stylesheet sheet;
		errorOccurred = ConsumeListOfRules(tokenizer, true, sheet.rules);
		return sheet;
	}

	bool
	Parser::ConsumeListOfRules(const Tokenizer &tokenizer, bool topLevelFlag, std::vector<Rule> &output) noexcept {
		for (auto it = std::begin(tokenizer.tokens); it != std::end(tokenizer.tokens); ) {
			const auto &token = *it;
			bool wasItMutatedManually = false;
			// NOTE that the subroutines called from the following switch
			// likely mutates the 'it'erator.

			switch (token.type) {
				case Token::Type::WHITESPACE:
					break;
				case Token::Type::CDC:
				case Token::Type::CDO:
					if (!topLevelFlag) {
						// wasItMutatedManually = true;
						// if (!ConsumeQualifiedRule(it, output)) { ... }
					}
					break;
				case Token::Type::AT_KEYWORD:
					// wasItMutatedManually = true;
					// if (!ConsumeAtRule(it, output)) { ... }
					break;
				default:
					// wasItMutatedManually = true;
					// if (!ConsumeAtRule(it, output)) { ... }
					break;
			}

			if (!wasItMutatedManually) {
				++it;
			}
		}

		return true;
	}

} // namespace CSS
