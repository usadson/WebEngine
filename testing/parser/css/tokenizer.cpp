/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

// This file includes all the tokenizer subfunctions

#include <climits>
#include <cstdlib>

#include <gtest/gtest.h>

#define PRIVATE_VISIBILITY public

#include "data/text/ustring.hpp"
#include "logger.hpp"
#include "parser/css/tokenizer.hpp"

namespace ParseErrorTester {

	CSS::ParseError lastParseError = CSS::ParseError::INVALID;

	void
	ReporterEndpoint(CSS::ParseError error) noexcept {
		lastParseError = error;
	}

	bool
	WasParseErrorFired() noexcept {
		bool was = lastParseError != CSS::ParseError::INVALID;
		lastParseError = CSS::ParseError::INVALID;
		return was;
	}

	bool
	WasParseErrorFired(CSS::ParseError error) noexcept {
		auto lastErrorCopy = lastParseError;
		lastParseError = CSS::ParseError::INVALID;

		if (lastErrorCopy == error) {
			return true;
		} else if (lastErrorCopy == CSS::ParseError::INVALID) {
			Logger::Info("WasParseErrorFired", "There wasn't a parse error fired, but was expected.");
		} else {
			Logger::Info("WasParseErrorFired", "There was a different parse error fired.");
		}
		return false;
	}

} // namespace ParseErrorTester

// Make sure to also add them to the ingredients in the Makefile!
#include "tokenizer_consumecomments.cpp"
#include "tokenizer_consumeescapedcodepoint.cpp"
#include "tokenizer_consumeidentliketoken.cpp"
#include "tokenizer_consumename.cpp"
#include "tokenizer_consumenumber.cpp"
#include "tokenizer_consumenumerictoken.cpp"
#include "tokenizer_consumeremnantsofbadurl.cpp"
#include "tokenizer_consumestringtoken.cpp"
#include "tokenizer_consumetoken.cpp"
#include "tokenizer_consumeurltoken.cpp"
#include "tokenizer_convertstringtonumber.cpp"

namespace CSS {

	class TokenizerTest : public ::testing::Test {
	public:
		Context context {&ParseErrorTester::ReporterEndpoint};

		void
		TestIdent(const Token &token, const std::vector<Unicode::CodePoint> &expected) {
			ASSERT_EQ(token.type, TokenType::IDENT);
			const auto *data = std::get_if<TokenCodePointsData>(&token.data);
			ASSERT_NE(data, nullptr);
			EXPECT_EQ(data->codePoints, expected);
		}

		void
		TestHash(const Token &token, const std::vector<Unicode::CodePoint> &expected) {
			ASSERT_EQ(token.type, TokenType::HASH);
			const auto *data = std::get_if<TokenHashData>(&token.data);
			ASSERT_NE(data, nullptr);
			// TODO test TokenHashType?
			EXPECT_EQ(data->codePoints, expected);
		}

		void
		TestAtKeyword(const Token &token, const std::vector<Unicode::CodePoint> &expected) {
			ASSERT_EQ(token.type, TokenType::AT_KEYWORD);
			const auto *data = std::get_if<TokenCodePointsData>(&token.data);
			ASSERT_NE(data, nullptr);
			EXPECT_EQ(data->codePoints, expected);
		}

		void
		TestURL(const Token &token, const std::vector<Unicode::CodePoint> &expected) {
			ASSERT_EQ(token.type, TokenType::URL);
			const auto *data = std::get_if<TokenCodePointsData>(&token.data);
			ASSERT_NE(data, nullptr);
			EXPECT_EQ(data->codePoints, expected);
		}

		void
		TestFunction(const Token &token, const std::vector<Unicode::CodePoint> &expected) {
			ASSERT_EQ(token.type, TokenType::FUNCTION);
			const auto *data = std::get_if<TokenCodePointsData>(&token.data);
			ASSERT_NE(data, nullptr);
			EXPECT_EQ(data->codePoints, expected);
		}

		template <typename T>
		void
		TestDimension(const Token &token, const T &expectedNumericValue,
			const std::vector<Unicode::CodePoint> &expectedCodePoints) {
			ASSERT_EQ(token.type, TokenType::DIMENSION);
			const auto *data = std::get_if<TokenDimensionData>(&token.data);
			ASSERT_NE(data, nullptr);
			EXPECT_EQ(data->codePoints, expectedCodePoints);
			if constexpr (std::is_same<T, CSS::IntegerType>::value) {
				ASSERT_EQ(data->type, TokenNumericType::INTEGER);
				EXPECT_EQ(data->integer, expectedNumericValue);
			} else {
				ASSERT_EQ(data->type, TokenNumericType::NUMBER);
				EXPECT_EQ(data->number, expectedNumericValue);
			}
		}

