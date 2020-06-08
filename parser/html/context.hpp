#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

namespace HTML::Tokenizer {
	class Context;
}

#include "dom/document.hpp"
#include "resources/document.hpp"

#include "error.hpp"
#include "state.hpp"
#include "token.hpp"

namespace HTML {
	class ParserContext {
	public:
		std::shared_ptr<DOM::Document> documentNode = std::make_shared<DOM::Document>();
	};
}

namespace HTML::Tokenizer {
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

		Unicode::CodePoint character;
		HTML::Tokenizer::CommentToken commentToken { HTML::Tokenizer::CommentToken::INVALID_TYPE };
		HTML::Tokenizer::DoctypeToken doctypeToken;
		HTML::Tokenizer::EndTagToken endTagToken;
		bool eof;
		Resources::DocumentResource *document { nullptr };
		size_t documentSize;
		size_t i;
		bool isEndTag;
		bool reconsume;
		HTML::Tokenizer::StartTagToken startTagToken;
		size_t toConsumeNext;
		size_t unknownStateCount;

	public: // Methods
		inline explicit
		Context(HTML::ParserContext &context)
			: parserContext(context) {
		}

		void
		LogError(const HTML::Tokenizer::ParserError &error);

		inline AmbiguousTagToken &
		GetCurrentTagToken() {
			return isEndTag ?
				static_cast<AmbiguousTagToken &>(endTagToken) :
				static_cast<AmbiguousTagToken &>(startTagToken);
		}
	};
}
