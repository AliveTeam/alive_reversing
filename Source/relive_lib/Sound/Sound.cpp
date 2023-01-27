#include "../stdafx.h"
#include "Sound.hpp"
#include "../../relive_lib/Function.hpp"
#include "../../AliveLibAE/stdlib.hpp"
#include "../../relive_lib/Sys.hpp"
#include <mutex>
#include "SoundSDL.hpp"
#include "SDLSoundBuffer.hpp"
#include "SDLSoundSystem.hpp"
#include "PsxSpuApi.hpp"
#include "../../relive_lib/FatalError.hpp"

s32 sLoadedSoundsCount_BBC394 = 0;
s32 sLastNotePlayTime_BBC33C = 0;


SoundBuffer sSoundBuffers_BBBAB8[32] = {};
s32 sVolumeTable_BBBD38[127] = {};
SoundEntry* sSoundSamples_BBBF38[256] = {};

const u32 k127_dword_575158 = 127;

static SoundApi sSoundApi;

SoundApi& GetSoundAPI()
{
    return sSoundApi;
}

#if USE_SDL2_SOUND
SDLSoundSystem* sDSound_BBC344 = nullptr;
#else
LPDIRECTSOUND sDSound_BBC344 = nullptr;
#endif

s32 SND_CreateDS_4EEAA0(u32 sampleRate, s32 bitsPerSample, s32 isStereo)
{
#if USE_SDL2_SOUND
    return SND_CreateDS_SDL(sampleRate, bitsPerSample, isStereo);
#else
    return SND_CreateDS_DSound(sampleRate, bitsPerSample, isStereo);
#endif
}

s32 SND_Clear_4EF350(SoundEntry* pSoundEntry, u32 sampleOffset, u32 size)
{
#if USE_SDL2_SOUND
    return SND_Clear_SDL(pSoundEntry, sampleOffset, size);
#else
    return SND_Clear_DSound(pSoundEntry, sampleOffset, size);
#endif
}

s32 SND_LoadSamples_4EF1C0(const SoundEntry* pSnd, u32 sampleOffset, u8* pSoundBuffer, u32 sampleCount)
{
#if USE_SDL2_SOUND
    return SND_LoadSamples_SDL(pSnd, sampleOffset, pSoundBuffer, sampleCount);
#else
    return SND_LoadSamples_DSound(pSnd, sampleOffset, pSoundBuffer, sampleCount);
#endif
}

const char_type* SND_HR_Err_To_String_4EEC70(HRESULT hr)
{
#if USE_SDL2_SOUND
    return SND_HR_Err_To_String_SDL(hr);
#else
    return SND_HR_Err_To_String_DSound(hr);
#endif
}

void SND_InitVolumeTable_4EEF60()
{
#if USE_SDL2_SOUND
    return SND_InitVolumeTable_SDL();
#else
    return SND_InitVolumeTable_DSound();
#endif
}

void SND_SsQuit_4EFD50()
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


s32 SND_Free_4EFA30(SoundEntry* pSnd)
{
    if (!sDSound_BBC344)
    {
        return -1;
    }

    pSnd->field_10 = 0;

    relive_delete[] pSnd->field_8_pSoundBuffer;
    pSnd->field_8_pSoundBuffer = nullptr;

    if (pSnd->field_4_pDSoundBuffer)
    {
        pSnd->field_4_pDSoundBuffer->Release();
        pSnd->field_4_pDSoundBuffer = nullptr;
    }

    sSoundSamples_BBBF38[pSnd->field_0_tableIdx] = nullptr;
    sLoadedSoundsCount_BBC394--;
    return 0;
}

