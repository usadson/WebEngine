#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <functional>
#include <optional>
#include <vector>

namespace DOM {
	struct Node;
}

namespace DOM {
	class Document;

	struct Node {
	  public: // Properties
		std::vector<Node> childNodes;
		std::optional<std::shared_ptr<DOM::Document> > document;
	};
} // namespace DOM
