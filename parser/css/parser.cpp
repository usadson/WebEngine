/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "parser/css/parser.hpp"
#include "logger.hpp"

namespace CSS {

	Stylesheet
	Parser::ParseStylesheet() noexcept {
		topLevelFlag = true;

		Stylesheet sheet;
		this->rules = &sheet.rules;
		ConsumeListOfRules();
		return sheet;
	}

	void
	Parser::ConsumeListOfRules() noexcept {
		this->it = std::begin(tokenizer.tokens);
		this->endIterator = std::end(tokenizer.tokens);
		for (; it != endIterator;) {
			const auto &token = *it;
			bool wasItMutatedManually = false;
			// NOTE that the subroutines called from the following switch
			// likely mutates the 'it'erator.

			switch (token.type) {
				case Token::Type::WHITESPACE:
					break;
				case Token::Type::CDC:
				case Token::Type::CDO:
					if (!topLevelFlag && !ConsumeQualifiedRule()) {
						wasItMutatedManually = true;
					}
					break;
				case Token::Type::AT_KEYWORD:
					if (!ConsumeAtRule()) {
						wasItMutatedManually = true;
					}
					break;
				default:
					if (!topLevelFlag && !ConsumeQualifiedRule()) {
						wasItMutatedManually = true;
					}
					break;
			}

			if (!wasItMutatedManually) {
				++it;
			}
		}
	}

	bool
	Parser::ConsumeAtRule() noexcept {
		return false;
	}

	bool
	Parser::ConsumeQualifiedRule() noexcept {
		for (; it != endIterator; ++it) {
		}

		// EOF is reached, consume all the tokens but we can't make a qualified
		// rule out of it unfortunately :(.
		Logger::Error(__FUNCTION__, "Parse error: EOF reached!");
		return true;
	}

	void
	Parser::ConsumeDeclaration() noexcept {
		const std::vector<Unicode::CodePoint> &name = std::get_if<TokenCodePointsData>(&it->data)->codePoints;
		while (it != endIterator && it->type == Token::Type::WHITESPACE) {
			++it;
		}
		if (it == endIterator || it->type != Token::Type::COLON) {
			Logger::Error(__FUNCTION__, "Parse error: ident should be followed by a colon!");
			return;
		}
		++it;
		while (it != endIterator && it->type == Token::Type::WHITESPACE) {
			++it;
		}
	}

} // namespace CSS
