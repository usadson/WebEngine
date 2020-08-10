/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "tokenizer.hpp"

#include <iostream>
#include <iterator>
#include <vector>

#include "data/text/unicode.hpp"
#include "data/text/ustring.hpp"

namespace CSS {

	bool
	Tokenizer::ConsumeComments() noexcept {
		std::vector<Unicode::CodePoint> data(std::cbegin(inputString), std::cend(inputString));
		std::vector<Unicode::CodePoint>::iterator commentStart = std::end(data);
		bool inComment = false;

		for (auto iterator = std::begin(data); iterator < std::end(data); ++iterator) {
			if (inComment) {
				if (iterator + 1 != std::end(data) && *iterator == '*' && *(iterator + 1) == '/') {
					inComment = false;
					iterator++;								// Consume '/' aswell
					data.erase(commentStart, iterator + 1); // +1 because std::vector<T>::erase is exclusive
					iterator = commentStart;
				}
			} else if (iterator + 1 != std::end(data) && *iterator == '/' && *(iterator + 1) == '*') {
				inComment = true;
				commentStart = iterator;
				iterator++; // Consume '*' aswell
			}
		}

		if (inComment) {
			context.ReportParseError(CSS::ParseError::EOF_IN_CONSUMING_COMMENT);
			return false;
		}

		string = Unicode::UString(std::move(data));
		stream.SetString(&string);
		return true;
	}

	bool
	Tokenizer::ConsumeToken() noexcept {
		return true;
	}

	bool
	Tokenizer::Run() noexcept {
		if (!ConsumeComments()) {
			return false;
		}

		while (stream.Peek(nullptr)) {
			if (!ConsumeToken()) {
				return false;
			}
		}

		return true;
	}

} // namespace CSS
