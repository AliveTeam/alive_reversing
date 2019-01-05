#pragma once

#include "Sound.hpp"

#if USE_SDL2_SOUND
#include "FunctionFwd.hpp"
#include "stdlib.hpp"
#include "SDL.h"

struct StereoSampleFloat
{
    float left;
    float right;
};

enum AE_SDL_Voice_State
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

class AE_SDL_Voice
{

public:
    AE_SDL_Voice();
    int SetVolume(int volume);
    int SetFrequency(int frequency);
    int SetCurrentPosition(int position);
    int GetCurrentPosition(DWORD * readPos, DWORD * writePos);
    int GetFrequency(DWORD * freq);
    int SetPan(int pan);
    void Release();
    int GetStatus(DWORD * r);
    void Destroy();
    int Play(int /*reserved*/, int /*priority*/, int flags);
    int Stop();

public:
    float fVolume;
    float fFrequency;
    float fPan;

    AE_SDL_Voice_State eState;
    bool bLoop;
    bool bIsReleased;
    float fPlaybackPosition;

    short * pBuffer;
    int iSampleCount;
    int iChannels;
    int iBlockAlign;
};

#endif