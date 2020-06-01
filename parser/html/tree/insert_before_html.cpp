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

	switch (inToken.Type) {
		case HTML::Tokenizer::TokenType::DOCTYPE:
			// Parse error. Ignore the token.
			return false;
		case HTML::Tokenizer::TokenType::COMMENT:
			commentToken = dynamic_cast<HTML::Tokenizer::CommentToken *>(&inToken);
			Context.ParserContext.DocumentNode.ChildNodes.push_back(DOM::Comment(commentToken->Contents));
			return false;
		case HTML::Tokenizer::TokenType::CHARACTER:
			characterToken = dynamic_cast<HTML::Tokenizer::CharacterToken *>(&inToken);
			switch (characterToken->Character) {
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

			if (startTagToken->TagName.EqualsA("html")) {
				std::shared_ptr<DOM::Element> element = std::make_shared<DOM::Element>();
				element->NamespaceURI = HTML::Constants::HTMLNamespace;
				element->LocalName = startTagToken->TagName;
				element->Document = Context.ParserContext.DocumentNode;
				Context.ParserContext.DocumentNode.Children.push_back(element);
				Constructor.OpenElementsStack.push_back(element);

				for (auto const &attribute : startTagToken->Attributes) {
					element->InternalAttributes.insert(attribute);
				}

				Constructor.CurrentMode = HTML::InsertionModeType::BEFORE_HEAD;
				return false;
			} else
				std::cout << startTagToken->TagName << " != html" << std::endl;

			/* Fallthrough */
		case HTML::Tokenizer::TokenType::ENDTAG:
			if (!startTagToken->TagName.EqualsA("html") &&
				!startTagToken->TagName.EqualsA("body") &&
				!startTagToken->TagName.EqualsA("html") &&
				!startTagToken->TagName.EqualsA("br")) {
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
	element->NamespaceURI = HTML::Constants::HTMLNamespace;
	element->LocalName = "html";
	element->Document = Context.ParserContext.DocumentNode;
	Context.ParserContext.DocumentNode.Children.push_back(element);
	Constructor.OpenElementsStack.push_back(element);

	Constructor.CurrentMode = HTML::InsertionModeType::BEFORE_HEAD;
	return true;
}
