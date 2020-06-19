#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <map>
#include <memory>

#include "data/text/ustring.hpp"
#include "node.hpp"

namespace DOM {

	/**
	 * Spec:
	 * https://dom.spec.whatwg.org/#interface-element
	 */
	enum class CustomElementState { CUSTOM, FAILED, UNCUSTOMIZED, UNDEFINED };

	/**
	 * In the following class, if std::optional<T> is used, it often means that
	 * he property is nullable without the property type natively supporting
	 * it, and a std::optional<T>::has_value() = false means that the element
	 * is effectively null.
	 */
	class Element : public Node {
	  public: // Internal Properties
		std::map<Unicode::UString, Unicode::UString> internalAttributes;

	  public: // Destructor
		virtual ~Element() = default;

	  public: // DOM Attributes
		/* Readonly */
		std::optional<Unicode::UString> namespaceURI;
		std::optional<Unicode::UString> namespacePrefix;
		Unicode::UString localName;
		Unicode::UString tagName;

		CustomElementState customElementState;
		/** TODO Custom Element Definition */

		std::optional<Unicode::UString> is;

		Unicode::UString ID;
		Unicode::UString className;

		// Children should govern the Node::ChildNodes vector somehow.
		std::vector<std::shared_ptr<DOM::Element>> children;
	};
} // namespace DOM
