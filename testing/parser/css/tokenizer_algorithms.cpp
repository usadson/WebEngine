/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <gtest/gtest.h>

#include "data/text/unicode.hpp"
#include "parser/css/tokenizer_algoritms.hpp"
#include "parser/css/tokenizer_stream.hpp"

namespace CSS {

	class TokenizerAlgorithmsTest : public ::testing::Test {
	public:
		Unicode::CodePoint destination = 'A';
	};

} // namespace CSS

int
main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
