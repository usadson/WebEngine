/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "tokenizer.hpp"

#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <vector>

#include <cstdlib>
#include <cstring>
#include <strings.h>

#include "data/text/named_characters.hpp"
#include "logger.hpp"
// find . | grep .hpp | cut -d'/' -f 2 | awk '{print "#include \x22parser/html/tokenizer/" $0 "\x22"}' | sort
#include "parser/html/tokenizer/after_attribute_name.hpp"
#include "parser/html/tokenizer/after_attribute_value_quoted.hpp"
#include "parser/html/tokenizer/after_doctype_name.hpp"
#include "parser/html/tokenizer/after_doctype_public_identifier.hpp"
#include "parser/html/tokenizer/after_doctype_public_keyword.hpp"
#include "parser/html/tokenizer/after_doctype_system_identifier.hpp"
#include "parser/html/tokenizer/after_doctype_system_keyword.hpp"
#include "parser/html/tokenizer/attribute_name.hpp"
#include "parser/html/tokenizer/attribute_value_dq.hpp"
#include "parser/html/tokenizer/attribute_value_nq.hpp"
#include "parser/html/tokenizer/attribute_value_sq.hpp"
#include "parser/html/tokenizer/before_attribute_name.hpp"
#include "parser/html/tokenizer/before_attribute_value.hpp"
#include "parser/html/tokenizer/before_doctype_name.hpp"
#include "parser/html/tokenizer/before_doctype_public_identifier.hpp"
#include "parser/html/tokenizer/before_doctype_system_identifier.hpp"
#include "parser/html/tokenizer/between_doctype_public_system_identifier.hpp"
#include "parser/html/tokenizer/bogus_doctype.hpp"
#include "parser/html/tokenizer/character_reference.hpp"
#include "parser/html/tokenizer/comment.hpp"
#include "parser/html/tokenizer/comment_end.hpp"
#include "parser/html/tokenizer/comment_end_bang.hpp"
#include "parser/html/tokenizer/comment_end_dash.hpp"
#include "parser/html/tokenizer/comment_lts.hpp"
#include "parser/html/tokenizer/comment_lts_bang.hpp"
#include "parser/html/tokenizer/comment_lts_bang_dash.hpp"
#include "parser/html/tokenizer/comment_lts_bang_dash_dash.hpp"
#include "parser/html/tokenizer/comment_start.hpp"
#include "parser/html/tokenizer/comment_start_dash.hpp"
#include "parser/html/tokenizer/data.hpp"
#include "parser/html/tokenizer/doctype.hpp"
#include "parser/html/tokenizer/doctype_name.hpp"
#include "parser/html/tokenizer/doctype_public_identifier_dq.hpp"
#include "parser/html/tokenizer/doctype_public_identifier_sq.hpp"
#include "parser/html/tokenizer/doctype_system_identifier_dq.hpp"
#include "parser/html/tokenizer/doctype_system_identifier_sq.hpp"
#include "parser/html/tokenizer/markup_declaration_open.hpp"
#include "parser/html/tokenizer/self_closing_start.hpp"
#include "parser/html/tokenizer/tag_end_open.hpp"
#include "parser/html/tokenizer/tag_name.hpp"
#include "parser/html/tokenizer/tag_open.hpp"
#include "parser/html/tokenizer/tokenizer_parser.hpp"
#include "parser/html/context.hpp"
#include "parser/html/error.hpp"
#include "parser/html/state.hpp"
#include "parser/html/token.hpp"
#include "parser/html/tree_constructor.hpp"

