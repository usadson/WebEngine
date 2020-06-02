/**
 * Copyright (C) 2020 Tristan
 *
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
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
};
 
