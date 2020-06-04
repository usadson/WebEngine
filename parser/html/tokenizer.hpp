#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <vector>

#include "resources/document.hpp"
#include "context.hpp"
#include "error.hpp"
#include "state.hpp"
#include "token.hpp"
#include "tree_constructor.hpp"

namespace HTML {
	namespace Tokenizer {
		class Tokenizer {
		public: // Properties
			HTML::Tokenizer::Context context;
			HTML::TreeConstructor treeConstructor;
		public: // Methods
			inline explicit
			Tokenizer(HTML::ParserContext &parserContext)
				: context(parserContext), treeConstructor(context) {
			}

			void
			Run(Resources::DocumentResource &);
		};
	}
}
