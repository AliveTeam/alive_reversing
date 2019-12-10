#pragma once

#include "FunctionFwd.hpp"


#if !USE_SDL2_SOUND
#include <mmeapi.h>
#include <dsound.h>
#else
/*
 *  extended waveform format structure used for all non-PCM formats. this
 *  structure is common to all non-PCM formats.
 */
 // NOTE: Windows type
typedef struct tWAVEFORMATEX
{
    WORD        wFormatTag;         /* format type */
    WORD        nChannels;          /* number of channels (i.e. mono, stereo...) */
    DWORD       nSamplesPerSec;     /* sample rate */
    DWORD       nAvgBytesPerSec;    /* for buffer estimation */
    WORD        nBlockAlign;        /* block size of data */
    WORD        wBitsPerSample;     /* number of bits per sample of mono data */
    WORD        cbSize;             /* the count in bytes of the size of */
                                    /* extra information (after cbSize) */
} WAVEFORMATEX, *PWAVEFORMATEX, *NPWAVEFORMATEX, *LPWAVEFORMATEX;

/* flags for wFormatTag field of WAVEFORMAT */
#define WAVE_FORMAT_PCM     1

// Note: Windows SDound type
typedef struct _DSBUFFERDESC
{
    DWORD           dwSize;
    DWORD           dwFlags;
    DWORD           dwBufferBytes;
    DWORD           dwReserved;
    LPWAVEFORMATEX  lpwfxFormat;
#if DIRECTSOUND_VERSION >= 0x0700
    GUID            guid3DAlgorithm;
#endif
} DSBUFFERDESC, *LPDSBUFFERDESC;
typedef const DSBUFFERDESC *LPCDSBUFFERDESC;

#define FAILED(hr) (((HRESULT)(hr)) < 0)
#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)

#ifndef _HRESULT_DEFINED
typedef int HRESULT;

#define S_OK                            ((HRESULT)0L)
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

struct SoundEntry
{
    int field_0_tableIdx;
    TSoundBufferType* field_4_pDSoundBuffer;
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

struct SoundBuffer
{
    TSoundBufferType * field_0_pDSoundBuffer;
    int field_4;
    int field_8_sample_idx;
    int field_C;
    int field_10;
};
ALIVE_ASSERT_SIZEOF(SoundBuffer, 0x14);

EXPORT unsigned int CC SND_Get_Sound_Entry_Pos_4EF620(SoundEntry* pSoundEntry);
EXPORT int CC SND_Clear_4EF350(SoundEntry* pSoundEntry, unsigned int sampleOffset, unsigned int size);
EXPORT void CC SND_Init_Ambiance_4CB480();
EXPORT void CC SND_SsQuit_4EFD50();
EXPORT void CC SND_Reset_Ambiance_4CB4B0();
EXPORT signed int CC SND_CreateDS_4EEAA0(unsigned int sampleRate, int bitsPerSample, int isStereo);
EXPORT void CC SND_Init_WaveFormatEx_4EEA00(WAVEFORMATEX *pWaveFormat, int sampleRate, unsigned __int8 bitsPerSample, int isStereo);
EXPORT signed int CC SND_New_4EEFF0(SoundEntry *pSnd, int sampleLength, int sampleRate, int bitsPerSample, int isStereo);
EXPORT int CC SND_Load_4EF680(SoundEntry* pSnd, const void* pWaveData, int waveDataLen);
EXPORT signed int CC SND_Free_4EFA30(SoundEntry* pSnd);
EXPORT void CC SND_Restart_4CB0E0();
EXPORT int CC SND_SetPrimarySoundBufferFormat_4EE990(int sampleRate, int bitsPerSample, unsigned __int8 isStereo);
EXPORT void CC SND_InitVolumeTable_4EEF60();
EXPORT char CC SND_CreatePrimarySoundBuffer_4EEEC0(int sampleRate, int bitsPerSample, int isStereo);
EXPORT signed int CC SND_Renew_4EEDD0(SoundEntry *pSnd);
EXPORT int CC SND_Get_Buffer_Status_4EE8F0(int idx);
EXPORT signed int CC SND_Stop_Sample_At_Idx_4EFA90(int idx);
EXPORT signed int CC SND_Buffer_Set_Volume_4EFAD0(int idx, int vol);
EXPORT SoundBuffer* CC SND_Get_Sound_Buffer_4EF970(int sampleIdx, int field10);
EXPORT int CC SND_Buffer_Set_Frequency_4EFC90(int idx, float hzChangeFreq);
EXPORT int CC SND_Buffer_Set_Frequency_4EFC00(int idx, float freq);
EXPORT void CC Start_Sounds_For_Objects_In_Near_Cameras_4CBB60();
EXPORT signed int CC SND_LoadSamples_4EF1C0(const SoundEntry* pSnd, DWORD sampleOffset, unsigned char* pSoundBuffer, unsigned int sampleCount);
EXPORT DWORD * CC SND_4F00B0(unsigned int* /*a1*/, unsigned int /*a2*/, int /*a3*/);

struct MIDI_Struct1;
EXPORT int CC SND_PlayEx_4EF740(const SoundEntry* pSnd, int panLeft, int panRight, float freq, MIDI_Struct1* pMidiStru, int playFlags, int priority);

enum class CameraPos : __int16;
struct Path_TLV;
EXPORT void CC Start_Sounds_for_TLV_4CB530(CameraPos direction, Path_TLV* pTlv);

EXPORT void CC Start_Slig_sounds_4CB980(CameraPos direction, char kZero);

ALIVE_ARY_EXTERN(int, 127, sVolumeTable_BBBD38);
ALIVE_ARY_EXTERN(SoundEntry*, 256, sSoundSamples_BBBF38);
ALIVE_ARY_EXTERN(SoundBuffer, 32, sSoundBuffers_BBBAB8);

ALIVE_VAR_EXTERN(int, sLoadedSoundsCount_BBC394);
ALIVE_VAR_EXTERN(int, sLastNotePlayTime_BBC33C);
