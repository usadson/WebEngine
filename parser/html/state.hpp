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

#pragma once

#include <map>

namespace HTML {
	namespace Tokenizer {
		enum class ParserState {
			UNDEFINED, // Only for return references!!! Use DATA as initial state.
			DATA, // 12.2.5.1 Data state
			RCDATA, // 12.2.5.2 RCDATA state
			RAWTEXT, // 12.2.5.3 RAWTEXT state
			SCRIPT_DATA, // 12.2.5.4 Script data state
			PLAINTEXT, // 12.2.5.5 PLAINTEXT state
			TAG_OPEN, // 12.2.5.6 Tag open state
			TAG_END_OPEN, // 12.2.5.7 End tag open state
			TAG_NAME, // 12.2.5.8 Tag name state
			RCDATA_LTS, // 12.2.5.9 RCDATA less-than sign state
			RCDATA_END_OPEN, // 12.2.5.10 RCDATA end tag open state
			RCDATA_END_NAME, // 12.2.5.11 RCDATA end tag name state
			RAWTEXT_LTS, // 12.2.5.12 RAWTEXT less-than sign state
			RAWTEXT_END_OPEN, // 12.2.5.13 RAWTEXT end tag open state
			RAWTEXT_END_NAME, // 12.2.5.14 RAWTEXT end tag name state
			/*, // 12.2.5.15 Script data less-than sign state
			, // 12.2.5.16 Script data end tag open state
			, // 12.2.5.17 Script data end tag name state
			, // 12.2.5.18 Script data escape start state
			, // 12.2.5.19 Script data escape start dash state
			, // 12.2.5.20 Script data escaped state
			, // 12.2.5.21 Script data escaped dash state
			, // 12.2.5.22 Script data escaped dash dash state
			, // 12.2.5.23 Script data escaped less-than sign state
			, // 12.2.5.24 Script data escaped end tag open state
			, // 12.2.5.25 Script data escaped end tag name state
			, // 12.2.5.26 Script data double escape start state
			, // 12.2.5.27 Script data double escaped state
			, // 12.2.5.28 Script data double escaped dash state
			, // 12.2.5.29 Script data double escaped dash dash state
			, // 12.2.5.30 Script data double escaped less-than sign state
			, // 12.2.5.31 Script data double escape end state
			*/
			BEFORE_ATTRIBUTE_NAME, // 12.2.5.32 Before attribute name state
			ATTRIBUTE_NAME, // 12.2.5.33 Attribute name state
			AFTER_ATTRIBUTE_NAME, // 12.2.5.34 After attribute name state
			BEFORE_ATTRIBUTE_VALUE, // 12.2.5.35 Before attribute value state
			ATTRIBUTE_VALUE_DQ, // 12.2.5.36 Attribute value (double-quoted) state
			ATTRIBUTE_VALUE_SQ, // 12.2.5.37 Attribute value (single-quoted) state
			ATTRIBUTE_VALUE_NQ, // 12.2.5.38 Attribute value (unquoted) state
			AFTER_ATTRIBUTE_VALUE_QUOTED, // 12.2.5.39 After attribute value (quoted) state
			SELF_CLOSING_START, // 12.2.5.40 Self-closing start tag state
			BOGUS_COMMENT, // 12.2.5.41 Bogus comment state
			MARKUP_DECLARATION_OPEN, // 12.2.5.42 Markup declaration open state
			COMMENT_START, // 12.2.5.43 Comment start state
			COMMENT_START_DASH, // 12.2.5.44 Comment start dash state
			COMMENT, // 12.2.5.45 Comment state
			COMMENT_LTS, // 12.2.5.46 Comment less-than sign state
			COMMENT_LTS_BANG, // 12.2.5.47 Comment less-than sign bang state
			COMMENT_LTS_BANG_DASH, // 12.2.5.48 Comment less-than sign bang dash state
			COMMENT_LTS_BANG_DASH_DASH, // 12.2.5.49 Comment less-than sign bang dash dash state
			COMMENT_END_DASH, // 12.2.5.50 Comment end dash state
			COMMENT_END, // 12.2.5.51 Comment end state
			COMMENT_END_BANG, // 12.2.5.52 Comment end bang state
			DOCTYPE, // 12.2.5.53 DOCTYPE state
			BEFORE_DOCTYPE_NAME, // 12.2.5.54 Before DOCTYPE name state
			DOCTYPE_NAME, // 12.2.5.55 DOCTYPE name state
			AFTER_DOCTYPE_NAME, // 12.2.5.56 After DOCTYPE name state
			AFTER_DOCTYPE_PUBLIC_KEYWORD, // 12.2.5.57 After DOCTYPE public keyword state
			BEFORE_DOCTYPE_PUBLIC_IDENTIFIER, // 12.2.5.58 Before DOCTYPE public identifier state
			DOCTYPE_PUBLIC_IDENTIFIER_DQ, // 12.2.5.59 DOCTYPE public identifier (double-quoted) state
			DOCTYPE_PUBLIC_IDENTIFIER_SQ, // 12.2.5.60 DOCTYPE public identifier (single-quoted) state
			AFTER_DOCTYPE_PUBLIC_IDENTIFIER, // 12.2.5.61 After DOCTYPE public identifier state
			BETWEEN_DOCTYPE_PUBLIC_SYSTEM_IDENTIFIER, // 12.2.5.62 Between DOCTYPE public and system identifiers state
			AFTER_DOCTYPE_SYSTEM_KEYWORD, // 12.2.5.63 After DOCTYPE system keyword state
			BEFORE_DOCTYPE_SYSTEM_IDENTIFIER, // 12.2.5.64 Before DOCTYPE system identifier state
			DOCTYPE_SYSTEM_IDENTIFIER_DQ, // 12.2.5.65 DOCTYPE system identifier (double-quoted) state
			DOCTYPE_SYSTEM_IDENTIFIER_SQ, // 12.2.5.66 DOCTYPE system identifier (single-quoted) state
			AFTER_DOCTYPE_SYSTEM_IDENTIFIER, // 12.2.5.67 After DOCTYPE system identifier state
			BOGUS_DOCTYPE, // 12.2.5.68 Bogus DOCTYPE state
			CDATA_SECTION, // 12.2.5.69 CDATA section state
			CDATA_SECTION_BRACKET, // 12.2.5.70 CDATA section bracket state
			CDATA_SECTION_END, // 12.2.5.71 CDATA section end state
			CHARACTER_REFERENCE, // 12.2.5.72 Character reference state
			NAMED_CHARACTER_REFERENCE, // 12.2.5.73 Named character reference state
			AMBIGOUS_AMPERSAND, // 12.2.5.74 Ambiguous ampersand state
			NUMERIC_CHARACTER_REFERENCE, // 12.2.5.75 Numeric character reference state
			HEXADECIMAL_CHARACTER_REFERENCE_START, // 12.2.5.76 Hexadecimal character reference start state
			DECIMAL_CHARACTER_REFERENCE_START, // 12.2.5.77 Decimal character reference start state
			HEXADECIMAL_CHARACTER_REFERENCE, // 12.2.5.78 Hexadecimal character reference state
			DECIMAL_CHARACTER_REFERENCE, // 12.2.5.79 Decimal character reference state
			NUMERIC_CHARACTER_REFERENCE_END // 12.2.5.80 Numeric character reference end state
		};

		struct ParserStateInfo {
			std::string Name;
			std::string Reference;
		};

		std::map<ParserState, ParserStateInfo> ParserStateNames = {
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

		inline std::ostream &
		operator<<(std::ostream &stream, ParserState &state) { 
			return stream << ParserStateNames[state].Name;
		}
	};
} 
