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

		/*
		 * The return value 'bool' is whether or not the token must be
		 * reprocessed in the next insertion mode.
		 */
		virtual bool EmitToken(HTML::Tokenizer::Token &inToken) = 0;
	};
}
