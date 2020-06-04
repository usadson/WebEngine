/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "tree_constructor.hpp"

#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#include "tree/insert_before_head.hpp"
#include "tree/insert_before_html.hpp"
#include "tree/insert_initial.hpp"
#include "logger.hpp"

namespace HTML {

	TreeConstructor::TreeConstructor(Tokenizer::Context &context)
		: context(context), currentMode(InsertionModeType::INITIAL), insertionModes({
			{ InsertionModeType::INITIAL, std::make_shared<InsertionModes::Initial>(*this) },
			{ InsertionModeType::BEFORE_HTML, std::make_shared<InsertionModes::BeforeHTML>(*this) },
			{ InsertionModeType::BEFORE_HEAD, std::make_shared<InsertionModes::BeforeHead>(*this) },
		}) {
	}

	void
	TreeConstructor::EmitToken(HTML::Tokenizer::Token &inToken) {
		bool	reprocess;
		size_t	reprocessCount;

		reprocessCount = 0;

		do {
			if (reprocessCount == 10) {
				std::stringstream info;
				info << "Reprocess loop detected! Reprocess requested 10 times! Quitting emission of token.";
				info << " InsertionMode: " << currentMode;
				Logger::Warning("TreeConstructor", info.str());
				return;
			}

			auto iterator = insertionModes.find(currentMode);

			if (iterator == insertionModes.end()) {
				std::stringstream output;
				output << "Unknown insertion mode: \033[1;35m";
				output << currentMode;
				Logger::Warning("TreeConstructor", output.str());
				reprocess = false;
			} else
				reprocess = iterator->second->EmitToken(inToken);

			reprocessCount++;
		} while (reprocess);
	}

	void
	TreeConstructor::EmitCharacterToken(char character) {
		HTML::Tokenizer::CharacterToken token(character);
		EmitToken(token);
	}

	void
	TreeConstructor::EmitDoctypeQuirksToken(void) {
		HTML::Tokenizer::DoctypeToken token;
		token.forceQuirks = true;
		EmitToken(token);
	}

	void
	TreeConstructor::EmitEOFToken(void) {
		HTML::Tokenizer::EOFToken token;
		EmitToken(token);
	}
}