void
HTML::Tokenizer::Tokenizer::Run(Resources::DocumentResource &document) {

	// clear && find . | grep .hpp | cut -d'/' -f 2 | cut -d'.' -f 1 | awk '{print "{ HTML::Tokenizer::ParserState::" toupper($0) ", std::make_shared<HTML::Tokenizer::AAAAAAAAA>(*this) },"}' | sort
	const std::map<HTML::Tokenizer::ParserState, std::shared_ptr<HTML::Tokenizer::Parser>> parserMap = {
		{ HTML::Tokenizer::ParserState::AFTER_ATTRIBUTE_NAME, std::make_shared<HTML::Tokenizer::AfterAttributeName>(*this) },
		{ HTML::Tokenizer::ParserState::AFTER_ATTRIBUTE_VALUE_QUOTED, std::make_shared<HTML::Tokenizer::AfterAttributeValueQuoted>(*this) },
		{ HTML::Tokenizer::ParserState::AFTER_DOCTYPE_NAME, std::make_shared<HTML::Tokenizer::AfterDoctypeName>(*this) },
		{ HTML::Tokenizer::ParserState::AFTER_DOCTYPE_PUBLIC_IDENTIFIER, std::make_shared<HTML::Tokenizer::AfterDoctypePublicIdentifier>(*this) },
		{ HTML::Tokenizer::ParserState::AFTER_DOCTYPE_PUBLIC_KEYWORD, std::make_shared<HTML::Tokenizer::AfterDoctypePublicKeyword>(*this) },
		{ HTML::Tokenizer::ParserState::AFTER_DOCTYPE_SYSTEM_IDENTIFIER, std::make_shared<HTML::Tokenizer::AfterDoctypeSystemIdentifier>(*this) },
		{ HTML::Tokenizer::ParserState::AFTER_DOCTYPE_SYSTEM_KEYWORD, std::make_shared<HTML::Tokenizer::AfterDoctypeSystemKeyword>(*this) },
		{ HTML::Tokenizer::ParserState::ATTRIBUTE_NAME, std::make_shared<HTML::Tokenizer::AttributeName>(*this) },
		{ HTML::Tokenizer::ParserState::ATTRIBUTE_VALUE_DQ, std::make_shared<HTML::Tokenizer::AttributeValueDQ>(*this) },
		{ HTML::Tokenizer::ParserState::ATTRIBUTE_VALUE_NQ, std::make_shared<HTML::Tokenizer::AttributeValueNQ>(*this) },
		{ HTML::Tokenizer::ParserState::ATTRIBUTE_VALUE_SQ, std::make_shared<HTML::Tokenizer::AttributeValueSQ>(*this) },
		{ HTML::Tokenizer::ParserState::BEFORE_ATTRIBUTE_NAME, std::make_shared<HTML::Tokenizer::BeforeAttributeName>(*this) },
		{ HTML::Tokenizer::ParserState::BEFORE_ATTRIBUTE_VALUE, std::make_shared<HTML::Tokenizer::BeforeAttributeValue>(*this) },
		{ HTML::Tokenizer::ParserState::BEFORE_DOCTYPE_NAME, std::make_shared<HTML::Tokenizer::BeforeDoctypeName>(*this) },
		{ HTML::Tokenizer::ParserState::BEFORE_DOCTYPE_PUBLIC_IDENTIFIER, std::make_shared<HTML::Tokenizer::BeforeDoctypePublicIdentifier>(*this) },
		{ HTML::Tokenizer::ParserState::BEFORE_DOCTYPE_SYSTEM_IDENTIFIER, std::make_shared<HTML::Tokenizer::BeforeDoctypeSystemIdentifier>(*this) },
		{ HTML::Tokenizer::ParserState::BETWEEN_DOCTYPE_PUBLIC_SYSTEM_IDENTIFIER, std::make_shared<HTML::Tokenizer::BetweenDoctypePublicSystemIdentifier>(*this) },
		{ HTML::Tokenizer::ParserState::BOGUS_DOCTYPE, std::make_shared<HTML::Tokenizer::BogusDoctype>(*this) },
		{ HTML::Tokenizer::ParserState::CHARACTER_REFERENCE, std::make_shared<HTML::Tokenizer::CharacterReference>(*this) },
		{ HTML::Tokenizer::ParserState::COMMENT, std::make_shared<HTML::Tokenizer::Comment>(*this) },
		{ HTML::Tokenizer::ParserState::COMMENT_END, std::make_shared<HTML::Tokenizer::CommentEnd>(*this) },
		{ HTML::Tokenizer::ParserState::COMMENT_END_BANG, std::make_shared<HTML::Tokenizer::CommentEndBang>(*this) },
		{ HTML::Tokenizer::ParserState::COMMENT_END_DASH, std::make_shared<HTML::Tokenizer::CommentEndDash>(*this) },
		{ HTML::Tokenizer::ParserState::COMMENT_LTS, std::make_shared<HTML::Tokenizer::CommentLTS>(*this) },
		{ HTML::Tokenizer::ParserState::COMMENT_LTS_BANG, std::make_shared<HTML::Tokenizer::CommentLTSBang>(*this) },
		{ HTML::Tokenizer::ParserState::COMMENT_LTS_BANG_DASH, std::make_shared<HTML::Tokenizer::CommentLTSBangDash>(*this) },
		{ HTML::Tokenizer::ParserState::COMMENT_LTS_BANG_DASH_DASH, std::make_shared<HTML::Tokenizer::CommentLTSBangDashDash>(*this) },
		{ HTML::Tokenizer::ParserState::COMMENT_START, std::make_shared<HTML::Tokenizer::CommentStart>(*this) },
		{ HTML::Tokenizer::ParserState::COMMENT_START_DASH, std::make_shared<HTML::Tokenizer::CommentStartDash>(*this) },
		{ HTML::Tokenizer::ParserState::DATA, std::make_shared<HTML::Tokenizer::Data>(*this) },
		{ HTML::Tokenizer::ParserState::DOCTYPE, std::make_shared<HTML::Tokenizer::Doctype>(*this) },
		{ HTML::Tokenizer::ParserState::DOCTYPE_NAME, std::make_shared<HTML::Tokenizer::DoctypeName>(*this) },
		{ HTML::Tokenizer::ParserState::DOCTYPE_PUBLIC_IDENTIFIER_DQ, std::make_shared<HTML::Tokenizer::DoctypePublicIdentifierDQ>(*this) },
		{ HTML::Tokenizer::ParserState::DOCTYPE_PUBLIC_IDENTIFIER_SQ, std::make_shared<HTML::Tokenizer::DoctypePublicIdentifierSQ>(*this) },
		{ HTML::Tokenizer::ParserState::DOCTYPE_SYSTEM_IDENTIFIER_DQ, std::make_shared<HTML::Tokenizer::DoctypeSystemIdentifierDQ>(*this) },
		{ HTML::Tokenizer::ParserState::DOCTYPE_SYSTEM_IDENTIFIER_SQ, std::make_shared<HTML::Tokenizer::DoctypeSystemIdentifierSQ>(*this) },
		{ HTML::Tokenizer::ParserState::MARKUP_DECLARATION_OPEN, std::make_shared<HTML::Tokenizer::MarkupDeclarationOpen>(*this) },
		{ HTML::Tokenizer::ParserState::SELF_CLOSING_START, std::make_shared<HTML::Tokenizer::SelfClosingStart>(*this) },
		{ HTML::Tokenizer::ParserState::TAG_END_OPEN, std::make_shared<HTML::Tokenizer::TagEndOpen>(*this) },
		{ HTML::Tokenizer::ParserState::TAG_NAME, std::make_shared<HTML::Tokenizer::TagName>(*this) },
		{ HTML::Tokenizer::ParserState::TAG_OPEN, std::make_shared<HTML::Tokenizer::TagOpen>(*this) },
	};

	context.document = &document;

	// At what line is the tokenizer?
	context.lineCount = 1;
	// At what character position in the line is the tokenizer?
	context.linePosition = 0;

	context.documentSize = context.document->data.length();
	std::cout << "InputDataSize: " << context.documentSize << std::endl;

	// Don't use 'character' if context.eof is true.
	context.eof = false;

	context.commentToken = HTML::Tokenizer::CommentToken::INVALID_TYPE;

	context.isEndTag = false;
	context.endTagToken = HTML::Tokenizer::EndTagToken::INVALID_TYPE_END_TAG;
	context.startTagToken = HTML::Tokenizer::StartTagToken::INVALID_TYPE_START_TAG;

	context.unknownStateCount = 0;
	context.reconsume = false;
	context.toConsumeNext = 0;

	for (context.i = 0; context.i <= context.documentSize; context.i++) {
		context.character = '\0';

// 		std::cout << "index=" << context.i << " state=" << context.state << std::endl;
		if (context.reconsume) {
			context.i--;
			context.reconsume = false;
		}

		if (context.i == context.documentSize) {
			context.eof = true;
			context.linePosition += 1;
		} else {
			bool repeatLineCheckLoop = true;
			while (repeatLineCheckLoop) {
				repeatLineCheckLoop = false;
				context.character = context.document->data[context.i];

				if (context.character == '\n') {
					context.lineCount += 1;
					context.linePosition = 1;
				} else {
					// std::cout << "CharAt(" << context.i << ") = " << context.character << std::endl;
					context.linePosition += 1;
				}

				if (context.toConsumeNext > 0) {
					context.i += 1;
					context.toConsumeNext -= 1;
					repeatLineCheckLoop = true;
				}
			}
		}

		context.beginLoopState = context.state;
		context.currentCharacter = context.character;

		auto parser = parserMap.find(context.state);

		if (parser == std::end(parserMap)) {
			std::cout << "Unknown state(" << ++context.unknownStateCount << "): " << context.state << std::endl;
		} else {
			if (!parser->second->Parse()) {
				std::stringstream info("ParserError: ");
				info << context.state << " failed on " << context.linePosition
					 << ':' << context.lineCount;
				Logger::Error(__PRETTY_FUNCTION__, info.str());

				/* FIXME: Handle Error */
				return;
			}
		}
	}
}
