#pragma once

#include "SoundSDL.hpp"
#include "Sound.hpp"

#if USE_SDL2_SOUND

#define REVERB_GRAINS_SIZE 50000
#define REVERB_ECHO_COUNT 128
#define REVERB_ROOM_SIZE 1000
#define REVERB_LOW_PASS 0.5f

struct Echo
{
    int delay;
    float volume;
};

void ReverbInit();
void PushReverbSample(StereoSampleFloat v);
inline void UpdateReverb(int index);
void Reverb_Mix(StereoSampleFloat * dst, SDL_AudioFormat format, Uint32 len, int volume);

#endif