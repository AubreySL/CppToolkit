import "XFTTS.h";
import "UserConfig.h";
import "SDLPlayer.h";
import <fstream>;
import <string>;
import <locale>;
import <filesystem>;
#include <assert.h>
#include <cstring>

#include<Windows.h>

#include "aikit_biz_api.h"
#include "aikit_constant.h"
#include "aikit_biz_config.h"
#include "aikit_biz_builder.h"

using namespace std;
using namespace AIKIT;

void OnOutput(AIKIT_HANDLE* handle, const AIKIT_OutputData* output);
void OnEvent(AIKIT_HANDLE* handle, AIKIT_EVENT eventType, const AIKIT_OutputEvent* eventValue);
void OnError(AIKIT_HANDLE* handle, int32_t err, const char* desc);

void XFTTS::textToSpeech(const std::string& text)
{
	setlocale(LC_ALL, "zh_CN.UTF-8");
	cout << "���ݣ�"<< text << endl;
	size_t token = std::hash<std::string>{}(text);
	
	std::filesystem::path target{ format("{}.pcm", token) };
	std::filesystem::directory_entry dirEntry{ target };

	SDLPlayer::playDeque.push_back(token);
	if (dirEntry.exists())
	{
		cout << "already exist." << endl;
		playPCM();
	}
	else
	{
		TestXTTS(const_cast<std::string&>(text));
	}
	
}

void OnOutput(AIKIT_HANDLE* handle, const AIKIT_OutputData* output) {
	printf("OnOutput key:%s\n", (char*)output->node->key);
	printf("OnOutput status:%d\n", output->node->status);
	if (output->node->value)
	{
		size_t token = SDLPlayer::playDeque.front();
		ofstream target{ format("{}.pcm", token), ios_base::out | ios_base::app | ios_base::binary };
		target.write(reinterpret_cast<char*>(output->node->value), output->node->len);
		target.close();
	}
	
}

void OnEvent(AIKIT_HANDLE* handle, AIKIT_EVENT eventType, const AIKIT_OutputEvent* eventValue) {
	printf("OnEvent:%d\n", eventType);
	if (eventType == AIKIT_Event_End) {
		//ttsFinished = true;
		XFTTS::ttsFinished.store(true);
		XFTTS::playPCM();
	}
}

void OnError(AIKIT_HANDLE* handle, int32_t err, const char* desc) {
	printf("OnError:%d\n", err);
}

int XFTTS::TestXiaoyan(AIKIT_ParamBuilder* paramBuilder, AIKIT_DataBuilder* dataBuilder, string& text) {
	//string text = "����֧���ܵĿ�,ȫ�������Ǵ�,�����ϰ��Ǻ��з�����Ѭ��,����֧��Ҳ������ӵ�.";
	AIKIT_HANDLE* handle = nullptr;
	ttsFinished = false;
	//XFTTS::ttsFinished.store(false);
	const char *role = "xiaoyan";	//xiaofeng xiaoyan
	paramBuilder = AIKIT_ParamBuilder::create();
	paramBuilder->clear();
	paramBuilder->param("vcn", role, strlen(role));//��ѡ������60020��xiaoyan������60030��xiaofeng������69010��johnӢ�ķ�����69020��catherineӢ�ķ�����
	paramBuilder->param("vcnModel", role, strlen(role));//��ѡ������ȡֵ��vcn����һ��
	paramBuilder->param("language", 1);//���������1�����ģ�2��Ӣ�ģ���Ӣ��ʱ����Ӧ�ı�Ҳ����Ӣ��
	paramBuilder->param("textEncoding", "UTF-8", strlen("UTF-8")); //��ѡ�������ı������ʽ��Ĭ��Ϊ65001��UTF8��ʽ
	dataBuilder = AIKIT_DataBuilder::create();
	AiText* aiText = AiText::get("text")->encoding(AiText::ENCODING_UTF8)->compress(AiText::COMPRESS_RAW)->format("json")->status(3)->data(text.c_str(), text.length())->valid();
	dataBuilder->payload(aiText);


	int ret = AIKIT_Start("e2e44feff", AIKIT_Builder::build(paramBuilder), nullptr, &handle);
	printf("AIKIT_Start:%d\n", ret);
	if (ret != 0) {
		return  ret;
	}

	ret = AIKIT_Write(handle, AIKIT_Builder::build(dataBuilder));
	printf("AIKIT_Write:%d\n", ret);
	if (ret != 0) {
		return  ret;
	}

	while (ttsFinished != true) {
		Sleep(1000);
	}
	ret = AIKIT_End(handle);

	delete dataBuilder;
	dataBuilder = nullptr;

	return ret;
}

void XFTTS::TestXTTS(std::string& text) {

	AIKIT_ParamBuilder* paramBuilder = nullptr;
	AIKIT_DataBuilder* dataBuilder = nullptr;
	AIKIT_OutputData* output = nullptr;
	AIKIT_HANDLE* handle = nullptr;
	int loopCount = 1;

	AIKIT_Callbacks cbs = { OnOutput,OnEvent,OnError };
	AIKIT_RegisterCallback(cbs);
	AIKIT_SetLogInfo(0, LOG_STDOUT, nullptr);
	
	// �˽ӿ�����������ȨУ��ļ��ʱ�䣨�����ʱ���ȥУ����Ȩһ�Σ�
	// AIKIT_SetAuthCheckInterval(TIME_ONE_DAY_SECONDS);

	auto config{ std::make_unique<UserConfig>() };
	config->init();
	
	AIKIT_Configurator::builder()
		.log()
			.logLevel(LOG_LVL_INFO)
			.logMode(2)
			.logPath("log.txt")
		.app()
			.appID(config->getAppId().c_str())
			.apiSecret(config->getApiSecret().c_str())
			.apiKey(config->getApiKey().c_str())
			.workDir("./")
			.resDir("./resource")
		.auth()
			.authType(0)
			.ability("e2e44feff");

	int ret = AIKIT_Init();
	if (ret != 0) {
		printf("AIKIT_Init failed:%d\n", ret);
		goto exit;
	}

	

	while (loopCount--) {
		ret = TestXiaoyan(paramBuilder, dataBuilder, text);
		if (ret != 0) {
			printf("TestXiaoyan failed %d\n", ret);
			goto exit;
		}
	}

exit:
	if (paramBuilder != nullptr) {
		delete paramBuilder;
		paramBuilder = nullptr;
	}

	if (dataBuilder != nullptr) {
		delete dataBuilder;
		dataBuilder = nullptr;
	}
	//���������˳�ʱ���ŵ���AIKIT_UnInit�ͷ�������Դ
	//sleep(20);
	AIKIT_UnInit();
}

void XFTTS::playPCM()
{
	size_t token = SDLPlayer::playDeque.front();
	Player* parent;
	SDLPlayer sdlPlayer{ format("{}.pcm", token).c_str() };
	parent = &sdlPlayer;
	parent->play();
	SDLPlayer::playDeque.pop_front();
	cout << "play done." << endl;
}