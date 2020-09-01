#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "parser/css/tokenizer.hpp"

namespace CSS {

	struct Rule {
		enum class Type {
			AT,
			QUALIFIED
		};

		Type type;
	};

} // namespace CSS::Parser
