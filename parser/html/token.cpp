/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "token.hpp"

#include <iostream>

#include "context.hpp"
#include "logger.hpp"

namespace HTML {
	namespace Tokenizer {
		const AmbiguousTagToken AmbiguousTagToken::INVALID_TYPE = AmbiguousTagToken(TokenType::INVALID);
		const CommentToken CommentToken::INVALID_TYPE = CommentToken(Unicode::UString("INVALID"));
		const EndTagToken EndTagToken::INVALID_TYPE_END_TAG = EndTagToken();
		const StartTagToken StartTagToken::INVALID_TYPE_START_TAG = StartTagToken();

		std::map<TokenType, std::string> tokenTypeNames = {{TokenType::CHARACTER, "CHARACTER"},
			{TokenType::COMMENT, "COMMENT"},
			{TokenType::DOCTYPE, "DOCTYPE"},
			{TokenType::ENDTAG, "ENDTAG"},
			{TokenType::EOF_TYPE, "EOF_TYPE"},
			{TokenType::INVALID, "EOF_TYPE"},
			{TokenType::STARTTAG, "STARTTAG"}};

		void
		AmbiguousTagToken::AddTokenAttribute(HTML::Tokenizer::Context &context) {
			if (attributeName.length() == 0) {
				Logger::Crash(__PRETTY_FUNCTION__, "0-length attributeName");
			}

			if (std::find_if(std::cbegin(attributes),
					std::cend(attributes),
					[this](const auto &attribute) { return attribute.first.EqualsIgnoreCase(attributeName); })
				!= std::cend(attributes)) {
				context.LogError(HTML::Tokenizer::ParserError::DUPLICATE_ATTRIBUTES);
				attributeName = Unicode::UString("");
				attributeValue = Unicode::UString("");
				return;
			}

			if (!attributes.insert(std::make_pair(attributeName, attributeValue)).second) {
				context.LogError(HTML::Tokenizer::ParserError::DUPLICATE_ATTRIBUTES);
				attributeName = Unicode::UString("");
				attributeValue = Unicode::UString("");
				return;
			}

			// Reset:
			attributeName = Unicode::UString("");
			attributeValue = Unicode::UString("");
		}

		std::optional<Unicode::UString>
		StartTagToken::GetAttribute(const Unicode::UString &name) {
			// Fast path since name cannot be empty, since adding it would crash the program
			if (name.length() == 0)
				return std::nullopt;

			auto attr = std::find_if(std::begin(attributes), std::end(attributes), [name](const auto &attribute) {
				return attribute.first.EqualsIgnoreCase(name);
			});

			if (attr == std::end(attributes))
				return std::nullopt;

			return {attr->second};
		}
	} // namespace Tokenizer
} // namespace HTML
