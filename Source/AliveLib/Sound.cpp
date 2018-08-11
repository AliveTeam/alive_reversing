#include "stdafx.h"
#include "Sound.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Error.hpp"
#include "Sys.hpp"
#include "Midi.hpp"
#include "BackgroundMusic.hpp"
#include <mmeapi.h>
#include <timeapi.h>
#include <dsound.h>
#include "Path.hpp"
#include "MusicController.hpp"

struct SoundBuffer
{
    LPDIRECTSOUNDBUFFER field_0_pDSoundBuffer;
    int field_4;
    int field_8;
    int field_C;
    int field_10;
};
ALIVE_ASSERT_SIZEOF(SoundBuffer, 0x14);

ALIVE_VAR(1, 0xBBC344, LPDIRECTSOUND, sDSound_BBC344, nullptr);
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

EXPORT void CC SND_SsQuit_4EFD50()
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

EXPORT void CC SND_Init_WaveFormatEx_4EEA00(WAVEFORMATEX *pWaveFormat, int sampleRate, unsigned __int8 bitsPerSample, int isStereo)
{
    pWaveFormat->nSamplesPerSec = 0;
    pWaveFormat->nAvgBytesPerSec = 0;
    pWaveFormat->cbSize = 0;
    pWaveFormat->nSamplesPerSec = sampleRate;
    pWaveFormat->nChannels = (isStereo != 0) + 1;
    pWaveFormat->wBitsPerSample = bitsPerSample;
    pWaveFormat->wFormatTag = WAVE_FORMAT_PCM;
    pWaveFormat->nBlockAlign = bitsPerSample * ((isStereo != 0) + 1) / 8;
    pWaveFormat->nAvgBytesPerSec = sampleRate * pWaveFormat->nBlockAlign;
}

EXPORT char * CC SND_HR_Err_To_String_4EEC70(HRESULT hr)
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

EXPORT int CC SND_SetPrimarySoundBufferFormat_4EE990(int sampleRate, int bitsPerSample, unsigned __int8 isStereo)
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
    SND_Init_WaveFormatEx_4EEA00(&pWaveFormat, sampleRate, static_cast<unsigned char>(bitsPerSample), isStereo);
    return -(sPrimarySoundBuffer_BBC388->SetFormat(&pWaveFormat) != 0);
}

EXPORT char CC SND_CreatePrimarySoundBuffer_4EEEC0(int sampleRate, int bitsPerSample, int isStereo)
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
        return SND_SetPrimarySoundBufferFormat_4EE990(sampleRate, bitsPerSample, static_cast<unsigned char>(isStereo)) != 0 ? 0xFD : 0;
    }

    sPrimarySoundBuffer_BBC388->Release();
    return -2;
}

EXPORT signed int CC SND_Renew_4EEDD0(SoundEntry *pSnd)
{
    if (!sDSound_BBC344)
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\SND.C", 351, -1, "DirectSound not initialized");
        return -1;
    }

    WAVEFORMATEX waveFormat;
    DSBUFFERDESC bufferDesc;

    waveFormat.wFormatTag = 0;
    waveFormat.nSamplesPerSec = 0;
    waveFormat.nAvgBytesPerSec = 0;
    waveFormat.nBlockAlign = 0;
    waveFormat.cbSize = 0;

    SND_Init_WaveFormatEx_4EEA00(&waveFormat, pSnd->field_18_sampleRate, pSnd->field_1C_bitsPerSample, pSnd->field_20_isStereo & 1);

    bufferDesc.dwBufferBytes = pSnd->field_14_buffer_size_bytes;
    bufferDesc.dwReserved = 0;
    bufferDesc.lpwfxFormat = &waveFormat;
    bufferDesc.dwSize = 20;
    bufferDesc.dwFlags = 82144; // TODO: Fix constants

    if (sDSound_BBC344->CreateSoundBuffer(&bufferDesc, &pSnd->field_4_pDSoundBuffer, 0))
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\SND.C", 371, -1, "SND_Renew(): Cannot create ds sound buffer");
        return -1;
    }
    else
    {
        pSnd->field_10 = 0;
        return 0;
    }
}

