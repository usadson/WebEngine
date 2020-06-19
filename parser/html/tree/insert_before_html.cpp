/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "insert_before_html.hpp"

#include <iostream>
#include <vector>

#include "dom/comment.hpp"
#include "dom/element.hpp"
#include "dom/html_html_element.hpp"
#include "logger.hpp"
#include "parser/html/constants.hpp"
#include "parser/html/context.hpp"

HTML::InsertionModeSubroutineStatus
HTML::InsertionModes::BeforeHTML::HandleCharacter(HTML::Tokenizer::Token &token) {
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
HTML::InsertionModes::BeforeHTML::HandleComment(HTML::Tokenizer::Token &token) {
	constructor.InsertComment(dynamic_cast<HTML::Tokenizer::CommentToken *>(&token)->contents);
	return HTML::InsertionModeSubroutineStatus::IGNORE;
}

HTML::InsertionModeSubroutineStatus
HTML::InsertionModes::BeforeHTML::HandleDoctype(HTML::Tokenizer::Token &token) {
	(void)token;
	context.parserContext.ReportParserError("BeforeHTMLInsertionMode", "DOCTYPEs should be declared only once");
	return HTML::InsertionModeSubroutineStatus::IGNORE;
}

HTML::InsertionModeSubroutineStatus
HTML::InsertionModes::BeforeHTML::HandleEndTag(HTML::Tokenizer::Token &token) {
	auto *endTagToken = dynamic_cast<HTML::Tokenizer::EndTagToken *>(&token);

	if (endTagToken->tagName.EqualsIgnoreCaseA(0, "head") || endTagToken->tagName.EqualsIgnoreCaseA(0, "body")
		|| endTagToken->tagName.EqualsIgnoreCaseA(0, "html") || endTagToken->tagName.EqualsIgnoreCaseA(0, "br")) {
		return HTML::InsertionModeSubroutineStatus::CONTINUE;
	}

	context.parserContext.ReportParserError("BeforeHTMLInsertionMode", "Unexpected end tag!");
	return HTML::InsertionModeSubroutineStatus::IGNORE;
}

HTML::InsertionModeSubroutineStatus
HTML::InsertionModes::BeforeHTML::HandleStartTag(HTML::Tokenizer::Token &token) {
	auto *startTagToken = dynamic_cast<HTML::Tokenizer::StartTagToken *>(&token);

	if (startTagToken->tagName.EqualsIgnoreCaseA(0, "html")) {
		auto element = constructor.CreateElementForToken(*startTagToken, HTML::Constants::HTMLNamespace);
		constructor.openElementsStack.push_back(element);
		context.parserContext.documentNode->childNodes.push_back(element);

		/* Future service-worker and manifest caching stuff */

		constructor.currentMode = HTML::InsertionModeType::BEFORE_HEAD;
		return HTML::InsertionModeSubroutineStatus::IGNORE;
	}

	return HTML::InsertionModeSubroutineStatus::CONTINUE;
}

bool
HTML::InsertionModes::BeforeHTML::EmitToken(HTML::Tokenizer::Token &inToken) {
	if (inToken.type() == HTML::Tokenizer::TokenType::DOCTYPE)
		return false; // Parse error. Ignore the token.

	std::map<HTML::Tokenizer::TokenType, HTML::InsertionModeSubroutineStatus (BeforeHTML::*)(HTML::Tokenizer::Token &)>
		funcMap = { { HTML::Tokenizer::TokenType::CHARACTER, &BeforeHTML::HandleCharacter },
			{ HTML::Tokenizer::TokenType::COMMENT, &BeforeHTML::HandleComment },
			{ HTML::Tokenizer::TokenType::DOCTYPE, &BeforeHTML::HandleDoctype },
			{ HTML::Tokenizer::TokenType::ENDTAG, &BeforeHTML::HandleEndTag },
			{ HTML::Tokenizer::TokenType::STARTTAG, &BeforeHTML::HandleStartTag } };

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
	auto htmlElement = std::make_shared<DOM::HTMLElement>();
	constructor.openElementsStack.push_back(htmlElement);
	context.parserContext.documentNode->childNodes.push_back(htmlElement);

	/* Future service-worker and manifest caching stuff */

	constructor.currentMode = HTML::InsertionModeType::BEFORE_HEAD;
	return true;
}
