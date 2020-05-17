#pragma once

#include "Sound.hpp"
#include "SoundSDL.hpp"
#include <mutex>

#define MAX_VOICE_COUNT 1024
extern std::atomic<SDLSoundBuffer*> sAE_ActiveVoices[MAX_VOICE_COUNT];


enum SDLSoundBufferStatus
{
    Stopped = 0,
    Paused = 1,
    Playing = 2,
};

class SDLSoundBuffer
{

public:
    SDLSoundBuffer(const SDLSoundBuffer& rhs);

    SDLSoundBuffer& operator = (const SDLSoundBuffer& rhs);

    SDLSoundBuffer();


    SDLSoundBuffer(const DSBUFFERDESC& bufferDesc, int soundSysFreq);


    HRESULT SetVolume(int volume);
    HRESULT Play(int /*reserved*/, int /*priority*/, int flags);
    HRESULT Stop();

    HRESULT SetFrequency(int frequency);
    HRESULT SetCurrentPosition(int position);
    HRESULT GetCurrentPosition(DWORD * readPos, DWORD * writePos);
    HRESULT GetFrequency(DWORD * freq);
    HRESULT SetPan(signed int pan);
    void Release();
    HRESULT GetStatus(DWORD * r);

    void Destroy();

    std::vector<BYTE>* GetBuffer();
    void Duplicate(SDLSoundBuffer ** dupePtr);

public:
    struct AE_SDL_Voice_State
    {
        int iVolume;
        bool bVolDirty;
        int iVolumeTarget;
        float fFrequency;
        signed int iPan;

        SDLSoundBufferStatus eStatus;
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
    std::mutex mLock;
};

