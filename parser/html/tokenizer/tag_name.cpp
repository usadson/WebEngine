/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "tag_name.hpp"

#include <iostream>
#include <vector>

bool
HTML::Tokenizer::TagName::Parse() {
	if (context.eof) {
		std::cout << "EOF IN TAG NAME TODO" << std::endl;
	} else {
		auto &tagToken = context.GetCurrentTagToken();
		switch (context.character) {
			case '\t':
			case '\n':
			case '\f':
			case ' ':
				context.state = HTML::Tokenizer::ParserState::BEFORE_ATTRIBUTE_NAME;
				break;
			case '/':
				context.state = HTML::Tokenizer::ParserState::SELF_CLOSING_START;
				break;
			case '>':
				if (context.isEndTag)
					tokenizer.treeConstructor.EmitToken(context.endTagToken);
				else
					tokenizer.treeConstructor.EmitToken(context.startTagToken);
				tagToken = HTML::Tokenizer::AmbiguousTagToken::INVALID_TYPE;
				context.state = HTML::Tokenizer::ParserState::DATA;
				break;
			case '\0':
				context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_NULL_CHARACTER);
				tagToken.tagName += Unicode::REPLACEMENT_CHARACTER;
				break;
			default:
				if (context.character >= 0x41 && context.character <= 0x5A) {// Is uppercase
					tagToken.tagName += static_cast<char>(context.character + 0x20);
				} else {
					tagToken.tagName += context.character;
				}
				break;
		}
	}

	return true;
}
