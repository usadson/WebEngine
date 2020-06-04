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
	class Element : public Node {
	public: // Internal Properties
		std::map<Unicode::UString, Unicode::UString> internalAttributes;
	public: // DOM Attributes
		/* Readonly */
		std::optional<Unicode::UString> namespaceURI;
		std::optional<Unicode::UString> prefix;
		Unicode::UString localName;
		Unicode::UString tagName;

		Unicode::UString ID;
		Unicode::UString className;

		// Children should govern the Node::ChildNodes vector somehow.
		std::vector<std::shared_ptr<DOM::Element>> children;
	};
}
