#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "data/text/unicode.hpp"
#include "data/text/ustring.hpp"
#include "parser/css/context.hpp"
#include "parser/css/parse_error.hpp"
#include "parser/css/token.hpp"
#include "parser/css/tokenizer_stream.hpp"

#ifndef PRIVATE_VISIBILITY
#define PRIVATE_VISIBILITY private
#endif

namespace CSS {
	class Tokenizer {
	  public:
		inline
		Tokenizer(CSS::Context &context, const Unicode::UString &input) :
			context(context), inputString(input), stream(&inputString) {
		}

		[[nodiscard]] bool
		Run() noexcept;

	  PRIVATE_VISIBILITY:
		CSS::Context &context;
		const Unicode::UString &inputString;
		TokenizerStream stream;
		Unicode::UString string;
		std::vector<Token> tokens{};

		// The 'consume comments' tokenizer algorithm.
		//
		// [Spec]
		// Title: CSS Syntax Module Level 3
		// Section: 4.3.2.
		// URL: https://www.w3.org/TR/css-syntax-3/#consume-comment
		[[nodiscard]] bool
		ConsumeComments() noexcept;

		// The 'consume an escaped code point' tokenizer algorithm.
		//
		// [Spec]
		// Title: CSS Syntax Module Level 3
		// Section: 4.3.7.
		// URL: https://www.w3.org/TR/css-syntax-3/#consume-escaped-code-point
		[[nodiscard]] Unicode::CodePoint
		ConsumeEscapedCodePoint() noexcept;

		// The 'consume a name' tokenizer algorithm.
		//
		// [Spec]
		// Title: CSS Syntax Module Level 3
		// Section: 4.3.11.
		// URL: https://www.w3.org/TR/css-syntax-3/#consume-a-name
		bool
		ConsumeName(std::vector<Unicode::CodePoint> &) noexcept;

		// The 'consume a number' tokenizer algorithm.
		//
		// [Spec]
		// Title: CSS Syntax Module Level 3
		// Section: 4.3.12.
		// URL: https://www.w3.org/TR/css-syntax-3/#consume-a-number
		[[nodiscard]] std::variant<std::monostate, std::int64_t, double>
		ConsumeNumber() noexcept;

		// The 'consume a numeric token' tokenizer algorithm.
		//
		// [Spec]
		// Title: CSS Syntax Module Level 3
		// Section: 4.3.3.
		// URL: https://www.w3.org/TR/css-syntax-3/#consume-a-numeric-token
		[[nodiscard]] bool
		ConsumeNumericToken() noexcept;

		// The 'consume a string token' tokenizer algorithm.
		//
		// [Spec]
		// Title: CSS Syntax Module Level 3
		// Section: 4.3.5.
		// URL: https://www.w3.org/TR/css-syntax-3/#consume-a-string-token
		[[nodiscard]] bool
		ConsumeStringToken(Unicode::CodePoint endingCodePoint) noexcept;

		// The 'consume a token' tokenizer algorithm.
		//
		// [Spec]
		// Title: CSS Syntax Module Level 3
		// Section: 4.3.1.
		// URL: https://www.w3.org/TR/css-syntax-3/#consume-token
		[[nodiscard]] bool
		ConsumeToken(Unicode::CodePoint character) noexcept;

		// The 'convert a string to a number' tokenizer algorithm.
		//
		// [Spec]
		// Title: CSS Syntax Module Level 3
		// Section: 4.3.13.
		// URL: https://www.w3.org/TR/css-syntax-3/#convert-string-to-number
		[[nodiscard]] std::variant<std::monostate, std::int64_t, double>
		ConvertStringToNumber(const std::vector<Unicode::CodePoint> &) noexcept;

		// Will try to run the algorithm of ConsumeToken when the code point is
		// NUMBER SIGN. Will return false if the if-statement is failing,
		// otherwise the algorithm will be executed.
		//
		// [Spec] (Subsection)
		// Title: CSS Syntax Module Level 3
		// Section: 4.3.1.
		// Case: U+0023 NUMBER SIGN (#)
		// URL: https://www.w3.org/TR/css-syntax-3/#consume-token
		[[nodiscard]] bool
		TryParseHashTokenName() noexcept;
	};
} // namespace CSS

