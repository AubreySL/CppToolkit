#pragma once
import <string>;
import "TTS.h";
import <atomic>;

#include <aikit_spark_api.h>

class XFTTS: public TTS
{
public:
    XFTTS() = default;
    virtual ~XFTTS() = default;

    // Í¨¹ý TTS ¼Ì³Ð
    void textToSpeech(const std::string& text) override;

    static inline std::atomic_bool ttsFinished{ false };
    static void playPCM();
private:
    static const int TIME_ONE_DAY_SECONDS = 24 * 60 * 60;
    int TestXiaoyan(AIKIT::AIKIT_ParamBuilder* paramBuilder, AIKIT::AIKIT_DataBuilder* dataBuilder, std::string& text);
    void TestXTTS(std::string& text);
};

