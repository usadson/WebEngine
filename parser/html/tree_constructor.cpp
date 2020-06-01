/**
 * Copyright (C) 2020 Tristan <tristan@thewoosh.me>
 *
 * All Rights Reserved.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
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
		: Context(context), CurrentMode(InsertionModeType::INITIAL), InsertionModes({
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
				info << " InsertionMode: " << CurrentMode;
				Logger::Warning("TreeConstructor", info.str());
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

	void
	TreeConstructor::EmitCharacterToken(char character) {
		HTML::Tokenizer::CharacterToken token(character);
		EmitToken(token);
	}

	void
	TreeConstructor::EmitDoctypeQuirksToken(void) {
		HTML::Tokenizer::DoctypeToken token;
		token.ForceQuirks = true;
		EmitToken(token);
	}

	void
	TreeConstructor::EmitEOFToken(void) {
		HTML::Tokenizer::EOFToken token;
		EmitToken(token);
	}
};
 
