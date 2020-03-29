#pragma once

#include <map>
#include <string>

namespace Options {
	enum class Type {
		TLS_SECURITY_LEVEL,
	};

	/* Settings should be verified before updating this map. */
	static std::map<Type, std::string> Values = {
		{ Type::TLS_SECURITY_LEVEL, "secure" },
	};

	inline std::string &Get(Type type) {
		return Values[type];
	}
}
