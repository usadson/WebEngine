/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "before_doctype_name.hpp"

#include <iostream>
#include <vector>

bool
HTML::Tokenizer::BeforeDoctypeName::Parse() {
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
				// Ignore
				break;
			case '>': {
				context.LogError(HTML::Tokenizer::ParserError::MISSING_DOCTYPE_NAME);
				tokenizer.treeConstructor.EmitDoctypeQuirksToken();
				context.state = HTML::Tokenizer::ParserState::DATA;
			} break;
			case '\0': {
				context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_NULL_CHARACTER);
				context.doctypeToken.name.emplace(Unicode::REPLACEMENT_CHARACTER);
				context.state = HTML::Tokenizer::ParserState::DOCTYPE_NAME;
			} break;
			default:
				if (context.character >= 0x41 && context.character <= 0x5A) { // Is uppercase
					context.doctypeToken.name.emplace("");
					context.doctypeToken.name.value() += static_cast<char>(context.character + 0x20);
					context.state = HTML::Tokenizer::ParserState::DOCTYPE_NAME;
				} else {
					context.doctypeToken.name.emplace("");
					context.doctypeToken.name.value() += context.character;
					context.state = HTML::Tokenizer::ParserState::DOCTYPE_NAME;
				}
				break;
		}
	}

	return true;
}
