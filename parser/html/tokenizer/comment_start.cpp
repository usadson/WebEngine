/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "comment_start.hpp"

#include <iostream>
#include <vector>

bool
HTML::Tokenizer::CommentStart::Parse() {
	if (context.character == '-') {
		context.state = HTML::Tokenizer::ParserState::COMMENT_START_DASH;
	} else if (context.character == '>') {
		context.LogError(HTML::Tokenizer::ParserError::ABRUBT_CLOSING_OF_EMPTY_COMMENT);
		context.state = HTML::Tokenizer::ParserState::DATA;
		tokenizer.treeConstructor.EmitToken(context.commentToken);
	} else {
		context.reconsume = true;
		context.state = HTML::Tokenizer::ParserState::COMMENT;
	}

	return true;
}
