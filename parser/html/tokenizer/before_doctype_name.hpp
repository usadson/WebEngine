#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "tokenizer_parser.hpp"

namespace HTML::Tokenizer {
	class BeforeDoctypeName : public Parser {
	  public: // Methods
		inline explicit BeforeDoctypeName(Tokenizer &tokenizer) : Parser(tokenizer) {
		}

		~BeforeDoctypeName() override = default;

		bool
		Parse() override;
	};
} // namespace HTML::Tokenizer
