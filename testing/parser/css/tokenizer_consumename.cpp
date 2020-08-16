/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <random>
#include <variant>
#include <vector>

#include <cstring>

#include "data/text/encoding/utf8.hpp"
#include "data/text/ustring.hpp"

namespace CSS {

	class TokenizerConsumeStringToken : public ::testing::Test {
	public:
		Context context { &ParseErrorTester::ReporterEndpoint };
	};

} // namespace CSS
