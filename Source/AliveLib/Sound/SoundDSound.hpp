#pragma once

#include "FunctionFwd.hpp"
#include "Sound/Sound.hpp"

#if !USE_SDL2_SOUND
EXPORT char * CC SND_HR_Err_To_String_4EEC70(HRESULT hr);
EXPORT signed int CC SND_CreateDS_4EEAA0(unsigned int sampleRate, int bitsPerSample, int isStereo);
EXPORT int CC SND_Reload_4EF350(SoundEntry* pSoundEntry, unsigned int sampleOffset, unsigned int size);
EXPORT void CC SND_SsQuit_4EFD50();
EXPORT signed int CC SND_Free_4EFA30(SoundEntry* pSnd);
EXPORT void CC SND_InitVolumeTable_4EEF60();
EXPORT char CC SND_CreatePrimarySoundBuffer_4EEEC0(int sampleRate, int bitsPerSample, int isStereo);
EXPORT void CC SND_Init_WaveFormatEx_4EEA00(WAVEFORMATEX *pWaveFormat, int sampleRate, unsigned __int8 bitsPerSample, int isStereo);
EXPORT int CC SND_SetPrimarySoundBufferFormat_4EE990(int sampleRate, int bitsPerSample, unsigned __int8 isStereo);
EXPORT signed int CC SND_Renew_4EEDD0(SoundEntry *pSnd);
EXPORT signed int CC SND_Reload_4EF1C0(const SoundEntry* pSnd, DWORD sampleOffset, unsigned char* pSoundBuffer, unsigned int sampleCount);
EXPORT signed int CC SND_New_4EEFF0(SoundEntry *pSnd, int sampleLength, int sampleRate, int bitsPerSample, int isStereo);
EXPORT int CC SND_PlayEx_4EF740(const SoundEntry* pSnd, int panLeft, int panRight, float freq, MIDI_Struct1* pMidiStru, int playFlags, int priority);
#endif