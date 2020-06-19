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
#include "dom/html_head_element.hpp"
#include "parser/html/constants.hpp"
#include "parser/html/insertion_mode.hpp"
#include "parser/html/token.hpp"
#include "tree/insertion_mode.hpp"

namespace HTML {
	class TreeConstructor {
	  public: // Properties
		Tokenizer::Context &context;
		InsertionModeType currentMode;
		std::vector<std::shared_ptr<DOM::Element> > openElementsStack;
		std::map<InsertionModeType, std::shared_ptr<InsertionMode> > insertionModes;

		bool executeScript{ false };
		bool fosterParenting{ false };

		std::shared_ptr<DOM::HTMLHeadElement> headElementPointer{ nullptr };

	  public: // Methods
		explicit TreeConstructor(Tokenizer::Context &);

		void
		EmitCharacterToken(char);

		void
		EmitDoctypeQuirksToken();

		void
		EmitEOFToken();

		void
		EmitToken(HTML::Tokenizer::Token &);

		std::shared_ptr<DOM::Element>
		InsertElement(const Unicode::UString &tagName,
					  const Unicode::UString &nameSpace,
					  std::map<Unicode::UString, Unicode::UString> &attributes);

		std::shared_ptr<DOM::Element>
		CreateElement(const Unicode::UString &localName,
					  const Unicode::UString &nameSpace,
					  std::optional<Unicode::UString> prefix,
					  std::optional<Unicode::UString> is,
					  bool synchronousCustomElementsFlag);

		void
		InsertComment(Unicode::UString &contents);

		std::shared_ptr<DOM::Element>
		CreateElementForToken(HTML::Tokenizer::StartTagToken &, const Unicode::UString &nameSpace);

		void
		InsertNodeInAppropriateLocation(std::shared_ptr<DOM::Node> node,
										std::optional<std::shared_ptr<DOM::Node> > overrideTarget = {});

		std::shared_ptr<DOM::Element>
		InsertForeignElement(HTML::Tokenizer::StartTagToken &, const Unicode::UString &nameSpace);

		inline std::shared_ptr<DOM::Element>
		InsertHTMLElement(HTML::Tokenizer::StartTagToken &token) {
			return InsertForeignElement(token, HTML::Constants::HTMLNamespace);
		}
	};
} // namespace HTML
