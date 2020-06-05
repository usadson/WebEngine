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
			// -- missing BOGUS_COMMENT
			// -- jump to MARKUP_DECLARATION_OPEN
			case HTML::Tokenizer::ParserState::COMMENT_LTS_BANG_DASH:
				if (context.character == '-') {
					context.state = HTML::Tokenizer::ParserState::COMMENT_LTS_BANG_DASH_DASH;
				} else {
					context.reconsume = true;
					context.state = HTML::Tokenizer::ParserState::COMMENT;
				}
				break;
			case HTML::Tokenizer::ParserState::COMMENT_LTS_BANG_DASH_DASH:
				if (context.eof || context.character == '>') {
					context.reconsume = true;
					context.state = HTML::Tokenizer::ParserState::COMMENT_END;
				} else {
					context.LogError(HTML::Tokenizer::ParserError::NESTED_COMMENT);
					context.reconsume = true;
					context.state = HTML::Tokenizer::ParserState::COMMENT_END;
				}
				break;
			case HTML::Tokenizer::ParserState::COMMENT_END_DASH:
				if (context.eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_COMMENT);
					treeConstructor.EmitToken(context.commentToken);
					treeConstructor.EmitEOFToken();
				} else if (context.character == '-') {
					context.state = HTML::Tokenizer::ParserState::COMMENT_END;
				} else {
					context.commentToken.contents += '-';
					context.reconsume = true;
					context.state = HTML::Tokenizer::ParserState::COMMENT;
				}
				break;
			case HTML::Tokenizer::ParserState::COMMENT_END:
				if (context.eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_COMMENT);
					treeConstructor.EmitToken(context.commentToken);
					treeConstructor.EmitEOFToken();
				} else if (context.character == '>') {
					treeConstructor.EmitToken(context.commentToken);
					context.commentToken = HTML::Tokenizer::CommentToken::INVALID_TYPE;
					context.state = HTML::Tokenizer::ParserState::DATA;
				} else if (context.character == '!') {
					context.state = HTML::Tokenizer::ParserState::COMMENT_END_BANG;
				} else if (context.character == '-') {
					context.commentToken.contents += '-';
				} else {
					context.commentToken.contents += "--";
					context.reconsume = true;
					context.state = HTML::Tokenizer::ParserState::COMMENT;
				}
				break;
			case HTML::Tokenizer::ParserState::COMMENT_END_BANG:
				if (context.eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_COMMENT);
					treeConstructor.EmitToken(context.commentToken);
					treeConstructor.EmitEOFToken();
				} else if (context.character == '-') {
					context.commentToken.contents += "--!";
					context.state = HTML::Tokenizer::ParserState::COMMENT_END_DASH;
				} else if (context.character == '>') {
					context.LogError(HTML::Tokenizer::ParserError::INCORRECTLY_CLOSED_COMMENT);
					treeConstructor.EmitToken(context.commentToken);
					context.commentToken = HTML::Tokenizer::CommentToken::INVALID_TYPE;
					context.state = HTML::Tokenizer::ParserState::DATA;
				} else {
					context.commentToken.contents += "--!";
					context.reconsume = true;
					context.state = HTML::Tokenizer::ParserState::COMMENT;
				}
				break;
			case HTML::Tokenizer::ParserState::DOCTYPE:
				if (context.eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_DOCTYPE);
					treeConstructor.EmitDoctypeQuirksToken();
					treeConstructor.EmitEOFToken();
				} else {
					switch (context.character) {
						case '\t':
						case '\n':
						case '\f':
						case ' ':
							context.state = HTML::Tokenizer::ParserState::BEFORE_DOCTYPE_NAME;
							break;
						case '>':
							context.reconsume = true;
							context.state = HTML::Tokenizer::ParserState::BEFORE_DOCTYPE_NAME;
							break;
						default:
							std::cout << "\ninvalid context.character: (" << static_cast<size_t>(context.character) << ")\n" << std::endl;
							context.LogError(HTML::Tokenizer::ParserError::MISSING_WHITESPACE_BEFORE_DOCTYPE_NAME);
							context.reconsume = true;
							context.state = HTML::Tokenizer::ParserState::BEFORE_DOCTYPE_NAME;
							break;
					}
				}
				break;
			case HTML::Tokenizer::ParserState::BEFORE_DOCTYPE_NAME:
				if (context.eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_DOCTYPE);
					treeConstructor.EmitDoctypeQuirksToken();
					treeConstructor.EmitEOFToken();
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
							treeConstructor.EmitDoctypeQuirksToken();
							context.state = HTML::Tokenizer::ParserState::DATA;
						} break;
						case '\0': {
							context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_NULL_CHARACTER);
							context.doctypeToken.name.emplace(Unicode::REPLACEMENT_CHARACTER);
							context.state = HTML::Tokenizer::ParserState::DOCTYPE_NAME;
						} break;
						default:
							if (context.character >= 0x41 && context.character <= 0x5A) {// Is uppercase
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
				break;
			case HTML::Tokenizer::ParserState::DOCTYPE_NAME:
				if (context.eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_DOCTYPE);
					treeConstructor.EmitDoctypeQuirksToken();
					treeConstructor.EmitEOFToken();
				} else {
					switch (context.character) {
						case '\t':
						case '\n':
						case '\f':
						case ' ':
							context.state = HTML::Tokenizer::ParserState::AFTER_DOCTYPE_NAME;
							break;
						case '>':
							std::cout << "Doctype name: " << context.doctypeToken.name.value() << std::endl;
							treeConstructor.EmitToken(context.doctypeToken);
							context.doctypeToken = HTML::Tokenizer::DoctypeToken(); // reset
							context.state = HTML::Tokenizer::ParserState::DATA;
							break;
						case '\0':
							context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_NULL_CHARACTER);
							context.doctypeToken.name = context.doctypeToken.name.value() + Unicode::REPLACEMENT_CHARACTER;
							break;
						default:
							if (context.character >= 0x41 && context.character <= 0x5A) {// Is uppercase
								context.doctypeToken.name = context.doctypeToken.name.value() + static_cast<char>(context.character + 0x20);;
							} else {
								context.doctypeToken.name = context.doctypeToken.name.value() + context.character;
							}
							break;
					}
				}
				break;
			case HTML::Tokenizer::ParserState::AFTER_DOCTYPE_NAME:
				if (context.eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_DOCTYPE);
					treeConstructor.EmitDoctypeQuirksToken();
					treeConstructor.EmitEOFToken();
				} else {
					switch (context.character) {
						case '\t':
						case '\n':
						case '\f':
						case ' ':
							// Ignore
							break;
						case '>':
							treeConstructor.EmitToken(context.doctypeToken);
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
				break;
			case HTML::Tokenizer::ParserState::AFTER_DOCTYPE_PUBLIC_KEYWORD:
				if (context.eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_DOCTYPE);
					treeConstructor.EmitDoctypeQuirksToken();
					treeConstructor.EmitEOFToken();
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
							treeConstructor.EmitToken(context.doctypeToken);
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
				break;
			case HTML::Tokenizer::ParserState::BEFORE_DOCTYPE_PUBLIC_IDENTIFIER:
				if (context.eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_DOCTYPE);
					treeConstructor.EmitDoctypeQuirksToken();
					treeConstructor.EmitEOFToken();
				} else {
					switch (context.character) {
						case '\t':
						case '\n':
						case '\f':
						case ' ':
							// Ignore
							break;
						case '"':
							context.doctypeToken.publicIdentifier.emplace("");
							context.state = HTML::Tokenizer::ParserState::DOCTYPE_PUBLIC_IDENTIFIER_DQ;
							break;
						case '\'':
							context.doctypeToken.publicIdentifier.emplace("");
							context.state = HTML::Tokenizer::ParserState::DOCTYPE_PUBLIC_IDENTIFIER_SQ;
							break;
						case '>':
							context.LogError(HTML::Tokenizer::ParserError::MISSING_DOCTYPE_PUBLIC_IDENTIFIER);
							context.doctypeToken.forceQuirks = true;
							treeConstructor.EmitToken(context.doctypeToken);
							context.doctypeToken = HTML::Tokenizer::DoctypeToken(); // reset
							context.state = HTML::Tokenizer::ParserState::DATA;
							break;
						default:
							context.LogError(HTML::Tokenizer::ParserError::MISSING_QOUTE_BEFORE_DOCTYPE_PUBLIC_IDENTIFIER);
							context.doctypeToken.forceQuirks = true;
							context.reconsume = true;
							context.state = HTML::Tokenizer::ParserState::BOGUS_DOCTYPE;
							break;
					}
				}
				break;
			case HTML::Tokenizer::ParserState::DOCTYPE_PUBLIC_IDENTIFIER_DQ:
				if (context.eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_DOCTYPE);
					treeConstructor.EmitDoctypeQuirksToken();
					treeConstructor.EmitEOFToken();
				} else {
					switch (context.character) {
						case '"':
							context.state = HTML::Tokenizer::ParserState::AFTER_DOCTYPE_PUBLIC_IDENTIFIER;
							break;
						case '\0':
							context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_NULL_CHARACTER);
							context.doctypeToken.publicIdentifier = context.doctypeToken.publicIdentifier.value() + Unicode::REPLACEMENT_CHARACTER;
							break;
						case '>':
							context.LogError(HTML::Tokenizer::ParserError::ABRUBT_DOCTYPE_PUBLIC_IDENTIFIER);
							context.doctypeToken.forceQuirks = true;
							treeConstructor.EmitToken(context.doctypeToken);
							context.doctypeToken = HTML::Tokenizer::DoctypeToken(); // reset
							context.state = HTML::Tokenizer::ParserState::DATA;
							break;
						default:
							context.doctypeToken.publicIdentifier = context.doctypeToken.publicIdentifier.value() + context.character;
							break;
					}
				}
				break;
			case HTML::Tokenizer::ParserState::DOCTYPE_PUBLIC_IDENTIFIER_SQ:
				if (context.eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_DOCTYPE);
					treeConstructor.EmitDoctypeQuirksToken();
					treeConstructor.EmitEOFToken();
				} else {
					switch (context.character) {
						case '\'':
							context.state = HTML::Tokenizer::ParserState::AFTER_DOCTYPE_PUBLIC_IDENTIFIER;
							break;
						case '\0':
							context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_NULL_CHARACTER);
							context.doctypeToken.publicIdentifier = context.doctypeToken.publicIdentifier.value() + Unicode::REPLACEMENT_CHARACTER;
							break;
						case '>':
							context.LogError(HTML::Tokenizer::ParserError::ABRUBT_DOCTYPE_PUBLIC_IDENTIFIER);
							context.doctypeToken.forceQuirks = true;
							treeConstructor.EmitToken(context.doctypeToken);
							context.doctypeToken = HTML::Tokenizer::DoctypeToken(); // reset
							context.state = HTML::Tokenizer::ParserState::DATA;
							break;
						default:
							context.doctypeToken.publicIdentifier = context.doctypeToken.publicIdentifier.value() + context.character;
							break;
					}
				}
				break;
			case HTML::Tokenizer::ParserState::AFTER_DOCTYPE_PUBLIC_IDENTIFIER:
				if (context.eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_DOCTYPE);
					treeConstructor.EmitDoctypeQuirksToken();
					treeConstructor.EmitEOFToken();
				} else {
					switch (context.character) {
						case '\t':
						case '\n':
						case '\f':
						case ' ':
							context.state = HTML::Tokenizer::ParserState::BETWEEN_DOCTYPE_PUBLIC_SYSTEM_IDENTIFIER;
							break;
						case '>':
							treeConstructor.EmitToken(context.doctypeToken);
							context.doctypeToken = HTML::Tokenizer::DoctypeToken(); // reset
							context.state = HTML::Tokenizer::ParserState::DATA;
							break;
						case '"':
							context.LogError(HTML::Tokenizer::ParserError::MISSING_WHITESPACE_BETWEEN_DOCTYPE_PUBLIC_AND_SYSTEM_IDENTIFIERS);
							context.doctypeToken.systemIdentifier.emplace("");
							context.state = HTML::Tokenizer::ParserState::DOCTYPE_SYSTEM_IDENTIFIER_DQ;
							break;
						case '\'':
							context.LogError(HTML::Tokenizer::ParserError::MISSING_WHITESPACE_BETWEEN_DOCTYPE_PUBLIC_AND_SYSTEM_IDENTIFIERS);
							context.doctypeToken.systemIdentifier.emplace("");
							context.state = HTML::Tokenizer::ParserState::DOCTYPE_SYSTEM_IDENTIFIER_SQ;
							break;
						default:
							context.LogError(HTML::Tokenizer::ParserError::MISSING_QOUTE_BEFORE_DOCTYPE_SYSTEM_IDENTIFIER);
							context.doctypeToken.forceQuirks = true;
							context.reconsume = true;
							context.state = HTML::Tokenizer::ParserState::BOGUS_DOCTYPE;
							break;
					}
				}
				break;
			case HTML::Tokenizer::ParserState::BETWEEN_DOCTYPE_PUBLIC_SYSTEM_IDENTIFIER:
				if (context.eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_DOCTYPE);
					treeConstructor.EmitDoctypeQuirksToken();
					treeConstructor.EmitEOFToken();
				} else {
					switch (context.character) {
						case '\t':
						case '\n':
						case '\f':
						case ' ':
							// Ignore
							break;
						case '>':
							treeConstructor.EmitToken(context.doctypeToken);
							context.doctypeToken = HTML::Tokenizer::DoctypeToken(); // reset
							context.state = HTML::Tokenizer::ParserState::DATA;
							break;
						case '"':
							context.doctypeToken.systemIdentifier.emplace("");
							context.state = HTML::Tokenizer::ParserState::DOCTYPE_SYSTEM_IDENTIFIER_DQ;
							break;
						case '\'':
							context.doctypeToken.systemIdentifier.emplace("");
							context.state = HTML::Tokenizer::ParserState::DOCTYPE_SYSTEM_IDENTIFIER_SQ;
							break;
						default:
							context.LogError(HTML::Tokenizer::ParserError::MISSING_QOUTE_BEFORE_DOCTYPE_PUBLIC_IDENTIFIER);
							context.doctypeToken.forceQuirks = true;
							context.reconsume = true;
							context.state = HTML::Tokenizer::ParserState::BOGUS_DOCTYPE;
							break;
					}
				}
				break;
			case HTML::Tokenizer::ParserState::AFTER_DOCTYPE_SYSTEM_KEYWORD:
				if (context.eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_DOCTYPE);
					treeConstructor.EmitDoctypeQuirksToken();
					treeConstructor.EmitEOFToken();
				} else {
					switch (context.character) {
						case '\t':
						case '\n':
						case '\f':
						case ' ':
							context.state = HTML::Tokenizer::ParserState::BEFORE_DOCTYPE_SYSTEM_IDENTIFIER;
							break;
						case '"':
							context.LogError(HTML::Tokenizer::ParserError::MISSING_WHITESPACE_AFTER_DOCTYPE_SYSTEM_KEYWORD);
							context.doctypeToken.systemIdentifier.emplace("");
							context.state = HTML::Tokenizer::ParserState::DOCTYPE_PUBLIC_IDENTIFIER_DQ;
							break;
						case '\'':
							context.LogError(HTML::Tokenizer::ParserError::MISSING_WHITESPACE_AFTER_DOCTYPE_SYSTEM_KEYWORD);
							context.doctypeToken.systemIdentifier.emplace("");
							context.state = HTML::Tokenizer::ParserState::DOCTYPE_PUBLIC_IDENTIFIER_SQ;
							break;
						case '>':
							context.LogError(HTML::Tokenizer::ParserError::MISSING_DOCTYPE_SYSTEM_IDENTIFIER);
							context.doctypeToken.forceQuirks = true;
							treeConstructor.EmitToken(context.doctypeToken);
							context.doctypeToken = HTML::Tokenizer::DoctypeToken(); // reset
							context.state = HTML::Tokenizer::ParserState::DATA;
							break;
						default:
							std::cout << "At AFTER_DOCTYPE_SYSTEM_KEYWORD" << std::endl;
							context.LogError(HTML::Tokenizer::ParserError::MISSING_QOUTE_BEFORE_DOCTYPE_SYSTEM_IDENTIFIER);
							context.doctypeToken.forceQuirks = true;
							context.reconsume = true;
							context.state = HTML::Tokenizer::ParserState::BOGUS_DOCTYPE;
							break;
					}
				}
				break;
			case HTML::Tokenizer::ParserState::BEFORE_DOCTYPE_SYSTEM_IDENTIFIER:
				if (context.eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_DOCTYPE);
					treeConstructor.EmitDoctypeQuirksToken();
					treeConstructor.EmitEOFToken();
				} else {
					switch (context.character) {
						case '\t':
						case '\n':
						case '\f':
						case ' ':
							// Ignore
							break;
						case '"':
							context.doctypeToken.systemIdentifier.emplace("");
							context.state = HTML::Tokenizer::ParserState::DOCTYPE_SYSTEM_IDENTIFIER_DQ;
							break;
						case '\'':
							context.doctypeToken.systemIdentifier.emplace("");
							context.state = HTML::Tokenizer::ParserState::DOCTYPE_SYSTEM_IDENTIFIER_SQ;
							break;
						case '>':
							context.LogError(HTML::Tokenizer::ParserError::MISSING_DOCTYPE_SYSTEM_IDENTIFIER);
							context.doctypeToken.forceQuirks = true;
							treeConstructor.EmitToken(context.doctypeToken);
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
				break;
			case HTML::Tokenizer::ParserState::DOCTYPE_SYSTEM_IDENTIFIER_DQ:
				if (context.eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_DOCTYPE);
					treeConstructor.EmitDoctypeQuirksToken();
					treeConstructor.EmitEOFToken();
				} else {
					switch (context.character) {
						case '"':
							context.state = HTML::Tokenizer::ParserState::AFTER_DOCTYPE_SYSTEM_IDENTIFIER;
							break;
						case '\0':
							context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_NULL_CHARACTER);
							context.doctypeToken.systemIdentifier = context.doctypeToken.systemIdentifier.value() + Unicode::REPLACEMENT_CHARACTER;
							break;
						case '>':
							context.LogError(HTML::Tokenizer::ParserError::ABRUBT_DOCTYPE_SYSTEM_IDENTIFIER);
							context.doctypeToken.forceQuirks = true;
							treeConstructor.EmitToken(context.doctypeToken);
							context.doctypeToken = HTML::Tokenizer::DoctypeToken(); // reset
							context.state = HTML::Tokenizer::ParserState::DATA;
							break;
						default:
							context.doctypeToken.systemIdentifier = context.doctypeToken.systemIdentifier.value() + context.character;
							break;
					}
				}
				break;
			case HTML::Tokenizer::ParserState::DOCTYPE_SYSTEM_IDENTIFIER_SQ:
				if (context.eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_DOCTYPE);
					treeConstructor.EmitDoctypeQuirksToken();
					treeConstructor.EmitEOFToken();
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
							treeConstructor.EmitToken(context.doctypeToken);
							context.doctypeToken = HTML::Tokenizer::DoctypeToken(); // reset
							context.state = HTML::Tokenizer::ParserState::DATA;
							break;
						default:
							context.doctypeToken.systemIdentifier = context.doctypeToken.systemIdentifier.value() + context.character;
							break;
					}
				}
				break;
			case HTML::Tokenizer::ParserState::AFTER_DOCTYPE_SYSTEM_IDENTIFIER:
				if (context.eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_DOCTYPE);
					treeConstructor.EmitDoctypeQuirksToken();
					treeConstructor.EmitEOFToken();
				} else {
					switch (context.character) {
						case '\t':
						case '\n':
						case '\f':
						case ' ':
							// Ignore
							break;
						case '>':
							treeConstructor.EmitToken(context.doctypeToken);
							context.doctypeToken = HTML::Tokenizer::DoctypeToken(); // reset
							context.state = HTML::Tokenizer::ParserState::DATA;
							break;
						default:
							context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_CHARACTER_AFTER_DOCTYPE_SYSTEM_IDENTIFIER);
							context.reconsume = true;
							context.state = HTML::Tokenizer::ParserState::BOGUS_DOCTYPE;
							break;
					}
				}
				break;
			case HTML::Tokenizer::ParserState::BOGUS_DOCTYPE:
				if (context.eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_DOCTYPE);
					treeConstructor.EmitToken(context.doctypeToken);
					treeConstructor.EmitEOFToken();
				} else {
					switch (context.character) {
						case '>':
							treeConstructor.EmitToken(context.doctypeToken);
							context.doctypeToken = HTML::Tokenizer::DoctypeToken(); // reset
							context.state = HTML::Tokenizer::ParserState::DATA;
							break;
						case '\0':
							context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_NULL_CHARACTER);
							// Ignore
							break;
						default:
							// Ignore
							break;
					}
				}
				break;
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
			case HTML::Tokenizer::ParserState::CHARACTER_REFERENCE:
				context.temporaryBuffer.clear();
				context.temporaryBuffer.push_back(Unicode::AMPERSAND);
				if (!context.eof) {
					if (context.character == Unicode::NUMBER_SIGN) {
						context.temporaryBuffer.push_back(context.character);
						context.state = HTML::Tokenizer::ParserState::NUMERIC_CHARACTER_REFERENCE;
						break;
					}

					if (Unicode::IsASCIIAlphaNumeric(context.character)) {
						context.reconsume = true;
						context.state = HTML::Tokenizer::ParserState::NAMED_CHARACTER_REFERENCE;
						break;
					}
				}
				break;
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
