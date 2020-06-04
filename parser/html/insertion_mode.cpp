/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "insertion_mode.hpp"

namespace HTML {
	std::map<InsertionModeType, std::string> insertionModeTypeNames = {
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
}
