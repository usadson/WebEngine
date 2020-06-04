/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "self_closing_start.hpp"

#include <iostream>
#include <vector>

bool
HTML::Tokenizer::SelfClosingStart::Parse() {
	if (context.eof) {
		context.LogError(HTML::Tokenizer::ParserError::EOF_IN_TAG);
		tokenizer.treeConstructor.EmitEOFToken();
	} else if (context.character == '>') {
		HTML::Tokenizer::AmbiguousTagToken &tagToken = context.isEndTag ?
					static_cast<HTML::Tokenizer::AmbiguousTagToken &>(context.endTagToken) :
					static_cast<HTML::Tokenizer::AmbiguousTagToken &>(context.startTagToken);

		tagToken.selfClosing = true;
		context.state = HTML::Tokenizer::ParserState::DATA;
		tokenizer.treeConstructor.EmitToken(tagToken);

		if (context.isEndTag)
			context.startTagToken = HTML::Tokenizer::StartTagToken(); // Reset
		else
			context.endTagToken = HTML::Tokenizer::EndTagToken(); // Reset
	}

	return true;
}
