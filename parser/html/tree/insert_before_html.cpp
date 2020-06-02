/**
 * Copyright (C) 2020 Tristan <tristan@thewoosh.me>
 *
 * All Rights Reserved.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
 */

#include "insert_before_html.hpp"

#include <iostream>
#include <vector>

#include "dom/comment.hpp"
#include "dom/element.hpp"
#include "parser/html/constants.hpp"
#include "logger.hpp"

bool
HTML::InsertionModes::BeforeHTML::EmitToken(HTML::Tokenizer::Token &inToken) {
	HTML::Tokenizer::CharacterToken *characterToken;
	HTML::Tokenizer::CommentToken	*commentToken;
	HTML::Tokenizer::StartTagToken	*startTagToken;

	startTagToken = nullptr;

	switch (inToken.type) {
		case HTML::Tokenizer::TokenType::DOCTYPE:
			// Parse error. Ignore the token.
			return false;
		case HTML::Tokenizer::TokenType::COMMENT:
			commentToken = dynamic_cast<HTML::Tokenizer::CommentToken *>(&inToken);
			context.parserContext.documentNode.childNodes.push_back(DOM::Comment(commentToken->contents));
			return false;
		case HTML::Tokenizer::TokenType::CHARACTER:
			characterToken = dynamic_cast<HTML::Tokenizer::CharacterToken *>(&inToken);
			switch (characterToken->character) {
				case Unicode::CHARACTER_TABULATION:
				case Unicode::LINE_FEED:
				case Unicode::FORM_FEED:
				case Unicode::CARRIAGE_RETURN:
				case Unicode::SPACE:
					// Ignore this token:
					return false;
				default:
					break;
			}
			/* Fallthrough */
		case HTML::Tokenizer::TokenType::STARTTAG:
			startTagToken = dynamic_cast<HTML::Tokenizer::StartTagToken *>(&inToken);

			if (startTagToken->tagName.EqualsA("html")) {
				std::shared_ptr<DOM::Element> element = std::make_shared<DOM::Element>();
				element->namespaceURI = HTML::Constants::HTMLNamespace;
				element->localName = startTagToken->tagName;
				element->document = context.parserContext.documentNode;
				context.parserContext.documentNode.children.push_back(element);
				constructor.openElementsStack.push_back(element);

				for (auto const &attribute : startTagToken->attributes) {
					element->internalAttributes.insert(attribute);
				}

				constructor.currentMode = HTML::InsertionModeType::BEFORE_HEAD;
				return false;
			} else
				std::cout << startTagToken->tagName << " != html" << std::endl;

			/* Fallthrough */
		case HTML::Tokenizer::TokenType::ENDTAG:
			if (startTagToken == nullptr) {
				Logger::Warning("HTMLParser::InsertBeforeHTML", "Invalid structure! ENDTAG before STARTTAG");
				return false;
			}

			if (!startTagToken->tagName.EqualsA("html") &&
				!startTagToken->tagName.EqualsA("body") &&
				!startTagToken->tagName.EqualsA("html") &&
				!startTagToken->tagName.EqualsA("br")) {
				// Parse error. Ignore the token.
				return false;
			}
			/* Else: Fallthrough */	
		default:
			break;
	}

	// This is just repeating the STARTTAG thing, so maybe create a subroutine?
	// also, the 'Application Cache Selection Algorithm' should be executed.
	std::shared_ptr<DOM::Element> element = std::make_shared<DOM::Element>();
	element->namespaceURI = HTML::Constants::HTMLNamespace;
	element->localName = Unicode::UString("html");
	element->document = context.parserContext.documentNode;
	context.parserContext.documentNode.children.push_back(element);
	constructor.openElementsStack.push_back(element);

	constructor.currentMode = HTML::InsertionModeType::BEFORE_HEAD;
	return true;
}
