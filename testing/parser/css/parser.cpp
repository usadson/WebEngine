/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <utility>

#include <gtest/gtest.h>

#include "data/text/ustring.hpp"
#include "parser/css/parser.hpp"
#include "parser/css/stylesheet.hpp"

namespace CSS {

	class ParserTest : public ::testing::Test {
	  public:
		template <typename T>
		[[nodiscard]] static Tokenizer
		Tokenize(const T &stringInput) noexcept {
			Context context;
			const auto &string = [&]() {
				if constexpr (std::is_same<T, Unicode::UString>::value)
					return stringInput;
				else {
					return Unicode::UString(stringInput);
				}
			}();
			Tokenizer tokenizer(context, string);
			EXPECT_TRUE(tokenizer.Run());
			return tokenizer;
		}
	};

	TEST_F(ParserTest, RunSimple) {
		const auto tokens = Tokenize("test{}");
		Parser parser(tokens);
		const auto result = parser.ParseStylesheet();
	}
} // namespace CSS

int
main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
