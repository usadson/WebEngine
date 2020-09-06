#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <memory>
#include <variant>
#include <vector>

#include "data/text/ustring.hpp"
#include "parser/css/rule.hpp"
#include "parser/css/token.hpp"

namespace CSS {

	struct ComponentValue;

	struct Block {
		std::vector<ComponentValue> list;
	};

	struct Function {
		Unicode::UString name;
		std::vector<ComponentValue> values;
	};

	struct ComponentValue {
		std::variant<Token, Function, Block> value;
	};

	struct Declaration {
		bool important{false};
	};

	struct Stylesheet {
		std::vector<Rule> rules;
	};

} // namespace CSS
