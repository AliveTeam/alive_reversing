#pragma once

#include "Sound.hpp"

#if USE_SDL2_SOUND
    #include "../AliveLibAE/stdlib.hpp"
    #include "SDL.h"
    #include <atomic>

    #define DSBSTATUS_PLAYING 0x00000001
    #define DSBSTATUS_BUFFERLOST 0x00000002
    #define DSBSTATUS_LOOPING 0x00000004
    #define DSBSTATUS_TERMINATED 0x00000020

    #define DSBPLAY_LOOPING 0x00000001

    #define DSBPAN_LEFT -10000
    #define DSBPAN_CENTER 0
    #define DSBPAN_RIGHT 10000


    #define DSBFREQUENCY_MIN 100
    #define DSBFREQUENCY_MAX 200000

    #define DSERR_BUFFERLOST 0x88780096

struct MIDI_Channel;
struct SoundEntry;

template <typename T>
struct StereoSample
{
    T left;
    T right;
};

using StereoSample_S16 = StereoSample<s16>;
using StereoSample_S32 = StereoSample<s32>;
using StereoSample_F32 = StereoSample<f32>;

void SND_InitVolumeTable_SDL();
s32 SND_LoadSamples_SDL(const SoundEntry* pSnd, u32 sampleOffset, u8* pSoundBuffer, u32 sampleCount);
s32 SND_CreateDS_SDL(u32 sampleRate, s32 bitsPerSample, s32 isStereo);
s32 SND_Clear_SDL(SoundEntry* pSoundEntry, u32 sampleOffset, u32 size);
const char_type* SND_HR_Err_To_String_SDL(long hr);

#endif
