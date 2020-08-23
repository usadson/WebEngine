/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

namespace CSS {

	class TokenizerConsumeNumericToken : public ::testing::Test {
	public:
		Context context {&ParseErrorTester::ReporterEndpoint};
		Unicode::UString streamContents {};
		Tokenizer tokenizer {context, streamContents};
		const Unicode::UString initialString {};

		void
		Tokenize(Unicode::UString &&string) {
			streamContents = std::move(string);
			tokenizer.stream.SetString(&streamContents);
			ASSERT_TRUE(tokenizer.ConsumeNumericToken());
		}
	};

} // namespace CSS
