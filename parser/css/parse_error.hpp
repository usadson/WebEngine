#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

namespace CSS {

	struct ParseError {
		const char *name;
		const char *description;
		const char *standardModule;
		const char *standardModuleName;
		const char *url;
	};

} // namespace CSS::ParseError

namespace CSS::ParseErrors {

	constexpr ParseError EOFInConsumingComment{
		"eof-in-consuming-comment",
		"EOF found whilst consuming comment (i.e. the comment isn't ended correctly!",
		"4.3.2.",
		"Consume comments",
		"https://www.w3.org/TR/css-syntax-3/#consume-comment"
	};

} // namespace CSS::ParseErrors
