#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "tokenizer_parser.hpp"

namespace HTML {
	namespace Tokenizer {
		class Data : public Parser {
		public: // Methods
			inline explicit
			Data(Tokenizer &tokenizer)
				: Parser(tokenizer) {}

			~Data() override = default;

			bool
			Parse() override;
		};
	}
}
