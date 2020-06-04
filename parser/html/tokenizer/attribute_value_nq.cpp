/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "attribute_value_nq.hpp"

#include <iostream>
#include <vector>

bool
HTML::Tokenizer::AttributeValueNQ::Parse() {
	if (context.eof) {
		context.LogError(HTML::Tokenizer::ParserError::EOF_IN_TAG);
		tokenizer.treeConstructor.EmitEOFToken();
	} else {
		HTML::Tokenizer::AmbiguousTagToken &tagToken = context.isEndTag ?
					static_cast<HTML::Tokenizer::AmbiguousTagToken &>(context.endTagToken) :
					static_cast<HTML::Tokenizer::AmbiguousTagToken &>(context.startTagToken);

		switch (context.character) {
			case '\t':
			case '\n':
			case '\f':
			case ' ':
				context.state = HTML::Tokenizer::ParserState::BEFORE_ATTRIBUTE_NAME;
				break;
			case '&':
				context.returnState = HTML::Tokenizer::ParserState::ATTRIBUTE_VALUE_NQ;
				context.state = HTML::Tokenizer::ParserState::CHARACTER_REFERENCE;
				break;
			case '>':
				context.LogError(HTML::Tokenizer::ParserError::MISSING_ATTRIBUTE_VALUE);
				context.state = HTML::Tokenizer::ParserState::DATA;
				tokenizer.treeConstructor.EmitToken(tagToken);
				if (context.isEndTag)
					context.startTagToken = HTML::Tokenizer::StartTagToken(); // Reset
				else
					context.endTagToken = HTML::Tokenizer::EndTagToken(); // Reset
				break;
			case '\0':
				context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_NULL_CHARACTER);
				tagToken.attributeValue += Unicode::REPLACEMENT_CHARACTER;
				break;
			case '"':
			case '\'':
			case '<':
			case '=':
			case '`':
				context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_CHARACTER_IN_UNQOUTED_ATTRIBUTE_VALUE);
				/* Intentional fallthrough */
			default:
				tagToken.attributeValue += context.character;
				break;
		}
	}

	return true;
}
