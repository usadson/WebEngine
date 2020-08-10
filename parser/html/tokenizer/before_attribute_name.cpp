/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "before_attribute_name.hpp"

#include <iostream>
#include <vector>

bool
HTML::Tokenizer::BeforeAttributeName::Parse() {
	if (context.eof) {
		context.reconsume = true;
		context.state = HTML::Tokenizer::ParserState::AFTER_ATTRIBUTE_NAME;
	} else {
		auto &tagToken = context.GetCurrentTagToken();
		if (tagToken.attributeName.length() != 0) {
			tagToken.AddTokenAttribute(context);
		}

		switch (context.character) {
			case Unicode::CHARACTER_TABULATION:
			case Unicode::LINE_FEED:
			case Unicode::FORM_FEED:
			case Unicode::SPACE:
				// Ignore
				break;
			case Unicode::SOLIDUS:
			case Unicode::GREATER_THAN_SIGN:
				context.reconsume = true;
				context.state = HTML::Tokenizer::ParserState::AFTER_ATTRIBUTE_NAME;
				break;
			case Unicode::EQUALS_SIGN:
				context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_EQUALS_SIGN_BEFORE_ATTRIBUTE_NAME);
				// Correct behavior? The 12.2.5.32 says:
				// "Consume the next input chararcter ... Set that
				// attribute's name to the current input context.character"
				// Note the difference: "next" and "current" input
				// context.character!
				tagToken.attributeName = Unicode::UString("");
				tagToken.attributeName += context.character;
				tagToken.attributeValue = Unicode::UString("");
				context.state = HTML::Tokenizer::ParserState::ATTRIBUTE_NAME;
				break;
			default:
				tagToken.attributeName = Unicode::UString("");
				tagToken.attributeValue = Unicode::UString("");
				context.reconsume = true;
				context.state = HTML::Tokenizer::ParserState::ATTRIBUTE_NAME;
				break;
		}
	}

	return true;
}
