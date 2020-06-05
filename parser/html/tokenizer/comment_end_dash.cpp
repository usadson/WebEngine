/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "comment_end_dash.hpp"

#include <iostream>
#include <vector>

bool
HTML::Tokenizer::CommentEndDash::Parse() {
	if (context.eof) {
		context.LogError(HTML::Tokenizer::ParserError::EOF_IN_COMMENT);
		tokenizer.treeConstructor.EmitToken(context.commentToken);
		tokenizer.treeConstructor.EmitEOFToken();
	} else if (context.character == '-') {
		context.state = HTML::Tokenizer::ParserState::COMMENT_END;
	} else {
		context.commentToken.contents += '-';
		context.reconsume = true;
		context.state = HTML::Tokenizer::ParserState::COMMENT;
	}

	return true;
}
