#pragma once

#include "node.hpp"

namespace DOM {
	enum class QuirksMode {
			LIMITED_QUIRKS,
			NO_QUIRKS,
			QUIRKS
	};

	class Document : public Node {
	public: // Properties
		QuirksMode Mode;
	};
}
