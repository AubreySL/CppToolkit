export module NewLoggerAdapter;
import NewLoggerInterface;
import Logger;
import <iostream>;
export class NewLoggerAdapter :public NewLoggerInterface {
public:
	NewLoggerAdapter();
	// Í¨¹ý NewLoggerInterface ¼Ì³Ð
	void log(std::string_view message) override;
private:
	Logger m_logger;
};

NewLoggerAdapter::NewLoggerAdapter()
{
	std::cout << "NewLoggerAdapter Constructor" << std::endl;
}

void NewLoggerAdapter::log(std::string_view message)
{
	m_logger.log(Logger::LogLevel::Info, message.data());
}
