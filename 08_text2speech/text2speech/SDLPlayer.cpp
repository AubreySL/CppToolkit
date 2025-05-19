import "SDLPlayer.h";
import <iostream>;

void SDLPlayer::play()
{
    std::cout << "sdl play" << std::endl;
    if (!SDL_Init(SDL_INIT_AUDIO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return;
    }

    PlayPcmAudioInner(m_fileName);

    SDL_Quit();

}

void SDLCALL SDLPlayer::AudioStreamCB(void* userdata, SDL_AudioStream* stream, int additional_amount, int total_amount)
{
    // notify main thread that pcm buffer is empty
    if (buffer_empty.load()) {
        cv.notify_one();
        return;
    }

    const auto remain_buffer_size = static_cast<int>(pcm_buffer_end - pcm_audio_pos);
    const int copy_size = std::min(additional_amount, remain_buffer_size);

    // change audio volume
    auto mixed_buffer = std::make_unique<uint8_t[]>(copy_size);
    SDL_MixAudio(mixed_buffer.get(), pcm_audio_pos, kAudioFormatS16Le, copy_size, kAudioVolume);

    // put mixed audio data to audio stream
    SDL_PutAudioStreamData(stream, mixed_buffer.get(), copy_size);
    pcm_audio_pos += copy_size;

    if (pcm_audio_pos >= pcm_buffer_end) {
        buffer_empty.store(true);
        cv.notify_one();
    }
}

void SDLPlayer::PlayPcmAudioInner(const std::string& pcm_file)
{
    SDL_AudioStream* stream = nullptr;
    constexpr SDL_AudioSpec spec = {
        .format = kAudioFormatS16Le,
        .channels = kAudioChannels,
        .freq = kAudioFreq
    }; // std=c++20

    // open device in a paused state
    stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, AudioStreamCB, nullptr);
    if (!stream) {
        SDL_Log("Couldn't open audio device: %s", SDL_GetError());
        return;
    }

    std::ifstream file(pcm_file, std::ios::binary);
    if (!file.is_open()) {
        SDL_Log("Couldn't open pcm file: %s", pcm_file.c_str());
        SDL_DestroyAudioStream(stream);
        return;
    }

    pcm_buffer = std::make_unique<uint8_t[]>(kPcmBufferSize);

    // begin to playback audio
    SDL_ResumeAudioStreamDevice(stream);

    // read pcm data from file
    uint64_t total_bytes_read = 0;
    while (true) {
        file.read(reinterpret_cast<char*>(pcm_buffer.get()), kPcmBufferSize);
        const uint64_t bytes_read = file.gcount();
        if (bytes_read == 0) {
            SDL_Log("End of pcm file");
            break;
        }

        total_bytes_read += bytes_read;
        SDL_Log("Read %lld bytes from pcm file, total = %lld", bytes_read, total_bytes_read);

        pcm_audio_pos = pcm_buffer.get();
        pcm_buffer_end = pcm_audio_pos + bytes_read;
        buffer_empty.store(false);

        // wait until pcm buffer is empty
        auto lock = std::unique_lock(cv_mutex); // std=c++17
        cv.wait(lock, [&]() -> bool { return buffer_empty.load(); });
    }

    SDL_DestroyAudioStream(stream);
    file.close();

}