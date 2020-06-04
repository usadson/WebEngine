#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#pragma once

#include <map>
#include <string>

namespace Options {

	bool
	ParseCommandLine(int, const char **);

	std::optional<const std::optional<std::string> *>
	GetCommandLineParameter(const std::string &);

}
