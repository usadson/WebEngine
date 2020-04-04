#pragma once

#include "data/text/ustring.hpp"
#include "node.hpp"

namespace DOM {
	class CharacterData : public Node {
	public: // Properties
		Unicode::UString Data;
	};
}
 
