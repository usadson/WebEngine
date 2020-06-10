#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <memory>

#include "data/text/ustring.hpp"
#include "dom/node.hpp"

namespace DOM {
	// https://dom.spec.whatwg.org/#interface-documenttype
	class DocumentType : public Node {
	  public: // Properties
		const Unicode::UString name;
		const Unicode::UString publicId;
		const Unicode::UString systemId;

		DocumentType(const Unicode::UString &name,
					 const Unicode::UString &publicId,
					 const Unicode::UString &systemId)
			: name(name), publicId(publicId), systemId(systemId) {
		}
	};
} // namespace DOM
