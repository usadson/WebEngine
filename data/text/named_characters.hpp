#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "unicode.hpp"
#include "ustring.hpp"

namespace NamedCharacters {

	enum class NCStatus {
		// The node has a value and the amount of paths taken == strlen but the
		// node has children, e.g. Find("int", ...), because 'int' is a value,
		// but 'integer' too, and 'int' has a child [e].
		AMBIGUOUS,
		// The node has a value and the amount of paths taken == strlen
		FOUND,
		// The node has children but the string doesn't have more characters,
		// and the last node doesn't have a value.
		INCOMPLETE,
		// Named character references may only consist of ASCII characters.
		INVALID_STRING,
		// The string hasn't ended but a child with the a character of the
		// string doesn't exists.
		NOT_FOUND
	};

	[[nodiscard]] NCStatus
	Find(const Unicode::UString &string, Unicode::CodePoint *result);

	void
	Setup();

} // namespace NamedCharacters
