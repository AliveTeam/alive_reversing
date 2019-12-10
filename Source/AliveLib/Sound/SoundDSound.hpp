#pragma once

#include "FunctionFwd.hpp"
#include "Sound/Sound.hpp"

#if !USE_SDL2_SOUND
signed int CC SND_CreateDS_DSound(unsigned int sampleRate, int bitsPerSample, int isStereo);
int CC SND_Clear_DSound(SoundEntry* pSoundEntry, unsigned int sampleOffset, unsigned int size);
EXPORT void CC SND_InitVolumeTable_4EEF60();
EXPORT char CC SND_CreatePrimarySoundBuffer_4EEEC0(int sampleRate, int bitsPerSample, int isStereo);
EXPORT int CC SND_SetPrimarySoundBufferFormat_4EE990(int sampleRate, int bitsPerSample, unsigned __int8 isStereo);
signed int CC SND_LoadSamples_DSound(const SoundEntry* pSnd, DWORD sampleOffset, unsigned char* pSoundBuffer, unsigned int sampleCount);
const char* SND_HR_Err_To_String_DSound(HRESULT hr);
#endif
