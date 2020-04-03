#pragma once

#include <map>
#include <memory>

namespace HTML {
	class TreeConstructor;
}

#include "parser/html/insertion_mode.hpp"
#include "tree/insertion_mode.hpp"
#include "token.hpp"

namespace HTML {
	class TreeConstructor {
	public: // Properties
		Tokenizer::Context &Context;
	private: // Private Properties
		InsertionModeType CurrentMode;
		std::map<InsertionModeType, std::shared_ptr<InsertionMode>> InsertionModes;
	public: // Methods
		TreeConstructor(Tokenizer::Context &context);

		void EmitCharacterToken(char character);
		void EmitDoctypeQuirksToken(void);
		void EmitEOFToken(void);
		void EmitToken(HTML::Tokenizer::Token &token);
	};
};
 
