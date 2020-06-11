#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <map>
#include <memory>

namespace DOM {
	class ElementInternals;
} // namespace DOM

#include "dom/element.hpp"

namespace DOM {

	// The base class for all HTML elements.
	// Not to be confused with the <html> element, HTMLHtmlElement.
	// Specification:
	// https://html.spec.whatwg.org/multipage/dom.html#htmlelement
	class ElementInternals {
		// Form-associated custom elements

		// TODO
	};
} // namespace DOM
