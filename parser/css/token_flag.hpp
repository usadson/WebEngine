#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

namespace CSS {

	// Token flags.
	// https://www.w3.org/TR/css-syntax-3/#tokenization
	enum class TokenFlag {
		// Fallback flag for all tokens
		NULL_FLAG,    // unnormative

		// Flags for <hash-token>
		ID,           // <id>
		UNRESTRICTED, // <unrestricted>

		// Flags for <number-token>, <percentage-token>, and <dimension-token>
		INTEGER,      // <integer>
		NUMBER,       // <number>
	};

} // namespace CSS
