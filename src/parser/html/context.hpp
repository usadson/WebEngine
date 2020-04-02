#pragma once

namespace HTML {
	namespace Tokenizer {
		class Context;
	}
}

#include "error.hpp"
#include "state.hpp"
#include "token.hpp"

namespace HTML {
	namespace Tokenizer {
		class Context {
		public:
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
			void LogError(const HTML::Tokenizer::ParserError &error);
		};
	};
};
