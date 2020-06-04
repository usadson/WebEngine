#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <map>
#include <string>

// The strings supplied to this structure MUST be lowercased before storing.
namespace Resources {

	class MediaType {
	public:
		std::string type;
		std::map<std::string, std::string> parameters;
	};

}
