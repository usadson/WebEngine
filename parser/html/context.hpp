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
		DOM::Document DocumentNode;
	};

	namespace Tokenizer {
		class Context {
		public:
			HTML::ParserContext &ParserContext;

			ParserState State = ParserState::DATA;
			ParserState ReturnState = ParserState::UNDEFINED;
			// The state at the start of each the tokenizer loop. This is a
			// lot of times the same as 'State'.
			ParserState BeginLoopState = ParserState::DATA;
			size_t LineCount;
			size_t LinePosition;
			char CurrentCharacter;
			std::vector<Unicode::CodePoint> TemporaryBuffer;
			Unicode::UString NCRefBuffer;
		public: // Methods
			inline Context(HTML::ParserContext &context)
				: ParserContext(context) {}

			void LogError(const HTML::Tokenizer::ParserError &error);
		};
	};
};
