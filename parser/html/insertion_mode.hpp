#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

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

	extern std::map<InsertionModeType, std::string> insertionModeTypeNames;

	inline std::ostream &
	operator<<(std::ostream &stream, InsertionModeType &state) {
		return stream << insertionModeTypeNames[state];
	}
}
