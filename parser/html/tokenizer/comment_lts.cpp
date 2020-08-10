/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "comment_lts.hpp"

#include <iostream>
#include <vector>

bool
HTML::Tokenizer::CommentLTS::Parse() {
	if (context.character == Unicode::EXCLAMATION_MARK) {
		context.commentToken.contents += Unicode::EXCLAMATION_MARK;
		context.state = HTML::Tokenizer::ParserState::COMMENT_LTS_BANG;
	} else if (context.character == Unicode::LESS_THAN_SIGN) {
		context.commentToken.contents += Unicode::LESS_THAN_SIGN;
	} else {
		context.reconsume = true;
		context.state = HTML::Tokenizer::ParserState::COMMENT;
	}

	return true;
}
