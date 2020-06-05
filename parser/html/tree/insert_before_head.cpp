/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "insert_before_head.hpp"

#include <iostream>
#include <sstream>
#include <vector>

#include "dom/comment.hpp"
#include "dom/element.hpp"
#include "logger.hpp"
#include "parser/html/constants.hpp"
#include "parser/html/context.hpp"

bool
HTML::InsertionModes::BeforeHead::EmitToken(HTML::Tokenizer::Token &inToken) {
	HTML::Tokenizer::CharacterToken *characterToken = nullptr;
	HTML::Tokenizer::CommentToken	*commentToken = nullptr;
	HTML::Tokenizer::StartTagToken	*startTagToken = nullptr;

	startTagToken = nullptr;

	switch (inToken.type()) {
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

			if (startTagToken->tagName.EqualsA("head")) {
				constructor.insertionModes[InsertionModeType::IN_BODY]->EmitToken(inToken);
				return false;
			}

			if (startTagToken->tagName.EqualsA("head")) {
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
			}

			std::cout << startTagToken->tagName << " != html" << std::endl;

			/* Fallthrough */
		case HTML::Tokenizer::TokenType::ENDTAG:
			if (startTagToken == nullptr) {
				Logger::Warning("HTMLParser::InsertBeforeHead", "Invalid structure! ENDTAG before STARTTAG");
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