// Never seems to get called?
// TODO: Clean up!
EXPORT DWORD * CC SND_4F00B0(unsigned int* /*a1*/, unsigned int /*a2*/, int /*a3*/)
{
    NOT_IMPLEMENTED();
    return 0;
    //DWORD *result; // eax
    //unsigned int *v4; // edx
    //unsigned int v5; // ecx
    //int v6; // esi
    //int v7; // esi

    //result = (DWORD *)a2;
    //v4 = a1;
    //v5 = a2 + a3;
    //if (a2 < a2 + a3)
    //{
    //    do
    //    {
    //        v6 = *result;
    //        ++result;
    //        *v4 = v6 ^ 0x80808080;
    //        ++v4;
    //    } while ((unsigned int)result < v5);
    //    if ((unsigned int)result < v5)
    //    {
    //        v7 = (char *)v4 - (char *)result;
    //        do
    //        {
    //            *((BYTE *)result + v7) = *(BYTE *)result ^ 0x80;
    //            result = (DWORD *)((char *)result + 1);
    //        } while ((unsigned int)result < v5);
    //    }
    //}
    //return result;
}

// TODO: Clean up!
EXPORT signed int CC SND_Reload_4EF1C0(const SoundEntry* pSnd, char* sampleOffset, unsigned char* pSoundBuffer, unsigned int sampleCount)
{
    const int offsetBytes = (DWORD)sampleOffset * pSnd->field_1D_blockAlign;
    const unsigned int bufferSizeBytes = sampleCount * pSnd->field_1D_blockAlign;

    if (!sDSound_BBC344)
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\SND.C", 644, -1, "DirectSound not initialized");
        return -1;
    }

    unsigned int *leftChannelBuffer;
    int leftChannelSize; 
    char * rightChannelBuffer;
    int rightChannelSize; 

    int lockHR = pSnd->field_4_pDSoundBuffer->Lock(offsetBytes, bufferSizeBytes, (LPVOID *)&leftChannelBuffer, (LPDWORD)&leftChannelSize, (LPVOID *)&rightChannelBuffer, (LPDWORD)&rightChannelSize, 0);

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
            SND_4F00B0(leftChannelBuffer, (unsigned int)pSoundBuffer, (int)leftChannelSize);
        }
        if (rightChannelBuffer)
        {
            SND_4F00B0((unsigned int *)rightChannelBuffer, (DWORD)pSoundBuffer + (unsigned int)leftChannelSize, rightChannelSize);
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

EXPORT signed int CC SND_CreateDS_4EEAA0(unsigned int sampleRate, int bitsPerSample, int isStereo)
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
                            int newBitsPerSample = bitsPerSample;
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
                sPrimarySoundBufferBitsPerSample_BBC338 = static_cast<char>(bitsPerSample);
                sPrimarySoundBufferChannels_BBBAB0 = static_cast<char>(isStereo);

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

EXPORT signed int CC SND_New_4EEFF0(SoundEntry *pSnd, int sampleLength, int sampleRate, int bitsPerSample, int isStereo)
{
    if (!sDSound_BBC344)
    {
        return -1;
    }

    if (sLoadedSoundsCount_BBC394 < 256)
    {
        WAVEFORMATEX waveFormatEx;
        DSBUFFERDESC bufferDesc;

        waveFormatEx.wFormatTag = 0;
        waveFormatEx.nSamplesPerSec = 0;
        waveFormatEx.nAvgBytesPerSec = 0;
        waveFormatEx.nBlockAlign = 0;
        waveFormatEx.cbSize = 0;
        SND_Init_WaveFormatEx_4EEA00(&waveFormatEx, sampleRate, static_cast<unsigned char>(bitsPerSample), isStereo & 1);

        const int sampleByteSize = sampleLength * waveFormatEx.nBlockAlign;
        bufferDesc.dwReserved = 0;
        bufferDesc.lpwfxFormat = &waveFormatEx;
        pSnd->field_1D_blockAlign = static_cast<unsigned char>(waveFormatEx.nBlockAlign);
        bufferDesc.dwSize = 20;
        bufferDesc.dwBufferBytes = sampleByteSize;
        bufferDesc.dwFlags = 82152;

        const HRESULT sbHR = sDSound_BBC344->CreateSoundBuffer(&bufferDesc, &pSnd->field_4_pDSoundBuffer, 0);

        if (sbHR)
        {
            Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\SND.C", 598, -1, "SND_New(): Cannot create ds sound buffer");
            Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\SND.C", 599, -1, SND_HR_Err_To_String_4EEC70(sbHR));
            return -1;
        }
        else
        {
            pSnd->field_10 = 0;
            unsigned char * bufferData = static_cast<unsigned char *>(malloc_4F4E60(sampleByteSize));
            pSnd->field_8_pSoundBuffer = bufferData;
            if (bufferData)
            {
                pSnd->field_18_sampleRate = sampleRate;
                pSnd->field_1C_bitsPerSample = static_cast<char>(bitsPerSample);
                pSnd->field_C_buffer_size_bytes = sampleByteSize;
                pSnd->field_14_buffer_size_bytes = sampleByteSize;
                pSnd->field_20_isStereo = isStereo;

                for (int i = 0; i < 256; i++)
                {
                    if (!sSoundSamples_BBBF38[i])
                    {
                        sSoundSamples_BBBF38[i] = pSnd;
                        pSnd->field_0_tableIdx = i;
                        sLoadedSoundsCount_BBC394++;
                        return 0;
                    }
                }

                return 0; // No free spaces left. Should never get here as all calls to Snd_NEW are checked before hand.
            }
            else
            {
                pSnd->field_4_pDSoundBuffer->Release();
                Error_PushErrorRecord_4F2920(
                    "C:\\abe2\\code\\POS\\SND.C",
                    608,
                    -1,
                    "SND_New(): Cannot create original data sound buffer");
                return -1;
            }
        }
    }
    else
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\SND.C", 568, -1, "SND_New: out of samples");
        return -1;
    }
}

