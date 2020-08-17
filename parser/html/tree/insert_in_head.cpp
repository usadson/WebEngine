/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "insert_in_head.hpp"

#include <iostream>
#include <vector>

#include "parser/html/context.hpp"

#include "dom/comment.hpp"
#include "dom/document_type.hpp"
#include "logger.hpp"

HTML::InsertionModeSubroutineStatus
HTML::InsertionModes::InHead::HandleCharacter(HTML::Tokenizer::Token &token) {
	switch (dynamic_cast<HTML::Tokenizer::CharacterToken *>(&token)->character) {
		case Unicode::CHARACTER_TABULATION:
		case Unicode::LINE_FEED:
		case Unicode::FORM_FEED:
		case Unicode::CARRIAGE_RETURN:
		case Unicode::SPACE:
			// TODO Insert the character
			return HTML::InsertionModeSubroutineStatus::IGNORE;
		default:
			return HTML::InsertionModeSubroutineStatus::CONTINUE;
	}
}

HTML::InsertionModeSubroutineStatus
HTML::InsertionModes::InHead::HandleComment(HTML::Tokenizer::Token &token) {
	constructor.InsertComment(dynamic_cast<HTML::Tokenizer::CommentToken *>(&token)->contents);
	return HTML::InsertionModeSubroutineStatus::CONTINUE;
}

HTML::InsertionModeSubroutineStatus
HTML::InsertionModes::InHead::HandleDoctype(HTML::Tokenizer::Token &token) {
	(void)token;
	context.parserContext.ReportParserError("InHeadInsertionMode", "Found DOCTYPE in unexpected location");
	return HTML::InsertionModeSubroutineStatus::IGNORE;
}

HTML::InsertionModeSubroutineStatus
HTML::InsertionModes::InHead::HandleEndTag(HTML::Tokenizer::Token &token) {
	(void)token;
	return HTML::InsertionModeSubroutineStatus::CONTINUE;
}

HTML::InsertionModeSubroutineStatus
HTML::InsertionModes::InHead::HandleStartTag(HTML::Tokenizer::Token &token) {
	auto *startTagToken = dynamic_cast<HTML::Tokenizer::StartTagToken *>(&token);
	if (startTagToken->tagName.EqualsIgnoreCaseA(0, "html")) {
		auto status = constructor.insertionModes.find(HTML::InsertionModeType::IN_BODY)->second->EmitToken(token);
		return status ? HTML::InsertionModeSubroutineStatus::RECONSUME : HTML::InsertionModeSubroutineStatus::IGNORE;
	}

	bool isMetaToken = startTagToken->tagName.EqualsIgnoreCaseA(0, "meta");
	if (isMetaToken || startTagToken->tagName.EqualsIgnoreCaseA(0, "base")
		|| startTagToken->tagName.EqualsIgnoreCaseA(0, "basefont")
		|| startTagToken->tagName.EqualsIgnoreCaseA(0, "bgsound")
		|| startTagToken->tagName.EqualsIgnoreCaseA(0, "link")) {
		constructor.InsertSelfClosingToken(*startTagToken);

		if (isMetaToken) {
			auto attribute = startTagToken->GetAttribute(HTML::Constants::AttributeNameMeta);
			if (attribute.has_value()) {
				Logger::Crash(__PRETTY_FUNCTION__, "<meta charset> is not supported yet.");
			}
			attribute = startTagToken->GetAttribute(HTML::Constants::AttributeNameHTTPEquiv);
			if (attribute.has_value()) {
				Logger::Crash(__PRETTY_FUNCTION__, "<meta http-equiv> is not supported yet.");
			}
		}

		return HTML::InsertionModeSubroutineStatus::IGNORE;
	}

	if (startTagToken->tagName.EqualsIgnoreCaseA(0, "head")) {
		context.parserContext.ReportParserError("InHeadInsertionMode", "Found <head> tag inside another head element");
		return HTML::InsertionModeSubroutineStatus::IGNORE;
	}

	return HTML::InsertionModeSubroutineStatus::CONTINUE;
}

bool
HTML::InsertionModes::InHead::EmitToken(HTML::Tokenizer::Token &inToken) {
	std::map<HTML::Tokenizer::TokenType, HTML::InsertionModeSubroutineStatus (InHead::*)(HTML::Tokenizer::Token &)>
		funcMap = {{HTML::Tokenizer::TokenType::CHARACTER, &InHead::HandleCharacter},
			{HTML::Tokenizer::TokenType::COMMENT, &InHead::HandleComment},
			{HTML::Tokenizer::TokenType::DOCTYPE, &InHead::HandleDoctype},
			{HTML::Tokenizer::TokenType::ENDTAG, &InHead::HandleEndTag},
			{HTML::Tokenizer::TokenType::STARTTAG, &InHead::HandleStartTag}};

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

	return false;
}
