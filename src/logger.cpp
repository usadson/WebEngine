/**
 * Copyright (C) 2020 Tristan <tristan@thewoosh.me>
 *
 * All Rights Reserved.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
 */

#include "logger.hpp"

#include <iostream>

void
Logger::Error(std::string source, std::string message) {
	std::cout << "\033[1;31m[" << source << "] [ERROR] " << message << "\033[1;0m" << std::endl;
}

void
Logger::Debug(std::string source, std::string message) {
	std::cout << "\033[1;35m[" << source << "] [DEBUG] " << message << "\033[1;0m" << std::endl;
}

void
Logger::Info(std::string source, std::string message) {
	std::cout << '[' << source << "] [INFO] " << message << std::endl;
}

void
Logger::Severe(std::string source, std::string message) {
	std::cout << "\033[1;91m[" << source << "] [SEVERE] " << message << "\033[1;0m" << std::endl;
}

void
Logger::SuccessLog(std::string source, std::string message) {
	std::cout << "\033[1;32m[" << source << "] [SUCCESS] " << message << "\033[1;0m" << std::endl;
}

void
Logger::Warning(std::string source, std::string message) {
	std::cout << "\033[1;33m[" << source << "] [WARNING] " << message << "\033[1;0m" << std::endl;
}
