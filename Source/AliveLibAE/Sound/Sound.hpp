#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "relive_config.h"


#if !USE_SDL2_SOUND
//#include <mmeapi.h>
//#include <dsound.h>
#else
/*
 *  extended waveform format structure used for all non-PCM formats. this
 *  structure is common to all non-PCM formats.
 */
// NOTE: Windows type
    #ifndef _WAVEFORMATEX_
        #define _WAVEFORMATEX_
typedef struct tWAVEFORMATEX
{
    u16 wFormatTag;
    u16 nChannels;
    u32 nSamplesPerSec;
    u32 nAvgBytesPerSec;
    u16 nBlockAlign;
    u16 wBitsPerSample;
    u16 cbSize;

} WAVEFORMATEX, *PWAVEFORMATEX, *NPWAVEFORMATEX, *LPWAVEFORMATEX;
    #endif

    /* flags for wFormatTag field of WAVEFORMAT */
    #define WAVE_FORMAT_PCM 1

// Note: Windows SDound type
typedef struct _DSBUFFERDESC
{
    u32 dwSize;
    u32 dwFlags;
    u32 dwBufferBytes;
    u32 dwReserved;
    LPWAVEFORMATEX lpwfxFormat;
    #if DIRECTSOUND_VERSION >= 0x0700
    GUID guid3DAlgorithm;
    #endif
} DSBUFFERDESC, *LPDSBUFFERDESC;
typedef const DSBUFFERDESC* LPCDSBUFFERDESC;

    #ifndef FAILED
        #define FAILED(hr) (((HRESULT)(hr)) < 0)
    #endif

    #ifndef SUCCEEDED
        #define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
    #endif

    #ifndef _HRESULT_DEFINED
typedef s32 HRESULT;

        #define S_OK ((HRESULT) 0L)
    #endif

#endif


#if USE_SDL2_SOUND
using TSoundBufferType = class SDLSoundBuffer;
extern bool gReverbEnabled;
extern bool gAudioStereo;
#else
using TSoundBufferType = struct IDirectSoundBuffer;
#endif

#if USE_SDL2_SOUND
class SDLSoundSystem;
ALIVE_VAR_EXTERN(SDLSoundSystem*, sDSound_BBC344);
#else
ALIVE_VAR_EXTERN(LPDIRECTSOUND, sDSound_BBC344);
#endif

struct SoundEntry final
{
    s32 field_0_tableIdx;
    TSoundBufferType* field_4_pDSoundBuffer;
    u8* field_8_pSoundBuffer;
    s32 field_C_buffer_size_bytes;
    s32 field_10;
    s32 field_14_buffer_size_bytes;
    s32 field_18_sampleRate;
    s8 field_1C_bitsPerSample;
    u8 field_1D_blockAlign;
    s8 field_1E;
    s8 field_1F;
    s32 field_20_isStereo;
};
ALIVE_ASSERT_SIZEOF(SoundEntry, 0x24);

struct SoundBuffer final
{
    TSoundBufferType* field_0_pDSoundBuffer;
    s32 field_4;
    s32 field_8_sample_idx;
    s32 field_C;
    s32 field_10;
};
ALIVE_ASSERT_SIZEOF(SoundBuffer, 0x14);

