/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "after_attribute_value_quoted.hpp"

#include <iostream>
#include <vector>

bool
HTML::Tokenizer::AfterAttributeValueQuoted::Parse() {
	if (context.eof) {
		context.LogError(HTML::Tokenizer::ParserError::EOF_IN_TAG);
		tokenizer.treeConstructor.EmitEOFToken();
	} else {
		switch (context.character) {
			case Unicode::CHARACTER_TABULATION:
			case Unicode::LINE_FEED:
			case Unicode::FORM_FEED:
			case Unicode::SPACE:
				context.state = HTML::Tokenizer::ParserState::BEFORE_ATTRIBUTE_NAME;
				break;
			case Unicode::SOLIDUS:
				context.state = HTML::Tokenizer::ParserState::SELF_CLOSING_START;
				break;
			case Unicode::GREATER_THAN_SIGN:
				context.state = HTML::Tokenizer::ParserState::DATA;
				tokenizer.treeConstructor.EmitToken(context.GetCurrentTagToken());
				if (context.isEndTag)
					context.startTagToken = HTML::Tokenizer::StartTagToken(); // Reset
				else
					context.endTagToken = HTML::Tokenizer::EndTagToken(); // Reset
				break;
			default:
				context.LogError(HTML::Tokenizer::ParserError::MISSING_WHITESPACE_BETWEEN_ATTRIBUTES);
				context.reconsume = true;
				context.state = HTML::Tokenizer::ParserState::BEFORE_ATTRIBUTE_NAME;
				break;
		}
	}

	return true;
}
