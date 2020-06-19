/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "error.hpp"

namespace HTML::Tokenizer {
	const ParserError ParserError::NULL_PARSER_ERROR = ParserError("NULL_PARSER_ERROR", "NULL_PARSER_ERROR");
	const ParserError ParserError::ABRUBT_CLOSING_OF_EMPTY_COMMENT = ParserError("abrupt-closing-of-empty-comment",
		"This error occurs if the parser encounters an empty comment that is abruptly closed by a U+003E (>) code "
		"point (i.e., <!--> or <!--->). The parser behaves as if the comment is closed correctly.");
	const ParserError ParserError::ABRUBT_DOCTYPE_PUBLIC_IDENTIFIER = ParserError("abrupt-doctype-public-identifier",
		"This error occurs if the parser encounters a U+003E (>) code point in the DOCTYPE public "
		"identifier (e.g., <!DOCTYPE html PUBLIC \"foo>). In such a case, if the DOCTYPE is correctly "
		"placed as a document preamble, the parser sets the Document to quirks mode.");
	const ParserError ParserError::ABRUBT_DOCTYPE_SYSTEM_IDENTIFIER = ParserError("abrupt-doctype-system-identifier",
		"This error occurs if the parser encounters a U+003E (>) code point in the DOCTYPE system identifier (e.g., "
		"<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"foo>). In such a case, if the DOCTYPE is correctly "
		"placed as a document preamble, the parser sets the Document to quirks mode.");
	const ParserError ParserError::ABSENCE_OF_DIGITS_IN_NUMERIC_CHARACTER_REFERENCE
		= ParserError("absence-of-digits-in-numeric-character-reference",
			"This error occurs if the parser encounters a numeric character reference that doesn't contain "
			"any digits (e.g., &#qux;). In this case the parser doesn't resolve the character reference.");
	const ParserError ParserError::CDATA_IN_HTML_CONTENT = ParserError("cdata-in-html-content",
		"This error occurs if the parser encounters a CDATA section outside of foreign content (SVG or MathML). The "
		"parser treats such CDATA sections (including leading \"[CDATA[\" and trailing \"]]\" strings) as comments.");
	const ParserError ParserError::CHARACTER_REFERENCE_OUTSIDE_UNICODE_RANGE
		= ParserError("character-reference-outside-unicode-range",
			"This error occurs if the parser encounters a numeric character reference that references a code "
			"point that is greater than the valid Unicode range. The parser resolves such a character "
			"reference to a U+FFFD REPLACEMENT CHARACTER.");
	const ParserError ParserError::CONTROL_CHARACTER_IN_INPUT_STREAM = ParserError("control-character-in-input-stream",
		"This error occurs if the input stream contains a control code point that is not ASCII "
		"whitespace or U+0000 NULL. Such code points are parsed as-is and usually, where parsing rules "
		"don't apply any additional restrictions, make their way into the DOM.");
	const ParserError ParserError::CONTROL_CHARACTER_REFERENCE = ParserError("control-character-reference",
		"This error occurs if the parser encounters a numeric character reference that references a control code point "
		"that is not ASCII whitespace or is a U+000D CARRIAGE RETURN. The parser resolves such character references "
		"as-is except C1 control references that are replaced according to the numeric character reference end state.");
	const ParserError ParserError::END_TAG_WITH_ATTRIBUTES = ParserError("end-tag-with-attributes",
		"This error occurs if the parser encounters an end tag with attributes. Attributes in end tags "
		"are completely ignored and do not make their way into the DOM.");
	const ParserError ParserError::DUPLICATE_ATTRIBUTES = ParserError("duplicate-attribute",
		"This error occurs if the parser encounters an attribute in a tag that already has an attribute "
		"with the same name. The parser ignores all such duplicate occurrences of the attribute. ");
	const ParserError ParserError::END_TAG_WITH_TRAILING_SOLIDUS = ParserError("end-tag-with-trailing-solidus",
		"This error occurs if the parser encounters an end tag that has a U+002F (/) code point right before the "
		"closing U+003E (>) code point (e.g., </div/>). Such a tag is treated as a regular end tag.");
	const ParserError ParserError::EOF_BEFORE_TAG_NAME = ParserError("eof-before-tag-name",
		"This error occurs if the parser encounters the end of the input stream where a tag name is expected. In this "
		"case the parser treats the beginning of a start tag (i.e., <) or an end tag (i.e., </) as text content.");
	const ParserError ParserError::EOF_IN_CDATA = ParserError("eof-in-cdata",
		"This error occurs if the parser encounters the end of the input stream in a CDATA section. The parser treats "
		"such CDATA sections as if they are closed immediately before the end of the input stream.");
	const ParserError ParserError::EOF_IN_COMMENT = ParserError("eof-in-comment",
		"This error occurs if the parser encounters the end of the input stream in a comment. The parser "
		"treats such comments as if they are closed immediately before the end of the input stream.");
	const ParserError ParserError::EOF_IN_DOCTYPE = ParserError("eof-in-doctype",
		"This error occurs if the parser encounters the end of the input stream in a DOCTYPE. In such a case, if the "
		"DOCTYPE is correctly placed as a document preamble, the parser sets the Document to quirks mode.");
	const ParserError ParserError::EOF_IN_SCRIPT_HTML_COMMENT_LIKE_TEXT
		= ParserError("eof-in-script-html-comment-like-text",
			"This error occurs if the parser encounters the end of the input stream in text that resembles "
			"an HTML comment inside script element content (e.g., <script><!-- foo).");
	const ParserError ParserError::EOF_IN_TAG = ParserError("eof-in-tag",
		"This error occurs if the parser encounters the end of the input stream in a start tag or an end "
		"tag (e.g., <div id=). Such a tag is completely ignored.");
	const ParserError ParserError::INCORRECTLY_CLOSED_COMMENT = ParserError("incorrectly-closed-comment",
		"This error occurs if the parser encounters a comment that is closed by the \"--!>\" code point sequence. The "
		"parser treats such comments as if they are correctly closed by the \"-->\" code point sequence.");
	const ParserError ParserError::INCORRECTLY_OPENED_COMMENT = ParserError("incorrectly-opened-comment",
		"This error occurs if the parser encounters the \"<!\" code point sequence that is not "
		"immidiately followed by two U+002D (-) code points and that is not the start of a DOCTYPE or a "
		"CDATA section. All content that follows the \"<!\" code point sequence up to a U+003E (>) code "
		"point (if present) or to the end of the input stream is treated as a comment.");
	const ParserError ParserError::INVALID_CHARACTER_SEQUENCE_AFTER_DOCTYPE_NAME = ParserError(
		"invalid-character-sequence-after-doctype-name",
		"This error occurs if the parser encounters any code point sequence other than \"PUBLIC\" and \"SYSTEM\" "
		"keywords after a DOCTYPE name. In such a case, the parser ignores any following public or system identifiers, "
		"and if the DOCTYPE is correctly placed as a document preamble, sets the Document to quirks mode.");
	const ParserError ParserError::INVALID_FIRST_CHARACTER_OF_TAG_NAME = ParserError(
		"invalid-first-character-of-tag-name",
		"This error occurs if the parser encounters a code point that is not an ASCII alpha where first code point of "
		"a start tag name or an end tag name is expected. If a start tag was expected such code point and a preceding "
		"U+003C (<) is treated as text content, and all content that follows is treated as markup. Whereas, if an end "
		"tag was expected, such code point and all content that follows up to a U+003E (>) code point (if present) or "
		"to the end of the input stream is treated as a comment.");
	const ParserError ParserError::MISSING_ATTRIBUTE_VALUE = ParserError("missing-attribute-value",
		"This error occurs if the parser encounters a U+003E (>) code point where an attribute value is "
		"expected (e.g., <div id=>). The parser treats the attribute as having an empty value.");
	const ParserError ParserError::MISSING_DOCTYPE_NAME = ParserError("missing-doctype-name",
		"This error occurs if the parser encounters a DOCTYPE that is missing a name (e.g., <!DOCTYPE>). "
		"In such a case, if the DOCTYPE is correctly placed as a document preamble, the parser sets the "
		"Document to quirks mode.");
	const ParserError ParserError::MISSING_DOCTYPE_PUBLIC_IDENTIFIER = ParserError("missing-doctype-public-identifier",
		"This error occurs if the parser encounters a U+003E (>) code point where start of the DOCTYPE "
		"public identifier is expected (e.g., <!DOCTYPE html PUBLIC >). In such a case, if the DOCTYPE "
		"is correctly placed as a document preamble, the parser sets the Document to quirks mode.");
	const ParserError ParserError::MISSING_DOCTYPE_SYSTEM_IDENTIFIER = ParserError("missing-doctype-system-identifier",
		"This error occurs if the parser encounters a U+003E (>) code point where start of the DOCTYPE "
		"system identifier is expected (e.g., <!DOCTYPE html SYSTEM >). In such a case, if the DOCTYPE "
		"is correctly placed as a document preamble, the parser sets the Document to quirks mode.");
	const ParserError ParserError::MISSING_END_TAG_NAME = ParserError("missing-end-tag-name",
		"This error occurs if the parser encounters a U+003E (>) code point where an end tag name is "
		"expected, i.e., </>. The parser completely ignores whole \"</>\" code point sequence.");
	const ParserError ParserError::MISSING_QOUTE_BEFORE_DOCTYPE_PUBLIC_IDENTIFIER = ParserError(
		"missing-quote-before-doctype-public-identifier",
		"This error occurs if the parser encounters the DOCTYPE public identifier that is not preceded by a quote "
		"(e.g., <!DOCTYPE html PUBLIC -//W3C//DTD HTML 4.01//EN\">). In such a case, the parser ignores the public "
		"identifier, and if the DOCTYPE is correctly placed as a document preamble, sets the Document to quirks mode.");
	const ParserError ParserError::MISSING_QOUTE_BEFORE_DOCTYPE_SYSTEM_IDENTIFIER = ParserError(
		"missing-quote-before-doctype-system-identifier",
		"This error occurs if the parser encounters the DOCTYPE system identifier that is not preceded by a quote "
		"(e.g., <!DOCTYPE html SYSTEM http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">). In such a case, the "
		"parser ignores the system identifier, and if the DOCTYPE is correctly placed as a document preamble, sets the "
		"Document to quirks mode.");
	const ParserError ParserError::MISSING_SEMICOLON_AFTER_CHARACTER_REFERENCE
		= ParserError("missing-semicolon-after-character-reference",
			"This error occurs if the parser encounters a character reference that is not terminated by a "
			"U+003B (;) code point. Usually the parser behaves as if character reference is terminated by "
			"the U+003B (;) code point; however, there are some ambiguous cases in which the parser includes "
			"subsequent code points in the character reference.");
	const ParserError ParserError::MISSING_WHITESPACE_AFTER_DOCTYPE_PUBLIC_KEYWORD = ParserError(
		"missing-whitespace-after-doctype-public-keyword",
		"This error occurs if the parser encounters a DOCTYPE whose \"PUBLIC\" keyword and public identifier are not "
		"separated by ASCII whitespace. In this case the parser behaves as if ASCII whitespace is present.");
	const ParserError ParserError::MISSING_WHITESPACE_AFTER_DOCTYPE_SYSTEM_KEYWORD = ParserError(
		"missing-whitespace-after-doctype-system-keyword",
		"This error occurs if the parser encounters a DOCTYPE whose \"SYSTEM\" keyword and system identifier are not "
		"separated by ASCII whitespace. In this case the parser behaves as if ASCII whitespace is present.");
	const ParserError ParserError::MISSING_WHITESPACE_BEFORE_DOCTYPE_NAME = ParserError(
		"missing-whitespace-before-doctype-name",
		"This error occurs if the parser encounters a DOCTYPE whose \"DOCTYPE\" keyword and name are not separated by "
		"ASCII whitespace. In this case the parser behaves as if ASCII whitespace is present.");
	const ParserError ParserError::MISSING_WHITESPACE_BETWEEN_ATTRIBUTES = ParserError(
		"missing-whitespace-between-attributes",
		"This error occurs if the parser encounters attributes that are not separated by ASCII whitespace (e.g., <div "
		"id=\"foo\"class=\"bar\">). In this case the parser behaves as if ASCII whitespace is present.");
	const ParserError ParserError::MISSING_WHITESPACE_BETWEEN_DOCTYPE_PUBLIC_AND_SYSTEM_IDENTIFIERS = ParserError(
		"missing-whitespace-between-doctype-public-and-system-identifiers",
		"This error occurs if the parser encounters a DOCTYPE whose public and system identifiers are not separated by "
		"ASCII whitespace. In this case the parser behaves as if ASCII whitespace is present.");
	const ParserError ParserError::NESTED_COMMENT = ParserError("nested-comment",
		"This error occurs if the parser encounters a nested comment (e.g., <!-- <!-- nested --> -->). "
		"Such a comment will be closed by the first occuring \"-->\" code point sequence and everything "
		"that follows will be treated as markup.");
	const ParserError ParserError::NONCHARACTER_CHARACTER_REFERENCE = ParserError("noncharacter-character-reference",
		"This error occurs if the parser encounters a numeric character reference that references a "
		"noncharacter. The parser resolves such character references as-is.");
	const ParserError ParserError::NONCHARACTER_IN_INPUT_STREAM = ParserError("noncharacter-in-input-stream",
		"This error occurs if the input stream contains a noncharacter. Such code points are parsed as-is and usually, "
		"where parsing rules don't apply any additional restrictions, make their way into the DOM.");
	const ParserError ParserError::NON_VOID_HTML_ELEMENT_START_TAG_WITH_TRAILING_SOLIDUS
		= ParserError("non-void-html-element-start-tag-with-trailing-solidus",
			"This error occurs if the parser encounters a start tag for an element that is not in the list "
			"of void elements or is not a part of foreign content (i.e., not an SVG or MathML element) that "
			"has a U+002F (/) code point right before the closing U+003E (>) code point. The parser behaves "
			"as if the U+002F (/) is not present.");
	const ParserError ParserError::NULL_CHARACTER_REFERENCE = ParserError("null-character-reference",
		"This error occurs if the parser encounters a numeric character reference that references a U+0000 NULL code "
		"point. The parser resolves such character references to a U+FFFD REPLACEMENT CHARACTER.");
	const ParserError ParserError::SURROGATE_CHARACTER_REFERENCE = ParserError("surrogate-character-reference",
		"This error occurs if the parser encounters a numeric character reference that references a "
		"surrogate. The parser resolves such character references to a U+FFFD REPLACEMENT CHARACTER.");
	const ParserError ParserError::SURROGATE_IN_INPUT_STREAM = ParserError("surrogate-in-input-stream",
		"This error occurs if the input stream contains a surrogate. Such code points are parsed as-is and usually, "
		"where parsing rules don't apply any additional restrictions, make their way into the DOM.");
	const ParserError ParserError::UNEXPECTED_CHARACTER_AFTER_DOCTYPE_SYSTEM_IDENTIFIER
		= ParserError("unexpected-character-after-doctype-system-identifier",
			"This error occurs if the parser encounters any code points other than ASCII whitespace or "
			"closing U+003E (>) after the DOCTYPE system identifier. The parser ignores these code points.");
	const ParserError ParserError::UNEXPECTED_CHARACTER_IN_ATTRIBUTE_NAME
		= ParserError("unexpected-character-in-attribute-name",
			"This error occurs if the parser encounters a U+0022 (\"), U+0027 ('), or U+003C (<) code point "
			"in an attribute name. The parser includes such code points in the attribute name.");
	const ParserError ParserError::UNEXPECTED_CHARACTER_IN_UNQOUTED_ATTRIBUTE_VALUE = ParserError(
		"unexpected-character-in-unquoted-attribute-value",
		"This error occurs if the parser encounters a U+0022 (\"), U+0027 ('), U+003C (<), U+003D (=), or U+0060 (`) "
		"code point in an unquoted attribute value. The parser includes such code points in the attribute value.");
	const ParserError ParserError::UNEXPECTED_EQUALS_SIGN_BEFORE_ATTRIBUTE_NAME
		= ParserError("unexpected-equals-sign-before-attribute-name",
			"This error occurs if the parser encounters a U+003D (=) code point before an attribute name. In "
			"this case the parser treats U+003D (=) as the first code point of the attribute name.");
	const ParserError ParserError::UNEXPECTED_NULL_CHARACTER = ParserError("unexpected-null-character",
		"This error occurs if the parser encounters a U+0000 NULL code point in the input stream in "
		"certain positions. In general, such code points are either completely ignored or, for security "
		"reasons, replaced with a U+FFFD REPLACEMENT CHARACTER.");
	const ParserError ParserError::UNEXPECTED_QUESTION_MARK_INSTEAD_OF_TAG_NAME
		= ParserError("unexpected-question-mark-instead-of-tag-name",
			"This error occurs if the parser encounters a U+003F (?) code point where first code point of a "
			"start tag name is expected. The U+003F (?) and all content that follows up to a U+003E (>) code "
			"point (if present) or to the end of the input stream is treated as a comment.");
	const ParserError ParserError::UNEXPECTED_SOLIDUS_IN_TAG = ParserError("unexpected-solidus-in-tag",
		"This error occurs if the parser encounters a U+002F (/) code point that is not a part of a "
		"quoted attribute value and not immediately followed by a U+003E (>) code point in a tag (e.g., "
		"<div / id=\"foo\">). In this case the parser behaves as if it encountered ASCII whitespace.");
	const ParserError ParserError::UNKNOWN_NAMED_CHARACTER_REFERENCE = ParserError("unknown-named-character-reference",
		"This error occurs if the parser encounters an ambiguous ampersand. In this case the parser "
		"doesn't resolve the character reference.");

	std::ostream &
	ParserErrorFunctions::operator<<(std::ostream &stream, const HTML::Tokenizer::ParserError &error) {
		return stream << error.name;
	}
} // namespace HTML::Tokenizer
