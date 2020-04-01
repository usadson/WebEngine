#pragma once

#include <map>
#include <string>

// The strings supplied to this structure MUST be lowercased before storing.
namespace Resources {
	class MimeType {
	public:
		std::string Type;
		std::map<std::string, std::string> Parameters;
	};
}
