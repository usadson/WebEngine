#pragma once

#include <string>

namespace Logger {
	void Error(std::string source, std::string message);
	void Debug(std::string source, std::string message);
	void Info(std::string source, std::string message);
	void Severe(std::string source, std::string message);
	void SuccessLog(std::string source, std::string message);
	void Warning(std::string source, std::string message);
}
