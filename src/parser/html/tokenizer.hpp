#pragma once

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
			HTML::Tokenizer::Context Context;
			HTML::TreeConstructor TreeConstructor;
		public: // Methods
			inline Tokenizer()
				: Context(), TreeConstructor(Context) {
			}

			void Run(Resources::DocumentResource &document);
		};
	}
}
