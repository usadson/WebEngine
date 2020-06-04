#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "parser/html/token.hpp"
#include "parser/html/tree_constructor.hpp"

namespace HTML {
	class TokenizerParser {
	public: // References
		TreeConstructor &constructor;
		Tokenizer::Context &context;
	public: // Methods
		inline
		TokenizerParser(TreeConstructor &constructor, Tokenizer::Context &context)
			: constructor(constructor), context(context) {}

		inline virtual
		~InsertionMode() {
		}

		/*
		 * The return value 'bool' is whether or not the token must be
		 * reprocessed in the next insertion mode.
		 */
		virtual bool
		EmitToken(HTML::Tokenizer::Token &inToken) = 0;
	};
}
