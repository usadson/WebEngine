/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "comment.hpp"

#include <iostream>
#include <vector>

bool
HTML::Tokenizer::Comment::Parse() {
	if (context.eof) {
		context.LogError(HTML::Tokenizer::ParserError::EOF_IN_COMMENT);
		tokenizer.treeConstructor.EmitToken(context.commentToken);
		tokenizer.treeConstructor.EmitEOFToken();
	} else {
		if (context.character == '<') {
			context.commentToken.contents += '<';
			context.state = HTML::Tokenizer::ParserState::COMMENT_LTS;
		} else if (context.character == '-') {
			context.state = HTML::Tokenizer::ParserState::COMMENT_END_DASH;
		} else if (context.character == '\0') {
			context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_NULL_CHARACTER);
			context.commentToken.contents += Unicode::REPLACEMENT_CHARACTER;
		} else {
			context.commentToken.contents += context.character;
		}
	}

	return true;
}
