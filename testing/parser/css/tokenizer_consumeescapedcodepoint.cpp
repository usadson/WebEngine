/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

namespace CSS {

	class TokenizerConsumeEscapedCodePointTest : public ::testing::Test {
	public:
		Context context { &ParseErrorTester::ReporterEndpoint };
	};

} // namespace CSS
