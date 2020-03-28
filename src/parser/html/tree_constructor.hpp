#pragma once

#include "token.hpp"

namespace HTML {
	class TreeConstructor {
	public: // Properties
		Tokenizer::Context &Context;
	public: // Methods
		inline TreeConstructor(Tokenizer::Context &context) : Context(context) {
		}

		void EmitCharacterToken(char character);
		void EmitDoctypeQuirksToken(void);
		void EmitEOFToken(void);
		void EmitToken(HTML::Tokenizer::Token &token);
	};
};
 
