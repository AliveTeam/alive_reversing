#pragma once

#include "SoundSDL.hpp"
#include "Sound.hpp"

#if USE_SDL2_SOUND

void Reverb_Init(s32 sampleRate);
void Reverb_DeInit();
void Reverb_PushSample(StereoSample_S16 v);
void Reverb_Update(s32 index);
void Reverb_Mix(StereoSample_S16* dst, SDL_AudioFormat format, Uint32 len, s32 volume);

#endif