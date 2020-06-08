#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

namespace HTML {
	class InsertionMode;
}

#include "parser/html/token.hpp"
#include "parser/html/tree_constructor.hpp"

namespace HTML {
	enum class InsertionModeSubroutineStatus {
		IGNORE,
		RECONSUME,
		PARSER_ERROR, // PARSER_ERROR -> effectively also IGNORE
		CONTINUE // Continue Execution
	};

	class InsertionMode {
	public: // References
		TreeConstructor &constructor;
		Tokenizer::Context &context;
	public: // Methods
		inline InsertionMode(TreeConstructor &constructor, Tokenizer::Context &context)
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
