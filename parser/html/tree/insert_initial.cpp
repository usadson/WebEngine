/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "insert_initial.hpp"

#include <iostream>
#include <vector>

#include "parser/html/context.hpp"

#include "dom/comment.hpp"
#include "logger.hpp"

const std::vector<const char *> quirkyPublicIdentifiersMissingSystem
	= { "-//W3C//DTD HTML 4.01 Frameset//", "-//W3C//DTD HTML 4.01 Transitional//" };

const std::vector<const char *> quirkyPublicIdentifiers
	= { "-//W3O//DTD W3 HTML Strict 3.0//EN//",
		"-/W3C/DTD HTML 4.0 Transitional/EN",
		"HTML",
		"+//Silmaril//dtd html Pro v0r11 19970101//",
		"-//AS//DTD HTML 3.0 asWedit + extensions//",
		"-//AdvaSoft Ltd//DTD HTML 3.0 asWedit + extensions//",
		"-//IETF//DTD HTML 2.0 Level 1//",
		"-//IETF//DTD HTML 2.0 Level 2//",
		"-//IETF//DTD HTML 2.0 Strict Level 1//",
		"-//IETF//DTD HTML 2.0 Strict Level 2//",
		"-//IETF//DTD HTML 2.0 Strict//",
		"-//IETF//DTD HTML 2.0//",
		"-//IETF//DTD HTML 2.1E//",
		"-//IETF//DTD HTML 3.0//",
		"-//IETF//DTD HTML 3.2 Final//",
		"-//IETF//DTD HTML 3.2//",
		"-//IETF//DTD HTML 3//",
		"-//IETF//DTD HTML Level 0//",
		"-//IETF//DTD HTML Level 1//",
		"-//IETF//DTD HTML Level 2//",
		"-//IETF//DTD HTML Level 3//",
		"-//IETF//DTD HTML Strict Level 0//",
		"-//IETF//DTD HTML Strict Level 1//",
		"-//IETF//DTD HTML Strict Level 2//",
		"-//IETF//DTD HTML Strict Level 3//",
		"-//IETF//DTD HTML Strict//",
		"-//IETF//DTD HTML//",
		"-//Metrius//DTD Metrius Presentational//",
		"-//Microsoft//DTD Internet Explorer 2.0 HTML Strict//",
		"-//Microsoft//DTD Internet Explorer 2.0 HTML//",
		"-//Microsoft//DTD Internet Explorer 2.0 Tables//",
		"-//Microsoft//DTD Internet Explorer 3.0 HTML Strict//",
		"-//Microsoft//DTD Internet Explorer 3.0 HTML//",
		"-//Microsoft//DTD Internet Explorer 3.0 Tables//",
		"-//Netscape Comm. Corp.//DTD HTML//",
		"-//Netscape Comm. Corp.//DTD Strict HTML//",
		"-//O'Reilly and Associates//DTD HTML 2.0//",
		"-//O'Reilly and Associates//DTD HTML Extended 1.0//",
		"-//O'Reilly and Associates//DTD HTML Extended Relaxed 1.0//",
		"-//SQ//DTD HTML 2.0 HoTMetaL + extensions//",
		"-//SoftQuad Software//DTD HoTMetaL PRO 6.0::19990601::extensions to HTML 4.0//",
		"-//SoftQuad//DTD HoTMetaL PRO 4.0::19971010::extensions to HTML 4.0//",
		"-//Spyglass//DTD HTML 2.0 Extended//",
		"-//Sun Microsystems Corp.//DTD HotJava HTML//",
		"-//Sun Microsystems Corp.//DTD HotJava Strict HTML//",
		"-//W3C//DTD HTML 3 1995-03-24//",
		"-//W3C//DTD HTML 3.2 Draft//",
		"-//W3C//DTD HTML 3.2 Final//",
		"-//W3C//DTD HTML 3.2//",
		"-//W3C//DTD HTML 3.2S Draft//",
		"-//W3C//DTD HTML 4.0 Frameset//",
		"-//W3C//DTD HTML 4.0 Transitional//",
		"-//W3C//DTD HTML Experimental 19960712//",
		"-//W3C//DTD HTML Experimental 970421//",
		"-//W3C//DTD W3 HTML//",
		"-//W3O//DTD W3 HTML 3.0//",
		"-//WebTechs//DTD Mozilla HTML 2.0//",
		"-//WebTechs//DTD Mozilla HTML//" };

bool
IsQuirkyDoctype(HTML::Tokenizer::DoctypeToken *token, bool isIFrameSrcDoc) {
	(void)isIFrameSrcDoc;

	if (token->forceQuirks)
		return true;

	if (!token->name.has_value())
		return true;

	if (token->name.value().EqualsA("html"))
		return true;

	if (token->systemIdentifier.has_value()) {
		if (token->name.value().EqualsA("html"))
			if (token->systemIdentifier.value().EqualsA("http://www.ibm.com/data/dtd/v11/ibmxhtml1-transitional.dtd")) {
				return true;
			}
	} else if (token->publicIdentifier.has_value()) {
		for (const auto &string : quirkyPublicIdentifiersMissingSystem) {
			if (token->publicIdentifier.value().EqualsA(string)) {
				return true;
			}
		}
	}

	if (token->publicIdentifier.has_value()) {
		for (const auto &string : quirkyPublicIdentifiers) {
			if (token->publicIdentifier.value().EqualsA(string)) {
				return true;
			}
		}
	}

	return false;
}

bool
HTML::InsertionModes::Initial::EmitToken(HTML::Tokenizer::Token &inToken) {
	HTML::Tokenizer::CharacterToken *characterToken;
	HTML::Tokenizer::CommentToken *commentToken;
	HTML::Tokenizer::DoctypeToken *doctypeToken;

	switch (inToken.type()) {
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
					Logger::Warning("InitialInsertionMode::EmitToken", "Invalid character token!");
					context.parserContext.documentNode->mode = DOM::QuirksMode::QUIRKS;
					break;
			}
			break;
		case HTML::Tokenizer::TokenType::COMMENT:
			commentToken = dynamic_cast<HTML::Tokenizer::CommentToken *>(&inToken);
			context.parserContext.documentNode->childNodes.push_back(DOM::Comment(commentToken->contents));
			return false;
		case HTML::Tokenizer::TokenType::DOCTYPE:
			doctypeToken = dynamic_cast<HTML::Tokenizer::DoctypeToken *>(&inToken);

			if (IsQuirkyDoctype(doctypeToken, false /* TODO */))
				context.parserContext.documentNode->mode = DOM::QuirksMode::QUIRKS;

			constructor.currentMode = HTML::InsertionModeType::BEFORE_HTML;
			return false;
		default:
			break;
	}
	// SMALL TODO 'If the document is not an iframe srcdoc document'.

	Logger::Warning("InitialInsertionMode::EmitToken", "Parser Error: Invalid token in INITIAL insertion mode!");
	std::cerr << "\tTokenType: " << inToken.type() << std::endl;

	// In any other case:
	constructor.currentMode = HTML::InsertionModeType::BEFORE_HTML;
	return true;
}
