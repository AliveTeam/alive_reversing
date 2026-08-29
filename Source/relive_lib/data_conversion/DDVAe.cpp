#include "stdafx.h"
#include "DDVAe.hpp"
#include "../relive_lib/Function.hpp"
#include "../relive_lib/Psx.hpp"
#include "../relive_lib/Sound/Sound.hpp"
#include "../relive_lib/Masher.hpp"
#include "../relive_lib/data_conversion/rgb_conversion.hpp"

#include <cstdio>

namespace relive
{

Masher* Masher_Alloc(
    const char_type* pFileName,
    Masher_Header** ppMasherHeader,
    Masher_VideoHeader** ppMasherVideoHeader,
    Masher_AudioHeader** ppMasherAudioHeader,
    s32* errCode)
{
    Masher* pMasher = relive_new Masher;
    if (pMasher)
    {
        *errCode = pMasher->Init(pFileName);
        if (*errCode)
        {
            relive_delete pMasher;
            return nullptr;
        }
        else
        {
            *ppMasherHeader = &pMasher->field_4_ddv_header;
            *ppMasherVideoHeader = &pMasher->field_14_video_header;
            *ppMasherAudioHeader = &pMasher->field_2C_audio_header;
            return pMasher;
        }
    }
    else
    {
        *errCode = 2;
        return nullptr;
    }
}

void Masher_DeAlloc(Masher* pMasher)
{
    relive_delete pMasher;
}

s32 Masher_ReadNextFrame(Masher* pMasher)
{
    return pMasher->ReadNextFrame();
}

void Masher_DecodeVideoFrame(Masher* pMasher, RGBA32* pSurface)
{
    pMasher->VideoFrameDecode(pSurface);
}

static bool sHasAudio = false;
static s32 sFmvNumReadFrames = 0;
static Masher_Header* sMasher_Header = nullptr;
static Masher_VideoHeader* sMasher_VideoHeader = nullptr;
static Masher_AudioHeader* sMasher_AudioHeader = nullptr;
static bool sNoAudioOrAudioError = false;
static Masher* sMasherInstance = nullptr;
static s32 sFmvSingleAudioFrameSizeInSamples = 0;

s8 DDV_StartAudio(IDDVReaderCallBacks& callBacks)
{
    if (!sHasAudio)
    {
        return 1;
    }

    // Keep reading frames till we have >= number of interleaved so that we have 1 full frame
    if (sFmvNumReadFrames < sMasher_AudioHeader->field_10_num_frames_interleave)
    {
        while (Masher::ReadNextFrameToMemory_4EAC30(sMasherInstance))
        {
            void* pDecompressedAudioBuffer = Masher::GetDecompressedAudioFrame_4EAC60(sMasherInstance);             
            callBacks.OnAudioFrame(pDecompressedAudioBuffer, (sMasherInstance->field_54_bits_per_sample / 8) * sMasherInstance->field_50_num_channels, sFmvSingleAudioFrameSizeInSamples);

            sFmvNumReadFrames++;
            
            if (sFmvNumReadFrames >= sMasher_AudioHeader->field_10_num_frames_interleave)
            {
                break;
            }
        }
    }

    if (sFmvNumReadFrames >= sMasher_AudioHeader->field_10_num_frames_interleave)
    {
        return 1;
    }

    return 0;
}

static Masher* Open_DDV(const char_type* pMovieName)
{
    s32 errCode = 0;
    Masher* pMasher = Masher_Alloc(
        pMovieName,
        &sMasher_Header,
        &sMasher_VideoHeader,
        &sMasher_AudioHeader,
        &errCode);

    if (errCode)
    {
        return nullptr;
    }
    return pMasher;
}

s8 DDV_Play_Impl(const char_type* pMovieName, IDDVReaderCallBacks& callBacks)
{
    if (!*pMovieName)
    {
        return 1;
    }

    sMasherInstance = Open_DDV(pMovieName);
    if (!sMasherInstance)
    {
        return 0;
    }

    sHasAudio = (sMasher_Header->field_4_contains >> 1) & 1;
    sFmvSingleAudioFrameSizeInSamples = sMasher_AudioHeader->field_C_single_audio_frame_size;

    sNoAudioOrAudioError = false;
    sFmvNumReadFrames = 0;

    if (DDV_StartAudio(callBacks) && Masher_ReadNextFrame(sMasherInstance) && Masher_ReadNextFrame(sMasherInstance))
    {
       
    }


    return 0;
}


s8 DDV_Play(const char_type* pDDVName, IDDVReaderCallBacks& callBacks)
{
    return relive::DDV_Play_Impl(pDDVName, callBacks);
}


DDVAe::DDVAe(const char_type* pDDVName, IDDVReaderCallBacks& callBacks)
 : mDDvName(pDDVName), mCallBacks(callBacks)
{

}

DDVAe::~DDVAe()
{
    Masher_DeAlloc(sMasherInstance);
    sMasherInstance = nullptr;
}

bool DDVAe::StepFrame()
{
    mPixels.resize(FrameWidth() * FrameHeight() * sizeof(u32));

    /*while*/ if (Masher_ReadNextFrame(sMasherInstance))
    {

        sFmvNumReadFrames++;

        Masher_DecodeVideoFrame(sMasherInstance, reinterpret_cast<RGBA32*>(mPixels.data()));

        mCallBacks.OnVideoFrame(reinterpret_cast<const u32*>(mPixels.data()), sMasher_VideoHeader->field_4_width, sMasher_VideoHeader->field_8_height);

        if (!sNoAudioOrAudioError)
        {
            void* pDecompressedAudioFrame = Masher::GetDecompressedAudioFrame_4EAC60(sMasherInstance);
            if (pDecompressedAudioFrame)
            {
                const u32 frameSizeBytes = (sMasherInstance->field_54_bits_per_sample / 8) * sMasherInstance->field_50_num_channels;
                mAudioFrames.resize(frameSizeBytes * sFmvSingleAudioFrameSizeInSamples);
                memcpy(mAudioFrames.data(), pDecompressedAudioFrame, mAudioFrames.size());
                mCallBacks.OnAudioFrame(pDecompressedAudioFrame, frameSizeBytes, sFmvSingleAudioFrameSizeInSamples);
            }
        }
        return true;
    }
    return false;
}

bool DDVAe::ReadInfo()
{
    // TODO: Audio can be called here
    return !DDV_Play(mDDvName.c_str(), mCallBacks);
}

u32 DDVAe::FrameWidth()
{
    return sMasher_VideoHeader->field_4_width;
}

u32 DDVAe::FrameHeight()
{
    return sMasher_VideoHeader->field_8_height;
}

u32 DDVAe::FrameRate()
{
    return sMasher_VideoHeader->field_14_key_frame_rate;
}

u32 DDVAe::TotalVideoFrames()
{
    return sMasher_Header->field_C_number_of_frames;
}

u32 DDVAe::AudioSampleRate()
{
    return sMasher_AudioHeader ? sMasher_AudioHeader->field_4_samples_per_second : 0u;
}

u32 DDVAe::AudioChannels()
{
    return sMasherInstance ? static_cast<u32>(sMasherInstance->field_50_num_channels) : 0u;
}

u32 DDVAe::AudioBitsPerSample()
{
    return sMasherInstance ? static_cast<u32>(sMasherInstance->field_54_bits_per_sample) : 0u;
}

} // namespace
