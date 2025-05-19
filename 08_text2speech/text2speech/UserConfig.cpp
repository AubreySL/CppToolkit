import "UserConfig.h";
import <exception>;

void UserConfig::init()
{
	std::ifstream in{ config };
	if (in.fail())
	{
		const std::string err{ std::format("Error opening {} for reading.", config.string()) };
		throw std::exception{ err.c_str()};
	}
	std::string result;

	while (!in.eof())
	{
		in >> result;
		if (result.find("appID") != std::string::npos)
		{
			m_appId = result.substr(result.find_first_of("=") + 1);
		}
		if (result.find("apiKey") != std::string::npos)
		{
			m_apiKey = result.substr(result.find_first_of("=") + 1);
		}
		if (result.find("apiSecret") != std::string::npos)
		{
			m_apiSecret = result.substr(result.find_first_of("=") + 1);
		}
	}

	in.close();
}

std::string UserConfig::getAll() const
{
	 std::string txt{ std::format(" appid: {}\r\n apikey:{} \r\n apiSecret:{} ", m_appId, m_apiKey, m_apiSecret) };
	 return txt;
}

std::string UserConfig::getAppId() const
{
	return m_appId;
}

std::string UserConfig::getApiSecret() const
{
	return m_apiSecret;
}

std::string UserConfig::getApiKey() const
{
	return m_apiKey;
}
