#include "stdafx.h"
#include "Sound.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Error.hpp"
#include "Sys.hpp"

#include <mmeapi.h>
#include <timeapi.h>
#include <dsound.h>

struct SoundBuffer
{
    LPDIRECTSOUNDBUFFER8 field_0_pDSoundBuffer;
    int field_4;
    int field_8;
    int field_C;
    int field_10;
};
ALIVE_ASSERT_SIZEOF(SoundBuffer, 0x14);

struct SoundEntry
{
    int field_0_tableIdx;
    LPDIRECTSOUNDBUFFER8 field_4_pDSoundBuffer;
    BYTE* field_8_pSoundBuffer;
    int field_C_buffer_size_bytes;
    int field_10;
    int field_14_buffer_size_bytes;
    int field_18_sampleRate;
    char field_1C_bitsPerSample;
    unsigned __int8 field_1D_blockAlign;
    char field_1E;
    char field_1F;
    int field_20_flags;
};
ALIVE_ASSERT_SIZEOF(SoundEntry, 0x24);

ALIVE_VAR(1, 0xBBC344, LPDIRECTSOUND8, sDSound_BBC344, nullptr);
ALIVE_VAR(1, 0xBBC394, int, sLoadedSoundsCount_BBC394, 0);
ALIVE_VAR(1, 0xbbc388, LPDIRECTSOUNDBUFFER, sPrimarySoundBuffer_BBC388, 0);
ALIVE_VAR(1, 0xbbbab0, char, sPrimarySoundBufferChannels_BBBAB0, 0);
ALIVE_VAR(1, 0xbbc338, char, sPrimarySoundBufferBitsPerSample_BBC338, 0);
ALIVE_VAR(1, 0xbbc340, int, sPrimarySoundBufferSampleRate_BBC340, 0);
ALIVE_VAR(1, 0xbbc33c, int, sLastNotePlayTime_BBC33C, 0);


ALIVE_ARY(1, 0xBBBAB8, SoundBuffer, 32, sSoundBuffers_BBBAB8, {});
ALIVE_ARY(1, 0xBBBD38, int, 127, sVolumeTable_BBBD38, {});
ALIVE_ARY(1, 0xBBBF38, SoundEntry*, 256, sSoundSamples_BBBF38, {});
ALIVE_ARY(1, 0xBBC348, char, 64, sDSoundErrorBuffer_BBC348, {});

EXPORT void CC SND_Close_4EFD50()
{
    if (sDSound_BBC344)
    {
        for (int i = 0; i < 32; i++)
        {
            if (sSoundBuffers_BBBAB8[i].field_0_pDSoundBuffer)
            {
                sSoundBuffers_BBBAB8[i].field_0_pDSoundBuffer->Stop();
                sSoundBuffers_BBBAB8[i].field_0_pDSoundBuffer->Release();
                sSoundBuffers_BBBAB8[i].field_0_pDSoundBuffer = nullptr;
            }
        }

        for (int i = 0; i < sLoadedSoundsCount_BBC394; i++)
        {
            SoundEntry* pEntry = sSoundSamples_BBBF38[i];
            if (pEntry->field_4_pDSoundBuffer)
            {
                pEntry->field_4_pDSoundBuffer->Stop();
                pEntry->field_4_pDSoundBuffer->Release();
                pEntry->field_4_pDSoundBuffer = nullptr;
                pEntry->field_10 = 0;
            }
        }

        sDSound_BBC344->Release();
        sDSound_BBC344 = nullptr;
    }
}


EXPORT signed int CC SND_Free_4EFA30(SoundEntry* pSnd)
{
    if (!sDSound_BBC344)
    {
        return -1;
    }

    pSnd->field_10 = 0;

    if (pSnd->field_8_pSoundBuffer)
    {
        mem_free_4F4EA0(pSnd->field_8_pSoundBuffer);
        pSnd->field_8_pSoundBuffer = 0;
    }

    if (pSnd->field_4_pDSoundBuffer)
    {
        pSnd->field_4_pDSoundBuffer->Release();
        pSnd->field_4_pDSoundBuffer = nullptr;
    }

    sSoundSamples_BBBF38[pSnd->field_0_tableIdx] = nullptr;
    sLoadedSoundsCount_BBC394--;
    return 0;
}

