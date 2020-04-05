#include "logger.hpp"

#include <iostream>

void Logger::Error(std::string source, std::string message) {
	std::cout << "\033[1;31m[" << source << "] [ERROR] " << message << "\033[1;0m" << std::endl;
}

void Logger::Debug(std::string source, std::string message) {
	std::cout << "\033[1;35m[" << source << "] [DEBUG] " << message << "\033[1;0m" << std::endl;
}

void Logger::Info(std::string source, std::string message) {
	std::cout << '[' << source << "] [INFO] " << message << std::endl;
}

void Logger::Severe(std::string source, std::string message) {
	std::cout << "\033[1;91m[" << source << "] [SEVERE] " << message << "\033[1;0m" << std::endl;
}

void Logger::SuccessLog(std::string source, std::string message) {
	std::cout << "\033[1;32m[" << source << "] [SUCCESS] " << message << "\033[1;0m" << std::endl;
}

void Logger::Warning(std::string source, std::string message) {
	std::cout << "\033[1;33m[" << source << "] [WARNING] " << message << "\033[1;0m" << std::endl;
}
