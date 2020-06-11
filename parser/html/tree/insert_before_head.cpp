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
		std::make_shared<DOM::Comment>(dynamic_cast<HTML::Tokenizer::CommentToken *>(&token)->contents));
	return HTML::InsertionModeSubroutineStatus::IGNORE;
}

HTML::InsertionModeSubroutineStatus
HTML::InsertionModes::BeforeHead::HandleEndTag(HTML::Tokenizer::Token &token) {
	auto endTagToken = dynamic_cast<HTML::Tokenizer::EndTagToken *>(&token);

	if (endTagToken->tagName.EqualsA("head") || endTagToken->tagName.EqualsA("body")
		|| endTagToken->tagName.EqualsA("html") || endTagToken->tagName.EqualsA("br")) {
		return HTML::InsertionModeSubroutineStatus::CONTINUE;
	}

	context.parserContext.ReportParserError("BeforeHTMLInsertionMode", "Unexpected end tag!");
	return HTML::InsertionModeSubroutineStatus::IGNORE;
}

HTML::InsertionModeSubroutineStatus
HTML::InsertionModes::BeforeHead::HandleStartTag(HTML::Tokenizer::Token &token) {
	(void) token;

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

	return true;
}
