#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <map>
#include <memory>

namespace HTML {
	class TreeConstructor;
}

#include "dom/element.hpp"
#include "parser/html/insertion_mode.hpp"
#include "tree/insertion_mode.hpp"
#include "token.hpp"

namespace HTML {
	class TreeConstructor {
	public: // Properties
		Tokenizer::Context &context;
		InsertionModeType currentMode;
		std::vector<std::shared_ptr<DOM::Element>> openElementsStack;
		std::map<InsertionModeType, std::shared_ptr<InsertionMode>> insertionModes;
	public: // Methods
		explicit
		TreeConstructor(Tokenizer::Context &);

		void
		EmitCharacterToken(char);

		void
		EmitDoctypeQuirksToken();

		void
		EmitEOFToken();

		void
		EmitToken(HTML::Tokenizer::Token &);
	};
}
