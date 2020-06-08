/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "context.hpp"

#include <iostream>

void
HTML::Tokenizer::Context::LogError(const HTML::Tokenizer::ParserError &error) {
	std::cerr << "\033[1;31mParserError: \033[1;33m" << error.name
				<< "\033[1;31m at \033[1;35m" << lineCount << ':' << linePosition
				<< "\033[1;31m in state \033[1;36m" << state;

	if (beginLoopState != state)
		std::cerr << "\033[1;31m (begin was: \033[1;33m" << beginLoopState << ')';

	std::cerr << "\033[1;31m (character: '" << currentCharacter << "' i.e. 0x"
				<< std::hex << (size_t) currentCharacter << std::dec
				<< ")\033[1;0m"
				<< std::endl;
}