s32 SND_PlayEx(const SoundEntry* pSnd, s32 panLeft, s32 panRight, f32 freq, MIDI_Channel* pMidiStru, s32 playFlags, s32 priority)
{
    if (!sDSound_BBC344)
    {
        return -1;
    }

    if (!pSnd)
    {
        ALIVE_FATAL("SND_PlayEx: NULL SAMPLE !!!");
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

    s32 panRightConverted = 120 * panRight2 * 127 >> 14; // >> 14 = 16384, note: was using k127_dword_575158

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
        u32 status = 0;
        if (FAILED(pDSoundBuffer->GetStatus(&status)))
        {
            return -1;
        }

        if (status & DSBSTATUS_PLAYING) // TODO: SDL2 didn't check this flag
        {
            pDSoundBuffer->SetFrequency(static_cast<u32>((pSnd->field_18_sampleRate * freq) + 0.5f)); // This freq don't get clamped for some reason
            pDSoundBuffer->SetVolume(sVolumeTable_BBBD38[panRightConverted]);                        // TODO: SDL2 / 10000.0f
            pDSoundBuffer->SetCurrentPosition(0);
            return 0;
        }
    }
    else
    {
        SoundBuffer* pSoundBuffer = GetSoundAPI().mSND_Get_Sound_Buffer(pSnd->field_0_tableIdx, panRightConverted + priority);
        if (!pSoundBuffer)
        {
            return -1;
        }

        HRESULT v15 = sDSound_BBC344->DuplicateSoundBuffer(pDSoundBuffer, &pSoundBuffer->field_0_pDSoundBuffer);
        if (FAILED(v15))
        {
            ALIVE_FATAL(GetSoundAPI().mSND_HR_Err_To_String(v15));
        }

        pDSoundBuffer = pSoundBuffer->field_0_pDSoundBuffer;

        if (pMidiStru)
        {
            pMidiStru->field_0_sound_buffer_field_4 = pSoundBuffer->field_4;
        }
    }

    u32 freqHz = static_cast<u32>((pSnd->field_18_sampleRate * freq) + 0.5f);
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
    pDSoundBuffer->SetPan(-panConverted);                                  // Fix Inverted Stereo

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
        if (GetSoundAPI().mSND_LoadSamples(pSnd, 0, pSnd->field_8_pSoundBuffer, pSnd->field_C_buffer_size_bytes / pSnd->field_1D_blockAlign) == 0)
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

void SND_Init_WaveFormatEx_4EEA00(WAVEFORMATEX* pWaveFormat, s32 sampleRate, u8 bitsPerSample, s32 isStereo)
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

s32 SND_New(SoundEntry* pSnd, s32 sampleLength, s32 sampleRate, s32 bitsPerSample, s32 isStereo)
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
        GetSoundAPI().mSND_Init_WaveFormatEx(&waveFormatEx, sampleRate, static_cast<u8>(bitsPerSample), isStereo & 1);

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
            ALIVE_FATAL("SND_New(): Cannot create ds sound buffer");
        }
        else
        {
            pSnd->field_10 = 0;
            u8* bufferData = relive_new u8[sampleByteSize];
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
                ALIVE_FATAL("SND_New(): Cannot create original data sound buffer");
            }
        }
    }
    else
    {
        ALIVE_FATAL("SND_New: out of samples");
    }
}


s32 SND_Renew_4EEDD0(SoundEntry* pSnd)
{
    if (!sDSound_BBC344)
    {
        ALIVE_FATAL("DirectSound not initialized");
    }

    WAVEFORMATEX waveFormat;
    DSBUFFERDESC bufferDesc;

    waveFormat.wFormatTag = 0;
    waveFormat.nSamplesPerSec = 0;
    waveFormat.nAvgBytesPerSec = 0;
    waveFormat.nBlockAlign = 0;
    waveFormat.cbSize = 0;

    GetSoundAPI().mSND_Init_WaveFormatEx(&waveFormat, pSnd->field_18_sampleRate, pSnd->field_1C_bitsPerSample, pSnd->field_20_isStereo & 1);

    bufferDesc.dwBufferBytes = pSnd->field_14_buffer_size_bytes;
    bufferDesc.dwReserved = 0;
    bufferDesc.lpwfxFormat = &waveFormat;
    bufferDesc.dwSize = 20;
    bufferDesc.dwFlags = 82144; // TODO: Fix constants

    if (FAILED(sDSound_BBC344->CreateSoundBuffer(&bufferDesc, &pSnd->field_4_pDSoundBuffer, 0)))
    {
        ALIVE_FATAL("SND_Renew(): Cannot create ds sound buffer");
    }
    else
    {
        pSnd->field_10 = 0;
        return 0;
    }
}

