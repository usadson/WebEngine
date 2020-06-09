/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "doctype_name.hpp"

#include <iostream>
#include <vector>

bool
HTML::Tokenizer::DoctypeName::Parse() {
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
				context.state = HTML::Tokenizer::ParserState::AFTER_DOCTYPE_NAME;
				break;
			case '>':
				std::cout << "Doctype name: " << context.doctypeToken.name.value() << std::endl;
				tokenizer.treeConstructor.EmitToken(context.doctypeToken);
				context.doctypeToken = HTML::Tokenizer::DoctypeToken(); // reset
				context.state = HTML::Tokenizer::ParserState::DATA;
				break;
			case '\0':
				context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_NULL_CHARACTER);
				context.doctypeToken.name = context.doctypeToken.name.value() + Unicode::REPLACEMENT_CHARACTER;
				break;
			default:
				if (context.character >= 0x41 && context.character <= 0x5A) { // Is uppercase
					context.doctypeToken.name
						= context.doctypeToken.name.value() + static_cast<char>(context.character + 0x20);
					;
				} else {
					context.doctypeToken.name = context.doctypeToken.name.value() + context.character;
				}
				break;
		}
	}

	return true;
}
