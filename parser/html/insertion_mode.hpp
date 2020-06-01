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

namespace HTML {
	enum class InsertionModeType {
		INITIAL,
		BEFORE_HTML,
		BEFORE_HEAD,
		IN_HEAD,
		IN_HEAD_NOSCRIPT,
		AFTER_HEAD,
		IN_BODY,
		TEXT,
		IN_TABLE,
		IN_TABLE_TEXT,
		IN_CAPTION,
		IN_COLUMN_GROUP,
		IN_TABLE_BODY,
		IN_ROW,
		IN_CELL,
		IN_SELECT,
		IN_SELECT_IN_TABLE,
		IN_TEMPLATE,
		AFTER_BODY,
		IN_FRAMESET,
		AFTER_FRAMESET,
		AFTER_AFTER_BODY,
		AFTER_AFTER_FRAMESET
	};

	extern std::map<InsertionModeType, std::string> InsertionModeTypeNames;

	inline std::ostream &
	operator<<(std::ostream &stream, InsertionModeType &state) { 
		return stream << InsertionModeTypeNames[state];
	}
} 
 
