/**
 * Copyright (C) 2020 Tristan <tristan@thewoosh.me>
 *
 * All Rights Reserved.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
 */

#pragma once

#include <map>
#include <memory>

namespace HTML {
	class TreeConstructor;
}

#include "parser/html/insertion_mode.hpp"
#include "tree/insertion_mode.hpp"
#include "token.hpp"

namespace HTML {
	class TreeConstructor {
	public: // Properties
		Tokenizer::Context &Context;
		InsertionModeType CurrentMode;
	private: // Private Properties
		std::map<InsertionModeType, std::shared_ptr<InsertionMode>> InsertionModes;
	public: // Methods
		TreeConstructor(Tokenizer::Context &context);

		void EmitCharacterToken(char character);
		void EmitDoctypeQuirksToken(void);
		void EmitEOFToken(void);
		void EmitToken(HTML::Tokenizer::Token &token);
	};
};
 