EXPORT int CC SND_Load_4EF680(SoundEntry* pSnd, const void* pWaveData, int waveDataLen)
{
    SND_Free_4EFA30(pSnd);
    if (!SND_New_4EEFF0(pSnd, waveDataLen, pSnd->field_18_sampleRate, pSnd->field_1C_bitsPerSample, pSnd->field_20_isStereo))
    {
        if (waveDataLen * pSnd->field_1D_blockAlign > pSnd->field_C_buffer_size_bytes)
        {
            Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\SND.C", 804, -1, "SND_Load(): data too big !!");
            return -1;
        }

        if (pSnd->field_8_pSoundBuffer)
        {
            memcpy(pSnd->field_8_pSoundBuffer, pWaveData, waveDataLen * pSnd->field_1D_blockAlign);
        }
    }
    return SND_Reload_4EF1C0(pSnd, nullptr, pSnd->field_8_pSoundBuffer, pSnd->field_C_buffer_size_bytes / pSnd->field_1D_blockAlign);
}

EXPORT int CC SND_Buffer_Set_Frequency_4EFC90(int idx, float hzChangeFreq)
{
    SoundBuffer* pSoundBuffer = &sSoundBuffers_BBBAB8[idx & 0x1FF];
    IDirectSoundBuffer* pDSoundBuffer = pSoundBuffer->field_0_pDSoundBuffer;

    if (!pDSoundBuffer || ((idx ^ pSoundBuffer->field_4) & 0xFFFFFE00)) // TODO: Refactor
    {
        return -1;
    }

    if (!sSoundSamples_BBBF38[pSoundBuffer->field_8])
    {
        return -2;
    }

    DWORD currrentFreqHz = 0;
    if (SUCCEEDED(pDSoundBuffer->GetFrequency(&currrentFreqHz)))
    {
        DWORD freqHz = static_cast<DWORD>(currrentFreqHz * hzChangeFreq);
        if (freqHz < DSBFREQUENCY_MIN)
        {
            freqHz = DSBFREQUENCY_MIN;
        }
        else if (freqHz >= DSBFREQUENCY_MAX)
        {
            freqHz = DSBFREQUENCY_MAX;
        }
        pSoundBuffer->field_0_pDSoundBuffer->SetFrequency(freqHz);
    }
    return 0;
}

EXPORT int CC SND_Buffer_Get_Status_4F00F0(int /*idx*/, int /*a2*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT SoundBuffer* CC SND_Recycle_Sound_Buffer_4EF9C0(int /*idx*/, int /*field8*/, int /*field10*/)
{
    NOT_IMPLEMENTED();
    return nullptr;
}

EXPORT SoundBuffer* CC SND_Get_Sound_Buffer_4EF970(int tableIdx, int field10)
{
    int idx = -1;
    int statusToMatch = 0xC0000000;

    for (int i = 0; i < 32; i++)
    {
        int status = SND_Buffer_Get_Status_4F00F0(i, field10);
        if (status >= statusToMatch)
        {
            idx = i;
            statusToMatch = status;
        }
    }

    if (idx < 0)
    {
        return &sSoundBuffers_BBBAB8[0];
    }
    return SND_Recycle_Sound_Buffer_4EF9C0(idx, tableIdx, field10);
}

const DWORD k127_dword_575158 = 127;

