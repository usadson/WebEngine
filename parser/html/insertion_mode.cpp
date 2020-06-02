/**
 * Copyright (C) 2020 Tristan
 *
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
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
