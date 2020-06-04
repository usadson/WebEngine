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
	HTML::Tokenizer::AmbiguousTagToken &tagToken = context.isEndTag ?
					static_cast<HTML::Tokenizer::AmbiguousTagToken &>(context.endTagToken) :
					static_cast<HTML::Tokenizer::AmbiguousTagToken &>(context.startTagToken);

	if (context.eof || context.character == '\t'
			|| context.character == '\n'
			|| context.character == '\f'
			|| context.character == ' '
			|| context.character == '\\'
			|| context.character == '>') {
		context.reconsume = true;
		context.state = HTML::Tokenizer::ParserState::AFTER_ATTRIBUTE_NAME;
	} else if (context.character == '=') {
		context.state = HTML::Tokenizer::ParserState::BEFORE_ATTRIBUTE_VALUE;
	} else if (context.character >= 0x41 && context.character <= 0x5A) {// Is uppercase
		tagToken.attributeName += static_cast<char>(context.character + 0x20);
	} else if (context.character == '\0') {
		context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_NULL_CHARACTER);
		tagToken.attributeName += Unicode::REPLACEMENT_CHARACTER;
	} else {
		if (context.character == '"' ||
				context.character == '\'' ||
				context.character == '<') {
			context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_CHARACTER_IN_ATTRIBUTE_NAME);
			// Intentional Fallthrough
		}
		tagToken.attributeName += context.character;
	}
	// TODO Check duplicate-attribute parser error

	return true;
}
