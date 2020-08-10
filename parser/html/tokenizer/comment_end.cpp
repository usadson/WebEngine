/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "comment_end.hpp"

#include <iostream>
#include <vector>

bool
HTML::Tokenizer::CommentEnd::Parse() {
	if (context.eof) {
		context.LogError(HTML::Tokenizer::ParserError::EOF_IN_COMMENT);
		tokenizer.treeConstructor.EmitToken(context.commentToken);
		tokenizer.treeConstructor.EmitEOFToken();
	} else if (context.character == Unicode::GREATER_THAN_SIGN) {
		tokenizer.treeConstructor.EmitToken(context.commentToken);
		context.commentToken = HTML::Tokenizer::CommentToken::INVALID_TYPE;
		context.state = HTML::Tokenizer::ParserState::DATA;
	} else if (context.character == Unicode::EXCLAMATION_MARK) {
		context.state = HTML::Tokenizer::ParserState::COMMENT_END_BANG;
	} else if (context.character == Unicode::HYPHEN_MINUS) {
		context.commentToken.contents += Unicode::HYPHEN_MINUS;
	} else {
		context.commentToken.contents += "--";
		context.reconsume = true;
		context.state = HTML::Tokenizer::ParserState::COMMENT;
	}

	return true;
}
