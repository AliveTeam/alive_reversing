#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "Sound/Sound.hpp"

#if !USE_SDL2_SOUND
s32 CC SND_CreateDS_DSound(u32 sampleRate, s32 bitsPerSample, s32 isStereo);
s32 CC SND_Clear_DSound(SoundEntry* pSoundEntry, u32 sampleOffset, u32 size);
void SND_InitVolumeTable_DSound();
EXPORT s8 CC SND_CreatePrimarySoundBuffer_4EEEC0(s32 sampleRate, s32 bitsPerSample, s32 isStereo);
EXPORT s32 CC SND_SetPrimarySoundBufferFormat_4EE990(s32 sampleRate, s32 bitsPerSample, u8 isStereo);
s32 CC SND_LoadSamples_DSound(const SoundEntry* pSnd, u32 sampleOffset, u8* pSoundBuffer, u32 sampleCount);
const s8* SND_HR_Err_To_String_DSound(HRESULT hr);
#endif
