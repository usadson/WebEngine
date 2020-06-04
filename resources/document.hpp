#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "data/text/ustring.hpp"
#include "resource.hpp"

namespace Resources {

	class DocumentResource : public Resource {
	public:
		Unicode::UString data;
	};

}
