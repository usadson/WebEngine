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

#include "logger.hpp"

#include <iostream>

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
Logger::Severe(const std::string & source, const std::string &message) {
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
