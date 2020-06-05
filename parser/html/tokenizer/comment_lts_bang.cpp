/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "comment_lts_bang.hpp"

#include <iostream>
#include <vector>

bool
HTML::Tokenizer::CommentLTSBang::Parse() {
	if (context.character == '!') {
		context.commentToken.contents += '!';
		context.state = HTML::Tokenizer::ParserState::COMMENT_LTS_BANG;
	} else if (context.character == '<') {
		context.commentToken.contents += '<';
	} else {
		context.reconsume = true;
		context.state = HTML::Tokenizer::ParserState::COMMENT;
	}

	return true;
}
