#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "insertion_mode.hpp"

namespace HTML::InsertionModes {
	class InHead : public HTML::InsertionMode {
	  public: // Methods
		inline explicit InHead(TreeConstructor &constructor) : HTML::InsertionMode(constructor, constructor.context) {
		}

		inline ~InHead() override {
		}

		bool
		EmitToken(HTML::Tokenizer::Token &inToken) override;

	private:
		HTML::InsertionModeSubroutineStatus
		HandleCharacter(HTML::Tokenizer::Token &);

		HTML::InsertionModeSubroutineStatus
		HandleComment(HTML::Tokenizer::Token &);

		HTML::InsertionModeSubroutineStatus
		HandleDoctype(HTML::Tokenizer::Token &);

		HTML::InsertionModeSubroutineStatus
		HandleEndTag(HTML::Tokenizer::Token &);

		HTML::InsertionModeSubroutineStatus
		HandleStartTag(HTML::Tokenizer::Token &);
	};
} // namespace HTML::InsertionModes
