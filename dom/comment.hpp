#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "character_data.hpp"

namespace DOM {
	class Comment : public CharacterData {
	  public: // Methods
		inline Comment() : CharacterData() {
		}

		inline explicit Comment(const Unicode::UString &data) {
			this->data = data;
		}
	};
} // namespace DOM
