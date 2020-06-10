#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "insertion_mode.hpp"

namespace HTML::InsertionModes {
	class BeforeHTML : public HTML::InsertionMode {
	  public: // Methods
		inline explicit BeforeHTML(TreeConstructor &constructor)
			: HTML::InsertionMode(constructor, constructor.context) {
		}

		~BeforeHTML() = default;

	  public: // Public Methods
		bool
		EmitToken(HTML::Tokenizer::Token &inToken) override;

	  private: // Private Methods
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
