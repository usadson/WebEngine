#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

namespace CSS {

	enum class TokenType {
		IDENT,        // <ident-token>
		FUNCTION,     // <function-token>
		AT_KEYWORD,   // <at-keyword-token>
		HASH,         // <hash-token>
		STRING,       // <string-token>
		BAD_STRING,   //<bad-string-token>
		URL,          // <url-token>
		BAD_URL,      // <bad-url-token>
		DELIM,        // <delim-token>
		NUMBER,       // <number-token>
		PERCENTAGE,   //<percentage-token>
		DIMENSION,    // <dimension-token>
		WHITESPACE,   // <whitespace-token>
		CDO,          // <CDO-token>
		CDC,          // <CDC-token>
		COLON,        // <colon-token>
		SEMICOLON,    // <semicolon-token>
		COMMA,        // <comma-token>
		SQUARE_OPEN,  // <[-token>
		SQUARE_CLOSE, // <]-token>
		PAREN_OPEN,   // <(-token>
		PAREN_CLOSE,  // <)-token>
		CURLY_OPEN,   // <{-token>
		CURLY_CLOSE   // <}-token>
	};

} // namespace CSS
