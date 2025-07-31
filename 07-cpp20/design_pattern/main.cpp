#include "Logger.h"
#include "UseLogger.h";

int main() {
	Logger concreteLogger{ "log.out" };
	concreteLogger.setLogLevel(ILogger::LogLevel::Debug);

	UseLogger u{ concreteLogger };
	u.doSomething();

	return 0;
}