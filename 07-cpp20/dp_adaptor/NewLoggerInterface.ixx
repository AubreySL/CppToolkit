export module NewLoggerInterface;
import <string>;
export class NewLoggerInterface {
public:
	virtual ~NewLoggerInterface() = default;
	virtual void log(std::string_view message) = 0;
};