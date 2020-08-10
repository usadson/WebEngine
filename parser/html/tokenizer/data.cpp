/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "data.hpp"

#include <iostream>
#include <vector>

bool
HTML::Tokenizer::Data::Parse() {
	if (context.eof) {
		tokenizer.treeConstructor.EmitEOFToken();
	} else {
		switch (context.character) {
			case Unicode::AMPERSAND:
				context.returnState = HTML::Tokenizer::ParserState::DATA;
				context.state = HTML::Tokenizer::ParserState::CHARACTER_REFERENCE;
				break;
			case Unicode::LESS_THAN_SIGN:
				context.state = HTML::Tokenizer::ParserState::TAG_OPEN;
				break;
			case Unicode::NULL_CHARACTER:
				context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_NULL_CHARACTER);
				tokenizer.treeConstructor.EmitCharacterToken(context.character);
				break;
			default:
				tokenizer.treeConstructor.EmitCharacterToken(context.character);
				break;
		}
	}

	return true;
}
