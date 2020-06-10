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
#include "dom/document_type.hpp"
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

DOM::QuirksMode
DetectQuirksMode(HTML::Tokenizer::DoctypeToken *token, bool isIFrameSrcDoc) {
	if (isIFrameSrcDoc)
		return DOM::QuirksMode::NO_QUIRKS;

	if (token->forceQuirks)
		return DOM::QuirksMode::QUIRKS;

	if (!token->name.has_value())
		return DOM::QuirksMode::QUIRKS;

	if (!token->name.value().EqualsA("html"))
		return DOM::QuirksMode::QUIRKS;

	if (token->systemIdentifier.has_value()) {
		if (token->name.value().EqualsA("html"))
			if (token->systemIdentifier.value().EqualsA("http://www.ibm.com/data/dtd/v11/ibmxhtml1-transitional.dtd")) {
				return DOM::QuirksMode::QUIRKS;
			}
	} else if (token->publicIdentifier.has_value()) {
		for (const auto &string : quirkyPublicIdentifiersMissingSystem) {
			if (token->publicIdentifier.value().EqualsA(string)) {
				return DOM::QuirksMode::QUIRKS;
			}
		}
	}

	if (token->publicIdentifier.has_value()) {
		for (const auto &string : quirkyPublicIdentifiers) {
			if (token->publicIdentifier.value().EqualsA(string)) {
				return DOM::QuirksMode::QUIRKS;
			}
		}
	}

	if (token->publicIdentifier.has_value() && (token->publicIdentifier->StartsWithA("-//W3C//DTD XHTML 1.0 Frameset//") || token->publicIdentifier->StartsWithA("-//W3C//DTD XHTML 1.0 Transitional//"))) {
		return DOM::QuirksMode::LIMITED_QUIRKS;
	}

	if (token->systemIdentifier.has_value() && (token->systemIdentifier->StartsWithA("-//W3C//DTD HTML 4.01 Frameset//") || token->systemIdentifier->StartsWithA("-//W3C//DTD HTML 4.01 Transitional//"))) {
		return DOM::QuirksMode::LIMITED_QUIRKS;
	}

	return DOM::QuirksMode::NO_QUIRKS;
}

void
HTML::InsertionModes::Initial::CheckDoctypeParserErrors(HTML::Tokenizer::DoctypeToken *token) const noexcept {
	if (!token->name.has_value())
		context.parserContext.ReportParserError("DoctypeParser", "No DOCTYPE name specified");
	if (!token->name.value().EqualsA("html"))
		context.parserContext.ReportParserError("DoctypeParser", "No DOCTYPE name not equal to \"html\"");
	if (token->publicIdentifier.has_value())
		context.parserContext.ReportParserError("DoctypeParser", "DOCTYPEs shouldn't have a PUBLIC attribute");
	if (token->systemIdentifier.has_value() && !token->systemIdentifier.value().EqualsA("about:legacy-compat"))
		context.parserContext.ReportParserError("DoctypeParser", "DOCTYPEs shouldn't have a SYSTEM identifier not equal to \"about:legacy-compat\". You can either omit the SYSTEM attribute or change its identifier to \"about:legacy-compat\"");
}

HTML::InsertionModeSubroutineStatus
HTML::InsertionModes::Initial::HandleDoctype(HTML::Tokenizer::Token &token) {
	auto doctypeToken = dynamic_cast<HTML::Tokenizer::DoctypeToken *>(&token);
	CheckDoctypeParserErrors(doctypeToken);

	auto documentTypeNode = std::make_shared<DOM::DocumentType>(
		doctypeToken->name.has_value() ? doctypeToken->name.value() : Unicode::UString(),
		doctypeToken->publicIdentifier.has_value() ? doctypeToken->publicIdentifier.value() : Unicode::UString(),
		doctypeToken->systemIdentifier.has_value() ? doctypeToken->systemIdentifier.value() : Unicode::UString());
	// The spec states that the other attributes must be set to null/empty, but
	// this is done by default by the constructor, right?

	context.parserContext.documentNode->doctype = documentTypeNode;
	context.parserContext.documentNode->childNodes.push_back(documentTypeNode);

	context.parserContext.documentNode->mode = DetectQuirksMode(doctypeToken, context.parserContext.isIframeSrcdoc);

	constructor.currentMode = HTML::InsertionModeType::BEFORE_HTML;
	return HTML::InsertionModeSubroutineStatus::IGNORE;
}

bool
HTML::InsertionModes::Initial::EmitToken(HTML::Tokenizer::Token &inToken) {
	switch (inToken.type()) {
		case HTML::Tokenizer::TokenType::CHARACTER:
			switch (dynamic_cast<HTML::Tokenizer::CharacterToken *>(&inToken)->character) {
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
			break;
		case HTML::Tokenizer::TokenType::COMMENT:
			context.parserContext.documentNode->childNodes.push_back(
				std::make_shared<DOM::Comment>(dynamic_cast<HTML::Tokenizer::CommentToken *>(&inToken)->contents));
			return false;
		case HTML::Tokenizer::TokenType::DOCTYPE:
			HandleDoctype(inToken);
		default:
			break;
	}
	// SMALL TODO 'If the document is not an iframe srcdoc document'.

	Logger::Warning("InitialInsertionMode::EmitToken", "Parser Error: Invalid token in INITIAL insertion mode!");
	std::cerr << "\tTokenType: " << inToken.type() << std::endl;

	// In any other case:
	context.parserContext.documentNode->mode = DOM::QuirksMode::QUIRKS;
	constructor.currentMode = HTML::InsertionModeType::BEFORE_HTML;
	return true;
}
