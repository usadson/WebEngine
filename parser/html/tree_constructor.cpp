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

#include "data/text/ustring.hpp"
#include "dom/comment.hpp"
#include "dom/resettable_element.hpp"
#include "logger.hpp"
#include "parser/html/context.hpp"
#include "parser/html/tree/insert_before_head.hpp"
#include "parser/html/tree/insert_before_html.hpp"
#include "parser/html/tree/insert_in_head.hpp"
#include "parser/html/tree/insert_initial.hpp"

namespace HTML {

	TreeConstructor::TreeConstructor(Tokenizer::Context &context)
		: context(context), currentMode(InsertionModeType::INITIAL),
		  insertionModes({
			  {InsertionModeType::INITIAL, std::make_shared<InsertionModes::Initial>(*this)},
			  {InsertionModeType::BEFORE_HTML, std::make_shared<InsertionModes::BeforeHTML>(*this)},
			  {InsertionModeType::BEFORE_HEAD, std::make_shared<InsertionModes::BeforeHead>(*this)},
			  {InsertionModeType::IN_HEAD, std::make_shared<InsertionModes::InHead>(*this)},
		  }) {
	}

	void
	TreeConstructor::EmitToken(HTML::Tokenizer::Token &inToken) {
		bool reprocess = false;
		std::size_t reprocessCount = 0;

		do {
			if (reprocessCount == 10) {
				std::stringstream info;
				info << "Reprocess loop detected! Reprocess requested 10 times! Quitting emission of token.";
				info << " InsertionMode: " << currentMode;
				Logger::Warning("TreeConstructor", info.str());
				return;
			}

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
	TreeConstructor::CreateElement(const Unicode::UString &localName, const Unicode::UString &nameSpace,
		std::optional<Unicode::UString> prefix, std::optional<Unicode::UString> is,
		bool synchronousCustomElementsFlag) {
		// TODO Check for custom element definition
		auto element = std::make_shared<DOM::Element>();
		element->namespaceURI = nameSpace;

		if (prefix.has_value()) {
			element->namespacePrefix = std::move(prefix.value());
		}

		element->localName = localName;
		element->customElementState = DOM::CustomElementState::UNCUSTOMIZED;

		if (is.has_value()) {
			element->is = std::move(is.value());
		}

		// TODO 7.3

		(void)synchronousCustomElementsFlag;

		return element;
	}

	/**
	 * Spec:
	 * https://html.spec.whatwg.org/multipage/parsing.html#create-an-element-for-the-token
	 */
	std::shared_ptr<DOM::Element>
	TreeConstructor::CreateElementForToken(
		HTML::Tokenizer::StartTagToken &tagToken, const Unicode::UString &nameSpace) {
		std::optional<Unicode::UString> is;

		auto attr = std::find_if(std::begin(tagToken.GetAttributes()), std::end(tagToken.GetAttributes()),
			[](const auto &attr) { return attr.first.EqualsIgnoreCaseA(2, "is"); });

		if (attr != std::end(tagToken.GetAttributes()))
			is = {attr->second};

		auto element = CreateElement(tagToken.tagName, nameSpace, {}, is, executeScript);
		/* Maybe, because the lifetime of the Token ends here, we can do a swap
		 * between the tokens attributes and the elements internalAttributes?
		 *
		 * Also, the appending of attributes to an element is not
		 * straightforward:
		 * https://dom.spec.whatwg.org/#concept-element-attributes-change-ext
		 */
		element->internalAttributes.insert(std::begin(tagToken.GetAttributes()), std::end(tagToken.GetAttributes()));
		/** TODO xmlns attribute check */

		auto resettableElement = std::dynamic_pointer_cast<DOM::ResettableElement>(element);
		if (resettableElement)
			resettableElement->reset();

		/** TODO Form-associated element */

		return element;
	}

	// Spec:
	// https://html.spec.whatwg.org/multipage/parsing.html#appropriate-place-for-inserting-a-node
	void
	TreeConstructor::InsertNodeInAppropriateLocation(
		const std::shared_ptr<DOM::Node> &node, std::optional<std::shared_ptr<DOM::Node>> overrideTarget) {
		/* TODO Foster parenting */

		if (overrideTarget.has_value()) {
			overrideTarget.value()->childNodes.push_back(node);
		} else if (!openElementsStack.empty()) {
			openElementsStack.back()->childNodes.push_back(node);
		} else {
			/* No nodes in the openElementsStack, thus use Document (this
			 * shouldn't be in the stack of open elements, states the standard implicitly:
			 * https://html.spec.whatwg.org/multipage/parsing.html#stack-of-open-elements
			 */
			context.parserContext.documentNode->childNodes.push_back(node);
		}
	}

	/**
	 * Spec:
	 * https://html.spec.whatwg.org/multipage/parsing.html#insert-a-foreign-element
	 */
	std::shared_ptr<DOM::Element>
	TreeConstructor::InsertForeignElement(HTML::Tokenizer::StartTagToken &token, const Unicode::UString &nameSpace) {
		auto element = CreateElementForToken(token, nameSpace);
		InsertNodeInAppropriateLocation(element);

		/* ... Element reactions ... */

		openElementsStack.push_back(element);
		return element;
	}

	std::shared_ptr<DOM::Element>
	TreeConstructor::InsertElement(const Unicode::UString &tagName, const Unicode::UString &nameSpace,
		const std::map<Unicode::UString, Unicode::UString> &attributes) {
		/* https://html.spec.whatwg.org/multipage/parsing.html#create-an-element-for-the-token */
		auto element = std::make_shared<DOM::Element>();
		element->namespaceURI = nameSpace;
		element->localName = tagName;
		context.parserContext.documentNode->children.push_back(element);
		openElementsStack.push_back(element);

		(void)attributes;

		return element;
	}

	/**
	 * Spec:
	 * https://html.spec.whatwg.org/multipage/parsing.html#insert-a-comment
	 */
	void
	TreeConstructor::InsertComment(Unicode::UString &contents) {
		InsertNodeInAppropriateLocation(std::make_shared<DOM::Comment>(contents));
	}

} // namespace HTML
