/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "doctype_public_identifier_dq.hpp"

#include <iostream>
#include <vector>

bool
HTML::Tokenizer::DoctypePublicIdentifierDQ::Parse() {
	if (context.eof) {
		context.LogError(HTML::Tokenizer::ParserError::EOF_IN_DOCTYPE);
		tokenizer.treeConstructor.EmitDoctypeQuirksToken();
		tokenizer.treeConstructor.EmitEOFToken();
	} else {
		switch (context.character) {
			case Unicode::QUOTATION_MARK:
				context.state = HTML::Tokenizer::ParserState::AFTER_DOCTYPE_PUBLIC_IDENTIFIER;
				break;
			case Unicode::NULL_CHARACTER:
				context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_NULL_CHARACTER);
				context.doctypeToken.publicIdentifier
					= context.doctypeToken.publicIdentifier.value() + Unicode::REPLACEMENT_CHARACTER;
				break;
			case Unicode::GREATER_THAN_SIGN:
				context.LogError(HTML::Tokenizer::ParserError::ABRUBT_DOCTYPE_PUBLIC_IDENTIFIER);
				context.doctypeToken.forceQuirks = true;
				tokenizer.treeConstructor.EmitToken(context.doctypeToken);
				context.doctypeToken = HTML::Tokenizer::DoctypeToken(); // reset
				context.state = HTML::Tokenizer::ParserState::DATA;
				break;
			default:
				context.doctypeToken.publicIdentifier
					= context.doctypeToken.publicIdentifier.value() + context.character;
				break;
		}
	}

	return true;
}
