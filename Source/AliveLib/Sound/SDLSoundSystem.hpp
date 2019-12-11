#pragma once

#include "Sound.hpp"
#include "SoundSDL.hpp"

enum AudioFilterMode
{
    NoFilter = 0,
    Linear = 1,
};

// An SDL implementation of used IDirectSound API's
class SDLSoundSystem
{
public:

    void Init(unsigned int sampleRate, int bitsPerSample, int isStereo);

    HRESULT DuplicateSoundBuffer(TSoundBufferType* pDSBufferOriginal, TSoundBufferType** ppDSBufferDuplicate);

    HRESULT CreateSoundBuffer(LPCDSBUFFERDESC pcDSBufferDesc, TSoundBufferType** ppDSBuffer, void* /*pUnkOuter*/);

    HRESULT Release();

    // Called by audio thread - time critical
    static void AudioCallBackStatic(void * userdata, Uint8 *stream, int len);

private:
    void AudioCallBack(Uint8* stream, int len);
    void RenderAudio(StereoSample_S16* pSampleBuffer, int sampleBufferCount);

    void RenderSoundBuffer(SDLSoundBuffer& entry, StereoSample_S16* pSampleBuffer, int sampleBufferCount);

    void RenderMonoSample(Sint16* pVoiceBufferPtr, SDLSoundBuffer* pVoice, int i);

    void RenderStereoSample(Sint16* pVoiceBufferPtr, SDLSoundBuffer* pVoice, int i);

private:
    SDL_AudioSpec mAudioDeviceSpec = {};
    static constexpr int kMixVolume = 127;

    int mCurrentSoundBufferSize = 0;
    AudioFilterMode mAudioFilterMode = AudioFilterMode::Linear;
    StereoSample_S16 * mTempSoundBuffer = nullptr;
    StereoSample_S16 * mNoReverbBuffer = nullptr;
};
