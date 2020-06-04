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

	// At what line is the tokenizer?
	context.lineCount = 1;
	// At what character position in the line is the tokenizer?
	context.linePosition = 0;

	const size_t documentSize = document.data.length();
	std::cout << "InputDataSize: " << documentSize << std::endl;

	// Don't use 'character' if eof is true.
	bool eof = false;

	HTML::Tokenizer::CommentToken commentToken = HTML::Tokenizer::CommentToken::INVALID_TYPE;
	HTML::Tokenizer::DoctypeToken doctypeToken;

	bool isEndTag = false;
	HTML::Tokenizer::EndTagToken endTagToken = HTML::Tokenizer::EndTagToken::INVALID_TYPE_END_TAG;
	HTML::Tokenizer::StartTagToken startTagToken = HTML::Tokenizer::StartTagToken::INVALID_TYPE_START_TAG;

	size_t unknownStateCount = 0;
	bool reconsume = false;
	size_t toConsumeNext = 0;

	for (size_t i = 0; i <= documentSize; i++) {
		Unicode::CodePoint character = '\0';

// 		std::cout << "index=" << i << " state=" << context.state << std::endl;
		if (reconsume) {
			i--;
			reconsume = false;
		}

		if (i == documentSize) {
			eof = true;
			context.linePosition += 1;
		} else {
			bool repeatLineCheckLoop = true;
			while (repeatLineCheckLoop) {
				repeatLineCheckLoop = false;
				character = document.data[i];

				if (character == '\n') {
					context.lineCount += 1;
					context.linePosition = 1;
				} else {
					// std::cout << "CharAt(" << i << ") = " << character << std::endl;
					context.linePosition += 1;
				}

				if (toConsumeNext > 0) {
					i += 1;
					toConsumeNext -= 1;
					repeatLineCheckLoop = true;
				}
			}
		}

		context.beginLoopState = context.state;
		context.currentCharacter = character;

		switch (context.state) {
			case HTML::Tokenizer::ParserState::DATA:
				if (eof) {
					treeConstructor.EmitEOFToken();
				} else {
					switch (character) {
						case '&':
							context.returnState = HTML::Tokenizer::ParserState::DATA;
							context.state = HTML::Tokenizer::ParserState::CHARACTER_REFERENCE;
							break;
						case '<':
							context.state = HTML::Tokenizer::ParserState::TAG_OPEN;
							break;
						case '\0':
							context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_NULL_CHARACTER);
							treeConstructor.EmitCharacterToken(character);
							break;
						default:
							treeConstructor.EmitCharacterToken(character);
							break;
					}
				}
				break;
			// -- jump to tag TAG_OPEN
			case HTML::Tokenizer::ParserState::TAG_OPEN:
				if (eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_BEFORE_TAG_NAME);
					treeConstructor.EmitCharacterToken('>');
					treeConstructor.EmitEOFToken();
				} else {
					switch (character) {
						case '!':
							context.state = HTML::Tokenizer::ParserState::MARKUP_DECLARATION_OPEN;
							break;
						case '/':
							context.state = HTML::Tokenizer::ParserState::TAG_END_OPEN;
							break;
						case '?':
							context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_QUESTION_MARK_INSTEAD_OF_TAG_NAME);
							break;
						default:
							if (Unicode::IsASCIIAlpha(character)) {
								isEndTag = false;
								startTagToken = HTML::Tokenizer::StartTagToken();
								reconsume = true;
								context.state = HTML::Tokenizer::ParserState::TAG_NAME;
							} else {
								std::cout << "DEBUG: Unexpected character: " << character << document.data[i+1] << document.data[i+2] << std::endl;
								context.LogError(HTML::Tokenizer::ParserError::INVALID_FIRST_CHARACTER_OF_TAG_NAME);
								reconsume = true;
								treeConstructor.EmitCharacterToken('>');
								context.state = HTML::Tokenizer::ParserState::DATA;
							}
							break;
					}
				}
				break;
			case HTML::Tokenizer::ParserState::TAG_END_OPEN:
				if (eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_BEFORE_TAG_NAME);
					treeConstructor.EmitCharacterToken('>');
					treeConstructor.EmitCharacterToken('\\');
					treeConstructor.EmitEOFToken();
				} else {
					if (character == '>') {
						context.LogError(HTML::Tokenizer::ParserError::MISSING_END_TAG_NAME);
						context.state = HTML::Tokenizer::ParserState::DATA;
					} else if (Unicode::IsASCIIAlpha(character)) {
						isEndTag = true;
						endTagToken = HTML::Tokenizer::EndTagToken();
						reconsume = true;
						context.state = HTML::Tokenizer::ParserState::TAG_NAME;
					} else {
						context.LogError(HTML::Tokenizer::ParserError::INVALID_FIRST_CHARACTER_OF_TAG_NAME);
						commentToken = HTML::Tokenizer::CommentToken(Unicode::UString(""));
						reconsume = true;
						context.state = HTML::Tokenizer::ParserState::BOGUS_COMMENT;
					}
				}
				break;
			case HTML::Tokenizer::ParserState::TAG_NAME:
				if (eof) {
					std::cout << "EOF IN TAG NAME TODO" << std::endl;
				} else {
					HTML::Tokenizer::AmbiguousTagToken &tagToken = isEndTag ?
								static_cast<HTML::Tokenizer::AmbiguousTagToken &>(endTagToken) :
								static_cast<HTML::Tokenizer::AmbiguousTagToken &>(startTagToken);
					switch (character) {
						case '\t':
						case '\n':
						case '\f':
						case ' ':
							context.state = HTML::Tokenizer::ParserState::BEFORE_ATTRIBUTE_NAME;
							break;
						case '/':
							context.state = HTML::Tokenizer::ParserState::SELF_CLOSING_START;
							break;
						case '>':
							if (isEndTag)
								treeConstructor.EmitToken(endTagToken);
							else
								treeConstructor.EmitToken(startTagToken);
							tagToken = HTML::Tokenizer::AmbiguousTagToken::INVALID_TYPE;
							context.state = HTML::Tokenizer::ParserState::DATA;
							break;
						case '\0':
							context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_NULL_CHARACTER);
							tagToken.tagName += Unicode::REPLACEMENT_CHARACTER;
							break;
						default:
							if (character >= 0x41 && character <= 0x5A) {// Is uppercase
								tagToken.tagName += static_cast<char>(character + 0x20);
							} else {
								tagToken.tagName += character;
							}
							break;
					}
				}
				break;
			// -- missing script shit
			case HTML::Tokenizer::ParserState::BEFORE_ATTRIBUTE_NAME:
				if (eof) {
					reconsume = true;
					context.state = HTML::Tokenizer::ParserState::AFTER_ATTRIBUTE_NAME;
				} else {
					HTML::Tokenizer::AmbiguousTagToken &tagToken = isEndTag ?
								static_cast<HTML::Tokenizer::AmbiguousTagToken &>(endTagToken) :
								static_cast<HTML::Tokenizer::AmbiguousTagToken &>(startTagToken);
					if (tagToken.attributeName.length() != 0) {
						tagToken.AddTokenAttribute(context);
					}

					switch (character) {
						case '\t':
						case '\n':
						case '\f':
						case ' ':
							// Ignore
							break;
						case '/':
						case '>':
							reconsume = true;
							context.state = HTML::Tokenizer::ParserState::AFTER_ATTRIBUTE_NAME;
							break;
						case '=':
							context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_EQUALS_SIGN_BEFORE_ATTRIBUTE_NAME);
							// Correct behavior? The 12.2.5.32 says:
							// "Consume the next input chararcter ... Set that
							// attribute's name to the current input character"
							// Note the difference: "next" and "current" input
							// character!
							tagToken.attributeName = Unicode::UString("");
							tagToken.attributeName += character;
							tagToken.attributeValue = Unicode::UString("");
							context.state = HTML::Tokenizer::ParserState::ATTRIBUTE_NAME;
							break;
						default:
							tagToken.attributeName = Unicode::UString("");
							tagToken.attributeValue = Unicode::UString("");
							reconsume = true;
							context.state = HTML::Tokenizer::ParserState::ATTRIBUTE_NAME;
							break;
					}
				}
				break;
			case HTML::Tokenizer::ParserState::ATTRIBUTE_NAME: {
				HTML::Tokenizer::AmbiguousTagToken &tagToken = isEndTag ?
								static_cast<HTML::Tokenizer::AmbiguousTagToken &>(endTagToken) :
								static_cast<HTML::Tokenizer::AmbiguousTagToken &>(startTagToken);
				if (eof || character == '\t'
						|| character == '\n'
						|| character == '\f'
						|| character == ' '
						|| character == '\\'
						|| character == '>') {
					reconsume = true;
					context.state = HTML::Tokenizer::ParserState::AFTER_ATTRIBUTE_NAME;
				} else if (character == '=') {
					context.state = HTML::Tokenizer::ParserState::BEFORE_ATTRIBUTE_VALUE;
				} else if (character >= 0x41 && character <= 0x5A) {// Is uppercase
					tagToken.attributeName += static_cast<char>(character + 0x20);
				} else if (character == '\0') {
					context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_NULL_CHARACTER);
					tagToken.attributeName += Unicode::REPLACEMENT_CHARACTER;
				} else {
					if (character == '"' ||
						   character == '\'' ||
						   character == '<') {
						context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_CHARACTER_IN_ATTRIBUTE_NAME);
						// Intentional Fallthrough
					}
					tagToken.attributeName += character;
				}
				// TODO Check duplicate-attribute parser error
			} break;
			case HTML::Tokenizer::ParserState::AFTER_ATTRIBUTE_NAME:
				if (eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_TAG);
					treeConstructor.EmitEOFToken();
				} else {
					HTML::Tokenizer::AmbiguousTagToken &tagToken = isEndTag ?
								static_cast<HTML::Tokenizer::AmbiguousTagToken &>(endTagToken) :
								static_cast<HTML::Tokenizer::AmbiguousTagToken &>(startTagToken);

					switch (character) {
						case '\t':
						case '\n':
						case '\f':
						case ' ':
							// Ignore
							break;
						case '/':
							context.state = HTML::Tokenizer::ParserState::SELF_CLOSING_START;
							break;
						case '=':
							context.state = HTML::Tokenizer::ParserState::BEFORE_ATTRIBUTE_VALUE;
							break;
						case '>':
							context.state = HTML::Tokenizer::ParserState::DATA;
							treeConstructor.EmitToken(tagToken);
							if (isEndTag)
								startTagToken = HTML::Tokenizer::StartTagToken(); // Reset
							else
								endTagToken = HTML::Tokenizer::EndTagToken(); // Reset
							break;
						default:
							// New attribute? Destroy old one?
							tagToken.attributeName = Unicode::UString("");
							tagToken.attributeValue = Unicode::UString("");
							reconsume = true;
							context.state = HTML::Tokenizer::ParserState::ATTRIBUTE_NAME;
							break;
					}
				}
				break;
			case HTML::Tokenizer::ParserState::BEFORE_ATTRIBUTE_VALUE:
				if (eof) {
					reconsume = true;
					context.state = HTML::Tokenizer::ParserState::ATTRIBUTE_VALUE_NQ;
				} else {
					HTML::Tokenizer::AmbiguousTagToken &tagToken = isEndTag ?
								static_cast<HTML::Tokenizer::AmbiguousTagToken &>(endTagToken) :
								static_cast<HTML::Tokenizer::AmbiguousTagToken &>(startTagToken);

					switch (character) {
						case '\t':
						case '\n':
						case '\f':
						case ' ':
							// Ignore
							break;
						case '"':
							context.state = HTML::Tokenizer::ParserState::ATTRIBUTE_VALUE_DQ;
							break;
						case '\'':
							context.state = HTML::Tokenizer::ParserState::ATTRIBUTE_VALUE_SQ;
							break;
						case '>':
							context.LogError(HTML::Tokenizer::ParserError::MISSING_ATTRIBUTE_VALUE);
							context.state = HTML::Tokenizer::ParserState::DATA;
							treeConstructor.EmitToken(tagToken);
							if (isEndTag)
								startTagToken = HTML::Tokenizer::StartTagToken(); // Reset
							else
								endTagToken = HTML::Tokenizer::EndTagToken(); // Reset
							break;
						default:
							reconsume = true;
							context.state = HTML::Tokenizer::ParserState::ATTRIBUTE_VALUE_NQ;
							break;
					}
				}
				break;
			case HTML::Tokenizer::ParserState::ATTRIBUTE_VALUE_DQ:
				if (eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_TAG);
					treeConstructor.EmitEOFToken();
				} else {
					HTML::Tokenizer::AmbiguousTagToken &tagToken = isEndTag ?
								static_cast<HTML::Tokenizer::AmbiguousTagToken &>(endTagToken) :
								static_cast<HTML::Tokenizer::AmbiguousTagToken &>(startTagToken);

					switch (character) {
						case '"':
							context.state = HTML::Tokenizer::ParserState::AFTER_ATTRIBUTE_VALUE_QUOTED;
							break;
						case '&':
							context.returnState = HTML::Tokenizer::ParserState::ATTRIBUTE_VALUE_DQ;
							context.state = HTML::Tokenizer::ParserState::CHARACTER_REFERENCE;
							break;
						case '\0':
							context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_NULL_CHARACTER);
							tagToken.attributeValue += Unicode::REPLACEMENT_CHARACTER;
							break;
						default:
							tagToken.attributeValue += character;
							break;
					}
				}
				break;
			case HTML::Tokenizer::ParserState::ATTRIBUTE_VALUE_SQ:
				if (eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_TAG);
					treeConstructor.EmitEOFToken();
				} else {
					HTML::Tokenizer::AmbiguousTagToken &tagToken = isEndTag ?
								static_cast<HTML::Tokenizer::AmbiguousTagToken &>(endTagToken) :
								static_cast<HTML::Tokenizer::AmbiguousTagToken &>(startTagToken);

					switch (character) {
						case '\'':
							context.state = HTML::Tokenizer::ParserState::AFTER_ATTRIBUTE_VALUE_QUOTED;
							break;
						case '&':
							context.returnState = HTML::Tokenizer::ParserState::ATTRIBUTE_VALUE_SQ;
							context.state = HTML::Tokenizer::ParserState::CHARACTER_REFERENCE;
							break;
						case '\0':
							context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_NULL_CHARACTER);
							tagToken.attributeValue += Unicode::REPLACEMENT_CHARACTER;
							break;
						default:
							tagToken.attributeValue += character;
							break;
					}
				}
				break;
			case HTML::Tokenizer::ParserState::ATTRIBUTE_VALUE_NQ:
				if (eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_TAG);
					treeConstructor.EmitEOFToken();
				} else {
					HTML::Tokenizer::AmbiguousTagToken &tagToken = isEndTag ?
								static_cast<HTML::Tokenizer::AmbiguousTagToken &>(endTagToken) :
								static_cast<HTML::Tokenizer::AmbiguousTagToken &>(startTagToken);

					switch (character) {
						case '\t':
						case '\n':
						case '\f':
						case ' ':
							context.state = HTML::Tokenizer::ParserState::BEFORE_ATTRIBUTE_NAME;
							break;
						case '&':
							context.returnState = HTML::Tokenizer::ParserState::ATTRIBUTE_VALUE_NQ;
							context.state = HTML::Tokenizer::ParserState::CHARACTER_REFERENCE;
							break;
						case '>':
							context.LogError(HTML::Tokenizer::ParserError::MISSING_ATTRIBUTE_VALUE);
							context.state = HTML::Tokenizer::ParserState::DATA;
							treeConstructor.EmitToken(tagToken);
							if (isEndTag)
								startTagToken = HTML::Tokenizer::StartTagToken(); // Reset
							else
								endTagToken = HTML::Tokenizer::EndTagToken(); // Reset
							break;
						case '\0':
							context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_NULL_CHARACTER);
							tagToken.attributeValue += Unicode::REPLACEMENT_CHARACTER;
							break;
						case '"':
						case '\'':
						case '<':
						case '=':
						case '`':
							context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_CHARACTER_IN_UNQOUTED_ATTRIBUTE_VALUE);
							/* Intentional fallthrough */
						default:
							tagToken.attributeValue += character;
							break;
					}
				}
				break;
			case HTML::Tokenizer::ParserState::AFTER_ATTRIBUTE_VALUE_QUOTED:
				if (eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_TAG);
					treeConstructor.EmitEOFToken();
				} else {
					HTML::Tokenizer::AmbiguousTagToken &tagToken = isEndTag ?
								static_cast<HTML::Tokenizer::AmbiguousTagToken &>(endTagToken) :
								static_cast<HTML::Tokenizer::AmbiguousTagToken &>(startTagToken);

					switch (character) {
						case '\t':
						case '\n':
						case '\f':
						case ' ':
							context.state = HTML::Tokenizer::ParserState::BEFORE_ATTRIBUTE_NAME;
							break;
						case '/':
							context.state = HTML::Tokenizer::ParserState::SELF_CLOSING_START;
							break;
						case '>':
							context.state = HTML::Tokenizer::ParserState::DATA;
							treeConstructor.EmitToken(tagToken);
							if (isEndTag)
								startTagToken = HTML::Tokenizer::StartTagToken(); // Reset
							else
								endTagToken = HTML::Tokenizer::EndTagToken(); // Reset
							break;
						default:
							context.LogError(HTML::Tokenizer::ParserError::MISSING_WHITESPACE_BETWEEN_ATTRIBUTES);
							reconsume = true;
							context.state = HTML::Tokenizer::ParserState::BEFORE_ATTRIBUTE_NAME;
							break;
					}
				}
				break;
			case HTML::Tokenizer::ParserState::SELF_CLOSING_START:
				if (eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_TAG);
					treeConstructor.EmitEOFToken();
				} else if (character == '>') {
					HTML::Tokenizer::AmbiguousTagToken &tagToken = isEndTag ?
								static_cast<HTML::Tokenizer::AmbiguousTagToken &>(endTagToken) :
								static_cast<HTML::Tokenizer::AmbiguousTagToken &>(startTagToken);

					tagToken.selfClosing = true;
					context.state = HTML::Tokenizer::ParserState::DATA;
					treeConstructor.EmitToken(tagToken);

					if (isEndTag)
						startTagToken = HTML::Tokenizer::StartTagToken(); // Reset
					else
						endTagToken = HTML::Tokenizer::EndTagToken(); // Reset
				}
				break;
			// -- missing BOGUS_COMMENT
			// -- jump to MARKUP_DECLARATION_OPEN
			case HTML::Tokenizer::ParserState::MARKUP_DECLARATION_OPEN:
				if (!eof) {
					if (i + 1 < documentSize
						&& character == '-'
						&& document.data[i+1] == '-') {
						toConsumeNext = 1;

						commentToken = HTML::Tokenizer::CommentToken(Unicode::UString(""));
						context.state = HTML::Tokenizer::ParserState::COMMENT_START;
						continue;
					}

					if (i + 6 < documentSize) {
						if (document.data.EqualsIgnoreCaseAL(i, "DOCTYPE", 7)) {
							toConsumeNext = 6;
							context.state = HTML::Tokenizer::ParserState::DOCTYPE;
							continue;
						}
						if (character == '['
							&& document.data.EqualsAL(i + 1, "CDATA", 5) // Case-sensitive!
							&& document.data[i + 6] == ']') {
							// TODO ?
							throw std::runtime_error("TODO in MARKUP_DECLARATION_OPEN / CDATA");
						}
					}
				}

				context.LogError(HTML::Tokenizer::ParserError::INCORRECTLY_OPENED_COMMENT);
				commentToken = HTML::Tokenizer::CommentToken(Unicode::UString(""));
				i--; // Don't consume anything
				context.state = HTML::Tokenizer::ParserState::BOGUS_COMMENT;
				break;
			case HTML::Tokenizer::ParserState::COMMENT_START:
				if (character == '-') {
					context.state = HTML::Tokenizer::ParserState::COMMENT_START_DASH;
				} else if (character == '>') {
					context.LogError(HTML::Tokenizer::ParserError::ABRUBT_CLOSING_OF_EMPTY_COMMENT);
					context.state = HTML::Tokenizer::ParserState::DATA;
					treeConstructor.EmitToken(commentToken);
				} else {
					reconsume = true;
					context.state = HTML::Tokenizer::ParserState::COMMENT;
				}
				break;
			case HTML::Tokenizer::ParserState::COMMENT_START_DASH:
				if (eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_COMMENT);
					treeConstructor.EmitToken(commentToken);
					treeConstructor.EmitEOFToken();
				} else {
					if (character == '-') {
						context.state = HTML::Tokenizer::ParserState::COMMENT_END;
					} else if (character == '>') {
						context.LogError(HTML::Tokenizer::ParserError::ABRUBT_CLOSING_OF_EMPTY_COMMENT);
						context.state = HTML::Tokenizer::ParserState::DATA;
						treeConstructor.EmitToken(commentToken);
					} else {
						// Weird, is this a loop?
						// 'Append a U+002D HYPHEN-MINUS character (-) to the
						//  comment token's data. Reconsume in the comment state.
						commentToken.contents += '-';
						reconsume = true;
						context.state = HTML::Tokenizer::ParserState::COMMENT;
					}
				}
				break;
			case HTML::Tokenizer::ParserState::COMMENT:
				if (eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_COMMENT);
					treeConstructor.EmitToken(commentToken);
					treeConstructor.EmitEOFToken();
				} else {
					if (character == '<') {
						commentToken.contents += '<';
						context.state = HTML::Tokenizer::ParserState::COMMENT_LTS;
					} else if (character == '-') {
						context.state = HTML::Tokenizer::ParserState::COMMENT_END_DASH;
					} else if (character == '\0') {
						context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_NULL_CHARACTER);
						commentToken.contents += Unicode::REPLACEMENT_CHARACTER;
					} else {
						commentToken.contents += character;
					}
				}
				break;
			case HTML::Tokenizer::ParserState::COMMENT_LTS:
				if (character == '!') {
					commentToken.contents += '!';
					context.state = HTML::Tokenizer::ParserState::COMMENT_LTS_BANG;
				} else if (character == '<') {
					commentToken.contents += '<';
				} else {
					reconsume = true;
					context.state = HTML::Tokenizer::ParserState::COMMENT;
				}
				break;
			case HTML::Tokenizer::ParserState::COMMENT_LTS_BANG:
				if (character == '-') {
					context.state = HTML::Tokenizer::ParserState::COMMENT_LTS_BANG_DASH;
				} else {
					reconsume = true;
					context.state = HTML::Tokenizer::ParserState::COMMENT;
				}
				break;
			case HTML::Tokenizer::ParserState::COMMENT_LTS_BANG_DASH:
				if (character == '-') {
					context.state = HTML::Tokenizer::ParserState::COMMENT_LTS_BANG_DASH_DASH;
				} else {
					reconsume = true;
					context.state = HTML::Tokenizer::ParserState::COMMENT;
				}
				break;
			case HTML::Tokenizer::ParserState::COMMENT_LTS_BANG_DASH_DASH:
				if (eof || character == '>') {
					reconsume = true;
					context.state = HTML::Tokenizer::ParserState::COMMENT_END;
				} else {
					context.LogError(HTML::Tokenizer::ParserError::NESTED_COMMENT);
					reconsume = true;
					context.state = HTML::Tokenizer::ParserState::COMMENT_END;
				}
				break;
			case HTML::Tokenizer::ParserState::COMMENT_END_DASH:
				if (eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_COMMENT);
					treeConstructor.EmitToken(commentToken);
					treeConstructor.EmitEOFToken();
				} else if (character == '-') {
					context.state = HTML::Tokenizer::ParserState::COMMENT_END;
				} else {
					commentToken.contents += '-';
					reconsume = true;
					context.state = HTML::Tokenizer::ParserState::COMMENT;
				}
				break;
			case HTML::Tokenizer::ParserState::COMMENT_END:
				if (eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_COMMENT);
					treeConstructor.EmitToken(commentToken);
					treeConstructor.EmitEOFToken();
				} else if (character == '>') {
					treeConstructor.EmitToken(commentToken);
					commentToken = HTML::Tokenizer::CommentToken::INVALID_TYPE;
					context.state = HTML::Tokenizer::ParserState::DATA;
				} else if (character == '!') {
					context.state = HTML::Tokenizer::ParserState::COMMENT_END_BANG;
				} else if (character == '-') {
					commentToken.contents += '-';
				} else {
					commentToken.contents += "--";
					reconsume = true;
					context.state = HTML::Tokenizer::ParserState::COMMENT;
				}
				break;
			case HTML::Tokenizer::ParserState::COMMENT_END_BANG:
				if (eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_COMMENT);
					treeConstructor.EmitToken(commentToken);
					treeConstructor.EmitEOFToken();
				} else if (character == '-') {
					commentToken.contents += "--!";
					context.state = HTML::Tokenizer::ParserState::COMMENT_END_DASH;
				} else if (character == '>') {
					context.LogError(HTML::Tokenizer::ParserError::INCORRECTLY_CLOSED_COMMENT);
					treeConstructor.EmitToken(commentToken);
					commentToken = HTML::Tokenizer::CommentToken::INVALID_TYPE;
					context.state = HTML::Tokenizer::ParserState::DATA;
				} else {
					commentToken.contents += "--!";
					reconsume = true;
					context.state = HTML::Tokenizer::ParserState::COMMENT;
				}
				break;
			case HTML::Tokenizer::ParserState::DOCTYPE:
				if (eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_DOCTYPE);
					treeConstructor.EmitDoctypeQuirksToken();
					treeConstructor.EmitEOFToken();
				} else {
					switch (character) {
						case '\t':
						case '\n':
						case '\f':
						case ' ':
							context.state = HTML::Tokenizer::ParserState::BEFORE_DOCTYPE_NAME;
							break;
						case '>':
							reconsume = true;
							context.state = HTML::Tokenizer::ParserState::BEFORE_DOCTYPE_NAME;
							break;
						default:
							std::cout << "\ninvalid character: (" << static_cast<size_t>(character) << ")\n" << std::endl;
							context.LogError(HTML::Tokenizer::ParserError::MISSING_WHITESPACE_BEFORE_DOCTYPE_NAME);
							reconsume = true;
							context.state = HTML::Tokenizer::ParserState::BEFORE_DOCTYPE_NAME;
							break;
					}
				}
				break;
			case HTML::Tokenizer::ParserState::BEFORE_DOCTYPE_NAME:
				if (eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_DOCTYPE);
					treeConstructor.EmitDoctypeQuirksToken();
					treeConstructor.EmitEOFToken();
				} else {
					switch (character) {
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
							doctypeToken.name.emplace(Unicode::REPLACEMENT_CHARACTER);
							context.state = HTML::Tokenizer::ParserState::DOCTYPE_NAME;
						} break;
						default:
							if (character >= 0x41 && character <= 0x5A) {// Is uppercase
								doctypeToken.name.emplace("");
								doctypeToken.name.value() += static_cast<char>(character + 0x20);
								context.state = HTML::Tokenizer::ParserState::DOCTYPE_NAME;
							} else {
								doctypeToken.name.emplace("");
								doctypeToken.name.value() += character;
								context.state = HTML::Tokenizer::ParserState::DOCTYPE_NAME;
							}
							break;
					}
				}
				break;
			case HTML::Tokenizer::ParserState::DOCTYPE_NAME:
				if (eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_DOCTYPE);
					treeConstructor.EmitDoctypeQuirksToken();
					treeConstructor.EmitEOFToken();
				} else {
					switch (character) {
						case '\t':
						case '\n':
						case '\f':
						case ' ':
							context.state = HTML::Tokenizer::ParserState::AFTER_DOCTYPE_NAME;
							break;
						case '>':
							std::cout << "Doctype name: " << doctypeToken.name.value() << std::endl;
							treeConstructor.EmitToken(doctypeToken);
							doctypeToken = HTML::Tokenizer::DoctypeToken(); // reset
							context.state = HTML::Tokenizer::ParserState::DATA;
							break;
						case '\0':
							context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_NULL_CHARACTER);
							doctypeToken.name = doctypeToken.name.value() + Unicode::REPLACEMENT_CHARACTER;
							break;
						default:
							if (character >= 0x41 && character <= 0x5A) {// Is uppercase
								doctypeToken.name = doctypeToken.name.value() + static_cast<char>(character + 0x20);;
							} else {
								doctypeToken.name = doctypeToken.name.value() + character;
							}
							break;
					}
				}
				break;
			case HTML::Tokenizer::ParserState::AFTER_DOCTYPE_NAME:
				if (eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_DOCTYPE);
					treeConstructor.EmitDoctypeQuirksToken();
					treeConstructor.EmitEOFToken();
				} else {
					switch (character) {
						case '\t':
						case '\n':
						case '\f':
						case ' ':
							// Ignore
							break;
						case '>':
							treeConstructor.EmitToken(doctypeToken);
							doctypeToken = HTML::Tokenizer::DoctypeToken(); // reset
							context.state = HTML::Tokenizer::ParserState::DATA;
							break;
						default:
							if (i + 5 < documentSize) { // 5 or 6 ? not sure..
								if (document.data.EqualsIgnoreCaseAL(i, "PUBLIC", 6)) {
									toConsumeNext = 5;
									context.state = HTML::Tokenizer::ParserState::AFTER_DOCTYPE_PUBLIC_KEYWORD;
									break;
								}

								if (document.data.EqualsIgnoreCaseAL(i, "SYSTEM", 6)) {
									toConsumeNext = 5;
									context.state = HTML::Tokenizer::ParserState::AFTER_DOCTYPE_SYSTEM_KEYWORD;
									break;
								}
							}

							context.LogError(HTML::Tokenizer::ParserError::INVALID_CHARACTER_SEQUENCE_AFTER_DOCTYPE_NAME);
							doctypeToken.forceQuirks = true;
							reconsume = true;
							context.state = HTML::Tokenizer::ParserState::BOGUS_DOCTYPE;
							break;
					}
				}
				break;
			case HTML::Tokenizer::ParserState::AFTER_DOCTYPE_PUBLIC_KEYWORD:
				if (eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_DOCTYPE);
					treeConstructor.EmitDoctypeQuirksToken();
					treeConstructor.EmitEOFToken();
				} else {
					switch (character) {
						case '\t':
						case '\n':
						case '\f':
						case ' ':
							context.state = HTML::Tokenizer::ParserState::BEFORE_DOCTYPE_PUBLIC_IDENTIFIER;
							break;
						case '"':
							context.LogError(HTML::Tokenizer::ParserError::MISSING_WHITESPACE_AFTER_DOCTYPE_PUBLIC_KEYWORD);
							doctypeToken.publicIdentifier.emplace("");
							context.state = HTML::Tokenizer::ParserState::DOCTYPE_PUBLIC_IDENTIFIER_DQ;
							break;
						case '\'':
							context.LogError(HTML::Tokenizer::ParserError::MISSING_WHITESPACE_AFTER_DOCTYPE_PUBLIC_KEYWORD);
							doctypeToken.publicIdentifier.emplace("");
							context.state = HTML::Tokenizer::ParserState::DOCTYPE_PUBLIC_IDENTIFIER_SQ;
							break;
						case '>':
							context.LogError(HTML::Tokenizer::ParserError::MISSING_DOCTYPE_PUBLIC_IDENTIFIER);
							doctypeToken.forceQuirks = true;
							treeConstructor.EmitToken(doctypeToken);
							doctypeToken = HTML::Tokenizer::DoctypeToken(); // reset
							context.state = HTML::Tokenizer::ParserState::DATA;
							break;
						default:
							std::cout << "At AFTER_DOCTYPE_PUBLIC_KEYWORD" << std::endl;
							context.LogError(HTML::Tokenizer::ParserError::MISSING_QOUTE_BEFORE_DOCTYPE_PUBLIC_IDENTIFIER);
							doctypeToken.forceQuirks = true;
							reconsume = true;
							context.state = HTML::Tokenizer::ParserState::BOGUS_DOCTYPE;
							break;
					}
				}
				break;
			case HTML::Tokenizer::ParserState::BEFORE_DOCTYPE_PUBLIC_IDENTIFIER:
				if (eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_DOCTYPE);
					treeConstructor.EmitDoctypeQuirksToken();
					treeConstructor.EmitEOFToken();
				} else {
					switch (character) {
						case '\t':
						case '\n':
						case '\f':
						case ' ':
							// Ignore
							break;
						case '"':
							doctypeToken.publicIdentifier.emplace("");
							context.state = HTML::Tokenizer::ParserState::DOCTYPE_PUBLIC_IDENTIFIER_DQ;
							break;
						case '\'':
							doctypeToken.publicIdentifier.emplace("");
							context.state = HTML::Tokenizer::ParserState::DOCTYPE_PUBLIC_IDENTIFIER_SQ;
							break;
						case '>':
							context.LogError(HTML::Tokenizer::ParserError::MISSING_DOCTYPE_PUBLIC_IDENTIFIER);
							doctypeToken.forceQuirks = true;
							treeConstructor.EmitToken(doctypeToken);
							doctypeToken = HTML::Tokenizer::DoctypeToken(); // reset
							context.state = HTML::Tokenizer::ParserState::DATA;
							break;
						default:
							context.LogError(HTML::Tokenizer::ParserError::MISSING_QOUTE_BEFORE_DOCTYPE_PUBLIC_IDENTIFIER);
							doctypeToken.forceQuirks = true;
							reconsume = true;
							context.state = HTML::Tokenizer::ParserState::BOGUS_DOCTYPE;
							break;
					}
				}
				break;
			case HTML::Tokenizer::ParserState::DOCTYPE_PUBLIC_IDENTIFIER_DQ:
				if (eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_DOCTYPE);
					treeConstructor.EmitDoctypeQuirksToken();
					treeConstructor.EmitEOFToken();
				} else {
					switch (character) {
						case '"':
							context.state = HTML::Tokenizer::ParserState::AFTER_DOCTYPE_PUBLIC_IDENTIFIER;
							break;
						case '\0':
							context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_NULL_CHARACTER);
							doctypeToken.publicIdentifier = doctypeToken.publicIdentifier.value() + Unicode::REPLACEMENT_CHARACTER;
							break;
						case '>':
							context.LogError(HTML::Tokenizer::ParserError::ABRUBT_DOCTYPE_PUBLIC_IDENTIFIER);
							doctypeToken.forceQuirks = true;
							treeConstructor.EmitToken(doctypeToken);
							doctypeToken = HTML::Tokenizer::DoctypeToken(); // reset
							context.state = HTML::Tokenizer::ParserState::DATA;
							break;
						default:
							doctypeToken.publicIdentifier = doctypeToken.publicIdentifier.value() + character;
							break;
					}
				}
				break;
			case HTML::Tokenizer::ParserState::DOCTYPE_PUBLIC_IDENTIFIER_SQ:
				if (eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_DOCTYPE);
					treeConstructor.EmitDoctypeQuirksToken();
					treeConstructor.EmitEOFToken();
				} else {
					switch (character) {
						case '\'':
							context.state = HTML::Tokenizer::ParserState::AFTER_DOCTYPE_PUBLIC_IDENTIFIER;
							break;
						case '\0':
							context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_NULL_CHARACTER);
							doctypeToken.publicIdentifier = doctypeToken.publicIdentifier.value() + Unicode::REPLACEMENT_CHARACTER;
							break;
						case '>':
							context.LogError(HTML::Tokenizer::ParserError::ABRUBT_DOCTYPE_PUBLIC_IDENTIFIER);
							doctypeToken.forceQuirks = true;
							treeConstructor.EmitToken(doctypeToken);
							doctypeToken = HTML::Tokenizer::DoctypeToken(); // reset
							context.state = HTML::Tokenizer::ParserState::DATA;
							break;
						default:
							doctypeToken.publicIdentifier = doctypeToken.publicIdentifier.value() + character;
							break;
					}
				}
				break;
			case HTML::Tokenizer::ParserState::AFTER_DOCTYPE_PUBLIC_IDENTIFIER:
				if (eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_DOCTYPE);
					treeConstructor.EmitDoctypeQuirksToken();
					treeConstructor.EmitEOFToken();
				} else {
					switch (character) {
						case '\t':
						case '\n':
						case '\f':
						case ' ':
							context.state = HTML::Tokenizer::ParserState::BETWEEN_DOCTYPE_PUBLIC_SYSTEM_IDENTIFIER;
							break;
						case '>':
							treeConstructor.EmitToken(doctypeToken);
							doctypeToken = HTML::Tokenizer::DoctypeToken(); // reset
							context.state = HTML::Tokenizer::ParserState::DATA;
							break;
						case '"':
							context.LogError(HTML::Tokenizer::ParserError::MISSING_WHITESPACE_BETWEEN_DOCTYPE_PUBLIC_AND_SYSTEM_IDENTIFIERS);
							doctypeToken.systemIdentifier.emplace("");
							context.state = HTML::Tokenizer::ParserState::DOCTYPE_SYSTEM_IDENTIFIER_DQ;
							break;
						case '\'':
							context.LogError(HTML::Tokenizer::ParserError::MISSING_WHITESPACE_BETWEEN_DOCTYPE_PUBLIC_AND_SYSTEM_IDENTIFIERS);
							doctypeToken.systemIdentifier.emplace("");
							context.state = HTML::Tokenizer::ParserState::DOCTYPE_SYSTEM_IDENTIFIER_SQ;
							break;
						default:
							context.LogError(HTML::Tokenizer::ParserError::MISSING_QOUTE_BEFORE_DOCTYPE_SYSTEM_IDENTIFIER);
							doctypeToken.forceQuirks = true;
							reconsume = true;
							context.state = HTML::Tokenizer::ParserState::BOGUS_DOCTYPE;
							break;
					}
				}
				break;
			case HTML::Tokenizer::ParserState::BETWEEN_DOCTYPE_PUBLIC_SYSTEM_IDENTIFIER:
				if (eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_DOCTYPE);
					treeConstructor.EmitDoctypeQuirksToken();
					treeConstructor.EmitEOFToken();
				} else {
					switch (character) {
						case '\t':
						case '\n':
						case '\f':
						case ' ':
							// Ignore
							break;
						case '>':
							treeConstructor.EmitToken(doctypeToken);
							doctypeToken = HTML::Tokenizer::DoctypeToken(); // reset
							context.state = HTML::Tokenizer::ParserState::DATA;
							break;
						case '"':
							doctypeToken.systemIdentifier.emplace("");
							context.state = HTML::Tokenizer::ParserState::DOCTYPE_SYSTEM_IDENTIFIER_DQ;
							break;
						case '\'':
							doctypeToken.systemIdentifier.emplace("");
							context.state = HTML::Tokenizer::ParserState::DOCTYPE_SYSTEM_IDENTIFIER_SQ;
							break;
						default:
							context.LogError(HTML::Tokenizer::ParserError::MISSING_QOUTE_BEFORE_DOCTYPE_PUBLIC_IDENTIFIER);
							doctypeToken.forceQuirks = true;
							reconsume = true;
							context.state = HTML::Tokenizer::ParserState::BOGUS_DOCTYPE;
							break;
					}
				}
				break;
			case HTML::Tokenizer::ParserState::AFTER_DOCTYPE_SYSTEM_KEYWORD:
				if (eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_DOCTYPE);
					treeConstructor.EmitDoctypeQuirksToken();
					treeConstructor.EmitEOFToken();
				} else {
					switch (character) {
						case '\t':
						case '\n':
						case '\f':
						case ' ':
							context.state = HTML::Tokenizer::ParserState::BEFORE_DOCTYPE_SYSTEM_IDENTIFIER;
							break;
						case '"':
							context.LogError(HTML::Tokenizer::ParserError::MISSING_WHITESPACE_AFTER_DOCTYPE_SYSTEM_KEYWORD);
							doctypeToken.systemIdentifier.emplace("");
							context.state = HTML::Tokenizer::ParserState::DOCTYPE_PUBLIC_IDENTIFIER_DQ;
							break;
						case '\'':
							context.LogError(HTML::Tokenizer::ParserError::MISSING_WHITESPACE_AFTER_DOCTYPE_SYSTEM_KEYWORD);
							doctypeToken.systemIdentifier.emplace("");
							context.state = HTML::Tokenizer::ParserState::DOCTYPE_PUBLIC_IDENTIFIER_SQ;
							break;
						case '>':
							context.LogError(HTML::Tokenizer::ParserError::MISSING_DOCTYPE_SYSTEM_IDENTIFIER);
							doctypeToken.forceQuirks = true;
							treeConstructor.EmitToken(doctypeToken);
							doctypeToken = HTML::Tokenizer::DoctypeToken(); // reset
							context.state = HTML::Tokenizer::ParserState::DATA;
							break;
						default:
							std::cout << "At AFTER_DOCTYPE_SYSTEM_KEYWORD" << std::endl;
							context.LogError(HTML::Tokenizer::ParserError::MISSING_QOUTE_BEFORE_DOCTYPE_SYSTEM_IDENTIFIER);
							doctypeToken.forceQuirks = true;
							reconsume = true;
							context.state = HTML::Tokenizer::ParserState::BOGUS_DOCTYPE;
							break;
					}
				}
				break;
			case HTML::Tokenizer::ParserState::BEFORE_DOCTYPE_SYSTEM_IDENTIFIER:
				if (eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_DOCTYPE);
					treeConstructor.EmitDoctypeQuirksToken();
					treeConstructor.EmitEOFToken();
				} else {
					switch (character) {
						case '\t':
						case '\n':
						case '\f':
						case ' ':
							// Ignore
							break;
						case '"':
							doctypeToken.systemIdentifier.emplace("");
							context.state = HTML::Tokenizer::ParserState::DOCTYPE_SYSTEM_IDENTIFIER_DQ;
							break;
						case '\'':
							doctypeToken.systemIdentifier.emplace("");
							context.state = HTML::Tokenizer::ParserState::DOCTYPE_SYSTEM_IDENTIFIER_SQ;
							break;
						case '>':
							context.LogError(HTML::Tokenizer::ParserError::MISSING_DOCTYPE_SYSTEM_IDENTIFIER);
							doctypeToken.forceQuirks = true;
							treeConstructor.EmitToken(doctypeToken);
							doctypeToken = HTML::Tokenizer::DoctypeToken(); // reset
							context.state = HTML::Tokenizer::ParserState::DATA;
							break;
						default:
							context.LogError(HTML::Tokenizer::ParserError::MISSING_QOUTE_BEFORE_DOCTYPE_SYSTEM_IDENTIFIER);
							doctypeToken.forceQuirks = true;
							reconsume = true;
							context.state = HTML::Tokenizer::ParserState::BOGUS_DOCTYPE;
							break;
					}
				}
				break;
			case HTML::Tokenizer::ParserState::DOCTYPE_SYSTEM_IDENTIFIER_DQ:
				if (eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_DOCTYPE);
					treeConstructor.EmitDoctypeQuirksToken();
					treeConstructor.EmitEOFToken();
				} else {
					switch (character) {
						case '"':
							context.state = HTML::Tokenizer::ParserState::AFTER_DOCTYPE_SYSTEM_IDENTIFIER;
							break;
						case '\0':
							context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_NULL_CHARACTER);
							doctypeToken.systemIdentifier = doctypeToken.systemIdentifier.value() + Unicode::REPLACEMENT_CHARACTER;
							break;
						case '>':
							context.LogError(HTML::Tokenizer::ParserError::ABRUBT_DOCTYPE_SYSTEM_IDENTIFIER);
							doctypeToken.forceQuirks = true;
							treeConstructor.EmitToken(doctypeToken);
							doctypeToken = HTML::Tokenizer::DoctypeToken(); // reset
							context.state = HTML::Tokenizer::ParserState::DATA;
							break;
						default:
							doctypeToken.systemIdentifier = doctypeToken.systemIdentifier.value() + character;
							break;
					}
				}
				break;
			case HTML::Tokenizer::ParserState::DOCTYPE_SYSTEM_IDENTIFIER_SQ:
				if (eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_DOCTYPE);
					treeConstructor.EmitDoctypeQuirksToken();
					treeConstructor.EmitEOFToken();
				} else {
					switch (character) {
						case '\'':
							context.state = HTML::Tokenizer::ParserState::AFTER_DOCTYPE_SYSTEM_IDENTIFIER;
							break;
						case '\0':
							context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_NULL_CHARACTER);
							doctypeToken.systemIdentifier = doctypeToken.systemIdentifier.value() + Unicode::REPLACEMENT_CHARACTER;
							break;
						case '>':
							context.LogError(HTML::Tokenizer::ParserError::ABRUBT_DOCTYPE_SYSTEM_IDENTIFIER);
							doctypeToken.forceQuirks = true;
							treeConstructor.EmitToken(doctypeToken);
							doctypeToken = HTML::Tokenizer::DoctypeToken(); // reset
							context.state = HTML::Tokenizer::ParserState::DATA;
							break;
						default:
							doctypeToken.systemIdentifier = doctypeToken.systemIdentifier.value() + character;
							break;
					}
				}
				break;
			case HTML::Tokenizer::ParserState::AFTER_DOCTYPE_SYSTEM_IDENTIFIER:
				if (eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_DOCTYPE);
					treeConstructor.EmitDoctypeQuirksToken();
					treeConstructor.EmitEOFToken();
				} else {
					switch (character) {
						case '\t':
						case '\n':
						case '\f':
						case ' ':
							// Ignore
							break;
						case '>':
							treeConstructor.EmitToken(doctypeToken);
							doctypeToken = HTML::Tokenizer::DoctypeToken(); // reset
							context.state = HTML::Tokenizer::ParserState::DATA;
							break;
						default:
							context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_CHARACTER_AFTER_DOCTYPE_SYSTEM_IDENTIFIER);
							reconsume = true;
							context.state = HTML::Tokenizer::ParserState::BOGUS_DOCTYPE;
							break;
					}
				}
				break;
			case HTML::Tokenizer::ParserState::BOGUS_DOCTYPE:
				if (eof) {
					context.LogError(HTML::Tokenizer::ParserError::EOF_IN_DOCTYPE);
					treeConstructor.EmitToken(doctypeToken);
					treeConstructor.EmitEOFToken();
				} else {
					switch (character) {
						case '>':
							treeConstructor.EmitToken(doctypeToken);
							doctypeToken = HTML::Tokenizer::DoctypeToken(); // reset
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
				if (!eof) {
					if (character == Unicode::NUMBER_SIGN) {
						context.temporaryBuffer.push_back(character);
						context.state = HTML::Tokenizer::ParserState::NUMERIC_CHARACTER_REFERENCE;
						break;
					}

					if (Unicode::IsASCIIAlphaNumeric(character)) {
						reconsume = true;
						context.state = HTML::Tokenizer::ParserState::NAMED_CHARACTER_REFERENCE;
						break;
					}
				}
				break;
				/*
			case HTML::Tokenizer::ParserState::NAMED_CHARACTER_REFERENCE: {
				// NamedCharacters::NCStatus status = NamedCharacters::Find(
				bool wasMatch = false;
				if (character != Unicode::SEMICOLON) {
					context.NCRefBuffer += character;
					std::cout << "NAMED_CHARACTER_REFERENCE: " << context.temporaryBuffer[0] << ' ' << character << std::endl;
					// TODO Check for match, if so do shit & "break;"
				}

				if (character != Unicode::SEMICOLON) {
					context.LogError(HTML::Tokenizer::ParserError::UNEXPECTED_NULL_CHARACTER);
				}

				// The named reference isn't found, thus:
				// flush code points consumed as a character reference
				switch (context.returnState) {
					case HTML::Tokenizer::ParserState::ATTRIBUTE_VALUE_DQ:
					case HTML::Tokenizer::ParserState::ATTRIBUTE_VALUE_SQ:
					case HTML::Tokenizer::ParserState::ATTRIBUTE_VALUE_NQ: {
						HTML::Tokenizer::AmbiguousTagToken &tagToken = (isEndTag ?
							(HTML::Tokenizer::AmbiguousTagToken &) endTagToken :
							(HTML::Tokenizer::AmbiguousTagToken &) startTagToken);

						for (size_t k = 0; k < context.NCRefBuffer.length(); i++) {
							tagToken.attributeValue += context.NCRefBuffer[i];
						}
					} break;
					default:
						for (size_t k = 0; k < context.NCRefBuffer.length(); i++) {
							treeConstructor.EmitCharacterToken(context.NCRefBuffer[i]);
						}
						break;
				}
				context.state = wasMatch ?
					context.returnState :
					HTML::Tokenizer::ParserState::AMBIGOUS_AMPERSAND;
			} break;
			*/
			default:
				std::cout << "Unknown state(" << ++unknownStateCount << "): " << context.state << std::endl;
				break;
		}
	}
}
