#pragma once

namespace HTML {
	class InsertionMode;
}

#include "parser/html/token.hpp"
#include "parser/html/tree_constructor.hpp"

namespace HTML {
	class InsertionMode {
	public: // References
		TreeConstructor &Constructor;
		Tokenizer::Context &Context;
	public: // Methods
		inline InsertionMode(TreeConstructor &constructor, Tokenizer::Context &context)
			: Constructor(constructor), Context(context) {}

		virtual void EmitToken(HTML::Tokenizer::Token &inToken) = 0;
	};
}
