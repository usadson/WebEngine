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

HTML::InsertionModeSubroutineStatus
HTML::InsertionModes::BeforeHead::HandleCharacter(HTML::Tokenizer::Token &token) {
	switch (dynamic_cast<HTML::Tokenizer::CharacterToken *>(&token)->character) {
		case Unicode::CHARACTER_TABULATION:
		case Unicode::LINE_FEED:
		case Unicode::FORM_FEED:
		case Unicode::CARRIAGE_RETURN:
		case Unicode::SPACE:
			return HTML::InsertionModeSubroutineStatus::IGNORE;
		default:
			return HTML::InsertionModeSubroutineStatus::CONTINUE;
	}
}

HTML::InsertionModeSubroutineStatus
HTML::InsertionModes::BeforeHead::HandleComment(HTML::Tokenizer::Token &token) {
	context.parserContext.documentNode->childNodes.push_back(
		DOM::Comment(dynamic_cast<HTML::Tokenizer::CommentToken *>(&token)->contents));
	return HTML::InsertionModeSubroutineStatus::IGNORE;
}

HTML::InsertionModeSubroutineStatus
HTML::InsertionModes::BeforeHead::HandleEndTag(HTML::Tokenizer::Token &token) {
	/*
	 * Wait... what is going on here?
	 * FIXME since June 8th 2020

	if (startTagToken == nullptr) {
		Logger::Warning("HTMLParser::InsertBeforeHTML", "Invalid structure! ENDTAG before STARTTAG");
		return HTML::InsertionModeSubroutineStatus::PARSER_ERROR;
	}

	if (!startTagToken->tagName.EqualsA("html") &&
		!startTagToken->tagName.EqualsA("body") &&
		!startTagToken->tagName.EqualsA("html") &&
		!startTagToken->tagName.EqualsA("br")) {
		// Parse error. Ignore the token.
		return HTML::InsertionModeSubroutineStatus::PARSER_ERROR;
	}

	return HTML::InsertionModeSubroutineStatus::CONTINUE;
	*/

	// FIXME Workaround:
	(void)token;
	return HTML::InsertionModeSubroutineStatus::IGNORE;
}

HTML::InsertionModeSubroutineStatus
HTML::InsertionModes::BeforeHead::HandleStartTag(HTML::Tokenizer::Token &token) {
	auto startTagToken = dynamic_cast<HTML::Tokenizer::StartTagToken *>(&token);

	if (startTagToken->tagName.EqualsA("html")) {
		std::shared_ptr<DOM::Element> element = std::make_shared<DOM::Element>();
		element->namespaceURI = HTML::Constants::HTMLNamespace;
		element->localName = startTagToken->tagName;
		element->document = context.parserContext.documentNode;
		context.parserContext.documentNode->children.push_back(element);
		constructor.openElementsStack.push_back(element);

		for (auto const &attribute : startTagToken->attributes) {
			element->internalAttributes.insert(attribute);
		}

		constructor.currentMode = HTML::InsertionModeType::BEFORE_HEAD;
		return HTML::InsertionModeSubroutineStatus::IGNORE;
	} else
		std::cout << startTagToken->tagName << " != html" << std::endl;

	return HTML::InsertionModeSubroutineStatus::CONTINUE;
}

bool
HTML::InsertionModes::BeforeHead::EmitToken(HTML::Tokenizer::Token &inToken) {
	std::map<HTML::Tokenizer::TokenType, HTML::InsertionModeSubroutineStatus (BeforeHead::*)(HTML::Tokenizer::Token &)>
		funcMap = { { HTML::Tokenizer::TokenType::CHARACTER, &BeforeHead::HandleCharacter },
					{ HTML::Tokenizer::TokenType::COMMENT, &BeforeHead::HandleComment },
					{ HTML::Tokenizer::TokenType::ENDTAG, &BeforeHead::HandleEndTag },
					{ HTML::Tokenizer::TokenType::STARTTAG, &BeforeHead::HandleStartTag } };

	auto it = funcMap.find(inToken.type());
	if (it != std::end(funcMap)) {
		auto status = (this->*(it->second))(inToken);
		if (status == HTML::InsertionModeSubroutineStatus::IGNORE
			|| status == HTML::InsertionModeSubroutineStatus::PARSER_ERROR) {
			return false;
		}
		if (status == HTML::InsertionModeSubroutineStatus::RECONSUME)
			return true;
	}

	// This is just repeating the STARTTAG thing, so maybe create a subroutine?
	// also, the 'Application Cache Selection Algorithm' should be executed.
	std::shared_ptr<DOM::Element> element = std::make_shared<DOM::Element>();
	element->namespaceURI = HTML::Constants::HTMLNamespace;
	element->localName = Unicode::UString("head");
	element->document = context.parserContext.documentNode;
	context.parserContext.documentNode->children.push_back(element);
	constructor.openElementsStack.push_back(element);

	constructor.currentMode = HTML::InsertionModeType::BEFORE_HEAD;
	return true;
}
