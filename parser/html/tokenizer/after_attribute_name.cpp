/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "after_attribute_name.hpp"

#include <iostream>
#include <vector>

bool
HTML::Tokenizer::AfterAttributeName::Parse() {
	if (context.eof) {
		context.LogError(HTML::Tokenizer::ParserError::EOF_IN_TAG);
		tokenizer.treeConstructor.EmitEOFToken();
	} else {
		switch (context.character) {
			case Unicode::CHARACTER_TABULATION:
			case Unicode::LINE_FEED:
			case Unicode::FORM_FEED:
			case Unicode::SPACE:
				// Ignore
				break;
			case Unicode::SOLIDUS:
				context.state = HTML::Tokenizer::ParserState::SELF_CLOSING_START;
				break;
			case Unicode::EQUALS_SIGN:
				context.state = HTML::Tokenizer::ParserState::BEFORE_ATTRIBUTE_VALUE;
				break;
			case Unicode::GREATER_THAN_SIGN:
				context.state = HTML::Tokenizer::ParserState::DATA;
				tokenizer.treeConstructor.EmitToken(context.GetCurrentTagToken());

				context.startTagToken = HTML::Tokenizer::StartTagToken();
				context.endTagToken = HTML::Tokenizer::EndTagToken();
				break;
			default: {
				// New attribute? Destroy old one?
				auto &tagToken = context.GetCurrentTagToken();
				tagToken.attributeName = Unicode::UString("");
				tagToken.attributeValue = Unicode::UString("");
				context.reconsume = true;
				context.state = HTML::Tokenizer::ParserState::ATTRIBUTE_NAME;
			} break;
		}
	}

	return true;
}
