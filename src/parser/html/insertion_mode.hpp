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

	static std::map<InsertionModeType, std::string> InsertionModeTypeNames = {
		{ InsertionModeType::INITIAL, "INITIAL" },
		{ InsertionModeType::BEFORE_HTML, "BEFORE_HTML" },
		{ InsertionModeType::BEFORE_HEAD, "BEFORE_HEAD" },
		{ InsertionModeType::IN_HEAD, "IN_HEAD" },
		{ InsertionModeType::IN_HEAD_NOSCRIPT, "IN_HEAD_NOSCRIPT" },
		{ InsertionModeType::AFTER_HEAD, "AFTER_HEAD" },
		{ InsertionModeType::IN_BODY, "IN_BODY" },
		{ InsertionModeType::TEXT, "TEXT" },
		{ InsertionModeType::IN_TABLE, "IN_TABLE" },
		{ InsertionModeType::IN_TABLE_TEXT, "IN_TABLE_TEXT" },
		{ InsertionModeType::IN_CAPTION, "IN_CAPTION" },
		{ InsertionModeType::IN_COLUMN_GROUP, "IN_COLUMN_GROUP" },
		{ InsertionModeType::IN_TABLE_BODY, "IN_TABLE_BODY" },
		{ InsertionModeType::IN_ROW, "IN_ROW" },
		{ InsertionModeType::IN_CELL, "IN_CELL" },
		{ InsertionModeType::IN_SELECT, "IN_SELECT" },
		{ InsertionModeType::IN_SELECT_IN_TABLE, "IN_SELECT_IN_TABLE" },
		{ InsertionModeType::IN_TEMPLATE, "IN_TEMPLATE" },
		{ InsertionModeType::AFTER_BODY, "AFTER_BODY" },
		{ InsertionModeType::IN_FRAMESET, "IN_FRAMESET" },
		{ InsertionModeType::AFTER_FRAMESET, "AFTER_FRAMESET" },
		{ InsertionModeType::AFTER_AFTER_BODY, "AFTER_AFTER_BODY" },
		{ InsertionModeType::AFTER_AFTER_FRAMESET, "AFTER_AFTER_FRAMESET" }
	};

	inline std::ostream &operator<<(std::ostream &stream, InsertionModeType &state) { 
		return stream << InsertionModeTypeNames[state];
	}
} 
 
