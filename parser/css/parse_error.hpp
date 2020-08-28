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
		EOF_IN_CONSUMING_URL,
		INVALID_AT_KEY_WORD_NAME,
		UNEXPECTED_CHARACTER_IN_URL,
		NEWLINE_IN_CONSUMING_STRING,
	};

} // namespace CSS
