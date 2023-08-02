#include "Logger.h"
#include <iostream>

namespace kiko {
	Logger g_logger(LogLevel::Info, &std::cout, "log.txt");

	bool Logger::Log(LogLevel logLevel, const std::string& filename, int line) {
		if (logLevel < m_logLevel) return false;

		switch (logLevel)
		{
		case LogLevel::Info:
			*this << "INFO : ";
			break;
		case LogLevel::Warning:
			*this << "WARNING : ";
			break;
		case LogLevel::Error:
			*this << "ERROR : ";
			break;
		case LogLevel::Assert:
			*this << "ASSERT : ";
			break;
		default:
			break;
		}

		*m_ostream << filename << "(" << line << ") ";

		return true;
	}
}