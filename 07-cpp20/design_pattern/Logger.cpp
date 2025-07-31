#include "Logger.h"
#include <format>
Logger::Logger(std::string_view logFilename)
{
	m_outputStream.open(logFilename.data(), std::ios_base::app);
	if (!m_outputStream.good()) {
		throw std::runtime_error{ "Unable to initialize the Logger." };
	}
}

Logger::~Logger()
{
	m_outputStream << "Logger shutting down." << std::endl;
	m_outputStream.close();
}

void Logger::setLogLevel(LogLevel level)
{
	m_logLevel = level;
}

void Logger::log(std::string_view message, LogLevel logLevel)
{
	if (m_logLevel < logLevel) { return; }
	m_outputStream << std::format("{}:{}", getLogLevelString(logLevel), message) << std::endl;
}

std::string_view Logger::getLogLevelString(LogLevel level) const
{
	switch (level)
	{
	case ILogger::LogLevel::Error:
		return "ERROR";
	case ILogger::LogLevel::Info:
		return "INFO";
	case ILogger::LogLevel::Debug:
		return "DEBUG";
	}
	throw std::runtime_error{ "Invalid log level." };
}



