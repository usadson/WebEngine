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
			std::string name;
			std::string reference;
		};

		extern std::map<ParserState, ParserStateInfo> parserStateNames;

		inline std::ostream &
		operator<<(std::ostream &stream, ParserState &state) { 
			return stream << parserStateNames[state].name;
		}
	};
} 
