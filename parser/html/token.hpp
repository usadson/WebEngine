#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#pragma once

#include <map>
#include <optional>
#include <string>

#include "data/text/unicode.hpp"
#include "data/text/ustring.hpp"

namespace HTML::Tokenizer {
	enum class TokenType { CHARACTER, COMMENT, DOCTYPE, ENDTAG, EOF_TYPE, INVALID, STARTTAG };

	// From context.hpp:
	class Context;

	extern std::map<TokenType, std::string> tokenTypeNames;

	inline std::ostream &
	operator<<(std::ostream &stream, const TokenType &type) {
		return stream << tokenTypeNames[type];
	}

	struct Token {
	  private:
		TokenType internalType;

	  public:
		inline explicit Token(TokenType inType) : internalType(inType) {
		}

		virtual ~Token() = default;

		inline TokenType
		type() {
			return internalType;
		}
	};

	// DOCTYPE, start tag, end tag, comment, character, end-of-file
	struct DoctypeToken : public Token {
		// Initial state is missing
		std::optional<Unicode::UString> name;
		std::optional<Unicode::UString> publicIdentifier;
		std::optional<Unicode::UString> systemIdentifier;
		// Initial state is off (= false)
		bool forceQuirks = false;

		inline DoctypeToken() : Token(TokenType::DOCTYPE) {
		}
	};

	/* Don't use the following, its just for conveniance */
	struct AmbiguousTagToken : public Token {
	protected:
		std::map<Unicode::UString, Unicode::UString> attributes;

	public:
		Unicode::UString tagName;
		bool selfClosing = false;
		// For the tokenizer:
		Unicode::UString attributeName;
		Unicode::UString attributeValue;

		inline explicit AmbiguousTagToken(TokenType type) : Token(type) {
		}

		/**
		 * The HTML Living Standard ensures that this function wont be called
		 * when attributeName is empty. It is incorrect behavior if this does
		 * happen, and a proper Logger::Crash will be triggered if it detects
		 * this.
		 *
		 * This crash is in fact covered by an appropriate test.
		 */
		void
		AddTokenAttribute(Context &);

		[[nodiscard]] inline const std::map<Unicode::UString, Unicode::UString> &
		GetAttributes() const noexcept {
			return attributes;
		}

		inline void
		ClearAttributes() {
			attributes.clear();
		}

		static const AmbiguousTagToken INVALID_TYPE;
	};

	struct StartTagToken : public AmbiguousTagToken {
		inline StartTagToken() : AmbiguousTagToken(TokenType::STARTTAG) {
		}

		[[nodiscard]] std::optional<Unicode::UString>
		GetAttribute(const Unicode::UString &name);

		static const StartTagToken INVALID_TYPE_START_TAG;
	};

	struct EndTagToken : public AmbiguousTagToken {
		inline EndTagToken() : AmbiguousTagToken(TokenType::ENDTAG) {
		}

		static const EndTagToken INVALID_TYPE_END_TAG;
	};

	struct CommentToken : public Token {
		Unicode::UString contents;

		inline explicit CommentToken(const Unicode::UString &inContents)
			: Token(TokenType::COMMENT), contents(inContents) {
		}

		static const CommentToken INVALID_TYPE;
	};

	struct CharacterToken : public Token {
		Unicode::CodePoint character;

		inline explicit CharacterToken(Unicode::CodePoint character)
			: Token(TokenType::CHARACTER), character(character) {
		}
	};

	struct EOFToken : public Token {
		inline EOFToken() : Token(TokenType::EOF_TYPE) {
		}
	};
} // namespace HTML::Tokenizer
