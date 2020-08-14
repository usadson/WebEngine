/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <random>
#include <variant>
#include <vector>

#include "data/text/ustring.hpp"

namespace CSS {

	class TokenizerConsumeStringToken : public ::testing::Test {
	public:
		Context context { &ParseErrorTester::ReporterEndpoint };

		std::random_device randomDevice;
		std::mt19937 randomGenerator{ randomDevice() };
		std::uniform_int_distribution<Unicode::CodePoint> integerDistributor{ 0, Unicode::LAST_ALLOWED_CODE_POINT };

		[[nodiscard]] Unicode::CodePoint
		GetRandomCodePoint() noexcept {
			return integerDistributor(randomGenerator);
		}
	};

} // namespace CSS