EXPORT void CC SND_InitVolumeTable_4EEF60()
{
    for (int i = 0; i < 127; i++)
    {
        sVolumeTable_BBBD38[i] = static_cast<int>(min(max(log2f((i + 1) / 128.0f) / log2f(2.0f) * 1000.0f, -10000), 0));
    }
    sVolumeTable_BBBD38[0] = -10000;
}

EXPORT void CC SND_Init_WaveFormatEx_4EEA00(WAVEFORMATEX *pWaveFormat, int sampleRate, unsigned __int8 bitsPerSample, int channels)
{
    pWaveFormat->nSamplesPerSec = 0;
    pWaveFormat->nAvgBytesPerSec = 0;
    pWaveFormat->cbSize = 0;
    pWaveFormat->nSamplesPerSec = sampleRate;
    pWaveFormat->nChannels = (channels != 0) + 1;
    pWaveFormat->wBitsPerSample = bitsPerSample;
    pWaveFormat->wFormatTag = WAVE_FORMAT_PCM;
    pWaveFormat->nBlockAlign = bitsPerSample * ((channels != 0) + 1) / 8;
    pWaveFormat->nAvgBytesPerSec = sampleRate * pWaveFormat->nBlockAlign;
}

EXPORT char * CC SND_HR_Err_To_String_4EEC70(HRESULT hr)
{
    switch (hr)
    {
    case DSERR_INVALIDCALL:
        return "DSERR_INVALIDCALL: This function is not valid for the current state of this object.";
    case DSERR_PRIOLEVELNEEDED:
        return "DSERR_PRIOLEVELNEEDED: The caller does not have the priority level required for the function to succeed.";
    case DSERR_NODRIVER:
        return "DSERR_NODRIVER: No sound driver is available for use.";
    case DSERR_OTHERAPPHASPRIO:
        return "DSERR_OTHERAPPHASPRIO: This value is obsolete and is not used.";
    case DSERR_UNINITIALIZED:
        return "DSERR_UNINITIALIZED: The IDirectSound::Initialize method has not been called or has not been called successfully before other methods were called.";
    case DSERR_CONTROLUNAVAIL:
        return "DSERR_CONTROLUNAVAIL: The control (volume, pan, and so forth) requested by the caller is not available.";
    case DSERR_INVALIDPARAM:
        return "DSERR_INVALIDPARAM: An invalid parameter was passed to the returning function.";
    case DSERR_ALLOCATED:
        return "DSERR_ALLOCATED: The request failed because resources, such as a priority level, were already in use by another caller.";
    case DSERR_OUTOFMEMORY:
        return "DSERR_OUTOFMEMORY: The DirectSound subsystem could not allocate sufficient memory to complete the caller's request.";
    case DSERR_UNSUPPORTED:
        return "DSERR_UNSUPPORTED: The function called is not supported at this time. ";
    case DSERR_GENERIC:
        return "DSERR_GENERIC: An undetermined error occurred inside the DirectSound subsystem.";
    case DSERR_NOAGGREGATION:
        return "DSERR_NOAGGREGATION: The object does not support aggregation.";
    }

    sprintf(sDSoundErrorBuffer_BBC348, "DirectSound error %ld %lx", hr, hr);
    return sDSoundErrorBuffer_BBC348;
}

EXPORT int CC SND_SetPrimarySoundBufferFormat_4EE990(int sampleRate, int bitsPerSample, unsigned __int8 channels)
{
    WAVEFORMATEX pWaveFormat; // [esp+0h] [ebp-14h]

    if (!sPrimarySoundBuffer_BBC388)
        return -2;
    pWaveFormat.wFormatTag = 0;
    pWaveFormat.nSamplesPerSec = 0;
    pWaveFormat.nAvgBytesPerSec = 0;
    pWaveFormat.nBlockAlign = 0;
    pWaveFormat.cbSize = 0;
    SND_Init_WaveFormatEx_4EEA00(&pWaveFormat, sampleRate, bitsPerSample, channels);
    return -(sPrimarySoundBuffer_BBC388->SetFormat(&pWaveFormat) != 0);
}

EXPORT char CC SND_CreatePrimarySoundBuffer_4EEEC0(int sampleRate, int bitsPerSample, int channels)
{
    DSBUFFERDESC v4; // [esp+8h] [ebp-14h]

    v4.dwSize = 20;
    v4.dwBufferBytes = 0;
    v4.dwFlags = 1;
    v4.dwReserved = 0;
    v4.lpwfxFormat = 0;
    if (sDSound_BBC344->CreateSoundBuffer(&v4, &sPrimarySoundBuffer_BBC388, 0))
        return -1;
    if (!sPrimarySoundBuffer_BBC388->Play(0, 0, 1))
        return SND_SetPrimarySoundBufferFormat_4EE990(sampleRate, bitsPerSample, channels) != 0 ? 0xFD : 0;
    sPrimarySoundBuffer_BBC388->Release();
    return -2;
}

