#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#pragma once

#include <optional>
#include <string>

namespace Options {

	struct OptionLookupResult {
		bool wasSpecified;
		std::optional<std::string> value;
	};

	bool
	ParseCommandLine(int, const char **);

	OptionLookupResult
	GetCommandLineParameter(const std::string &);

} // namespace Options
