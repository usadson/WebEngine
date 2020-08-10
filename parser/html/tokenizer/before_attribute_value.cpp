/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "before_attribute_value.hpp"

#include <iostream>
#include <vector>

bool
HTML::Tokenizer::BeforeAttributeValue::Parse() {
	if (context.eof) {
		context.reconsume = true;
		context.state = HTML::Tokenizer::ParserState::ATTRIBUTE_VALUE_NQ;
	} else {
		auto &tagToken = context.GetCurrentTagToken();

		switch (context.character) {
			case Unicode::CHARACTER_TABULATION:
			case Unicode::LINE_FEED:
			case Unicode::FORM_FEED:
			case Unicode::SPACE:
				// Ignore
				break;
			case Unicode::QUOTATION_MARK:
				context.state = HTML::Tokenizer::ParserState::ATTRIBUTE_VALUE_DQ;
				break;
			case Unicode::APOSTROPHE:
				context.state = HTML::Tokenizer::ParserState::ATTRIBUTE_VALUE_SQ;
				break;
			case Unicode::GREATER_THAN_SIGN:
				context.LogError(HTML::Tokenizer::ParserError::MISSING_ATTRIBUTE_VALUE);
				context.state = HTML::Tokenizer::ParserState::DATA;
				tokenizer.treeConstructor.EmitToken(tagToken);
				if (context.isEndTag)
					context.startTagToken = HTML::Tokenizer::StartTagToken(); // Reset
				else
					context.endTagToken = HTML::Tokenizer::EndTagToken(); // Reset
				break;
			default:
				context.reconsume = true;
				context.state = HTML::Tokenizer::ParserState::ATTRIBUTE_VALUE_NQ;
				break;
		}
	}

	return true;
}
