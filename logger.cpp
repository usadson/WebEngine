/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "logger.hpp"

#include <iostream>

#include <cstdlib>

void
Logger::Error(const std::string &source, const std::string &message) {
	std::cerr << "\033[1;31m[" << source << "] [ERROR] " << message << "\033[1;0m" << std::endl;
}

void
Logger::Debug(const std::string &source, const std::string &message) {
	std::cout << "\033[1;35m[" << source << "] [DEBUG] " << message << "\033[1;0m" << std::endl;
}

void
Logger::Info(const std::string &source, const std::string &message) {
	std::cout << '[' << source << "] [INFO] " << message << std::endl;
}

void
Logger::Severe(const std::string &source, const std::string &message) {
	std::cerr << "\033[1;91m[" << source << "] [SEVERE] " << message << "\033[1;0m" << std::endl;
}

void
Logger::SuccessLog(const std::string &source, const std::string &message) {
	std::cout << "\033[1;32m[" << source << "] [SUCCESS] " << message << "\033[1;0m" << std::endl;
}

void
Logger::Warning(const std::string &source, const std::string &message) {
	std::cout << "\033[1;33m[" << source << "] [WARNING] " << message << "\033[1;0m" << std::endl;
}

void
Logger::Crash(const std::string &source, const std::string &message) {
	std::cout << "\033[1;33m[" << source << "] [CRASH] " << message << "\033[1;0m" << std::endl;
	abort();
}
