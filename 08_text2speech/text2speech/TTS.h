#pragma once
import <string>;

class TTS
{
public:
	virtual void textToSpeech(const std::string& text) = 0;
};

