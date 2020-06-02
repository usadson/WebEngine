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

#include "state.hpp"

namespace HTML {
	namespace Tokenizer {
		std::map<ParserState, ParserStateInfo> parserStateNames = {
			{ ParserState::UNDEFINED, { "UNDEFINED", "Only for return references!!! Use DATA as initial state." } },
			{ ParserState::DATA, { "DATA", "12.2.5.1 Data state" } },
			{ ParserState::RCDATA, { "RCDATA", "12.2.5.2 RCDATA state" } },
			{ ParserState::RAWTEXT, { "RAWTEXT", "12.2.5.3 RAWTEXT state" } },
			{ ParserState::SCRIPT_DATA, { "SCRIPT_DATA", "12.2.5.4 Script data state" } },
			{ ParserState::PLAINTEXT, { "PLAINTEXT", "12.2.5.5 PLAINTEXT state" } },
			{ ParserState::TAG_OPEN, { "TAG_OPEN", "12.2.5.6 Tag open state" } },
			{ ParserState::TAG_END_OPEN, { "TAG_END_OPEN", "12.2.5.7 End tag open state" } },
			{ ParserState::TAG_NAME, { "TAG_NAME", "12.2.5.8 Tag name state" } },
			{ ParserState::RCDATA_LTS, { "RCDATA_LTS", "12.2.5.9 RCDATA less-than sign state" } },
			{ ParserState::RCDATA_END_OPEN, { "RCDATA_END_OPEN", "12.2.5.10 RCDATA end tag open state" } },
			{ ParserState::RCDATA_END_NAME, { "RCDATA_END_NAME", "12.2.5.11 RCDATA end tag name state" } },
			{ ParserState::RAWTEXT_LTS, { "RAWTEXT_LTS", "12.2.5.12 RAWTEXT less-than sign state" } },
			{ ParserState::RAWTEXT_END_OPEN, { "RAWTEXT_END_OPEN", "12.2.5.13 RAWTEXT end tag open state" } },
			{ ParserState::RAWTEXT_END_NAME, { "RAWTEXT_END_NAME", "12.2.5.14 RAWTEXT end tag name state" } },
			{ ParserState::BEFORE_ATTRIBUTE_NAME, { "BEFORE_ATTRIBUTE_NAME", "12.2.5.32 Before attribute name state" } },
			{ ParserState::ATTRIBUTE_NAME, { "ATTRIBUTE_NAME", "12.2.5.33 Attribute name state" } },
			{ ParserState::AFTER_ATTRIBUTE_NAME, { "AFTER_ATTRIBUTE_NAME", "12.2.5.34 After attribute name state" } },
			{ ParserState::BEFORE_ATTRIBUTE_VALUE, { "BEFORE_ATTRIBUTE_VALUE", "12.2.5.35 Before attribute value state" } },
			{ ParserState::ATTRIBUTE_VALUE_DQ, { "ATTRIBUTE_VALUE_DQ", "12.2.5.36 Attribute value (double-quoted) state" } },
			{ ParserState::ATTRIBUTE_VALUE_SQ, { "ATTRIBUTE_VALUE_SQ", "12.2.5.37 Attribute value (single-quoted) state" } },
			{ ParserState::ATTRIBUTE_VALUE_NQ, { "ATTRIBUTE_VALUE_NQ", "12.2.5.38 Attribute value (unquoted) state" } },
			{ ParserState::AFTER_ATTRIBUTE_VALUE_QUOTED, { "AFTER_ATTRIBUTE_VALUE_QUOTED", "12.2.5.39 After attribute value (quoted) state" } },
			{ ParserState::SELF_CLOSING_START, { "SELF_CLOSING_START", "12.2.5.40 Self-closing start tag state" } },
			{ ParserState::BOGUS_COMMENT, { "BOGUS_COMMENT", "12.2.5.41 Bogus comment state" } },
			{ ParserState::MARKUP_DECLARATION_OPEN, { "MARKUP_DECLARATION_OPEN", "12.2.5.42 Markup declaration open state" } },
			{ ParserState::COMMENT_START, { "COMMENT_START", "12.2.5.43 Comment start state" } },
			{ ParserState::COMMENT_START_DASH, { "COMMENT_START_DASH", "12.2.5.44 Comment start dash state" } },
			{ ParserState::COMMENT, { "COMMENT", "12.2.5.45 Comment state" } },
			{ ParserState::COMMENT_LTS, { "COMMENT_LTS", "12.2.5.46 Comment less-than sign state" } },
			{ ParserState::COMMENT_LTS_BANG, { "COMMENT_LTS_BANG", "12.2.5.47 Comment less-than sign bang state" } },
			{ ParserState::COMMENT_LTS_BANG_DASH, { "COMMENT_LTS_BANG_DASH", "12.2.5.48 Comment less-than sign bang dash state" } },
			{ ParserState::COMMENT_LTS_BANG_DASH_DASH, { "COMMENT_LTS_BANG_DASH_DASH", "12.2.5.49 Comment less-than sign bang dash dash state" } },
			{ ParserState::COMMENT_END_DASH, { "COMMENT_END_DASH", "12.2.5.50 Comment end dash state" } },
			{ ParserState::COMMENT_END, { "COMMENT_END", "12.2.5.51 Comment end state" } },
			{ ParserState::COMMENT_END_BANG, { "COMMENT_END_BANG", "12.2.5.52 Comment end bang state" } },

			{ ParserState::DOCTYPE, { "DOCTYPE", "12.2.5.53 DOCTYPE state" } },
			{ ParserState::BEFORE_DOCTYPE_NAME, { "BEFORE_DOCTYPE_NAME", "12.2.5.54 Before DOCTYPE name state" } },
			{ ParserState::DOCTYPE_NAME, { "DOCTYPE_NAME", "12.2.5.55 DOCTYPE name state" } },
			{ ParserState::AFTER_DOCTYPE_NAME, { "AFTER_DOCTYPE_NAME", "12.2.5.56 After DOCTYPE name state" } },
			{ ParserState::AFTER_DOCTYPE_PUBLIC_KEYWORD, { "AFTER_DOCTYPE_PUBLIC_KEYWORD", "12.2.5.57 After DOCTYPE public keyword state" } },
			{ ParserState::BEFORE_DOCTYPE_PUBLIC_IDENTIFIER, { "BEFORE_DOCTYPE_PUBLIC_IDENTIFIER", "12.2.5.58 Before DOCTYPE public identifier state" } },
			{ ParserState::DOCTYPE_PUBLIC_IDENTIFIER_DQ, { "DOCTYPE_PUBLIC_IDENTIFIER_DQ", "12.2.5.59 DOCTYPE public identifier (double-quoted) state" } },
			{ ParserState::DOCTYPE_PUBLIC_IDENTIFIER_SQ, { "DOCTYPE_PUBLIC_IDENTIFIER_SQ", "12.2.5.60 DOCTYPE public identifier (single-quoted) state" } },
			{ ParserState::AFTER_DOCTYPE_PUBLIC_IDENTIFIER, { "AFTER_DOCTYPE_PUBLIC_IDENTIFIER", "12.2.5.61 After DOCTYPE public identifier state" } },
			{ ParserState::BETWEEN_DOCTYPE_PUBLIC_SYSTEM_IDENTIFIER, { "BETWEEN_DOCTYPE_PUBLIC_SYSTEM_IDENTIFIER", "12.2.5.62 Between DOCTYPE public and system identifiers state" } },
			{ ParserState::AFTER_DOCTYPE_SYSTEM_KEYWORD, { "AFTER_DOCTYPE_SYSTEM_KEYWORD", "12.2.5.63 After DOCTYPE system keyword state" } },
			{ ParserState::BEFORE_DOCTYPE_SYSTEM_IDENTIFIER, { "BEFORE_DOCTYPE_SYSTEM_IDENTIFIER", "12.2.5.64 Before DOCTYPE system identifier state" } },
			{ ParserState::DOCTYPE_SYSTEM_IDENTIFIER_DQ, { "DOCTYPE_SYSTEM_IDENTIFIER_DQ", "12.2.5.65 DOCTYPE system identifier (double-quoted) state" } },
			{ ParserState::DOCTYPE_SYSTEM_IDENTIFIER_SQ, { "DOCTYPE_SYSTEM_IDENTIFIER_SQ", "12.2.5.66 DOCTYPE system identifier (single-quoted) state" } },
			{ ParserState::AFTER_DOCTYPE_SYSTEM_IDENTIFIER, { "AFTER_DOCTYPE_SYSTEM_IDENTIFIER", "12.2.5.67 After DOCTYPE system identifier state" } },
			{ ParserState::BOGUS_DOCTYPE, { "BOGUS_DOCTYPE", "12.2.5.68 Bogus DOCTYPE state" } },

			{ ParserState::CDATA_SECTION, { "CDATA_SECTION", "12.2.5.69 CDATA section state" } },
			{ ParserState::CDATA_SECTION_BRACKET, { "CDATA_SECTION_BRACKET", "12.2.5.70 CDATA section bracket state" } },
			{ ParserState::CDATA_SECTION_END, { "CDATA_SECTION_END", "12.2.5.71 CDATA section end state" } },
			{ ParserState::CHARACTER_REFERENCE, { "CHARACTER_REFERENCE", "12.2.5.72 Character reference state" } },
			{ ParserState::NAMED_CHARACTER_REFERENCE, { "NAMED_CHARACTER_REFERENCE", "12.2.5.73 Named character reference state" } },
			{ ParserState::AMBIGOUS_AMPERSAND, { "AMBIGOUS_AMPERSAND", "12.2.5.74 Ambiguous ampersand state" } },
			{ ParserState::NUMERIC_CHARACTER_REFERENCE, { "NUMERIC_CHARACTER_REFERENCE", "12.2.5.75 Numeric character reference state" } },
			{ ParserState::HEXADECIMAL_CHARACTER_REFERENCE_START, { "HEXADECIMAL_CHARACTER_REFERENCE_START", "12.2.5.76 Hexadecimal character reference start state" } },
			{ ParserState::DECIMAL_CHARACTER_REFERENCE_START, { "DECIMAL_CHARACTER_REFERENCE_START", "12.2.5.77 Decimal character reference start state" } },
			{ ParserState::HEXADECIMAL_CHARACTER_REFERENCE, { "HEXADECIMAL_CHARACTER_REFERENCE", "12.2.5.78 Hexadecimal character reference state" } },
			{ ParserState::DECIMAL_CHARACTER_REFERENCE, { "DECIMAL_CHARACTER_REFERENCE", "12.2.5.79 Decimal character reference state" } },
			{ ParserState::NUMERIC_CHARACTER_REFERENCE_END, { "NUMERIC_CHARACTER_REFERENCE_END", "12.2.5.80 Numeric character reference end state" } },
		};
	}
}
