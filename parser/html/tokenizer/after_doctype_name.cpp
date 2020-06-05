/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "after_doctype_name.hpp"

#include <iostream>
#include <vector>

bool
HTML::Tokenizer::AfterDoctypeName::Parse() {
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
			case '>':
				tokenizer.treeConstructor.EmitToken(context.doctypeToken);
				context.doctypeToken = HTML::Tokenizer::DoctypeToken(); // reset
				context.state = HTML::Tokenizer::ParserState::DATA;
				break;
			default:
				if (context.i + 5 < context.documentSize) { // 5 or 6 ? not sure..
					if (context.document->data.EqualsIgnoreCaseAL(context.i, "PUBLIC", 6)) {
						context.toConsumeNext = 5;
						context.state = HTML::Tokenizer::ParserState::AFTER_DOCTYPE_PUBLIC_KEYWORD;
						break;
					}

					if (context.document->data.EqualsIgnoreCaseAL(context.i, "SYSTEM", 6)) {
						context.toConsumeNext = 5;
						context.state = HTML::Tokenizer::ParserState::AFTER_DOCTYPE_SYSTEM_KEYWORD;
						break;
					}
				}

				context.LogError(HTML::Tokenizer::ParserError::INVALID_CHARACTER_SEQUENCE_AFTER_DOCTYPE_NAME);
				context.doctypeToken.forceQuirks = true;
				context.reconsume = true;
				context.state = HTML::Tokenizer::ParserState::BOGUS_DOCTYPE;
				break;
		}
	}

	return true;
}