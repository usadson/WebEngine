#pragma once

#include "insertion_mode.hpp"

namespace HTML {
	namespace InsertionModes {
		class Initial : public HTML::InsertionMode {
		public: // Methods
			inline Initial(TreeConstructor &constructor)
				: HTML::InsertionMode(constructor, constructor.Context) {}

			virtual void EmitToken(HTML::Tokenizer::Token &inToken);
		};
	}
}
