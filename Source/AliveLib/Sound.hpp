#pragma once

#include "FunctionFwd.hpp"
#include <mmeapi.h>
#include <dsound.h>

struct SoundEntry;

EXPORT void CC SND_Init_Buffers_4CB480();
EXPORT void CC SND_SsQuit_4EFD50();
EXPORT void CC SND_Clear_4CB4B0();
EXPORT signed int CC SND_CreateDS_4EEAA0(unsigned int sampleRate, int bitsPerSample, int isStereo);
EXPORT signed int CC SND_New_4EEFF0(SoundEntry *pSnd, int sampleLength, int sampleRate, int bitsPerSample, int isStereo);
EXPORT int CC SND_Load_4EF680(SoundEntry* pSnd, const void* pWaveData, int waveDataLen);
EXPORT signed int CC SND_Free_4EFA30(SoundEntry* pSnd);
EXPORT void CC SND_Restart_4CB0E0();
EXPORT int CC SND_Get_Buffer_Status_4EE8F0(int idx);
EXPORT signed int CC SND_Stop_Sample_At_Idx_4EFA90(int idx);
EXPORT signed int CC SND_Buffer_Set_Volume_4EFAD0(int idx, int vol);
EXPORT int CC SND_Buffer_Set_Frequency_4EFC90(int idx, float hzChangeFreq);
EXPORT int CC SND_Buffer_Set_Frequency_4EFC00(int idx, float freq);

struct MIDI_Struct1;
EXPORT int CC SND_PlayEx_4EF740(const SoundEntry* pSnd, int panLeft, int panRight, float freq, MIDI_Struct1* pMidiStru, int playFlags, int priority);

struct SoundEntry
{
    int field_0_tableIdx;
    LPDIRECTSOUNDBUFFER field_4_pDSoundBuffer;
    BYTE* field_8_pSoundBuffer;
    int field_C_buffer_size_bytes;
    int field_10;
    int field_14_buffer_size_bytes;
    int field_18_sampleRate;
    char field_1C_bitsPerSample;
    unsigned __int8 field_1D_blockAlign;
    char field_1E;
    char field_1F;
    int field_20_isStereo;
};
ALIVE_ASSERT_SIZEOF(SoundEntry, 0x24);


