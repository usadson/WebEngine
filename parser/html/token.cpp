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

#include "token.hpp"

#include <iostream>

#include "logger.hpp"

namespace HTML {
	namespace Tokenizer {
		const AmbiguousTagToken AmbiguousTagToken::INVALID_TYPE = AmbiguousTagToken(TokenType::INVALID);
		const CommentToken CommentToken::INVALID_TYPE = CommentToken(Unicode::UString("INVALID"));
		const EndTagToken EndTagToken::INVALID_TYPE_END_TAG = EndTagToken();
		const StartTagToken StartTagToken::INVALID_TYPE_START_TAG = StartTagToken();

		std::map<TokenType, std::string> tokenTypeNames = {
			{ TokenType::CHARACTER, "CHARACTER" },
			{ TokenType::COMMENT, "COMMENT" },
			{ TokenType::DOCTYPE, "DOCTYPE" },
			{ TokenType::ENDTAG, "ENDTAG" },
			{ TokenType::EOF_TYPE, "EOF_TYPE" },
			{ TokenType::INVALID, "EOF_TYPE" },
			{ TokenType::STARTTAG, "STARTTAG" }
		};

		void
		AmbiguousTagToken::AddTokenAttribute(HTML::Tokenizer::Context &context) {
			if (!attributes.insert(std::make_pair(attributeName, attributeValue)).second) {
				context.LogError(HTML::Tokenizer::ParserError::DUPLICATE_ATTRIBUTES);
			}

			// Reset:
			attributeName = Unicode::UString("");
			attributeValue = Unicode::UString("");
		}
	};
};
