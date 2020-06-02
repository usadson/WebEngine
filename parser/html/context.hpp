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
