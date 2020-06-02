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

namespace HTML {
	namespace Tokenizer {
		class Context;
	}
}

#include "dom/document.hpp"

#include "error.hpp"
#include "state.hpp"
#include "token.hpp"

namespace HTML {
	class ParserContext {
	public:
		DOM::Document documentNode;
	};

	namespace Tokenizer {
		class Context {
		public:
			HTML::ParserContext &parserContext;

			ParserState state = ParserState::DATA;
			ParserState returnState = ParserState::UNDEFINED;
			// The state at the start of each the tokenizer loop. This is a
			// lot of times the same as 'State'.
			ParserState beginLoopState = ParserState::DATA;
			size_t lineCount = 0;
			size_t linePosition = 0;
			char currentCharacter = '\0';
			std::vector<Unicode::CodePoint> temporaryBuffer;
			Unicode::UString ncRefBuffer;
		public: // Methods
			inline explicit
			Context(HTML::ParserContext &context)
				: parserContext(context) {
			}

			void
			LogError(const HTML::Tokenizer::ParserError &error);
		};
	};
};
