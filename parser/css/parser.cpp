/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "parser/css/parser.hpp"

namespace CSS {

	std::tuple<bool, Stylesheet>
	Parser::ParseStylesheet(const Tokenizer &tokenizer) noexcept {
		return {true, {}};
	}

} // namespace CSS
