/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "doctype.hpp"

#include <iostream>
#include <vector>

bool
HTML::Tokenizer::Doctype::Parse() {
	if (context.eof) {
		context.LogError(HTML::Tokenizer::ParserError::EOF_IN_DOCTYPE);
		tokenizer.treeConstructor.EmitDoctypeQuirksToken();
		tokenizer.treeConstructor.EmitEOFToken();
	} else {
		switch (context.character) {
			case Unicode::CHARACTER_TABULATION:
			case Unicode::LINE_FEED:
			case Unicode::FORM_FEED:
			case Unicode::SPACE:
				context.state = HTML::Tokenizer::ParserState::BEFORE_DOCTYPE_NAME;
				break;
			case Unicode::GREATER_THAN_SIGN:
				context.reconsume = true;
				context.state = HTML::Tokenizer::ParserState::BEFORE_DOCTYPE_NAME;
				break;
			default:
				std::cout << "\ninvalid context.character: (" << static_cast<std::size_t>(context.character) << ")\n"
						  << std::endl;
				context.LogError(HTML::Tokenizer::ParserError::MISSING_WHITESPACE_BEFORE_DOCTYPE_NAME);
				context.reconsume = true;
				context.state = HTML::Tokenizer::ParserState::BEFORE_DOCTYPE_NAME;
				break;
		}
	}

	return true;
}
