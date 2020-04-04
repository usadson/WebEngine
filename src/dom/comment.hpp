#pragma once

#include "character_data.hpp"

namespace DOM {
	class Comment : public CharacterData {
	public: // Methods
		inline Comment() : CharacterData() {}
		inline Comment(Unicode::UString data) {
			Data = data;
		}
	};
}
 
 
