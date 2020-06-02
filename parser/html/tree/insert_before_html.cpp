/**
 * Copyright (C) 2020 Tristan
 *
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
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
