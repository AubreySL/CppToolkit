export module Logger;
import <string>;
import <iostream>;
import <format>;

export class Logger {
public:
	enum class LogLevel
	{
		Error,
		Info,
		Debug
	};
	Logger();
	virtual ~Logger() = default;
	void log(LogLevel level, const std::string& message);
private:
	std::string_view getLogLevelString(LogLevel level)const;
};

Logger::Logger() {
	std::cout << "Logger Constructor" << std::endl;
}
void Logger::log(LogLevel level, const std::string& message) {
	std::cout << std::format("{}:{}", getLogLevelString(level), message) << std::endl;
}
std::string_view Logger::getLogLevelString(LogLevel level)const {
	switch (level)
	{
	case Logger::LogLevel::Error:
		return "ERROR";
	case Logger::LogLevel::Info:
		return "INFO";
	case Logger::LogLevel::Debug:
		return "DEBUG";
	}
	throw std::runtime_error{ "Invalid log level." };
}