EXPORT int CC SND_PlayEx_4EF740(const SoundEntry* pSnd, int panLeft, int panRight, float freq, MIDI_Struct1* pMidiStru, int playFlags, int volume)
{
    if (!sDSound_BBC344)
    {
        return -1;
    }

    if (!pSnd)
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\SND.C", 845, -1, "SND_PlayEx: NULL SAMPLE !!!");
        return -1;
    }

    IDirectSoundBuffer* pDSoundBuffer = pSnd->field_4_pDSoundBuffer;
    if (!pDSoundBuffer)
    {
        return -1;
    }

    sLastNotePlayTime_BBC33C = timeGetTime();

    int panLeft2 = panLeft;
    int panRight2 = panRight;
    if (panLeft > panRight)
    {
        panRight2 = panLeft;
    }

    int panRightConverted = 120 * panRight2 * k127_dword_575158 >> 14;// >> 14 = 16384
    if (panRightConverted < 0)
    {
        panRightConverted = 0;
    }
    else if (panRightConverted > 127)
    {
        panRightConverted = 127;
    }

    if (pSnd->field_20_isStereo & 2)
    {
        DWORD status = 0;
        if (FAILED(pDSoundBuffer->GetStatus(&status)))
        {
            return -1;
        }
        
        if (status & DSBSTATUS_PLAYING)
        {
            pDSoundBuffer->SetFrequency(static_cast<DWORD>((pSnd->field_18_sampleRate * freq) + 0.5)); // This freq don't get clamped for some reason
            pDSoundBuffer->SetVolume(sVolumeTable_BBBD38[panRightConverted]);
            pDSoundBuffer->SetCurrentPosition(0);
            return 0;
        }
    }
    else
    {
        SoundBuffer* pSoundBuffer = SND_Get_Sound_Buffer_4EF970(pSnd->field_0_tableIdx, panRightConverted + volume);
        if (!pSoundBuffer)
        {
            return -1;
        }

        HRESULT v15 = sDSound_BBC344->DuplicateSoundBuffer(pDSoundBuffer, &pSoundBuffer->field_0_pDSoundBuffer);
        if (FAILED(v15))
        {
            Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\SND.C", 921, -1, SND_HR_Err_To_String_4EEC70(v15));
            return -1;
        }

        pDSoundBuffer = pSoundBuffer->field_0_pDSoundBuffer;
        
        if (pMidiStru)
        {
            pMidiStru->field_0_sound_buffer_field_4 = pSoundBuffer->field_4;
        }
    }

    DWORD freqHz = static_cast<DWORD>((pSnd->field_18_sampleRate * freq) + 0.5);
    if (freqHz < DSBFREQUENCY_MIN)
    {
        freqHz = DSBFREQUENCY_MIN;
    }
    else if (freqHz >= DSBFREQUENCY_MAX)
    {
        freqHz = DSBFREQUENCY_MAX;
    }

    pDSoundBuffer->SetFrequency(freqHz);
    pDSoundBuffer->SetVolume(sVolumeTable_BBBD38[panRightConverted]);
    
    const int panConverted = (DSBPAN_RIGHT * (panLeft2 - panRight)) / 127; // From PSX pan range to DSound pan range
    pDSoundBuffer->SetPan(panConverted);
    
    if (playFlags & DSBPLAY_LOOPING)
    {
        playFlags = DSBPLAY_LOOPING;
    }

    HRESULT hr = pDSoundBuffer->Play(0, 0, playFlags);
    if (SUCCEEDED(hr))
    {
        return 0;
    }

    if (hr == DSERR_BUFFERLOST)
    {
        // Restore the lost buffer
        if (SND_Reload_4EF1C0(pSnd, 0, pSnd->field_8_pSoundBuffer, pSnd->field_C_buffer_size_bytes / pSnd->field_1D_blockAlign) == 0)
        {
            // Try again
            if (SUCCEEDED(pDSoundBuffer->Play(0, 0, playFlags)))
            {
                return 0;
            }
        }
    }

    return -1;
}

EXPORT void CC SND_Init_Buffers_4CB480()
{
    // Note: Not empty in real but does nothing
}

EXPORT void CC SND_Clear_4CB4B0()
{
    // Note: Not empty in real but does nothing
}

EXPORT void CC Start_Sounds_For_Objects_In_Near_Cameras_4CBB60()
{
    SND_Clear_4CB4B0();
    Path::Start_Sounds_For_Objects_In_Camera_4CBAF0(3, -1, 0);    // left
    Path::Start_Sounds_For_Objects_In_Camera_4CBAF0(4, 1, 0);     // right
    Path::Start_Sounds_For_Objects_In_Camera_4CBAF0(1, 0, -1);    // bottom
    Path::Start_Sounds_For_Objects_In_Camera_4CBAF0(2, 0, 1);     // top
}

EXPORT void CC SND_Restart_4CB0E0()
{
    MusicController::sub_47FE10(1);
    BackgroundMusic::Play_4CB030();
    Start_Sounds_For_Objects_In_Near_Cameras_4CBB60();
}
