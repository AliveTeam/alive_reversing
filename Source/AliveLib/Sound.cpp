#include "stdafx.h"
#include "Sound.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include <mmeapi.h>
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
    char *result; // eax

    if (hr > DSERR_CONTROLUNAVAIL)
    {
        switch (hr)
        {
        case DSERR_INVALIDCALL:
            result = "DSERR_INVALIDCALL: This function is not valid for the current state of this object.";
            break;
        case DSERR_PRIOLEVELNEEDED:
            result = "DSERR_PRIOLEVELNEEDED: The caller does not have the priority level required for the function to succeed.";
            break;
        case DSERR_NODRIVER:
            result = "DSERR_NODRIVER: No sound driver is available for use.";
            break;
        case DSERR_OTHERAPPHASPRIO:
            result = "DSERR_OTHERAPPHASPRIO: This value is obsolete and is not used.";
            break;
        case DSERR_UNINITIALIZED:
            result = "DSERR_UNINITIALIZED: The IDirectSound::Initialize method has not been called or has not been called suc"
                "cessfully before other methods were called.";
            break;
        default:
            goto LABEL_23;
        }
    }
    else if (hr == DSERR_CONTROLUNAVAIL)
    {
        result = "DSERR_CONTROLUNAVAIL: The control (volume, pan, and so forth) requested by the caller is not available.";
    }
    else
    {
        if (hr > DSERR_OUTOFMEMORY)
        {
            if (hr == DSERR_INVALIDPARAM)
                return "DSERR_INVALIDPARAM: An invalid parameter was passed to the returning function.";
            if (hr == DSERR_ALLOCATED)
                return "DSERR_ALLOCATED: The request failed because resources, such as a priority level, were already in use by another caller.";
        }
        else
        {
            switch (hr)
            {
            case DSERR_OUTOFMEMORY:
                return "DSERR_OUTOFMEMORY: The DirectSound subsystem could not allocate sufficient memory to complete the caller's request.";
            case DSERR_UNSUPPORTED:
                return "DSERR_UNSUPPORTED: The function called is not supported at this time. ";
            case DSERR_GENERIC:
                return "DSERR_GENERIC: An undetermined error occurred inside the DirectSound subsystem.";
            case DSERR_NOAGGREGATION:
                return "DSERR_NOAGGREGATION: The object does not support aggregation.";
            }
        }
    LABEL_23:
        if (hr)
        {
            sprintf(sDSoundErrorBuffer_BBC348, "DirectSound error %ld %lx", hr, hr);
            result = sDSoundErrorBuffer_BBC348;
        }
        else
        {
            result = "";
        }
    }
    return result;
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