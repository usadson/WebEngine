#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <map>
#include <memory>

#include "dom/html_element.hpp"

namespace DOM {

	// The <html> element, *not* the base class for HTML elements, i.e. HTMLElement
	// Specification:
	// https://html.spec.whatwg.org/multipage/semantics.html#the-html-element
	class HTMLHtmlElement : public HTMLElement {
	};
} // namespace DOM
