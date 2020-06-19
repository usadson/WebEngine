/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "parser/html/token.hpp"
#include "data/text/ustring.hpp"
#include "parser/html/context.hpp"
#include "parser/html/error.hpp"

#include <climits>

#include <gtest/gtest.h>

namespace HTML::Tokenizer {
	class ContextFixture : public Context {
	public:
		std::reference_wrapper<const ParserError> lastError = ParserError::NULL_PARSER_ERROR;

		explicit
		ContextFixture(ParserContext &parserContext) : Context(parserContext) {
		}

		void
		LogError(const ParserError &error) override {
			lastError = error;
		}
	};

	class StartTagTokenTest : public ::testing::Test {
	protected:
		StartTagToken token;
		ParserContext parserContext;
		ContextFixture context { ContextFixture(parserContext) };
	};

	TEST_F(StartTagTokenTest, DuplicateAttributes) {
		ASSERT_EQ(context.lastError.get().name, ParserError::NULL_PARSER_ERROR.name);

		token.attributeName = Unicode::UString("hello");
		token.attributeValue = Unicode::UString("world");
		token.AddTokenAttribute(context);
		ASSERT_EQ(context.lastError.get().name, ParserError::NULL_PARSER_ERROR.name);

		token.attributeName = Unicode::UString("hello");
		token.attributeValue = Unicode::UString("world");
		token.AddTokenAttribute(context);
		ASSERT_EQ(context.lastError.get().name, ParserError::DUPLICATE_ATTRIBUTES.name);

		// A different attribute value
		context.lastError = ParserError::NULL_PARSER_ERROR;

		token.attributeName = Unicode::UString("hello");
		token.attributeValue = Unicode::UString("another");
		token.AddTokenAttribute(context);
		ASSERT_EQ(context.lastError.get().name, ParserError::DUPLICATE_ATTRIBUTES.name);
	}

	TEST_F(StartTagTokenTest, GetAttribute) {
		context.lastError = ParserError::NULL_PARSER_ERROR;
		token.attributes.clear();

		token.attributeName = Unicode::UString("hello");
		token.attributeValue = Unicode::UString("world");

		token.AddTokenAttribute(context);
		ASSERT_EQ(context.lastError.get().name, ParserError::NULL_PARSER_ERROR.name);

		ASSERT_EQ(token.attributes.size(), 1);
		std::optional<Unicode::UString> value = token.GetAttribute(Unicode::UString("hello"));
		ASSERT_EQ(token.attributes.size(), 1);

		ASSERT_TRUE(value.has_value());
		ASSERT_EQ(value, Unicode::UString("world"));

		ASSERT_FALSE(token.GetAttribute(Unicode::UString("this key does not exists in the attribute map")).has_value());
	}

} // namespace HTML::Tokenizer

int
main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
