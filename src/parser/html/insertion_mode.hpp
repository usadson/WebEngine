#pragma once

#include <map>

namespace HTML {
	namespace Tokenizer {
		enum class InsertionMode {
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

		static std::map<InsertionMode, std::string> InsertionModeNames = {
			{ INITIAL, "INITIAL" },
			{ BEFORE_HTML, "BEFORE_HTML" },
			{ BEFORE_HEAD, "BEFORE_HEAD" },
			{ IN_HEAD, "IN_HEAD" },
			{ IN_HEAD_NOSCRIPT, "IN_HEAD_NOSCRIPT" },
			{ AFTER_HEAD, "AFTER_HEAD" },
			{ IN_BODY, "IN_BODY" },
			{ TEXT, "TEXT" },
			{ IN_TABLE, "IN_TABLE" },
			{ IN_TABLE_TEXT, "IN_TABLE_TEXT" },
			{ IN_CAPTION, "IN_CAPTION" },
			{ IN_COLUMN_GROUP, "IN_COLUMN_GROUP" },
			{ IN_TABLE_BODY, "IN_TABLE_BODY" },
			{ IN_ROW, "IN_ROW" },
			{ IN_CELL, "IN_CELL" },
			{ IN_SELECT, "IN_SELECT" },
			{ IN_SELECT_IN_TABLE, "IN_SELECT_IN_TABLE" },
			{ IN_TEMPLATE, "IN_TEMPLATE" },
			{ AFTER_BODY, "AFTER_BODY" },
			{ IN_FRAMESET, "IN_FRAMESET" },
			{ AFTER_FRAMESET, "AFTER_FRAMESET" },
			{ AFTER_AFTER_BODY, "AFTER_AFTER_BODY" },
			{ AFTER_AFTER_FRAMESET, "AFTER_AFTER_FRAMESET" }
		};

		inline std::ostream &operator<<(std::ostream &stream, InsertionMode &state) { 
			return stream << ParserStateNames[state].Name;
		}
	};
} 
 
