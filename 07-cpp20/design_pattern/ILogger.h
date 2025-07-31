#pragma once
#include <string_view>

class ILogger
{
public:
	virtual ~ILogger() = default;

	enum class LogLevel
	{
		Error,
		Info,
		Debug
	};
	//set log level
	virtual void setLogLevel(LogLevel level) = 0;

	//log message
	virtual void log(std::string_view message, LogLevel logLevel) = 0;
};

