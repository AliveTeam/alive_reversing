#include "stdafx.h"
#include "Sound.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Error.hpp"
#include "Sys.hpp"
#include <mutex>
#include "Sound/SoundSDL.hpp"
#include "Sound/SoundDSound.hpp"
#include "Sound/SDLSoundBuffer.hpp"
#include "Sound/SDLSoundSystem.hpp"
#include "Sound/PsxSpuApi.hpp"

ALIVE_VAR(1, 0xBBC394, s32, sLoadedSoundsCount_BBC394, 0);
ALIVE_VAR(1, 0xbbc33c, s32, sLastNotePlayTime_BBC33C, 0);


ALIVE_ARY(1, 0xBBBAB8, SoundBuffer, 32, sSoundBuffers_BBBAB8, {});
ALIVE_ARY(1, 0xBBBD38, s32, 127, sVolumeTable_BBBD38, {});
ALIVE_ARY(1, 0xBBBF38, SoundEntry*, 256, sSoundSamples_BBBF38, {});

const DWORD k127_dword_575158 = 127;

static SoundApi sSoundApi;

EXPORT SoundApi& GetSoundAPI()
{
    return sSoundApi;
}

#if USE_SDL2_SOUND
ALIVE_VAR(1, 0xBBC344, SDLSoundSystem*, sDSound_BBC344, nullptr);
#else
ALIVE_VAR(1, 0xBBC344, LPDIRECTSOUND, sDSound_BBC344, nullptr);
#endif

EXPORT s32 CC SND_CreateDS_4EEAA0(u32 sampleRate, s32 bitsPerSample, s32 isStereo)
{
#if USE_SDL2_SOUND
    return SND_CreateDS_SDL(sampleRate, bitsPerSample, isStereo);
#else
    return SND_CreateDS_DSound(sampleRate, bitsPerSample, isStereo);
#endif
}

EXPORT s32 CC SND_Clear_4EF350(SoundEntry* pSoundEntry, u32 sampleOffset, u32 size)
{
#if USE_SDL2_SOUND
    return SND_Clear_SDL(pSoundEntry, sampleOffset, size);
#else
    return SND_Clear_DSound(pSoundEntry, sampleOffset, size);
#endif
}

EXPORT s32 CC SND_LoadSamples_4EF1C0(const SoundEntry* pSnd, DWORD sampleOffset, u8* pSoundBuffer, u32 sampleCount)
{
#if USE_SDL2_SOUND
    return SND_LoadSamples_SDL(pSnd, sampleOffset, pSoundBuffer, sampleCount);
#else
    return SND_LoadSamples_DSound(pSnd, sampleOffset, pSoundBuffer, sampleCount);
#endif
}

EXPORT const s8 * CC SND_HR_Err_To_String_4EEC70(HRESULT hr)
{
#if USE_SDL2_SOUND
    return SND_HR_Err_To_String_SDL(hr);
#else
    return SND_HR_Err_To_String_DSound(hr);
#endif
}

EXPORT void CC SND_InitVolumeTable_4EEF60()
{
#if USE_SDL2_SOUND
    return SND_InitVolumeTable_SDL();
#else
    return SND_InitVolumeTable_DSound();
#endif
}

