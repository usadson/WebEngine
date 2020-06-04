#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "insertion_mode.hpp"

namespace HTML {
	namespace InsertionModes {
		class Initial : public HTML::InsertionMode {
		public: // Methods
			inline explicit
			Initial(TreeConstructor &constructor)
				: HTML::InsertionMode(constructor, constructor.context) {}

			inline
			~Initial() override {
			}

			bool
			EmitToken(HTML::Tokenizer::Token &inToken) override;
		};
	}
}
