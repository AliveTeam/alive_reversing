#pragma once

#include "SoundSDL.hpp"
#include "Sound.hpp"

#if USE_SDL2_SOUND

void Reverb_Init(int sampleRate);
void Reverb_PushSample(StereoSampleFloat v);
inline void Reverb_Update(int index);
void Reverb_Mix(StereoSampleFloat * dst, SDL_AudioFormat format, Uint32 len, int volume);

#endif