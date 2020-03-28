#include "context.hpp"

#include <iostream>

namespace HTML {
	namespace Tokenizer {
		void Context::LogError(const HTML::Tokenizer::ParserError &error) {
			std::cerr << "\033[1;31mParserError: \033[1;33m" << error.Name 
					  << "\033[1;31m at \033[1;35m" << LineCount << ':' << LinePosition
					  << "\033[1;31m in state \033[1;36m" << State;

			if (BeginLoopState != State)
				std::cerr << "\033[1;31m (begin was: \033[1;33m" << BeginLoopState << ')';

			std::cerr << "\033[1;31m (character: '" << CurrentCharacter << "' i.e. 0x"
					  << std::hex << (size_t) CurrentCharacter << std::dec
					  << ")\033[1;0m"
					  << std::endl;
		}
	};
};
