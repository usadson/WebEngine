/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "comment_start_dash.hpp"

#include <iostream>
#include <vector>

bool
HTML::Tokenizer::CommentStartDash::Parse() {
	if (context.eof) {
		context.LogError(HTML::Tokenizer::ParserError::EOF_IN_COMMENT);
		tokenizer.treeConstructor.EmitToken(context.commentToken);
		tokenizer.treeConstructor.EmitEOFToken();
	} else {
		if (context.character == '-') {
			context.state = HTML::Tokenizer::ParserState::COMMENT_END;
		} else if (context.character == '>') {
			context.LogError(HTML::Tokenizer::ParserError::ABRUBT_CLOSING_OF_EMPTY_COMMENT);
			context.state = HTML::Tokenizer::ParserState::DATA;
			tokenizer.treeConstructor.EmitToken(context.commentToken);
		} else {
			// Weird, is this a loop?
			// 'Append a U+002D HYPHEN-MINUS context.character (-) to the
			//  comment token's data. Reconsume in the comment state.
			context.commentToken.contents += '-';
			context.reconsume = true;
			context.state = HTML::Tokenizer::ParserState::COMMENT;
		}
	}

	return true;
}
