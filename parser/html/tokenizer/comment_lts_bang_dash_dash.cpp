/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "comment_lts_bang_dash_dash.hpp"

#include <iostream>
#include <vector>

bool
HTML::Tokenizer::CommentLTSBangDashDash::Parse() {
	if (context.eof || context.character == '>') {
		context.reconsume = true;
		context.state = HTML::Tokenizer::ParserState::COMMENT_END;
	} else {
		context.LogError(HTML::Tokenizer::ParserError::NESTED_COMMENT);
		context.reconsume = true;
		context.state = HTML::Tokenizer::ParserState::COMMENT_END;
	}

	return true;
}