		template <typename T>
		void
		TestNumber(const Token &token, const T &expectedNumericValue) {
			ASSERT_EQ(token.type, TokenType::NUMBER);
			const auto *data = std::get_if<TokenNumericData>(&token.data);
			ASSERT_NE(data, nullptr);
			if constexpr (std::is_same<T, CSS::IntegerType>::value) {
				ASSERT_EQ(data->type, TokenNumericType::INTEGER);
				EXPECT_EQ(data->integer, expectedNumericValue);
			} else {
				ASSERT_EQ(data->type, TokenNumericType::NUMBER);
				EXPECT_EQ(data->number, expectedNumericValue);
			}
		}
	};

	TEST_F(TokenizerTest, RuleTest) {
		const Unicode::UString input("width: 100px;");
		Tokenizer tokenizer(context, input);
		EXPECT_TRUE(tokenizer.Run());
		ASSERT_EQ(tokenizer.tokens.size(), 5);
		TestIdent(tokenizer.tokens[0], {'w', 'i', 'd', 't', 'h'});
		EXPECT_EQ(tokenizer.tokens[1].type, TokenType::COLON);
		EXPECT_EQ(tokenizer.tokens[2].type, TokenType::WHITESPACE);
		TestDimension<CSS::IntegerType>(tokenizer.tokens[3], 100, {'p', 'x'});
		EXPECT_EQ(tokenizer.tokens[4].type, TokenType::SEMICOLON);
	}

	TEST_F(TokenizerTest, DeclarationBlockTest) {
		const Unicode::UString input("\
p#test {\n\
color: red;\n\
}");
		Tokenizer tokenizer(context, input);
		EXPECT_TRUE(tokenizer.Run());
		ASSERT_EQ(tokenizer.tokens.size(), 12);
		TestIdent(tokenizer.tokens[0], {'p'});
		TestHash(tokenizer.tokens[1], {'t', 'e', 's', 't'});
		EXPECT_EQ(tokenizer.tokens[2].type, TokenType::WHITESPACE);
		EXPECT_EQ(tokenizer.tokens[3].type, TokenType::CURLY_OPEN);
		EXPECT_EQ(tokenizer.tokens[4].type, TokenType::WHITESPACE);
		TestIdent(tokenizer.tokens[5], {'c', 'o', 'l', 'o', 'r'});
		EXPECT_EQ(tokenizer.tokens[6].type, TokenType::COLON);
		EXPECT_EQ(tokenizer.tokens[7].type, TokenType::WHITESPACE);
		TestIdent(tokenizer.tokens[8], {'r', 'e', 'd'});
		EXPECT_EQ(tokenizer.tokens[9].type, TokenType::SEMICOLON);
		EXPECT_EQ(tokenizer.tokens[10].type, TokenType::WHITESPACE);
		EXPECT_EQ(tokenizer.tokens[11].type, TokenType::CURLY_CLOSE);
	}

	TEST_F(TokenizerTest, ImportURLTest) {
		const Unicode::UString input("@import url(test.css);");
		Tokenizer tokenizer(context, input);
		EXPECT_TRUE(tokenizer.Run());
		ASSERT_EQ(tokenizer.tokens.size(), 4);
		TestAtKeyword(tokenizer.tokens[0], {'i', 'm', 'p', 'o', 'r', 't'});
		EXPECT_EQ(tokenizer.tokens[1].type, TokenType::WHITESPACE);
		TestURL(tokenizer.tokens[2], {'t', 'e', 's', 't', '.', 'c', 's', 's'});
		EXPECT_EQ(tokenizer.tokens[3].type, TokenType::SEMICOLON);
	}

	TEST_F(TokenizerTest, ImportBadURLTest) {
		const Unicode::UString input("@import url(b(a);");
		Tokenizer tokenizer(context, input);
		EXPECT_TRUE(tokenizer.Run());
		ASSERT_EQ(tokenizer.tokens.size(), 4);
		TestAtKeyword(tokenizer.tokens[0], {'i', 'm', 'p', 'o', 'r', 't'});
		EXPECT_EQ(tokenizer.tokens[1].type, TokenType::WHITESPACE);
		EXPECT_EQ(tokenizer.tokens[2].type, TokenType::BAD_URL);
		EXPECT_EQ(tokenizer.tokens[3].type, TokenType::SEMICOLON);
	}

} // namespace CSS

int
main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
