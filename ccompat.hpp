#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 *
 * CCompat is the Compatibility Layer for working with C, when it is needed.
 */

#include <vector>

namespace CCompat {

	/** External Global Variable **/
	extern const std::vector<const char *> errnoNames;

	/** Function Prototypes **/
	const char *
	GetErrnoName(int error = 0);

	void
	CloseStandardIO();

}
