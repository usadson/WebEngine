/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "tokenizer.hpp"

#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <vector>

#include <cstdlib>
#include <cstring>
#include <strings.h>

#include "data/text/named_characters.hpp"
#include "logger.hpp"
#include "parser/html/tokenizer/tokenizer_parser.hpp"
#include "parser/html/context.hpp"
#include "parser/html/error.hpp"
#include "parser/html/state.hpp"
#include "parser/html/token.hpp"
#include "parser/html/tree_constructor.hpp"

void
HTML::Tokenizer::Tokenizer::Run(Resources::DocumentResource &document) {

	static const std::map<HTML::Tokenizer::ParserState, std::shared_ptr<HTML::Tokenizer::Parser>> parserMap = {
	};

	context.document = &document;

	// At what line is the tokenizer?
	context.lineCount = 1;
	// At what character position in the line is the tokenizer?
	context.linePosition = 0;

	context.documentSize = context.document->data.length();
	std::cout << "InputDataSize: " << context.documentSize << std::endl;

	// Don't use 'character' if context.eof is true.
	context.eof = false;

	context.commentToken = HTML::Tokenizer::CommentToken::INVALID_TYPE;

	context.isEndTag = false;
	context.endTagToken = HTML::Tokenizer::EndTagToken::INVALID_TYPE_END_TAG;
	context.startTagToken = HTML::Tokenizer::StartTagToken::INVALID_TYPE_START_TAG;

	context.unknownStateCount = 0;
	context.reconsume = false;
	context.toConsumeNext = 0;

	for (context.i = 0; context.i <= context.documentSize; context.i++) {
		context.character = '\0';

// 		std::cout << "index=" << context.i << " state=" << context.state << std::endl;
		if (context.reconsume) {
			context.i--;
			context.reconsume = false;
		}

		if (context.i == context.documentSize) {
			context.eof = true;
			context.linePosition += 1;
		} else {
			bool repeatLineCheckLoop = true;
			while (repeatLineCheckLoop) {
				repeatLineCheckLoop = false;
				context.character = context.document->data[context.i];

				if (context.character == '\n') {
					context.lineCount += 1;
					context.linePosition = 1;
				} else {
					// std::cout << "CharAt(" << context.i << ") = " << context.character << std::endl;
					context.linePosition += 1;
				}

				if (context.toConsumeNext > 0) {
					context.i += 1;
					context.toConsumeNext -= 1;
					repeatLineCheckLoop = true;
				}
			}
		}

		context.beginLoopState = context.state;
		context.currentCharacter = context.character;

		auto parser = parserMap.find(context.state);

		if (parser == std::end(parserMap)) {
			std::cout << "Unknown state(" << ++context.unknownStateCount << "): " << context.state << std::endl;
		} else {
			if (!parser->second->Parse()) {
				std::stringstream info("ParserError: ");
				info << context.state << " failed on " << context.linePosition
					 << ':' << context.lineCount;
				Logger::Error(__PRETTY_FUNCTION__, info.str());

				/* FIXME: Handle Error */
				return;
			}
		}
	}
}
