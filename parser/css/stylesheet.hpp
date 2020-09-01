#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "parser/css/rule.hpp"
#include "parser/css/tokenizer.hpp"

namespace CSS {

	struct Stylesheet {
		std::vector<Rule> rules;
	};

} // namespace CSS::Parser
