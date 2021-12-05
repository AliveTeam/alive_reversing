#pragma once

#include "Sound.hpp"
#include "SoundSDL.hpp"
#include <thread>
#include <condition_variable>
#include <mutex>

#define CI_DISABLE_ASSERTS
#include <cinder/audio/dsp/RingBuffer.h>

enum AudioFilterMode
{
    NoFilter = 0,
    Linear = 1,
};

// An SDL implementation of used IDirectSound API's
class SDLSoundSystem final
{
public:
    void Init(u32 sampleRate, s32 bitsPerSample, s32 isStereo);

    HRESULT DuplicateSoundBuffer(TSoundBufferType* pDSBufferOriginal, TSoundBufferType** ppDSBufferDuplicate);

    HRESULT CreateSoundBuffer(LPCDSBUFFERDESC pcDSBufferDesc, TSoundBufferType** ppDSBuffer, void* /*pUnkOuter*/);

    HRESULT Release();

    // Called by audio thread - time critical
    static void AudioCallBackStatic(void* userdata, Uint8* stream, s32 len);

private:
    ~SDLSoundSystem();

    void AudioCallBack(Uint8* stream, s32 len);

    void RenderAudioThread();


    void RenderAudio(StereoSample_S16* pSampleBuffer, s32 sampleBufferCount);

    void RenderSoundBuffer(SDLSoundBuffer& entry, StereoSample_S16* pSampleBuffer, s32 sampleBufferCount);

    void RenderMonoSample(Sint16* pVoiceBufferPtr, SDLSoundBuffer* pVoice, s32 i);

    void RenderStereoSample(Sint16* pVoiceBufferPtr, SDLSoundBuffer* pVoice, s32 i);

private:
    SDL_AudioSpec mAudioDeviceSpec = {};
    static constexpr s32 kMixVolume = 127;

    s32 mCurrentSoundBufferSize = 0;
    AudioFilterMode mAudioFilterMode = AudioFilterMode::Linear;
    std::vector<StereoSample_S16> mTempSoundBuffer;
    std::vector<StereoSample_S16> mNoReverbBuffer;
    cinder::audio::dsp::RingBufferT<StereoSample_S16> mAudioRingBuffer;

    // to avoid busy loop
    std::mutex mAudioRingBufferMutex;
    std::condition_variable mAudioRingBufferConditionVariable;

    std::atomic_bool mRenderAudioThreadQuit{false};
    std::unique_ptr<std::thread> mRenderAudioThread;


    bool mCreated = false;
};
