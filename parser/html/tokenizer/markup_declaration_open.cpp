/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "markup_declaration_open.hpp"

#include <iostream>
#include <vector>

bool
HTML::Tokenizer::MarkupDeclarationOpen::Parse() {
	if (!context.eof) {
		if (context.i + 1 < context.documentSize && context.character == Unicode::HYPHEN_MINUS
			&& context.document->data[context.i + 1] == Unicode::HYPHEN_MINUS) {
			context.toConsumeNext = 1;

			context.commentToken = HTML::Tokenizer::CommentToken(Unicode::UString(""));
			context.state = HTML::Tokenizer::ParserState::COMMENT_START;
			return true;
		}

		if (context.i + 6 < context.documentSize) {
			if (context.document->data.StartsWithIgnoreCaseAL(context.i, "DOCTYPE", 7)) {
				context.toConsumeNext = 6;
				context.state = HTML::Tokenizer::ParserState::DOCTYPE;
				return true;
			}
			if (context.character == Unicode::LEFT_SQUARE_BRACKET
				&& context.document->data.EqualsAL(context.i + 1, "CDATA", 5) // Case-sensitive!
				&& context.document->data[context.i + 6] == Unicode::RIGHT_SQUARE_BRACKET) {
				// TODO ?
				throw std::runtime_error("TODO in MARKUP_DECLARATION_OPEN / CDATA");
			}
		}
	}

	context.LogError(HTML::Tokenizer::ParserError::INCORRECTLY_OPENED_COMMENT);
	context.commentToken = HTML::Tokenizer::CommentToken(Unicode::UString(""));
	context.i--; // Don't consume anything
	context.state = HTML::Tokenizer::ParserState::BOGUS_COMMENT;

	return true;
}
