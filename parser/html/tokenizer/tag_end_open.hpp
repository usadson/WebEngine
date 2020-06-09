#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "tokenizer_parser.hpp"

namespace HTML::Tokenizer {
	class TagEndOpen : public Parser {
	  public: // Methods
		inline explicit TagEndOpen(Tokenizer &tokenizer) : Parser(tokenizer) {
		}

		~TagEndOpen() override = default;

		bool
		Parse() override;
	};
} // namespace HTML::Tokenizer
