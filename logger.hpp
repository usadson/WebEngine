#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <string>

namespace Logger {
	/**
	 * These logging functions are used like this:
	 * <logtype>(<source>, <message>)
	 *
	 * e.g.:
	 * Error("NetworkStack::Start", "Couldn't connect to https://example.org");
	 */

	void
	Error(const std::string &, const std::string &);

	void
	Debug(const std::string &, const std::string &);

	void
	Info(const std::string &, const std::string &);

	void
	Severe(const std::string &, const std::string &);

#ifndef Success
	void
	Success(const std::string &, const std::string &);
#endif /* Success */

	void
	Warning(const std::string &, const std::string &);

	void
	Crash(const std::string &, const std::string &);

} // namespace Logger
