/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "character_reference.hpp"

#include <iostream>
#include <vector>

bool
HTML::Tokenizer::CharacterReference::Parse() {
	context.temporaryBuffer.clear();
	context.temporaryBuffer.push_back(Unicode::AMPERSAND);

	if (!context.eof) {
		if (context.character == Unicode::NUMBER_SIGN) {
			context.temporaryBuffer.push_back(context.character);
			context.state = HTML::Tokenizer::ParserState::NUMERIC_CHARACTER_REFERENCE;
			break;
		}

		if (Unicode::IsASCIIAlphaNumeric(context.character)) {
			context.reconsume = true;
			context.state = HTML::Tokenizer::ParserState::NAMED_CHARACTER_REFERENCE;
			break;
		}
	}

	return true;
}
