#pragma once

#include "FunctionFwd.hpp"
#include "Sound/Sound.hpp"

#if !USE_SDL2_SOUND
signed int CC SND_CreateDS_DSound(u32 sampleRate, int bitsPerSample, int isStereo);
int CC SND_Clear_DSound(SoundEntry* pSoundEntry, u32 sampleOffset, u32 size);
void SND_InitVolumeTable_DSound();
EXPORT char CC SND_CreatePrimarySoundBuffer_4EEEC0(int sampleRate, int bitsPerSample, int isStereo);
EXPORT int CC SND_SetPrimarySoundBufferFormat_4EE990(int sampleRate, int bitsPerSample, u8 isStereo);
signed int CC SND_LoadSamples_DSound(const SoundEntry* pSnd, DWORD sampleOffset, u8* pSoundBuffer, u32 sampleCount);
const char* SND_HR_Err_To_String_DSound(HRESULT hr);
#endif
