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
	TokenizerStream::Next(Unicode::CodePoint *dest) noexcept {
		if (position >= string->length()) {
			return false;
		}

		if (dest != nullptr) {
			*dest = string->operator[](position++);
		}

		return true;
	}

	bool
	TokenizerStream::Peek(Unicode::CodePoint *dest, std::size_t offset) noexcept {
		if (position + offset >= string->length()) {
			return false;
		}

		if (dest) {
			*dest = string->operator[](position + offset);
		}

		return true;
	}

	void
	TokenizerStream::SetString(const Unicode::UString *string) noexcept {
		this->string = string;
		this->position = 0;
	}

} // namespace CSS
