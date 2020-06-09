/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "token.hpp"

#include <iostream>

#include "context.hpp"
#include "logger.hpp"

namespace HTML {
	namespace Tokenizer {
		const AmbiguousTagToken AmbiguousTagToken::INVALID_TYPE = AmbiguousTagToken(TokenType::INVALID);
		const CommentToken CommentToken::INVALID_TYPE = CommentToken(Unicode::UString("INVALID"));
		const EndTagToken EndTagToken::INVALID_TYPE_END_TAG = EndTagToken();
		const StartTagToken StartTagToken::INVALID_TYPE_START_TAG = StartTagToken();

		std::map<TokenType, std::string> tokenTypeNames
			= { { TokenType::CHARACTER, "CHARACTER" }, { TokenType::COMMENT, "COMMENT" },
				{ TokenType::DOCTYPE, "DOCTYPE" },	   { TokenType::ENDTAG, "ENDTAG" },
				{ TokenType::EOF_TYPE, "EOF_TYPE" },   { TokenType::INVALID, "EOF_TYPE" },
				{ TokenType::STARTTAG, "STARTTAG" } };

		void
		AmbiguousTagToken::AddTokenAttribute(HTML::Tokenizer::Context &context) {
			if (!attributes.insert(std::make_pair(attributeName, attributeValue)).second) {
				context.LogError(HTML::Tokenizer::ParserError::DUPLICATE_ATTRIBUTES);
			}

			// Reset:
			attributeName = Unicode::UString("");
			attributeValue = Unicode::UString("");
		}
	} // namespace Tokenizer
} // namespace HTML
