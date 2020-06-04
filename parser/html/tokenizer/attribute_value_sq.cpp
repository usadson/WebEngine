/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "attribute_value_sq.hpp"

#include <iostream>
#include <vector>

bool
HTML::Tokenizer::AttributeValueSQ::Parse() {
	if (context.eof) {
		context.LogError(HTML::Tokenizer::ParserError::EOF_IN_TAG);
		tokenizer.treeConstructor.EmitEOFToken();
	} else {
		HTML::Tokenizer::AmbiguousTagToken &tagToken = context.isEndTag ?
					static_cast<HTML::Tokenizer::AmbiguousTagToken &>(context.endTagToken) :
					static_cast<HTML::Tokenizer::AmbiguousTagToken &>(context.startTagToken);

		switch (context.character) {
			case '\'':
				context.state = HTML::Tokenizer::ParserState::AFTER_ATTRIBUTE_VALUE_QUOTED;
				break;
			case '&':
				context.returnState = HTML::Tokenizer::ParserState::ATTRIBUTE_VALUE_SQ;
				context.state = HTML::Tokenizer::ParserState::CHARACTER_REFERENCE;
				break;
			case '\0':
				context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_NULL_CHARACTER);
				tagToken.attributeValue += Unicode::REPLACEMENT_CHARACTER;
				break;
			default:
				tagToken.attributeValue += context.character;
				break;
		}
	}

	return true;
}
