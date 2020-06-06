/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "tree_constructor.hpp"

#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#include "logger.hpp"
#include "data/text/ustring.hpp"
#include "dom/resettable_element.hpp"
#include "parser/html/context.hpp"
#include "parser/html/tree/insert_before_head.hpp"
#include "parser/html/tree/insert_before_html.hpp"
#include "parser/html/tree/insert_initial.hpp"

namespace HTML {

	TreeConstructor::TreeConstructor(Tokenizer::Context &context)
		: context(context), currentMode(InsertionModeType::INITIAL), insertionModes({
			{ InsertionModeType::INITIAL, std::make_shared<InsertionModes::Initial>(*this) },
			{ InsertionModeType::BEFORE_HTML, std::make_shared<InsertionModes::BeforeHTML>(*this) },
			{ InsertionModeType::BEFORE_HEAD, std::make_shared<InsertionModes::BeforeHead>(*this) },
		}) {
	}

	void
	TreeConstructor::EmitToken(HTML::Tokenizer::Token &inToken) {
		bool	reprocess = false;
		size_t	reprocessCount = 0;

		do {
			if (reprocessCount == 10) {
				std::stringstream info;
				info << "Reprocess loop detected! Reprocess requested 10 times! Quitting emission of token.";
				info << " InsertionMode: " << currentMode;
				Logger::Warning("TreeConstructor", info.str());
				return;
			}



			std::cout << "Mode is " << currentMode << " for type " << inToken.type();
			if (inToken.type() == Tokenizer::TokenType::STARTTAG)
				std::cout << " TagName='" << dynamic_cast<Tokenizer::StartTagToken *>(&inToken)->tagName << '\'';
			std::cout << '\n';



			auto iterator = insertionModes.find(currentMode);

			if (iterator == insertionModes.end()) {
				std::stringstream output;
				output << "Unknown insertion mode: \033[1;35m";
				output << currentMode;
				Logger::Warning("TreeConstructor", output.str());
				reprocess = false;
			} else
				reprocess = iterator->second->EmitToken(inToken);

			reprocessCount++;
		} while (reprocess);
	}

	void
	TreeConstructor::EmitCharacterToken(char character) {
		HTML::Tokenizer::CharacterToken token(character);
		EmitToken(token);
	}

	void
	TreeConstructor::EmitDoctypeQuirksToken() {
		HTML::Tokenizer::DoctypeToken token;
		token.forceQuirks = true;
		EmitToken(token);
	}

	void
	TreeConstructor::EmitEOFToken() {
		HTML::Tokenizer::EOFToken token;
		EmitToken(token);
	}

	/**
	 * Spec:
	 * https://dom.spec.whatwg.org/#concept-create-element
	 */
	std::shared_ptr<DOM::Element>
	TreeConstructor::CreateElement(std::shared_ptr<DOM::Document> document,
					Unicode::UString localName,
					Unicode::UString nameSpace,
					std::optional<Unicode::UString> prefix,
					std::optional<Unicode::UString> is,
					bool synchronousCustomElementsFlag) {
		// TODO Check for custom element definition
		auto element = std::make_shared<DOM::Element>();
		element->namespaceURI = nameSpace;
		element->namespacePrefix = prefix;
		element->localName = localName;
		element->customElementState = DOM::CustomElementState::UNCUSTOMIZED;
		element->is = is;
		element->document = document;

		// TODO 7.3

		return element;
	}

	/**
	 * Spec:
	 * https://html.spec.whatwg.org/multipage/parsing.html#create-an-element-for-the-token
	 */
	std::shared_ptr<DOM::Element>
	TreeConstructor::CreateElementForToken(
						  HTML::Tokenizer::StartTagToken &tagToken,
						  Unicode::UString nameSpace,
						  std::shared_ptr<DOM::Node> intendedParent) {
		std::optional<Unicode::UString> is;

		auto attr = std::find_if(std::begin(tagToken.attributes), std::end(tagToken.attributes),
								 [] (const auto &attr) {
									 return attr.first.EqualsIgnoreCaseA(2, "is");
								 });

		if (attr != std::end(tagToken.attributes))
			is = { attr->second };

		auto element = CreateElement(intendedParent->document.value(),
									 tagToken.tagName,
									 nameSpace,
									 {},
									 is,
									 executeScript);
		/* Maybe, because the lifetime of the Token ends here, we can do a swap
		 * between the tokens attributes and the elements internalAttributes?
		 *
		 * Also, the appending of attributes to an element is not
		 * straightforward:
		 * https://dom.spec.whatwg.org/#concept-element-attributes-change-ext
		 */
		element->internalAttributes.insert(std::begin(tagToken.attributes),
										   std::end(tagToken.attributes));
		/** TODO xmlns attribute check */

		auto resettableElement = std::dynamic_pointer_cast<DOM::ResettableElement>(element);
		if (resettableElement)
			resettableElement->reset();

		return element;
	}

	std::shared_ptr<DOM::Element>
	TreeConstructor::InsertElement(Unicode::UString tagName, Unicode::UString nameSpace, std::map<Unicode::UString, Unicode::UString> attributes) {
		/* https://html.spec.whatwg.org/multipage/parsing.html#create-an-element-for-the-token */
		auto element = std::make_shared<DOM::Element>();
		element->namespaceURI = std::move(nameSpace);
		element->localName = std::move(tagName);
		element->document = context.parserContext.documentNode;
		context.parserContext.documentNode->children.push_back(element);
		openElementsStack.push_back(element);

		return element;
	}
}
