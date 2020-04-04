#include "tree_constructor.hpp"

#include <memory>
#include <iostream>
#include <sstream>
#include <vector>

#include "tree/insert_initial.hpp"
#include "logger.hpp"

namespace HTML {

	TreeConstructor::TreeConstructor(Tokenizer::Context &context)
		: Context(context), CurrentMode(InsertionModeType::INITIAL), InsertionModes({
			{ InsertionModeType::INITIAL, std::shared_ptr<InsertionModes::Initial>(new InsertionModes::Initial(*this)) },
		}) {
	}

	void TreeConstructor::EmitToken(HTML::Tokenizer::Token &inToken) {
		bool	reprocess;
		size_t	reprocessCount;

		do {
			if (reprocessCount == 10) {
				Logger::Warning("TreeConstructor", "Reprocess loop detected! Reprocess requested 10 times! Quitting emission of token.");
				return;
			}

			auto iterator = InsertionModes.find(CurrentMode);

			if (iterator == InsertionModes.end()) {
				std::stringstream output;
				output << "Unknown insertion mode: \033[1;35m";
				output << CurrentMode;
				Logger::Warning("TreeConstructor", output.str());
				reprocess = false;
			} else
				reprocess = iterator->second->EmitToken(inToken);

			reprocessCount++;
		} while (reprocess);
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
 
