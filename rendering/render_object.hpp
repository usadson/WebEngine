#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "render_bounds.hpp"

namespace Rendering {

	enum class RenderObjectType {
		RECT,
		TEXT
	};

	class RenderObject {
	public: // Properties
		RenderBounds bounds;

	public: // Internal Properties
		void *renderContext;
		RenderObjectType type;

	public: // Methods
		inline explicit
		RenderObject(RenderObjectType inType)
			: bounds({ 0, 0, 0, 0 }), renderContext(nullptr), type(inType) {
		}

		virtual
		~RenderObject() = default;
	};

}
