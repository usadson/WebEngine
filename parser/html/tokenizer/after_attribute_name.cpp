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
		HTML::Tokenizer::AmbiguousTagToken &tagToken = context.isEndTag ?
					static_cast<HTML::Tokenizer::AmbiguousTagToken &>(context.endTagToken) :
					static_cast<HTML::Tokenizer::AmbiguousTagToken &>(context.startTagToken);

		switch (context.character) {
			case '\t':
			case '\n':
			case '\f':
			case ' ':
				// Ignore
				break;
			case '/':
				context.state = HTML::Tokenizer::ParserState::SELF_CLOSING_START;
				break;
			case '=':
				context.state = HTML::Tokenizer::ParserState::BEFORE_ATTRIBUTE_VALUE;
				break;
			case '>':
				context.state = HTML::Tokenizer::ParserState::DATA;
				tokenizer.treeConstructor.EmitToken(tagToken);
				if (context.isEndTag)
					context.startTagToken = HTML::Tokenizer::StartTagToken(); // Reset
				else
					context.endTagToken = HTML::Tokenizer::EndTagToken(); // Reset
				break;
			default:
				// New attribute? Destroy old one?
				tagToken.attributeName = Unicode::UString("");
				tagToken.attributeValue = Unicode::UString("");
				context.reconsume = true;
				context.state = HTML::Tokenizer::ParserState::ATTRIBUTE_NAME;
				break;
		}
	}

	return true;
}
