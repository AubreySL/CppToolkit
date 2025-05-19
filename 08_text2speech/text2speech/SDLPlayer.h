#pragma once
import "Player.h";
import <atomic>;
import <string>;
import <fstream>;
import <condition_variable>;
import <SDL3\SDL.h>;
import <deque>;

class SDLPlayer :
    public Player
{
public:
    SDLPlayer() = default;
    ~SDLPlayer() = default;
    SDLPlayer(const std::string& m_fileName)
        : m_fileName(m_fileName)
    {
    }

    // Í¨¹ý Player ¼Ì³Ð
    void play() override;

    static void SDLCALL AudioStreamCB(void* userdata, SDL_AudioStream* stream, int additional_amount, int total_amount);
    static void PlayPcmAudioInner(const std::string& pcm_file);
    static inline std::deque<size_t> playDeque;

private:
    std::string m_fileName;
    static  constexpr int kAudioChannels = 1;
    static  constexpr int kAudioFreq = 16000;
    static  constexpr float kAudioVolume = 1.0; // volume ranges from 0.0 - 1.0
    static  constexpr int kPcmBufferSize = 16000 * 2 / 100; // must > 1920B = 48000*2*2/100, bytes per 10ms
    static  constexpr SDL_AudioFormat kAudioFormatS16Le = SDL_AUDIO_S16;

    static inline std::atomic_bool buffer_empty{ true };
    static inline std::unique_ptr<uint8_t[]> pcm_buffer;
    static inline uint8_t* pcm_buffer_end = nullptr;
    static inline uint8_t* pcm_audio_pos = nullptr;
    static inline std::condition_variable cv;
    static inline std::mutex cv_mutex;
};

