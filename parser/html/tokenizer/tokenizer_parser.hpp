#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "parser/html/tokenizer.hpp"

namespace HTML::Tokenizer {
	class Parser {
	  public: // References
		Context &context;
		Tokenizer &tokenizer;

	  public: // Methods
		inline Parser(Tokenizer &tokenizer) : context(tokenizer.context), tokenizer(tokenizer) {
		}

		inline virtual ~Parser() {
		}

		virtual bool
		Parse()
			= 0;
	};
} // namespace HTML::Tokenizer
