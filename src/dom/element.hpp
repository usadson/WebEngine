#pragma once

#include <map>

#include "data/text/ustring.hpp"
#include "node.hpp"

namespace DOM {
	class Element : public Node {
	public: // Internal Properties
		std::map<Unicode::UString, Unicode::UString> InternalAttributes;
	public: // DOM Attributes
		/* Readonly */
		std::optional<Unicode::UString> NamespaceURI;
		std::optional<Unicode::UString> Prefix;
		Unicode::UString LocalName;
		Unicode::UString TagName;

		Unicode::UString Id;
		Unicode::UString ClassName;
	};
}
