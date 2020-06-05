/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "doctype_system_identifier_sq.hpp"

#include <iostream>
#include <vector>

bool
HTML::Tokenizer::DoctypeSystemIdentifierSQ::Parse() {
	if (context.eof) {
		context.LogError(HTML::Tokenizer::ParserError::EOF_IN_DOCTYPE);
		tokenizer.treeConstructor.EmitDoctypeQuirksToken();
		tokenizer.treeConstructor.EmitEOFToken();
	} else {
		switch (context.character) {
			case '\'':
				context.state = HTML::Tokenizer::ParserState::AFTER_DOCTYPE_SYSTEM_IDENTIFIER;
				break;
			case '\0':
				context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_NULL_CHARACTER);
				context.doctypeToken.systemIdentifier = context.doctypeToken.systemIdentifier.value() + Unicode::REPLACEMENT_CHARACTER;
				break;
			case '>':
				context.LogError(HTML::Tokenizer::ParserError::ABRUBT_DOCTYPE_SYSTEM_IDENTIFIER);
				context.doctypeToken.forceQuirks = true;
				tokenizer.treeConstructor.EmitToken(context.doctypeToken);
				context.doctypeToken = HTML::Tokenizer::DoctypeToken(); // reset
				context.state = HTML::Tokenizer::ParserState::DATA;
				break;
			default:
				context.doctypeToken.systemIdentifier = context.doctypeToken.systemIdentifier.value() + context.character;
				break;
		}
	}

	return true;
}
