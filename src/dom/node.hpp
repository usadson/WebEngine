#pragma once

#include <functional>
#include <optional>
#include <vector>

namespace DOM {
	class Node;
};

namespace DOM {
	class Document;

	class Node {
	public: // Properties
		std::vector<Node> ChildNodes;
		std::optional<std::reference_wrapper<DOM::Document>> Document;
	};
}
