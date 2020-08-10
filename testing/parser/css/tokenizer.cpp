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

int
main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
