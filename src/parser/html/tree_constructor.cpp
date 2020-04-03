#include "tree_constructor.hpp"

#include <memory>
#include <iostream>
#include <vector>

#include <cstring>
#include <strings.h>

#include "tree/insert_initial.hpp"
#include "logger.hpp"

namespace HTML {

	TreeConstructor::TreeConstructor(Tokenizer::Context &context)
		: Context(context), CurrentMode(InsertionModeType::INITIAL), InsertionModes({
			{ InsertionModeType::INITIAL, std::shared_ptr<InsertionModes::Initial>(new InsertionModes::Initial(*this)) },
		}) {
	}

	void TreeConstructor::EmitToken(HTML::Tokenizer::Token &inToken) {
		auto iterator = InsertionModes.find(CurrentMode);
		if (iterator == InsertionModes.end()) {
			Logger::Warning("TreeConstructor", "Unknown insertion mode!");
		} else {
			iterator->second->EmitToken(inToken);
		}
	}

	void TreeConstructor::EmitCharacterToken(char character) {
		HTML::Tokenizer::CharacterToken token(character);
		EmitToken(token);
	}

	void TreeConstructor::EmitDoctypeQuirksToken(void) {
		HTML::Tokenizer::DoctypeToken token;
		token.ForceQuirks = true;
		EmitToken(token);
	}

	void TreeConstructor::EmitEOFToken(void) {
		HTML::Tokenizer::EOFToken token;
		EmitToken(token);
	}
};
 
