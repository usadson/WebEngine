/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "logger.hpp"
static void (*abortFunction)() = &std::terminate;

void
Logger::RunAbort() {
	abortFunction();
}

void
Logger::SetAbortFunction(void (*function)()) noexcept {
	abortFunction = function;
}

void
Logger::SetOutputState(bool state) noexcept {
	shouldLog = state;
}
