#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "insertion_mode.hpp"

namespace HTML {
	namespace InsertionModes {
		class BeforeHead : public HTML::InsertionMode {
		public: // Methods
			inline explicit
			BeforeHead(TreeConstructor &constructor)
				: HTML::InsertionMode(constructor, constructor.context) {}

			inline
			~BeforeHead() override {
			}

			bool
			EmitToken(HTML::Tokenizer::Token &inToken) override;

		private: // Private Methods
			HTML::InsertionModeSubroutineStatus
			HandleCharacter(HTML::Tokenizer::Token &);

			HTML::InsertionModeSubroutineStatus
			HandleComment(HTML::Tokenizer::Token &);

			HTML::InsertionModeSubroutineStatus
			HandleEndTag(HTML::Tokenizer::Token &);

			HTML::InsertionModeSubroutineStatus
			HandleStartTag(HTML::Tokenizer::Token &);
		};
	}
}
