#pragma once

#include "ILogger.h";
#include <fstream>;

class Logger :
	public ILogger
{
public:
	explicit Logger(std::string_view logFilename);
	virtual ~Logger();

	void setLogLevel(LogLevel level) override;
	void log(std::string_view message, LogLevel logLevel) override;
private:

	std::string_view getLogLevelString(LogLevel level)const;
	std::ofstream m_outputStream;
	LogLevel m_logLevel{ LogLevel::Error };
};

