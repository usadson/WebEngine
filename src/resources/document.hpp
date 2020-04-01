#pragma once

#include "data/text/encoding/utf8.hpp"
#include "data/text/ustring.hpp"
#include "resource.hpp"

namespace Resources {
	class DocumentResource : public Resource {
	public:
		Unicode::UString Data;
	};
}
