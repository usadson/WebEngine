/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "tokenizer.hpp"

#include <iostream>
#include <memory>
#include <vector>

#include <cstdlib>
#include <cstring>
#include <strings.h>

#include "context.hpp"
#include "data/text/named_characters.hpp"
#include "error.hpp"
#include "state.hpp"
#include "token.hpp"
#include "tree_constructor.hpp"

void
HTML::Tokenizer::Tokenizer::Run(Resources::DocumentResource &document) {

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

		switch (context.state) {
			// Skipped some CDATA stuff
			/*
			CHARACTER_REFERENCE, // 12.2.5.72 Character reference state
			NAMED_CHARACTER_REFERENCE, // 12.2.5.73 Named character reference state
			AMBIGOUS_AMPERSAND, // 12.2.5.74 Ambiguous ampersand state
			NUMERIC_CHARACTER_REFERENCE, // 12.2.5.75 Numeric character reference state
			HEXADECIMAL_CHARACTER_REFERENCE_START, // 12.2.5.76 Hexadecimal character reference start state
			DECIMAL_CHARACTER_REFERENCE_START, // 12.2.5.77 Decimal character reference start state
			HEXADECIMAL_CHARACTER_REFERENCE, // 12.2.5.78 Hexadecimal character reference state
			DECIMAL_CHARACTER_REFERENCE, // 12.2.5.79 Decimal character reference state
			NUMERIC_CHARACTER_REFERENCE_END // 12.2.5.80 Numeric character reference end state
			 */
				/*
			case HTML::Tokenizer::ParserState::NAMED_CHARACTER_REFERENCE: {
				// NamedCharacters::NCStatus status = NamedCharacters::Find(
				bool wasMatch = false;
				if (context.character != Unicode::SEMICOLON) {
					context.NCRefBuffer += context.character;
					std::cout << "NAMED_CHARACTER_REFERENCE: " << context.temporaryBuffer[0] << ' ' << context.character << std::endl;
					// TODO Check for match, if so do shit & "break;"
				}

				if (context.character != Unicode::SEMICOLON) {
					context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_NULL_CHARACTER);
				}

				// The named reference isn't found, thus:
				// flush code points consumed as a character reference
				switch (context.returnState) {
					case HTML::Tokenizer::ParserState::ATTRIBUTE_VALUE_DQ:
					case HTML::Tokenizer::ParserState::ATTRIBUTE_VALUE_SQ:
					case HTML::Tokenizer::ParserState::ATTRIBUTE_VALUE_NQ: {
						HTML::Tokenizer::AmbiguousTagToken &tagToken = (context.isEndTag ?
							(HTML::Tokenizer::AmbiguousTagToken &) context.endTagToken :
							(HTML::Tokenizer::AmbiguousTagToken &) context.startTagToken);

						for (size_t k = 0; k < context.NCRefBuffer.length(); k++) {
							tagToken.attributeValue += context.NCRefBuffer[context.i];
						}
					} break;
					default:
						for (size_t k = 0; k < context.NCRefBuffer.length(); k++) {
							treeConstructor.EmitCharacterToken(context.NCRefBuffer[context.i]);
						}
						break;
				}
				context.state = wasMatch ?
					context.returnState :
					HTML::Tokenizer::ParserState::AMBIGOUS_AMPERSAND;
			} break;
			*/
			default:
				std::cout << "Unknown state(" << ++context.unknownStateCount << "): " << context.state << std::endl;
				break;
		}
	}
}
