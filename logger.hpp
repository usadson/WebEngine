#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <iostream>

#include <cstdlib>

namespace Logger {
	/**
	 * These logging functions are used like this:
	 * <logtype>(<source>, <message>)
	 *
	 * e.g.:
	 * Error("NetworkStack::Start", "Couldn't connect to https://example.org");
	 */

	extern bool shouldLog;

	void
	RunAbort();

	void
	SetOutputState(bool) noexcept;

	void
	SetAbortFunction(void (*)()) noexcept;

	template<typename SourceType, typename MessageType>
	void
	Error(const SourceType &source, const MessageType &message) noexcept {
		if (shouldLog) {
			std::cerr << "\033[1;31m[" << source << "] [ERROR] " << message << "\033[1;0m" << std::endl;
		}
	}

	template<typename SourceType, typename MessageType>
	void
	Debug(const SourceType &source, const MessageType &message) noexcept {
		if (shouldLog) {
			std::cout << "\033[1;35m[" << source << "] [DEBUG] " << message << "\033[1;0m" << std::endl;
		}
	}

	template<typename SourceType, typename MessageType>
	void
	Info(const SourceType &source, const MessageType &message) noexcept {
		if (shouldLog) {
			std::cout << '[' << source << "] [INFO] " << message << std::endl;
		}
	}

	template<typename SourceType, typename MessageType>
	void
	Severe(const SourceType &source, const MessageType &message) noexcept {
		if (shouldLog) {
			std::cerr << "\033[1;91m[" << source << "] [SEVERE] " << message << "\033[1;0m" << std::endl;
		}
	}

	#ifndef Success
	template<typename SourceType, typename MessageType>
	void
	Success(const SourceType &source, const MessageType &message) noexcept {
		if (shouldLog) {
			std::cout << "\033[1;32m[" << source << "] [SUCCESS] " << message << "\033[1;0m" << std::endl;
		}
	}
	#endif /* Success */

	template<typename SourceType, typename MessageType>
	void
	Warning(const SourceType &source, const MessageType &message) noexcept {
		if (shouldLog) {
			std::cout << "\033[1;33m[" << source << "] [WARNING] " << message << "\033[1;0m" << std::endl;
		}
	}

	template<typename SourceType, typename MessageType>
	void
	Crash(const SourceType &source, const MessageType &message) noexcept {
		if (shouldLog) {
			std::cout << "\033[1;33m[" << source << "] [CRASH] " << message << "\033[1;0m" << std::endl;
		}
		RunAbort();
	}

} // namespace Logger
