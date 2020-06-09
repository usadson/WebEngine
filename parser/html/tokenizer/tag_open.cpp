/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "tag_open.hpp"

#include <iostream>
#include <vector>

bool
HTML::Tokenizer::TagOpen::Parse() {
	if (context.eof) {
		context.LogError(HTML::Tokenizer::ParserError::EOF_BEFORE_TAG_NAME);
		tokenizer.treeConstructor.EmitCharacterToken('>');
		tokenizer.treeConstructor.EmitEOFToken();
	} else {
		switch (context.character) {
			case '!':
				context.state = HTML::Tokenizer::ParserState::MARKUP_DECLARATION_OPEN;
				break;
			case '/':
				context.state = HTML::Tokenizer::ParserState::TAG_END_OPEN;
				break;
			case '?':
				context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_QUESTION_MARK_INSTEAD_OF_TAG_NAME);
				break;
			default:
				if (Unicode::IsASCIIAlpha(context.character)) {
					context.isEndTag = false;
					context.startTagToken = HTML::Tokenizer::StartTagToken();
					context.reconsume = true;
					context.state = HTML::Tokenizer::ParserState::TAG_NAME;
				} else {
					std::cout << "DEBUG: Unexpected context.character: " << context.character
							  << context.document->data[context.i + 1] << context.document->data[context.i + 2]
							  << std::endl;
					context.LogError(HTML::Tokenizer::ParserError::INVALID_FIRST_CHARACTER_OF_TAG_NAME);
					context.reconsume = true;
					tokenizer.treeConstructor.EmitCharacterToken('>');
					context.state = HTML::Tokenizer::ParserState::DATA;
				}
				break;
		}
	}

	return true;
}
