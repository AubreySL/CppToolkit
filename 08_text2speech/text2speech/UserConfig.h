#pragma once
import <string>;
import <fstream>;
import <filesystem>;
import <iostream>;
import <exception>;

class UserConfig
{
public:
	UserConfig() = default;
	~UserConfig() = default;
	void init();
	std::string getAll() const;
	std::string getAppId() const;
	std::string getApiSecret() const;
	std::string getApiKey() const;
private:
	std::string m_appId;
	std::string m_apiSecret;
	std::string m_apiKey;
	static inline std::filesystem::path config{ R"(./config.ini)" };
};

