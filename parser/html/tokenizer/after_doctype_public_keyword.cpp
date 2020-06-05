/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "after_doctype_public_keyword.hpp"

#include <iostream>
#include <vector>

bool
HTML::Tokenizer::AfterDoctypePublicKeyword::Parse() {
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
				context.state = HTML::Tokenizer::ParserState::BEFORE_DOCTYPE_PUBLIC_IDENTIFIER;
				break;
			case '"':
				context.LogError(HTML::Tokenizer::ParserError::MISSING_WHITESPACE_AFTER_DOCTYPE_PUBLIC_KEYWORD);
				context.doctypeToken.publicIdentifier.emplace("");
				context.state = HTML::Tokenizer::ParserState::DOCTYPE_PUBLIC_IDENTIFIER_DQ;
				break;
			case '\'':
				context.LogError(HTML::Tokenizer::ParserError::MISSING_WHITESPACE_AFTER_DOCTYPE_PUBLIC_KEYWORD);
				context.doctypeToken.publicIdentifier.emplace("");
				context.state = HTML::Tokenizer::ParserState::DOCTYPE_PUBLIC_IDENTIFIER_SQ;
				break;
			case '>':
				context.LogError(HTML::Tokenizer::ParserError::MISSING_DOCTYPE_PUBLIC_IDENTIFIER);
				context.doctypeToken.forceQuirks = true;
				tokenizer.treeConstructor.EmitToken(context.doctypeToken);
				context.doctypeToken = HTML::Tokenizer::DoctypeToken(); // reset
				context.state = HTML::Tokenizer::ParserState::DATA;
				break;
			default:
				std::cout << "At AFTER_DOCTYPE_PUBLIC_KEYWORD" << std::endl;
				context.LogError(HTML::Tokenizer::ParserError::MISSING_QOUTE_BEFORE_DOCTYPE_PUBLIC_IDENTIFIER);
				context.doctypeToken.forceQuirks = true;
				context.reconsume = true;
				context.state = HTML::Tokenizer::ParserState::BOGUS_DOCTYPE;
				break;
		}
	}

	return true;
}
