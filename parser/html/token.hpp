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
		
		extern std::map<TokenType, std::string> tokenTypeNames;

		inline std::ostream &
		operator<<(std::ostream &stream, const TokenType &type) {
			return stream << tokenTypeNames[type];
		}

		struct Token {
			TokenType type;

			inline explicit
			Token(TokenType inType) : type(inType) {
			}

			virtual
			~Token() = default;
		};

		// DOCTYPE, start tag, end tag, comment, character, end-of-file
		struct DoctypeToken : public Token {
			// Initial state is missing
			std::optional<Unicode::UString> name;
			std::optional<Unicode::UString> publicIdentifier;
			std::optional<Unicode::UString> systemIdentifier;
			// Initial state is off (= false)
			bool forceQuirks = false;

			inline
			DoctypeToken() : Token(TokenType::DOCTYPE) {
			}
		};

		/* Don't use the following, its just for conveniance */
		struct AmbiguousTagToken : public Token {
			Unicode::UString tagName;
			bool selfClosing = false;
			std::map<Unicode::UString, Unicode::UString> attributes;
			// For the tokenizer:
			Unicode::UString attributeName;
			Unicode::UString attributeValue;
			
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
			Unicode::UString contents;

			inline explicit
			CommentToken(const Unicode::UString &inContents)
				: Token(TokenType::COMMENT), contents(inContents) {
			}

			static const CommentToken INVALID_TYPE;
		};

		struct CharacterToken : public Token {
			Unicode::CodePoint character;

			inline explicit
			CharacterToken(Unicode::CodePoint character)
				: Token(TokenType::CHARACTER), character(character) {
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
