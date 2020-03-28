#pragma once

#include <map>
#include <optional>
#include <string>

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
			STARTTAG,
		};

		struct Token {
			TokenType Type;
			inline Token(TokenType type) : Type(type) {
			}
			virtual ~Token() = default;
		};
		// DOCTYPE, start tag, end tag, comment, character, end-of-file
		struct DoctypeToken : public Token {
			// Initial state is missing
			std::optional<std::string> Name;
			std::optional<std::string> PublicIdentifier;
			std::optional<std::string> SystemIdentifier;
			// Initial state is off (= false)
			bool ForceQuirks = false;

			inline DoctypeToken() : Token(TokenType::DOCTYPE) {
			}
		};
		/* Don't use the following, its just for conveniance */
		struct AmbiguousTagToken : public Token {
			std::string TagName;
			bool SelfClosing = false;
			std::map<std::string, std::string> Attributes;
			// For the tokenizer:
			std::string AttributeName;
			std::string AttributeValue;
			
			inline AmbiguousTagToken(TokenType type) : Token(type) {
			}

			void AddTokenAttribute(HTML::Tokenizer::Context &context);

			static const AmbiguousTagToken INVALID_TYPE;
		};

		struct StartTagToken : public AmbiguousTagToken {
			inline StartTagToken() : AmbiguousTagToken(TokenType::STARTTAG) {
			}

			static const StartTagToken INVALID_TYPE;
		};
		struct EndTagToken : public AmbiguousTagToken {
			inline EndTagToken() : AmbiguousTagToken(TokenType::ENDTAG) {
			}

			static const EndTagToken INVALID_TYPE;
		};
		struct CommentToken : public Token {
			std::string Contents;

			inline CommentToken(std::string contents) : Token(TokenType::COMMENT), Contents(contents) {
			}

			static const CommentToken INVALID_TYPE;
		};

		struct CharacterToken : public Token {
			char Character;

			inline CharacterToken(char character) : Token(TokenType::CHARACTER), Character(character) {
			}
		};

		struct EOFToken : public Token {
			inline EOFToken() : Token(TokenType::EOF_TYPE) {
			}
		};
	};
};
