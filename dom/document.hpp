#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <memory>

#include "element.hpp"
#include "node.hpp"

namespace DOM {
	enum class QuirksMode { LIMITED_QUIRKS, NO_QUIRKS, QUIRKS };

	class Document : public Node {
	  public: // Properties
		QuirksMode mode;

		// Children should govern the Node::ChildNodes vector somehow.
		std::vector<std::shared_ptr<DOM::Element> > children;
	};
} // namespace DOM
