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
		class ParserError {
		public:
			static const ParserError ABRUBT_CLOSING_OF_EMPTY_COMMENT;
			static const ParserError ABRUBT_DOCTYPE_PUBLIC_IDENTIFIER;
			static const ParserError ABRUBT_DOCTYPE_SYSTEM_IDENTIFIER;
			static const ParserError ABSENCE_OF_DIGITS_IN_NUMERIC_CHARACTER_REFERENCE;
			static const ParserError CDATA_IN_HTML_CONTENT;
			static const ParserError CHARACTER_REFERENCE_OUTSIDE_UNICODE_RANGE;
			static const ParserError CONTROL_CHARACTER_IN_INPUT_STREAM;
			static const ParserError CONTROL_CHARACTER_REFERENCE;
			static const ParserError END_TAG_WITH_ATTRIBUTES;
			static const ParserError DUPLICATE_ATTRIBUTES;
			static const ParserError END_TAG_WITH_TRAILING_SOLIDUS;
			static const ParserError EOF_BEFORE_TAG_NAME;
			static const ParserError EOF_IN_CDATA;
			static const ParserError EOF_IN_COMMENT;
			static const ParserError EOF_IN_DOCTYPE;
			static const ParserError EOF_IN_SCRIPT_HTML_COMMENT_LIKE_TEXT;
			static const ParserError EOF_IN_TAG;
			static const ParserError INCORRECTLY_CLOSED_COMMENT;
			static const ParserError INCORRECTLY_OPENED_COMMENT;
			static const ParserError INVALID_CHARACTER_SEQUENCE_AFTER_DOCTYPE_NAME;
			static const ParserError INVALID_FIRST_CHARACTER_OF_TAG_NAME;
			static const ParserError MISSING_ATTRIBUTE_VALUE;
			static const ParserError MISSING_DOCTYPE_NAME;
			static const ParserError MISSING_DOCTYPE_PUBLIC_IDENTIFIER;
			static const ParserError MISSING_DOCTYPE_SYSTEM_IDENTIFIER;
			static const ParserError MISSING_END_TAG_NAME;
			static const ParserError MISSING_QOUTE_BEFORE_DOCTYPE_PUBLIC_IDENTIFIER;
			static const ParserError MISSING_QOUTE_BEFORE_DOCTYPE_SYSTEM_IDENTIFIER;
			static const ParserError MISSING_SEMICOLON_AFTER_CHARACTER_REFERENCE;
			static const ParserError MISSING_WHITESPACE_AFTER_DOCTYPE_PUBLIC_KEYWORD;
			static const ParserError MISSING_WHITESPACE_AFTER_DOCTYPE_SYSTEM_KEYWORD;
			static const ParserError MISSING_WHITESPACE_BEFORE_DOCTYPE_NAME;
			static const ParserError MISSING_WHITESPACE_BETWEEN_ATTRIBUTES;
			static const ParserError MISSING_WHITESPACE_BETWEEN_DOCTYPE_PUBLIC_AND_SYSTEM_IDENTIFIERS;
			static const ParserError NESTED_COMMENT;
			static const ParserError NONCHARACTER_CHARACTER_REFERENCE;
			static const ParserError NONCHARACTER_IN_INPUT_STREAM;
			static const ParserError NON_VOID_HTML_ELEMENT_START_TAG_WITH_TRAILING_SOLIDUS;
			static const ParserError NULL_CHARACTER_REFERENCE;
			static const ParserError SURROGATE_CHARACTER_REFERENCE;
			static const ParserError SURROGATE_IN_INPUT_STREAM;
			static const ParserError UNEXPECTED_CHARACTER_AFTER_DOCTYPE_SYSTEM_IDENTIFIER;
			static const ParserError UNEXPECTED_CHARACTER_IN_ATTRIBUTE_NAME;
			static const ParserError UNEXPECTED_CHARACTER_IN_UNQOUTED_ATTRIBUTE_VALUE;
			static const ParserError UNEXPECTED_EQUALS_SIGN_BEFORE_ATTRIBUTE_NAME;
			static const ParserError UNEXPECTED_NULL_CHARACTER;
			static const ParserError UNEXPECTED_QUESTION_MARK_INSTEAD_OF_TAG_NAME;
			static const ParserError UNEXPECTED_SOLIDUS_IN_TAG;
			static const ParserError UNKNOWN_NAMED_CHARACTER_REFERENCE;
			
		public:
			const std::string Name;
			const std::string Description;
		private:
			inline
			ParserError(const std::string &name, const std::string &description)
				: Name(name), Description(description) {
			}
		};
 
		namespace ParserErrorFunctions {
			std::ostream &
			operator<<(std::ostream &, const HTML::Tokenizer::ParserError &);
		}
	};
} 
