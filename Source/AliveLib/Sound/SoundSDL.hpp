#pragma once

#include "Sound.hpp"
#include "Sound/Midi.hpp"

#if USE_SDL2_SOUND
#include "FunctionFwd.hpp"
#include "stdlib.hpp"
#include "SDL.h"
#include <atomic>

#define DSBSTATUS_PLAYING           0x00000001
#define DSBSTATUS_BUFFERLOST        0x00000002
#define DSBSTATUS_LOOPING           0x00000004
#define DSBSTATUS_TERMINATED        0x00000020

#define DSBPAN_LEFT                 -10000
#define DSBPAN_CENTER               0
#define DSBPAN_RIGHT                10000

#define DSBPLAY_LOOPING             0x00000001

#define DSBFREQUENCY_MIN            100
#define DSBFREQUENCY_MAX            200000

#define DSERR_BUFFERLOST            0x88780096

struct MIDI_Struct1;
struct SoundEntry;

template <typename T>
struct StereoSample
{
    T left;
    T right;
};

typedef StereoSample<signed short> StereoSample_S16;
typedef StereoSample<signed int> StereoSample_S32;
typedef StereoSample<float> StereoSample_F32;

enum AE_SDL_Voice_Status
{
    Stopped = 0,
    Paused = 1,
    Playing = 2,
};

enum AudioFilterMode
{
    NoFilter = 0,
    Linear = 1,
};

class SDLSoundBuffer
{

public:
    SDLSoundBuffer();


    SDLSoundBuffer(const DSBUFFERDESC& bufferDesc, int soundSysFreq);


    int SetVolume(int volume);
    int Play(int /*reserved*/, int /*priority*/, int flags);
    int Stop();

    int SetFrequency(int frequency);
    int SetCurrentPosition(int position);
    int GetCurrentPosition(DWORD * readPos, DWORD * writePos);
    int GetFrequency(DWORD * freq);
    int SetPan(signed int pan);
    void Release();
    int GetStatus(DWORD * r);
    void Destroy();
    
    std::vector<BYTE>* GetBuffer();
    int Duplicate(SDLSoundBuffer ** dupePtr);

public:
    struct AE_SDL_Voice_State
    {
        int iVolume;
        bool bVolDirty;
        int iVolumeTarget;
        float fFrequency;
        signed int iPan;

        AE_SDL_Voice_Status eStatus;
        bool bLoop;
        bool bIsReleased;
        float fPlaybackPosition;

        int iSampleCount;
        int iChannels;
        int iBlockAlign;
    };

    int mSoundSysFreq = 0;
    AE_SDL_Voice_State mState;
    std::shared_ptr<std::vector<BYTE>> mBuffer;
    
};


// An SDL implementation of used IDirectSound API's
class SDLSoundSystem
{
public:

    void Init(unsigned int sampleRate, int bitsPerSample, int isStereo);

    HRESULT DuplicateSoundBuffer(TSoundBufferType* pDSBufferOriginal, TSoundBufferType** ppDSBufferDuplicate)
    {
        pDSBufferOriginal->Duplicate(ppDSBufferDuplicate);
        return S_OK;
    }

    HRESULT CreateSoundBuffer(LPCDSBUFFERDESC pcDSBufferDesc, TSoundBufferType** ppDSBuffer, void* /*pUnkOuter*/);

    HRESULT Release()
    {
        delete this;
        return S_OK;
    }

    // Called by audio thread - time critical
    static void AudioCallBackStatic(void * userdata, Uint8 *stream, int len);

private:
    void AudioCallBack(Uint8* stream, int len);
    void RenderAudio(StereoSample_S16* pSampleBuffer, int sampleBufferCount);

    void RenderSoundBuffer(SDLSoundBuffer& entry, StereoSample_S16* pSampleBuffer, int sampleBufferCount);

private:
    SDL_AudioSpec mAudioDeviceSpec = {};
    static constexpr int kMixVolume = 127;

    int mCurrentSoundBufferSize = 0;
    AudioFilterMode mAudioFilterMode = AudioFilterMode::Linear;
    StereoSample_S16 * mTempSoundBuffer = nullptr;
    StereoSample_S16 * mNoReverbBuffer = nullptr;
};

void SND_InitVolumeTable_SDL();
signed int SND_LoadSamples_SDL(const SoundEntry* pSnd, DWORD sampleOffset, unsigned char* pSoundBuffer, unsigned int sampleCount);
signed int SND_CreateDS_SDL(unsigned int sampleRate, int bitsPerSample, int isStereo);
int SND_Clear_SDL(SoundEntry* pSoundEntry, unsigned int sampleOffset, unsigned int size);
const char* SND_HR_Err_To_String_SDL(long hr);


#endif
