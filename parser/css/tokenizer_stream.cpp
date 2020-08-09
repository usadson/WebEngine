/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "parser/css/tokenizer_stream.hpp"

#include "data/text/ustring.hpp"

namespace CSS {

	TokenizerStream::TokenizerStream(const Unicode::UString *string) noexcept : string(string) {
	}

	bool
	TokenizerStream::Next(char *dest) noexcept {
		if (position >= string->length()) {
			return false;
		}

		*dest = string->operator[](position++);
		return true;
	}

	bool
	TokenizerStream::Peek(char *dest, std::size_t offset) noexcept {
		if (position + offset >= string->length()) {
			return false;
		}

		*dest = string->operator[](position + offset);
		return true;
	}

	void
	TokenizerStream::SetString(const Unicode::UString *string) noexcept {
		this->string = string;
		this->position = 0;
	}

} // namespace CSS