EXPORT void CC SND_SsQuit_4EFD50()
{
    if (sDSound_BBC344)
    {
        for (s32 i = 0; i < 32; i++)
        {
            if (sSoundBuffers_BBBAB8[i].field_0_pDSoundBuffer)
            {
                sSoundBuffers_BBBAB8[i].field_0_pDSoundBuffer->Stop();
                sSoundBuffers_BBBAB8[i].field_0_pDSoundBuffer->Release();
                sSoundBuffers_BBBAB8[i].field_0_pDSoundBuffer = nullptr;
            }
        }

        for (s32 i = 0; i < sLoadedSoundsCount_BBC394; i++)
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


EXPORT s32 CC SND_Free_4EFA30(SoundEntry* pSnd)
{
    if (!sDSound_BBC344)
    {
        return -1;
    }

    pSnd->field_10 = 0;

    if (pSnd->field_8_pSoundBuffer)
    {
        ae_free_4F4EA0(pSnd->field_8_pSoundBuffer);
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

EXPORT s32 CC SND_PlayEx_4EF740(const SoundEntry* pSnd, s32 panLeft, s32 panRight, float freq, MIDI_Channel* pMidiStru, s32 playFlags, s32 priority)
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

    TSoundBufferType* pDSoundBuffer = pSnd->field_4_pDSoundBuffer;
    if (!pDSoundBuffer)
    {
        return -1;
    }

    sLastNotePlayTime_BBC33C = SYS_GetTicks();

    s32 panLeft2 = panLeft;
    s32 panRight2 = panRight;
    if (panLeft > panRight)
    {
        panRight2 = panLeft;
    }

    s32 panRightConverted = 120 * panRight2 * 127 >> 14;// >> 14 = 16384, note: was using k127_dword_575158

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

        if (status & DSBSTATUS_PLAYING) // TODO: SDL2 didn't check this flag
        {
            pDSoundBuffer->SetFrequency(static_cast<DWORD>((pSnd->field_18_sampleRate * freq) + 0.5)); // This freq don't get clamped for some reason
            pDSoundBuffer->SetVolume(sVolumeTable_BBBD38[panRightConverted]); // TODO: SDL2 / 10000.0f
            pDSoundBuffer->SetCurrentPosition(0);
            return 0;
        }
    }
    else
    {
        SoundBuffer* pSoundBuffer = GetSoundAPI().SND_Get_Sound_Buffer(pSnd->field_0_tableIdx, panRightConverted + priority);
        if (!pSoundBuffer)
        {
            return -1;
        }

        HRESULT v15 = sDSound_BBC344->DuplicateSoundBuffer(pDSoundBuffer, &pSoundBuffer->field_0_pDSoundBuffer);
        if (FAILED(v15))
        {
            Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\SND.C", 921, -1, GetSoundAPI().SND_HR_Err_To_String(v15));
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

    const s32 panConverted = (DSBPAN_RIGHT * (panLeft2 - panRight)) / 127; // From PSX pan range to DSound pan range
    pDSoundBuffer->SetPan(-panConverted); // Fix Inverted Stereo

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
        if (GetSoundAPI().SND_LoadSamples(pSnd, 0, pSnd->field_8_pSoundBuffer, pSnd->field_C_buffer_size_bytes / pSnd->field_1D_blockAlign) == 0)
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

EXPORT void CC SND_Init_WaveFormatEx_4EEA00(WAVEFORMATEX *pWaveFormat, s32 sampleRate, u8 bitsPerSample, s32 isStereo)
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

EXPORT s32 CC SND_New_4EEFF0(SoundEntry *pSnd, s32 sampleLength, s32 sampleRate, s32 bitsPerSample, s32 isStereo)
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
        GetSoundAPI().SND_Init_WaveFormatEx(&waveFormatEx, sampleRate, static_cast<u8>(bitsPerSample), isStereo & 1);

        const s32 sampleByteSize = sampleLength * waveFormatEx.nBlockAlign;
        bufferDesc.dwReserved = 0;
        bufferDesc.lpwfxFormat = &waveFormatEx;
        pSnd->field_1D_blockAlign = static_cast<u8>(waveFormatEx.nBlockAlign);
        bufferDesc.dwSize = 20;
        bufferDesc.dwBufferBytes = sampleByteSize;
        bufferDesc.dwFlags = 82152;

        const HRESULT sbHR = sDSound_BBC344->CreateSoundBuffer(&bufferDesc, &pSnd->field_4_pDSoundBuffer, 0);
        if (FAILED(sbHR))
        {
            Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\SND.C", 598, -1, "SND_New(): Cannot create ds sound buffer");
            Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\SND.C", 599, -1, GetSoundAPI().SND_HR_Err_To_String(sbHR));
            return -1;
        }
        else
        {
            pSnd->field_10 = 0;
            u8 * bufferData = static_cast<u8 *>(ae_malloc_4F4E60(sampleByteSize));
            pSnd->field_8_pSoundBuffer = bufferData;
            if (bufferData)
            {
                pSnd->field_18_sampleRate = sampleRate;
                pSnd->field_1C_bitsPerSample = static_cast<s8>(bitsPerSample);
                pSnd->field_C_buffer_size_bytes = sampleByteSize;
                pSnd->field_14_buffer_size_bytes = sampleByteSize;
                pSnd->field_20_isStereo = isStereo;

                for (s32 i = 0; i < 256; i++)
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


EXPORT s32 CC SND_Renew_4EEDD0(SoundEntry *pSnd)
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

    GetSoundAPI().SND_Init_WaveFormatEx(&waveFormat, pSnd->field_18_sampleRate, pSnd->field_1C_bitsPerSample, pSnd->field_20_isStereo & 1);

    bufferDesc.dwBufferBytes = pSnd->field_14_buffer_size_bytes;
    bufferDesc.dwReserved = 0;
    bufferDesc.lpwfxFormat = &waveFormat;
    bufferDesc.dwSize = 20;
    bufferDesc.dwFlags = 82144; // TODO: Fix constants

    if (FAILED(sDSound_BBC344->CreateSoundBuffer(&bufferDesc, &pSnd->field_4_pDSoundBuffer, 0)))
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

EXPORT u32 CC SND_Get_Sound_Entry_Pos_4EF620(SoundEntry* pSoundEntry)
{
    DWORD dwReadPos = 0;
    DWORD dwWritePos = 0;
    pSoundEntry->field_4_pDSoundBuffer->GetCurrentPosition(&dwReadPos, &dwWritePos);
    return dwReadPos / pSoundEntry->field_1D_blockAlign;
}

// Never seems to get called?
// TODO: Clean up!
EXPORT DWORD * CC SND_4F00B0(u32* /*a1*/, u32 /*a2*/, s32 /*a3*/)
{
    ALIVE_FATAL("Never expected SND_4F00B0 to be called");
    //return 0;
    //DWORD *result; // eax
    //u32 *v4; // edx
    //u32 v5; // ecx
    //s32 v6; // esi
    //s32 v7; // esi

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
    //    } while ((u32)result < v5);
    //    if ((u32)result < v5)
    //    {
    //        v7 = (s8 *)v4 - (s8 *)result;
    //        do
    //        {
    //            *((u8 *)result + v7) = *(u8 *)result ^ 0x80;
    //            result = (DWORD *)((s8 *)result + 1);
    //        } while ((u32)result < v5);
    //    }
    //}
    //return result;
}

EXPORT s32 CC SND_Load_4EF680(SoundEntry* pSnd, const void* pWaveData, s32 waveDataLen)
{
    GetSoundAPI().SND_Free(pSnd);
    if (!GetSoundAPI().SND_New(pSnd, waveDataLen, pSnd->field_18_sampleRate, pSnd->field_1C_bitsPerSample, pSnd->field_20_isStereo))
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
    return GetSoundAPI().SND_LoadSamples(pSnd, 0, pSnd->field_8_pSoundBuffer, pSnd->field_C_buffer_size_bytes / pSnd->field_1D_blockAlign);
}

EXPORT s32 CC SND_Buffer_Set_Frequency_4EFC90(s32 idx, float hzChangeFreq)
{
    SoundBuffer* pSoundBuffer = &sSoundBuffers_BBBAB8[idx & 511];
    TSoundBufferType* pDSoundBuffer = pSoundBuffer->field_0_pDSoundBuffer;

    if (!pDSoundBuffer || ((idx ^ pSoundBuffer->field_4) & ~511)) // TODO: Refactor
    {
        return -1;
    }

    if (!sSoundSamples_BBBF38[pSoundBuffer->field_8_sample_idx])
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

EXPORT s32 CC SND_Buffer_Get_Status_4F00F0(s32 idx, s32 a2)
{
    // TODO: Figure out the meaning of the constants in here
    SoundBuffer* pSoundBuffer = &sSoundBuffers_BBBAB8[idx];
    if (!pSoundBuffer->field_0_pDSoundBuffer)
    {
        return 0x40000000;
    }

    DWORD status = 0;
    pSoundBuffer->field_0_pDSoundBuffer->GetStatus(&status);

    s32 fromStatus = 0;
    if (!(status & 1))
    {
        fromStatus = 0x8000000;
    }
    if (!(status & 4))
    {
        fromStatus += 0x8000000;
    }

    s32 v6 = (a2 - pSoundBuffer->field_10) >> 2; // >> 2 = / 2 ? 
    if (v6 > 2)
    {
        v6 = 2;
    }
    else if (v6 < -2)
    {
        v6 = -2;
    }

    return fromStatus + 2 * (sLastNotePlayTime_BBC33C + (v6 << 8) - pSoundBuffer->field_C); // << 8 = * 256 ?
}

EXPORT s32 CC SND_Buffer_Set_Volume_4EFAD0(s32 idx, s32 vol)
{
    TSoundBufferType* pSoundBuffer = sSoundBuffers_BBBAB8[idx & 511].field_0_pDSoundBuffer;
    if (!pSoundBuffer || (idx ^ sSoundBuffers_BBBAB8[idx & 511].field_4) & ~511)
    {
        return -1;
    }

    u32 volConverted = (u32)(vol * k127_dword_575158) >> 7; // Conversion used else where
    if (volConverted > 127)
    {
        volConverted = 127;
    }
    pSoundBuffer->SetVolume(sVolumeTable_BBBD38[120 * volConverted >> 7]);
    return 0;
}

EXPORT s32 CC SND_Buffer_Set_Frequency_4EFC00(s32 idx, float freq)
{
    const s32 idxMasked = idx & 511;
    if (idxMasked > 32)
    {
        LOG_ERROR("Idx out of bounds " << idxMasked << " unmasked " << idx);
        return -1;
    }

    TSoundBufferType* pDSoundBuffer = sSoundBuffers_BBBAB8[idxMasked].field_0_pDSoundBuffer;
    SoundBuffer* pSoundBuffer = &sSoundBuffers_BBBAB8[idxMasked];

    if (!pDSoundBuffer || (idx ^ pSoundBuffer->field_4) & ~511)
    {
        return -1;
    }

    DWORD freqHz = static_cast<DWORD>(sSoundSamples_BBBF38[pSoundBuffer->field_8_sample_idx]->field_18_sampleRate * freq);
    if (freqHz < DSBFREQUENCY_MIN)
    {
        freqHz = DSBFREQUENCY_MIN;
    }
    else if (freqHz >= DSBFREQUENCY_MAX)
    {
        freqHz = DSBFREQUENCY_MAX;
    }
    pDSoundBuffer->SetFrequency(freqHz);
    return 0;

}

EXPORT s32 CC SND_Stop_Sample_At_Idx_4EFA90(s32 idx)
{
    TSoundBufferType* pBuffer = sSoundBuffers_BBBAB8[idx & 511].field_0_pDSoundBuffer;
    if (!pBuffer || (idx ^ sSoundBuffers_BBBAB8[idx & 511].field_4) & ~511) // TODO: Same unknown field_4 conversion
    {
        return -1;
    }
    pBuffer->Stop();
    return 0;
}

EXPORT SoundBuffer* CC SND_Recycle_Sound_Buffer_4EF9C0(s32 idx, s32 sampleIdx, s32 field10)
{
    SoundBuffer* pSoundBuffer = &sSoundBuffers_BBBAB8[idx];
    if (pSoundBuffer->field_0_pDSoundBuffer)
    {
        pSoundBuffer->field_0_pDSoundBuffer->Release();
        pSoundBuffer->field_0_pDSoundBuffer = nullptr;
    }

    // TODO: What is this shorthand for ?
    const s32 oldField4 = pSoundBuffer->field_4 ^ (((u16)idx ^ (u16)pSoundBuffer->field_4) & 511);
    pSoundBuffer->field_4 = oldField4 + 512;

    pSoundBuffer->field_8_sample_idx = sampleIdx;
    pSoundBuffer->field_C = sLastNotePlayTime_BBC33C;
    pSoundBuffer->field_10 = field10;
    return pSoundBuffer;
}

EXPORT s32 CC SND_Get_Buffer_Status_4EE8F0(s32 idx)
{
    TSoundBufferType* pBuffer = sSoundBuffers_BBBAB8[idx & 511].field_0_pDSoundBuffer;
    if (!pBuffer || (idx ^ sSoundBuffers_BBBAB8[idx & 511].field_4) & ~511)
    {
        return 0;
    }

    DWORD status = 0;
    pBuffer->GetStatus(&status);
    if (status & 4)
    {
        // Looped
        return 2;
    }
    else
    {
        // Playing
        return status & 1;
    }
}

EXPORT SoundBuffer* CC SND_Get_Sound_Buffer_4EF970(s32 sampleIdx, s32 field10)
{
    s32 idx = -1;
    s32 statusToMatch = 0xC0000000;

    for (s32 i = 0; i < 32; i++)
    {
        s32 status = SND_Buffer_Get_Status_4F00F0(i, field10);
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
    return SND_Recycle_Sound_Buffer_4EF9C0(idx, sampleIdx, field10);
}

EXPORT SoundApi::SoundApi()
{
   
    SND_Get_Sound_Entry_Pos = SND_Get_Sound_Entry_Pos_4EF620;
   
    SND_Clear = SND_Clear_4EF350;
    SND_SsQuit = SND_SsQuit_4EFD50;
    SND_CreateDS = SND_CreateDS_4EEAA0;
    SND_Init_WaveFormatEx = SND_Init_WaveFormatEx_4EEA00;
    SND_New = SND_New_4EEFF0;
    SND_Load = SND_Load_4EF680;
    SND_HR_Err_To_String = SND_HR_Err_To_String_4EEC70;
    SND_Free = SND_Free_4EFA30;
    SND_Restart = SND_Restart_4CB0E0;
    #if !USE_SDL2_SOUND
    SND_SetPrimarySoundBufferFormat = SND_SetPrimarySoundBufferFormat_4EE990;
    #endif
    SND_InitVolumeTable = SND_InitVolumeTable_4EEF60;
    #if !USE_SDL2_SOUND
    SND_CreatePrimarySoundBuffer = SND_CreatePrimarySoundBuffer_4EEEC0;
    #endif
    SND_Renew = SND_Renew_4EEDD0;
    SND_Get_Buffer_Status = SND_Get_Buffer_Status_4EE8F0;
    SND_Stop_Sample_At_Idx = SND_Stop_Sample_At_Idx_4EFA90;
    SND_Buffer_Set_Volume = SND_Buffer_Set_Volume_4EFAD0;
    SND_Get_Sound_Buffer = SND_Get_Sound_Buffer_4EF970;
    SND_Buffer_Set_Frequency1 = SND_Buffer_Set_Frequency_4EFC90;
    SND_Buffer_Set_Frequency2 = SND_Buffer_Set_Frequency_4EFC00;
    SND_LoadSamples = SND_LoadSamples_4EF1C0;
    SND_unknown = SND_4F00B0;
    SND_PlayEx = SND_PlayEx_4EF740;
}
