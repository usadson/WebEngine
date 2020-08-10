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
		return true;
	}

	switch (context.character) {
		case Unicode::CHARACTER_TABULATION:
		case Unicode::LINE_FEED:
		case Unicode::FORM_FEED:
		case Unicode::SPACE:
			context.state = HTML::Tokenizer::ParserState::BEFORE_ATTRIBUTE_NAME;
			break;
		case Unicode::AMPERSAND:
			context.returnState = HTML::Tokenizer::ParserState::ATTRIBUTE_VALUE_NQ;
			context.state = HTML::Tokenizer::ParserState::CHARACTER_REFERENCE;
			break;
		case Unicode::GREATER_THAN_SIGN:
			context.LogError(HTML::Tokenizer::ParserError::MISSING_ATTRIBUTE_VALUE);
			context.state = HTML::Tokenizer::ParserState::DATA;
			tokenizer.treeConstructor.EmitToken(context.GetCurrentTagToken());
			if (context.isEndTag)
				context.startTagToken = HTML::Tokenizer::StartTagToken();
			else
				context.endTagToken = HTML::Tokenizer::EndTagToken();
			break;
		case Unicode::NULL_CHARACTER:
			context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_NULL_CHARACTER);
			context.GetCurrentTagToken().attributeValue += Unicode::REPLACEMENT_CHARACTER;
			break;
		default:
			if (context.character == Unicode::QUOTATION_MARK || context.character == Unicode::APOSTROPHE || context.character == Unicode::LESS_THAN_SIGN
				|| context.character == Unicode::EQUALS_SIGN || context.character == Unicode::GRAVE_ACCENT)
				context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_CHARACTER_IN_UNQOUTED_ATTRIBUTE_VALUE);
			context.GetCurrentTagToken().attributeValue += context.character;
			break;
	}

	return true;
}
