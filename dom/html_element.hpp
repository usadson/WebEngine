#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <map>
#include <memory>

#include "dom/element.hpp"
#include "dom/element_internals.hpp"

namespace DOM {

	// The base class for all HTML elements.
	// Not to be confused with the <html> element, HTMLHtmlElement.
	// Specification:
	// https://html.spec.whatwg.org/multipage/dom.html#htmlelement
	class HTMLElement : public Element {
	  public: // DOM Attributes
		std::optional<Unicode::UString> title;
		std::optional<Unicode::UString> lang;
		bool translate{ false };
		std::optional<Unicode::UString> dir;

		bool hidden{ false };
		std::optional<Unicode::UString> accessKey;
		const std::optional<Unicode::UString> accessKeyLabel{};
		bool draggable{ false };
		bool spellcheck{ false };
		bool autocapitalize{ false };

		// The 'innerText' variable, but in a getter.
		// This function is TODO
		inline std::optional<Unicode::UString>
		innerText() const {
			return {};
		}

		ElementInternals
		attachInternals();
	};

	class HTMLUnknownElement : public HTMLElement {};
} // namespace DOM