u32 SND_Get_Sound_Entry_Pos_4EF620(SoundEntry* pSoundEntry)
{
    u32 dwReadPos = 0;
    u32 dwWritePos = 0;
    pSoundEntry->field_4_pDSoundBuffer->GetCurrentPosition(&dwReadPos, &dwWritePos);
    return dwReadPos / pSoundEntry->field_1D_blockAlign;
}

// Never seems to get called?
// TODO: Clean up!
u32* SND_4F00B0(u32* /*a1*/, u32 /*a2*/, s32 /*a3*/)
{
    ALIVE_FATAL("Never expected SND_4F00B0 to be called");
    //return 0;
    //u32 *result; // eax
    //u32 *v4; // edx
    //u32 v5; // ecx
    //s32 v6; // esi
    //s32 v7; // esi

    //result = (u32 *)a2;
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
    //            result = (u32 *)((s8 *)result + 1);
    //        } while ((u32)result < v5);
    //    }
    //}
    //return result;
}

s32 SND_Load(SoundEntry* pSnd, const void* pWaveData, s32 waveDataLen)
{
    GetSoundAPI().mSND_Free(pSnd);
    if (!GetSoundAPI().mSND_New(pSnd, waveDataLen, pSnd->field_18_sampleRate, pSnd->field_1C_bitsPerSample, pSnd->field_20_isStereo))
    {
        if (waveDataLen * pSnd->field_1D_blockAlign > pSnd->field_C_buffer_size_bytes)
        {
            ALIVE_FATAL("SND_Load(): data too big !!");
        }

        if (pSnd->field_8_pSoundBuffer)
        {
            memcpy(pSnd->field_8_pSoundBuffer, pWaveData, waveDataLen * pSnd->field_1D_blockAlign);
        }
    }
    return GetSoundAPI().mSND_LoadSamples(pSnd, 0, pSnd->field_8_pSoundBuffer, pSnd->field_C_buffer_size_bytes / pSnd->field_1D_blockAlign);
}

