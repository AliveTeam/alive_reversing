#include "stdafx.h"
#include "SoundDSound.hpp"
#include "Function.hpp"
#include "Sys.hpp"
#include "stdlib.hpp"
#include "Error.hpp"

#if !USE_SDL2_SOUND

#include <mmeapi.h>
#include <timeapi.h>

ALIVE_VAR(1, 0xbbc388, LPDIRECTSOUNDBUFFER, sPrimarySoundBuffer_BBC388, 0);

ALIVE_VAR(1, 0xbbc340, s32, sPrimarySoundBufferSampleRate_BBC340, 0);
ALIVE_VAR(1, 0xbbbab0, s8, sPrimarySoundBufferChannels_BBBAB0, 0);
ALIVE_VAR(1, 0xbbc338, s8, sPrimarySoundBufferBitsPerSample_BBC338, 0);

ALIVE_ARY(1, 0xBBC348, s8, 64, sDSoundErrorBuffer_BBC348, {});

const s8* SND_HR_Err_To_String_DSound(HRESULT hr)
{
    switch (hr)
    {
    case S_OK:
        return "";
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

    if (FAILED(hr))
    {
        sprintf(sDSoundErrorBuffer_BBC348, "DirectSound error %ld %lx", hr, hr);
        return sDSoundErrorBuffer_BBC348;
    }

    return "";
}

s32 CC SND_CreateDS_DSound(u32 sampleRate, s32 bitsPerSample, s32 isStereo)
{
    if (sDSound_BBC344)
    {
        // Already created
        return 0;
    }

    HRESULT dsoundHR = DirectSoundCreate(0, &sDSound_BBC344, 0);

    if (dsoundHR)
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\SND.C", 471, -1, SND_HR_Err_To_String_4EEC70(dsoundHR));
        return -1;
    }
    else
    {
        // TODO: HACK
        HWND hwnd;
#if USE_SDL2
        hwnd = Sys_Win32FromSDLWindow(Sys_GetHWnd_4F2C70());
#else
        hwnd = Sys_GetHWnd_4F2C70();
#endif

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
                            isStereo = 1;
                        }
                    }
                    else
                    {
                        isStereo = 0;
                    }
                    if (dsCaps.dwPlayCpuOverheadSwBuffers > 5)
                    {
                        if (dsCaps.dwPlayCpuOverheadSwBuffers > 10)
                        {
                            s32 newBitsPerSample = bitsPerSample;
                            if (dsCaps.dwPlayCpuOverheadSwBuffers > 20)
                            {
                                newBitsPerSample = bitsPerSample >> 1;
                            }
                            SND_CreatePrimarySoundBuffer_4EEEC0(sampleRate >> 1, newBitsPerSample, 0);
                        }
                        else
                        {
                            SND_CreatePrimarySoundBuffer_4EEEC0(sampleRate >> 1, bitsPerSample, isStereo);
                        }
                    }
                    else
                    {
                        SND_CreatePrimarySoundBuffer_4EEEC0(sampleRate, bitsPerSample, isStereo);
                    }
                }

                sPrimarySoundBufferSampleRate_BBC340 = sampleRate;
                sPrimarySoundBufferBitsPerSample_BBC338 = static_cast<s8>(bitsPerSample);
                sPrimarySoundBufferChannels_BBBAB0 = static_cast<s8>(isStereo);

                SND_InitVolumeTable_4EEF60();

                if (sLoadedSoundsCount_BBC394)
                {
                    for (s32 i = 0; i < 256; i++)
                    {
                        if (sSoundSamples_BBBF38[i])
                        {
                            SND_Renew_4EEDD0(sSoundSamples_BBBF38[i]);
                            SND_LoadSamples_4EF1C0(sSoundSamples_BBBF38[i], 0, sSoundSamples_BBBF38[i]->field_8_pSoundBuffer, sSoundSamples_BBBF38[i]->field_C_buffer_size_bytes / (u8)sSoundSamples_BBBF38[i]->field_1D_blockAlign);
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


s32 CC SND_Clear_DSound(SoundEntry* pSoundEntry, u32 sampleOffset, u32 size)
{
    if (!sDSound_BBC344)
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\SND.C", 692, -1, "DirectSound not initialized");
        return -1;
    }

    const u32 alignedOffset = sampleOffset * pSoundEntry->field_1D_blockAlign;
    const u32 alignedSize = size * pSoundEntry->field_1D_blockAlign;

    LPVOID pLocked1 = nullptr;
    u32 locked1Size = 0;

    LPVOID pLocked2 = nullptr;
    u32 locked2Size = 0;

    HRESULT hr = pSoundEntry->field_4_pDSoundBuffer->Lock(
        alignedOffset,
        alignedSize,
        &pLocked1,
        &locked1Size,
        &pLocked2,
        &locked2Size,
        0);

    if (hr == DSERR_BUFFERLOST)
    {
        if (FAILED(pSoundEntry->field_4_pDSoundBuffer->Restore()))
        {
            Error_PushErrorRecord_4F2920(
                "C:\\abe2\\code\\POS\\SND.C",
                702,
                -1,
                "SND_Reload(): Cannot restore the lost ds buffer");
            return -1;
        }

        hr = pSoundEntry->field_4_pDSoundBuffer->Lock(
            alignedOffset,
            alignedSize,
            &pLocked1,
            &locked1Size,
            &pLocked2,
            &locked2Size,
            0);
    }

    if (FAILED(hr))
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\SND.C", 710, -1, "SND_Reload(): Cannot lock the ds buffer");
        return -1;
    }

    if (pLocked1)
    {
        u8* ptr = (u8*)pLocked1;
        memset(ptr, 0, locked1Size);
    }

    if (pLocked2)
    {
        u8* ptr = (u8*)pLocked2;
        memset(ptr, 0, locked2Size);
    }

    pSoundEntry->field_4_pDSoundBuffer->Unlock(pLocked1, locked1Size, pLocked2, locked2Size);
    return 0;
}

void SND_InitVolumeTable_DSound()
{
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

    for (s32 i = 0; i < 127; i++)
    {
        sVolumeTable_BBBD38[i] = static_cast<s32>(min(max(log2f((i + 1) / 128.0f) / log2f(2.0f) * 1000.0f, -10000), 0));
    }
    sVolumeTable_BBBD38[0] = -10000;
}

EXPORT s8 CC SND_CreatePrimarySoundBuffer_4EEEC0(s32 sampleRate, s32 bitsPerSample, s32 isStereo)
{
    DSBUFFERDESC bufferDesc = {};
    bufferDesc.dwSize = sizeof(DSBUFFERDESC);
    bufferDesc.dwBufferBytes = 0;
    bufferDesc.dwFlags = 1;
    bufferDesc.dwReserved = 0;
    bufferDesc.lpwfxFormat = 0;

    if (FAILED(sDSound_BBC344->CreateSoundBuffer(&bufferDesc, &sPrimarySoundBuffer_BBC388, nullptr)))
    {
        return -1;
    }

    if (SUCCEEDED(sPrimarySoundBuffer_BBC388->Play(0, 0, 1)))
    {
        return SND_SetPrimarySoundBufferFormat_4EE990(sampleRate, bitsPerSample, static_cast<u8>(isStereo)) != 0 ? 0xFD : 0;
    }

    sPrimarySoundBuffer_BBC388->Release();
    return -2;
}

EXPORT s32 CC SND_SetPrimarySoundBufferFormat_4EE990(s32 sampleRate, s32 bitsPerSample, u8 isStereo)
{
    WAVEFORMATEX pWaveFormat = {};

    if (!sPrimarySoundBuffer_BBC388)
    {
        return -2;
    }

    pWaveFormat.wFormatTag = 0;
    pWaveFormat.nSamplesPerSec = 0;
    pWaveFormat.nAvgBytesPerSec = 0;
    pWaveFormat.nBlockAlign = 0;
    pWaveFormat.cbSize = 0;
    SND_Init_WaveFormatEx_4EEA00(&pWaveFormat, sampleRate, static_cast<u8>(bitsPerSample), isStereo);
    return -(sPrimarySoundBuffer_BBC388->SetFormat(&pWaveFormat) != 0);
}

s32 CC SND_LoadSamples_DSound(const SoundEntry* pSnd, u32 sampleOffset, u8* pSoundBuffer, u32 sampleCount)
{
    const s32 offsetBytes = sampleOffset * pSnd->field_1D_blockAlign;
    const u32 bufferSizeBytes = sampleCount * pSnd->field_1D_blockAlign;

    if (!sDSound_BBC344)
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\SND.C", 644, -1, "DirectSound not initialized");
        return -1;
    }

    u32 *leftChannelBuffer;
    s32 leftChannelSize;
    s8 * rightChannelBuffer;
    s32 rightChannelSize;

    s32 lockHR = pSnd->field_4_pDSoundBuffer->Lock(offsetBytes, bufferSizeBytes, (LPVOID *)&leftChannelBuffer, (LPDWORD)&leftChannelSize, (LPVOID *)&rightChannelBuffer, (LPDWORD)&rightChannelSize, 0);

    if (lockHR == DSERR_BUFFERLOST)
    {
        if (pSnd->field_4_pDSoundBuffer->Restore())
        {
            Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\SND.C", 654, -1, "SND_Reload(): Cannot restore the lost ds buffer");
            return -1;
        }
        lockHR = pSnd->field_4_pDSoundBuffer->Lock(offsetBytes, bufferSizeBytes, (LPVOID *)&leftChannelBuffer, (LPDWORD)&leftChannelSize, (LPVOID *)&rightChannelBuffer, (LPDWORD)&rightChannelSize, 0);
    }

    if (lockHR)
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\SND.C", 662, -1, "SND_Reload(): Cannot lock the ds buffer");
        return -1;
    }

    if (pSnd->field_1C_bitsPerSample == 8) // All samples are 16 bits per sample, so....
    {
        if (leftChannelBuffer)
        {
            SND_4F00B0(leftChannelBuffer, (u32)pSoundBuffer, (s32)leftChannelSize);
        }
        if (rightChannelBuffer)
        {
            SND_4F00B0((u32 *)rightChannelBuffer, (u32)pSoundBuffer + (u32)leftChannelSize, rightChannelSize);
        }
    }
    else
    {
        if (leftChannelBuffer)
        {
            memcpy(leftChannelBuffer, pSoundBuffer, leftChannelSize);
        }
        if (rightChannelBuffer) // dual buffers never really exist, so this is always null
        {
            memcpy(rightChannelBuffer, &pSoundBuffer[leftChannelSize], rightChannelSize);
        }
    }

    pSnd->field_4_pDSoundBuffer->Unlock(leftChannelBuffer, leftChannelSize, rightChannelBuffer, rightChannelSize);

    return 0;
}

#endif
