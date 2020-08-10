/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "before_doctype_system_identifier.hpp"

#include <iostream>
#include <vector>

bool
HTML::Tokenizer::BeforeDoctypeSystemIdentifier::Parse() {
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
				// Ignore
				break;
			case Unicode::QUOTATION_MARK:
				context.doctypeToken.systemIdentifier.emplace("");
				context.state = HTML::Tokenizer::ParserState::DOCTYPE_SYSTEM_IDENTIFIER_DQ;
				break;
			case Unicode::APOSTROPHE:
				context.doctypeToken.systemIdentifier.emplace("");
				context.state = HTML::Tokenizer::ParserState::DOCTYPE_SYSTEM_IDENTIFIER_SQ;
				break;
			case Unicode::GREATER_THAN_SIGN:
				context.LogError(HTML::Tokenizer::ParserError::MISSING_DOCTYPE_SYSTEM_IDENTIFIER);
				context.doctypeToken.forceQuirks = true;
				tokenizer.treeConstructor.EmitToken(context.doctypeToken);
				context.doctypeToken = HTML::Tokenizer::DoctypeToken(); // reset
				context.state = HTML::Tokenizer::ParserState::DATA;
				break;
			default:
				context.LogError(HTML::Tokenizer::ParserError::MISSING_QOUTE_BEFORE_DOCTYPE_SYSTEM_IDENTIFIER);
				context.doctypeToken.forceQuirks = true;
				context.reconsume = true;
				context.state = HTML::Tokenizer::ParserState::BOGUS_DOCTYPE;
				break;
		}
	}

	return true;
}
