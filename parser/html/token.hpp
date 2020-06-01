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
#include <optional>
#include <string>

#include "data/text/unicode.hpp"
#include "data/text/ustring.hpp"
#include "context.hpp"

namespace HTML {
	namespace Tokenizer {
		enum class TokenType {
			CHARACTER,
			COMMENT,
			DOCTYPE,
			ENDTAG,
			EOF_TYPE,
			INVALID,
			STARTTAG
		};
		
		extern std::map<TokenType, std::string> TokenTypeNames;

		inline std::ostream &
		operator<<(std::ostream &stream, const TokenType &type) {
			return stream << TokenTypeNames[type];
		}

		struct Token {
			TokenType Type;

			inline explicit
			Token(TokenType type) : Type(type) {
			}

			virtual
			~Token() = default;
		};

		// DOCTYPE, start tag, end tag, comment, character, end-of-file
		struct DoctypeToken : public Token {
			// Initial state is missing
			std::optional<Unicode::UString> Name;
			std::optional<Unicode::UString> PublicIdentifier;
			std::optional<Unicode::UString> SystemIdentifier;
			// Initial state is off (= false)
			bool ForceQuirks = false;

			inline
			DoctypeToken() : Token(TokenType::DOCTYPE) {
			}
		};

		/* Don't use the following, its just for conveniance */
		struct AmbiguousTagToken : public Token {
			Unicode::UString TagName;
			bool SelfClosing = false;
			std::map<Unicode::UString, Unicode::UString> Attributes;
			// For the tokenizer:
			Unicode::UString AttributeName;
			Unicode::UString AttributeValue;
			
			inline explicit
			AmbiguousTagToken(TokenType type) : Token(type) {
			}

			void
			AddTokenAttribute(HTML::Tokenizer::Context &);

			static const AmbiguousTagToken INVALID_TYPE;
		};

		struct StartTagToken : public AmbiguousTagToken {
			inline
			StartTagToken() : AmbiguousTagToken(TokenType::STARTTAG) {
			}

			static const StartTagToken INVALID_TYPE_START_TAG;
		};

		struct EndTagToken : public AmbiguousTagToken {
			inline
			EndTagToken() : AmbiguousTagToken(TokenType::ENDTAG) {
			}

			static const EndTagToken INVALID_TYPE_END_TAG;
		};

		struct CommentToken : public Token {
			Unicode::UString Contents;

			inline explicit
			CommentToken(const Unicode::UString &contents)
				: Token(TokenType::COMMENT), Contents(contents) {
			}

			static const CommentToken INVALID_TYPE;
		};

		struct CharacterToken : public Token {
			Unicode::CodePoint Character;

			inline explicit
			CharacterToken(Unicode::CodePoint character)
				: Token(TokenType::CHARACTER), Character(character) {
			}
		};

		struct EOFToken : public Token {
			inline
			EOFToken()
				: Token(TokenType::EOF_TYPE) {
			}
		};
	};
};
