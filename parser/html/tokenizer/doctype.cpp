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
			case '\t':
			case '\n':
			case '\f':
			case ' ':
				context.state = HTML::Tokenizer::ParserState::BEFORE_DOCTYPE_NAME;
				break;
			case '>':
				context.reconsume = true;
				context.state = HTML::Tokenizer::ParserState::BEFORE_DOCTYPE_NAME;
				break;
			default:
				std::cout << "\ninvalid context.character: (" << static_cast<size_t>(context.character) << ")\n"
						  << std::endl;
				context.LogError(HTML::Tokenizer::ParserError::MISSING_WHITESPACE_BEFORE_DOCTYPE_NAME);
				context.reconsume = true;
				context.state = HTML::Tokenizer::ParserState::BEFORE_DOCTYPE_NAME;
				break;
		}
	}

	return true;
}
