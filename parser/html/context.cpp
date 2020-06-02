/**
 * Copyright (C) 2020 Tristan <tristan@thewoosh.me>
 *
 * All Rights Reserved.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
 */

#include "context.hpp"

#include <iostream>

namespace HTML {
	namespace Tokenizer {
		void
		Context::LogError(const HTML::Tokenizer::ParserError &error) {
			std::cerr << "\033[1;31mParserError: \033[1;33m" << error.Name 
					  << "\033[1;31m at \033[1;35m" << lineCount << ':' << linePosition
					  << "\033[1;31m in state \033[1;36m" << state;

			if (beginLoopState != state)
				std::cerr << "\033[1;31m (begin was: \033[1;33m" << beginLoopState << ')';

			std::cerr << "\033[1;31m (character: '" << currentCharacter << "' i.e. 0x"
					  << std::hex << (size_t) currentCharacter << std::dec
					  << ")\033[1;0m"
					  << std::endl;
		}
	};
};
