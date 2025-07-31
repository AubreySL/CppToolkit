#pragma once
#include "ILogger.h";
class UseLogger
{
public:
	explicit UseLogger(ILogger& logger) :m_logger{ logger }
	{
	}

	void doSomething() {
		m_logger.log("This is dependency injection.", ILogger::LogLevel::Info);
	}

private:
	ILogger& m_logger;
};

