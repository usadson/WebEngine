#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <map>
#include <memory>

namespace HTML {
	class TreeConstructor;
}

#include "dom/element.hpp"
#include "parser/html/constants.hpp"
#include "parser/html/insertion_mode.hpp"
#include "parser/html/token.hpp"
#include "tree/insertion_mode.hpp"

namespace HTML {
	class TreeConstructor {
	public: // Properties
		Tokenizer::Context &context;
		InsertionModeType currentMode;
		std::vector<std::shared_ptr<DOM::Element>> openElementsStack;
		std::map<InsertionModeType, std::shared_ptr<InsertionMode>> insertionModes;

		bool executeScript { false };
		bool fosterParenting { false };

	public: // Methods
		explicit
		TreeConstructor(Tokenizer::Context &);

		void
		EmitCharacterToken(char);

		void
		EmitDoctypeQuirksToken();

		void
		EmitEOFToken();

		void
		EmitToken(HTML::Tokenizer::Token &);

		std::shared_ptr<DOM::Element>
		InsertElement(Unicode::UString tagName,
					  Unicode::UString nameSpace,
					  std::map<Unicode::UString, Unicode::UString> attributes);

		std::shared_ptr<DOM::Element>
		CreateElement(std::shared_ptr<DOM::Document> document,
					  Unicode::UString localName,
					  Unicode::UString nameSpace,
					  std::optional<Unicode::UString> prefix,
					  std::optional<Unicode::UString> is,
					  bool synchronousCustomElementsFlag
		);

		std::shared_ptr<DOM::Element>
		CreateElementForToken(HTML::Tokenizer::StartTagToken &,
							  Unicode::UString nameSpace,
							  std::shared_ptr<DOM::Node> intendedParent
		);

		void
		InsertForeignElement(Unicode::UString nameSpace,
							 std::shared_ptr<DOM::Node> parent);

		inline std::shared_ptr<DOM::Element>
		InsertHTMLElement(Unicode::UString tagName,
										   std::map<Unicode::UString, Unicode::UString> attributes) {
			return InsertElement(tagName, HTML::Constants::HTMLNamespace, attributes);
		}
	};
}
