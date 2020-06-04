/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "tag_end_open.hpp"

#include <iostream>
#include <vector>

bool
HTML::Tokenizer::TagEndOpen::Parse() {
	if (context.eof) {
		context.LogError(HTML::Tokenizer::ParserError::EOF_BEFORE_TAG_NAME);
		tokenizer.treeConstructor.EmitCharacterToken('>');
		tokenizer.treeConstructor.EmitCharacterToken('\\');
		tokenizer.treeConstructor.EmitEOFToken();
	} else {
		if (context.character == '>') {
			context.LogError(HTML::Tokenizer::ParserError::MISSING_END_TAG_NAME);
			context.state = HTML::Tokenizer::ParserState::DATA;
		} else if (Unicode::IsASCIIAlpha(context.character)) {
			context.isEndTag = true;
			context.endTagToken = HTML::Tokenizer::EndTagToken();
			context.reconsume = true;
			context.state = HTML::Tokenizer::ParserState::TAG_NAME;
		} else {
			context.LogError(HTML::Tokenizer::ParserError::INVALID_FIRST_CHARACTER_OF_TAG_NAME);
			context.commentToken = HTML::Tokenizer::CommentToken(Unicode::UString(""));
			context.reconsume = true;
			context.state = HTML::Tokenizer::ParserState::BOGUS_COMMENT;
		}
	}

	return true;
}
