#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "dom/element.hpp"

namespace DOM {
	class ResettableElement : public Element {
	  public:
		/* Spec:
		 * https://html.spec.whatwg.org/multipage/form-control-infrastructure.html#concept-form-reset
		 */
		virtual void
		reset()
			= 0;
	};
} // namespace DOM
