#include "http_response_info.hpp"

#include <sstream>

#include <cstring>

namespace Net {
	namespace HTTP {
		std::optional<const char *> HTTPResponseInfo::GetHeader(const char *name) const {
			for (const HTTPHeaderField &headerField : Headers) {
				if (strcasecmp(headerField.FieldName.c_str(), name) == 0)
					return std::optional<const char *>(headerField.FieldValue.c_str());
			}

			return std::optional<const char *>();
		}

		std::optional<size_t> HTTPResponseInfo::GetHeaderUnsigned(const char *name) const {
			size_t res;
			std::optional<const char *> value;

			value = GetHeader(name);
			if (value.has_value() && sscanf(value.value(), "%zu", &res) == 1)
				return res;
			
			return std::optional<size_t>();
		}
	}
}