EXPORT signed int CC SND_Renew_4EEDD0(SoundEntry *pSnd)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT signed int CC SND_Reload_4EF1C0(SoundEntry *pSnd, char *a2, unsigned char *pSoundBuffer, unsigned int a4)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT signed int CC SND_CreateDS_4EEAA0(unsigned int sampleRate, int bitsPerSample, int channels)
{
    if (sDSound_BBC344)
    {
        return 0;
    }

    HRESULT dsoundHR = DirectSoundCreate8(0, &sDSound_BBC344, 0);

    if (dsoundHR)
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\SND.C", 471, -1, SND_HR_Err_To_String_4EEC70(dsoundHR));
        return -1;
    }
    else
    {
        HWND hwnd = Sys_GetHWnd_4F2C70();
        if (hwnd)
        {
            if (sDSound_BBC344->SetCooperativeLevel(hwnd, DSSCL_EXCLUSIVE))
            {
                Error_PushErrorRecord_4F2920(
                    "C:\\abe2\\code\\POS\\SND.C",
                    483,
                    -1,
                    "SND_Init(): IDirectSound_SetCooperativeLevel failed");
                return -1;
            }
            else
            {
                DSCAPS dsCaps;
                dsCaps.dwSize = 96;
                if (!sDSound_BBC344->GetCaps(&dsCaps))
                {
                    if (dsCaps.dwFlags & DSCAPS_PRIMARY16BIT)
                    {
                        bitsPerSample = 16;
                    }
                    else if (dsCaps.dwFlags & DSCAPS_PRIMARY8BIT)
                    {
                        bitsPerSample = 8;
                    }

                    if (dsCaps.dwFlags & DSCAPS_PRIMARYSTEREO)
                    {
                        if (!(dsCaps.dwFlags & DSCAPS_PRIMARYMONO))
                        {
                            channels = 1;
                        }
                    }
                    else
                    {
                        channels = 0;
                    }
                    if (dsCaps.dwPlayCpuOverheadSwBuffers > 5)
                    {
                        if (dsCaps.dwPlayCpuOverheadSwBuffers > 10)
                        {
                            int newBitsPerSample = bitsPerSample;
                            if (dsCaps.dwPlayCpuOverheadSwBuffers > 20)
                            {
                                newBitsPerSample = bitsPerSample >> 1;
                            }
                            SND_CreatePrimarySoundBuffer_4EEEC0(sampleRate >> 1, newBitsPerSample, 0);
                        }
                        else
                        {
                            SND_CreatePrimarySoundBuffer_4EEEC0(sampleRate >> 1, bitsPerSample, channels);
                        }
                    }
                    else
                    {
                        SND_CreatePrimarySoundBuffer_4EEEC0(sampleRate, bitsPerSample, channels);
                    }
                }

                sPrimarySoundBufferSampleRate_BBC340 = sampleRate;
                sPrimarySoundBufferBitsPerSample_BBC338 = bitsPerSample;
                sPrimarySoundBufferChannels_BBBAB0 = channels;

                SND_InitVolumeTable_4EEF60();

                if (sLoadedSoundsCount_BBC394)
                {
                    for (int i = 0; i < 256; i++)
                    {
                        if (sSoundSamples_BBBF38[i])
                        {
                            SND_Renew_4EEDD0(sSoundSamples_BBBF38[i]);
                            SND_Reload_4EF1C0(sSoundSamples_BBBF38[i], 0, sSoundSamples_BBBF38[i]->field_8_pSoundBuffer, sSoundSamples_BBBF38[i]->field_C_buffer_size_bytes / (unsigned __int8)sSoundSamples_BBBF38[i]->field_1D_blockAlign);
                            if ((i + 1) == sLoadedSoundsCount_BBC394)
                                break;
                        }
                    }
                }
                sLastNotePlayTime_BBC33C = timeGetTime();
                return 0;
            }
        }
        else
        {
            Error_PushErrorRecord_4F2920(
                "C:\\abe2\\code\\POS\\SND.C",
                477,
                -1,
                "SND_Init(): SYS_GetOutputDevice() == NULL !!");
            return -1;
        }
    }
}