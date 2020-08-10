#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

namespace CSS {

	enum class ParseError {
		INVALID,
		EOF_IN_CONSUMING_COMMENT,
		EOF_IN_CONSUMING_ESCAPED_CODE_POINT,
		EOF_IN_CONSUMING_STRING,
		NEWLINE_IN_CONSUMING_STRING,
	};

} // namespace CSS