s32 SND_Buffer_Set_Frequency_4EFC90(s32 idx, f32 hzChangeFreq)
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

    u32 currrentFreqHz = 0;
    if (SUCCEEDED(pDSoundBuffer->GetFrequency(&currrentFreqHz)))
    {
        u32 freqHz = static_cast<u32>(currrentFreqHz * hzChangeFreq);
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

s32 SND_Buffer_Get_Status_4F00F0(s32 idx, s32 a2)
{
    // TODO: Figure out the meaning of the constants in here
    SoundBuffer* pSoundBuffer = &sSoundBuffers_BBBAB8[idx];
    if (!pSoundBuffer->field_0_pDSoundBuffer)
    {
        return 0x40000000;
    }

    u32 status = 0;
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

    return fromStatus + 2 * (sLastNotePlayTime_BBC33C + (v6 * 256) - pSoundBuffer->field_C); // << 8 = * 256 ?
}

s32 SND_Buffer_Set_Volume_4EFAD0(s32 idx, s32 vol)
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

s32 SND_Buffer_Set_Frequency_4EFC00(s32 idx, f32 freq)
{
    const s32 idxMasked = idx & 511;
    if (idxMasked > 32)
    {
        LOG_ERROR("Idx out of bounds %d unmasked %d", idxMasked, idx);
        return -1;
    }

    TSoundBufferType* pDSoundBuffer = sSoundBuffers_BBBAB8[idxMasked].field_0_pDSoundBuffer;
    SoundBuffer* pSoundBuffer = &sSoundBuffers_BBBAB8[idxMasked];

    if (!pDSoundBuffer || (idx ^ pSoundBuffer->field_4) & ~511)
    {
        return -1;
    }

    u32 freqHz = static_cast<u32>(sSoundSamples_BBBF38[pSoundBuffer->field_8_sample_idx]->field_18_sampleRate * freq);
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

s32 SND_Stop_Sample_At_Idx(s32 idx)
{
    TSoundBufferType* pBuffer = sSoundBuffers_BBBAB8[idx & 511].field_0_pDSoundBuffer;
    if (!pBuffer || (idx ^ sSoundBuffers_BBBAB8[idx & 511].field_4) & ~511) // TODO: Same unknown field_4 conversion
    {
        return -1;
    }
    pBuffer->Stop();
    return 0;
}

SoundBuffer* SND_Recycle_Sound_Buffer_4EF9C0(s32 idx, s32 sampleIdx, s32 field10)
{
    SoundBuffer* pSoundBuffer = &sSoundBuffers_BBBAB8[idx];
    if (pSoundBuffer->field_0_pDSoundBuffer)
    {
        pSoundBuffer->field_0_pDSoundBuffer->Release();
        pSoundBuffer->field_0_pDSoundBuffer = nullptr;
    }

    // TODO: What is this shorthand for ?
    const s32 oldField4 = pSoundBuffer->field_4 ^ (((u16) idx ^ (u16) pSoundBuffer->field_4) & 511);
    pSoundBuffer->field_4 = oldField4 + 512;

    pSoundBuffer->field_8_sample_idx = sampleIdx;
    pSoundBuffer->field_C = sLastNotePlayTime_BBC33C;
    pSoundBuffer->field_10 = field10;
    return pSoundBuffer;
}

s32 SND_Get_Buffer_Status(s32 idx)
{
    TSoundBufferType* pBuffer = sSoundBuffers_BBBAB8[idx & 511].field_0_pDSoundBuffer;
    if (!pBuffer || (idx ^ sSoundBuffers_BBBAB8[idx & 511].field_4) & ~511)
    {
        return 0;
    }

    u32 status = 0;
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

SoundBuffer* SND_Get_Sound_Buffer_4EF970(s32 sampleIdx, s32 field10)
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

SoundApi::SoundApi()
{
    mSND_Get_Sound_Entry_Pos = SND_Get_Sound_Entry_Pos_4EF620;

    mSND_Clear = SND_Clear_4EF350;
    mSND_SsQuit = SND_SsQuit_4EFD50;
    mSND_CreateDS = SND_CreateDS_4EEAA0;
    mSND_Init_WaveFormatEx = SND_Init_WaveFormatEx_4EEA00;
    mSND_New = SND_New;
    mSND_Load = SND_Load;
    mSND_HR_Err_To_String = SND_HR_Err_To_String_4EEC70;
    mSND_Free = SND_Free_4EFA30;
    mSND_Restart = SND_Restart_4CB0E0;
#if !USE_SDL2_SOUND
    SND_SetPrimarySoundBufferFormat = SND_SetPrimarySoundBufferFormat_4EE990;
#endif
    SND_InitVolumeTable = SND_InitVolumeTable_4EEF60;
#if !USE_SDL2_SOUND
    SND_CreatePrimarySoundBuffer = SND_CreatePrimarySoundBuffer_4EEEC0;
#endif
    mSND_Renew = SND_Renew_4EEDD0;
    mSND_Get_Buffer_Status = ::SND_Get_Buffer_Status;
    mSND_Stop_Sample_At_Idx = ::SND_Stop_Sample_At_Idx;
    mSND_Buffer_Set_Volume = SND_Buffer_Set_Volume_4EFAD0;
    mSND_Get_Sound_Buffer = SND_Get_Sound_Buffer_4EF970;
    mSND_Buffer_Set_Frequency1 = SND_Buffer_Set_Frequency_4EFC90;
    mSND_Buffer_Set_Frequency2 = SND_Buffer_Set_Frequency_4EFC00;
    mSND_LoadSamples = SND_LoadSamples_4EF1C0;
    mSND_unknown = SND_4F00B0;
}
