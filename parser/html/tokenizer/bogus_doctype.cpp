/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "bogus_doctype.hpp"

#include <iostream>
#include <vector>

bool
HTML::Tokenizer::BogusDoctype::Parse() {
	if (context.eof) {
		context.LogError(HTML::Tokenizer::ParserError::EOF_IN_DOCTYPE);
		tokenizer.treeConstructor.EmitToken(context.doctypeToken);
		tokenizer.treeConstructor.EmitEOFToken();
	} else {
		switch (context.character) {
			case '>':
				tokenizer.treeConstructor.EmitToken(context.doctypeToken);
				context.doctypeToken = HTML::Tokenizer::DoctypeToken(); // reset
				context.state = HTML::Tokenizer::ParserState::DATA;
				break;
			case '\0':
				context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_NULL_CHARACTER);
				// Ignore
				break;
			default:
				// Ignore
				break;
		}
	}

	return true;
}