EXPORT u32 CC SND_Get_Sound_Entry_Pos_4EF620(SoundEntry* pSoundEntry);
EXPORT s32 CC SND_Clear_4EF350(SoundEntry* pSoundEntry, u32 sampleOffset, u32 size);
EXPORT void CC SND_SsQuit_4EFD50();
EXPORT s32 CC SND_CreateDS_4EEAA0(u32 sampleRate, s32 bitsPerSample, s32 isStereo);
EXPORT void CC SND_Init_WaveFormatEx_4EEA00(WAVEFORMATEX* pWaveFormat, s32 sampleRate, u8 bitsPerSample, s32 isStereo);
EXPORT s32 CC SND_New_4EEFF0(SoundEntry* pSnd, s32 sampleLength, s32 sampleRate, s32 bitsPerSample, s32 isStereo);
EXPORT s32 CC SND_Load_4EF680(SoundEntry* pSnd, const void* pWaveData, s32 waveDataLen);
EXPORT const char_type* CC SND_HR_Err_To_String_4EEC70(HRESULT hr);
EXPORT s32 CC SND_Free_4EFA30(SoundEntry* pSnd);
EXPORT void CC SND_Restart_4CB0E0();
EXPORT s32 CC SND_SetPrimarySoundBufferFormat_4EE990(s32 sampleRate, s32 bitsPerSample, u8 isStereo);
EXPORT void CC SND_InitVolumeTable_4EEF60();
EXPORT s8 CC SND_CreatePrimarySoundBuffer_4EEEC0(s32 sampleRate, s32 bitsPerSample, s32 isStereo);
EXPORT s32 CC SND_Renew_4EEDD0(SoundEntry* pSnd);
EXPORT s32 CC SND_Get_Buffer_Status_4EE8F0(s32 idx);
EXPORT s32 CC SND_Stop_Sample_At_Idx_4EFA90(s32 idx);
EXPORT s32 CC SND_Buffer_Set_Volume_4EFAD0(s32 idx, s32 vol);
EXPORT SoundBuffer* CC SND_Get_Sound_Buffer_4EF970(s32 sampleIdx, s32 field10);
EXPORT s32 CC SND_Buffer_Set_Frequency_4EFC90(s32 idx, f32 hzChangeFreq);
EXPORT s32 CC SND_Buffer_Set_Frequency_4EFC00(s32 idx, f32 freq);
EXPORT s32 CC SND_LoadSamples_4EF1C0(const SoundEntry* pSnd, u32 sampleOffset, u8* pSoundBuffer, u32 sampleCount);
EXPORT u32* CC SND_4F00B0(u32* /*a1*/, u32 /*a2*/, s32 /*a3*/);

struct MIDI_Channel;
EXPORT s32 CC SND_PlayEx_4EF740(const SoundEntry* pSnd, s32 panLeft, s32 panRight, f32 freq, MIDI_Channel* pMidiStru, s32 playFlags, s32 priority);

struct SoundApi final
{
    EXPORT SoundApi();
    decltype(&SND_Get_Sound_Entry_Pos_4EF620) SND_Get_Sound_Entry_Pos;
    decltype(&SND_Clear_4EF350) SND_Clear;
    decltype(&SND_SsQuit_4EFD50) SND_SsQuit;
    decltype(&SND_CreateDS_4EEAA0) SND_CreateDS;
    decltype(&SND_Init_WaveFormatEx_4EEA00) SND_Init_WaveFormatEx;
    decltype(&SND_New_4EEFF0) SND_New;
    decltype(&SND_Load_4EF680) SND_Load;
    decltype(&SND_HR_Err_To_String_4EEC70) SND_HR_Err_To_String;
    decltype(&SND_Free_4EFA30) SND_Free;
    decltype(&SND_Restart_4CB0E0) SND_Restart;
#if !USE_SDL2_SOUND
    decltype(&SND_SetPrimarySoundBufferFormat_4EE990) SND_SetPrimarySoundBufferFormat;
#endif
    decltype(&SND_InitVolumeTable_4EEF60) SND_InitVolumeTable;
#if !USE_SDL2_SOUND
    decltype(&SND_CreatePrimarySoundBuffer_4EEEC0) SND_CreatePrimarySoundBuffer;
#endif
    decltype(&SND_Renew_4EEDD0) SND_Renew;
    decltype(&SND_Get_Buffer_Status_4EE8F0) SND_Get_Buffer_Status;
    decltype(&SND_Stop_Sample_At_Idx_4EFA90) SND_Stop_Sample_At_Idx;
    decltype(&SND_Buffer_Set_Volume_4EFAD0) SND_Buffer_Set_Volume;
    decltype(&SND_Get_Sound_Buffer_4EF970) SND_Get_Sound_Buffer;
    decltype(&SND_Buffer_Set_Frequency_4EFC90) SND_Buffer_Set_Frequency1;
    decltype(&SND_Buffer_Set_Frequency_4EFC00) SND_Buffer_Set_Frequency2;
    decltype(&SND_LoadSamples_4EF1C0) SND_LoadSamples;
    decltype(&SND_4F00B0) SND_unknown;
    decltype(&SND_PlayEx_4EF740) SND_PlayEx;
};

EXPORT SoundApi& GetSoundAPI();

// Vars used only by other sound driver impls
ALIVE_ARY_EXTERN(s32, 127, sVolumeTable_BBBD38);
ALIVE_ARY_EXTERN(SoundEntry*, 256, sSoundSamples_BBBF38);
ALIVE_ARY_EXTERN(SoundBuffer, 32, sSoundBuffers_BBBAB8);

ALIVE_VAR_EXTERN(s32, sLoadedSoundsCount_BBC394);
ALIVE_VAR_EXTERN(s32, sLastNotePlayTime_BBC33C);
