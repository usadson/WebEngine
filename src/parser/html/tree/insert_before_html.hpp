#pragma once

#include "insertion_mode.hpp"

namespace HTML {
	namespace InsertionModes {
		class BeforeHTML : public HTML::InsertionMode {
		public: // Methods
			inline BeforeHTML(TreeConstructor &constructor)
				: HTML::InsertionMode(constructor, constructor.Context) {}

			bool EmitToken(HTML::Tokenizer::Token &inToken);
		};
	}
}
 
