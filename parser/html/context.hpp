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
	enum CharacterEncodingConfidence {
		TENTATIVE,
		CERTAIN,
		IRRELEVANT
	};

	class ParserContext {
	  public:
		std::shared_ptr<DOM::Document> documentNode = std::make_shared<DOM::Document>();
		bool isIframeSrcdoc{ false };

		CharacterEncodingConfidence confidence { CharacterEncodingConfidence::IRRELEVANT };

		void
		ReportParserError(const std::string &source, const std::string &message);
	};
} // namespace HTML

namespace HTML::Tokenizer {
	class Context {
	  public:
		HTML::ParserContext &parserContext;

		ParserState state = ParserState::DATA;
		ParserState returnState = ParserState::UNDEFINED;
		// The state at the start of each the tokenizer loop. This is a
		// lot of times the same as 'State'.
		ParserState beginLoopState = ParserState::DATA;
		std::size_t lineCount = 0;
		std::size_t linePosition = 0;
		char currentCharacter = '\0';
		std::vector<Unicode::CodePoint> temporaryBuffer;
		Unicode::UString ncRefBuffer;

		Unicode::CodePoint character;
		CommentToken commentToken{ CommentToken::INVALID_TYPE };
		DoctypeToken doctypeToken;
		EndTagToken endTagToken;
		bool eof;
		Resources::DocumentResource *document{ nullptr };
		std::size_t documentSize;
		std::size_t i;
		bool isEndTag;
		bool reconsume;
		StartTagToken startTagToken;
		std::size_t toConsumeNext;
		std::size_t unknownStateCount;

	  public: // Methods
		inline explicit Context(HTML::ParserContext &context) : parserContext(context) {
		}

		virtual
		~Context() = default;

		virtual void
		LogError(const ParserError &error);

		inline AmbiguousTagToken &
		GetCurrentTagToken() {
			return isEndTag ? static_cast<AmbiguousTagToken &>(endTagToken)
							: static_cast<AmbiguousTagToken &>(startTagToken);
		}
	};
} // namespace HTML::Tokenizer
