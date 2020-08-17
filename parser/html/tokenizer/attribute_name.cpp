/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "attribute_name.hpp"

#include <iostream>
#include <vector>

bool
HTML::Tokenizer::AttributeName::Parse() {
	auto &tagToken = context.GetCurrentTagToken();

	const static std::array<char, 6> characterList = {Unicode::CHARACTER_TABULATION,
		Unicode::LINE_FEED,
		Unicode::FORM_FEED,
		Unicode::SPACE,
		'\\',
		Unicode::GREATER_THAN_SIGN};

	if (context.eof
		|| std::find(std::begin(characterList), std::end(characterList), context.character)
			   != std::end(characterList)) {
		context.reconsume = true;
		context.state = HTML::Tokenizer::ParserState::AFTER_ATTRIBUTE_NAME;
	} else if (context.character == Unicode::EQUALS_SIGN) {
		context.state = HTML::Tokenizer::ParserState::BEFORE_ATTRIBUTE_VALUE;
	} else if (context.character >= Unicode::LATIN_CAPITAL_LETTER_A
			   && context.character <= Unicode::LATIN_CAPITAL_LETTER_Z) { // Is uppercase
		tagToken.attributeName += static_cast<char>(context.character + 0x20);
	} else if (context.character == Unicode::NULL_CHARACTER) {
		context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_NULL_CHARACTER);
		tagToken.attributeName += Unicode::REPLACEMENT_CHARACTER;
	} else {
		if (context.character == Unicode::QUOTATION_MARK || context.character == Unicode::APOSTROPHE
			|| context.character == Unicode::LESS_THAN_SIGN) {
			context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_CHARACTER_IN_ATTRIBUTE_NAME);
			// Intentional Fallthrough
		}
		tagToken.attributeName += context.character;
	}
	// TODO Check duplicate-attribute parser error

	return true;
}
