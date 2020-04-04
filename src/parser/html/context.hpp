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
