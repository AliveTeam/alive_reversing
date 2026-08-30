#include "stdafx.h"
#include "DDVAe.hpp"
#include "../relive_lib/Function.hpp"
#include "../relive_lib/Masher.hpp"
#include "../relive_lib/data_conversion/rgb_conversion.hpp"

#include <cstdio>

namespace relive
{
DDVAe::DDVAe(const char_type* pDDVName, IDDVReaderCallBacks* callBacks)
 : mDDvName(pDDVName), mCallBacks(callBacks)
{

}

DDVAe::~DDVAe()
{
    relive_delete sMasherInstance;
    sMasherInstance = nullptr;
}

bool DDVAe::StepFrame()
{
    mPixels.resize(FrameWidth() * FrameHeight() * sizeof(u32));

    if (sMasherInstance->ReadNextFrame())
    {
        sMasherInstance->VideoFrameDecode(reinterpret_cast<RGBA32*>(mPixels.data()));

        if (mCallBacks)
        {
            mCallBacks->OnVideoFrame(reinterpret_cast<const u32*>(mPixels.data()), FrameWidth(), FrameHeight());
        }

        mAudioFrames.clear();
        void* pDecompressedAudioFrame = Masher::GetDecompressedAudioFrame_4EAC60(sMasherInstance);
        if (pDecompressedAudioFrame)
        {
            const u32 frameSizeBytes = (sMasherInstance->field_54_bits_per_sample / 8) * sMasherInstance->field_50_num_channels;
            mAudioFrames.resize(frameSizeBytes * sFmvSingleAudioFrameSizeInSamples);
            memcpy(mAudioFrames.data(), pDecompressedAudioFrame, mAudioFrames.size());

            mInterleaveAudioFrames.push_back(mAudioFrames);

            if (mCallBacks)
            {
                mCallBacks->OnAudioFrame(pDecompressedAudioFrame, frameSizeBytes, sFmvSingleAudioFrameSizeInSamples);
            }
        }

        return true;
    }
    return false;
}


s8 DDVAe::DDV_StartAudio(bool sHasAudio)
{
    if (!sHasAudio)
    {
        return 1;
    }

    // Keep reading frames till we have >= number of interleaved so that we have 1 full frame
    s32 sFmvNumReadFrames = 0;
    if (sFmvNumReadFrames < sMasherInstance->field_2C_audio_header.field_10_num_frames_interleave)
    {
        while (Masher::ReadNextFrameToMemory_4EAC30(sMasherInstance))
        {
            void* pDecompressedAudioBuffer = Masher::GetDecompressedAudioFrame_4EAC60(sMasherInstance);             

            const u32 frameSizeBytes = (sMasherInstance->field_54_bits_per_sample / 8) * sMasherInstance->field_50_num_channels;
            std::vector<u8> data;
            data.resize(frameSizeBytes * sFmvSingleAudioFrameSizeInSamples);
            memcpy(data.data(), pDecompressedAudioBuffer, data.size());

            mInterleaveAudioFrames.push_back(data);

            if (mCallBacks)
            {
                mCallBacks->OnAudioFrame(pDecompressedAudioBuffer, (sMasherInstance->field_54_bits_per_sample / 8) * sMasherInstance->field_50_num_channels, sFmvSingleAudioFrameSizeInSamples);
            }

            sFmvNumReadFrames++;
            
            if (sFmvNumReadFrames >= sMasherInstance->field_2C_audio_header.field_10_num_frames_interleave)
            {
                break;
            }
        }
    }

    if (sFmvNumReadFrames >= sMasherInstance->field_2C_audio_header.field_10_num_frames_interleave)
    {
        return 1;
    }

    return 0;
}

bool DDVAe::ReadInfo()
{
    if (mDDvName.empty())
    {
        return false;
    }

    sMasherInstance = relive_new Masher();
    if (sMasherInstance->Init(mDDvName.c_str()))
    {
        relive_delete sMasherInstance;
        sMasherInstance = nullptr;
    }

    if (!sMasherInstance)
    {
        return false;
    }

    const bool sHasAudio = (sMasherInstance->field_4_ddv_header.field_4_contains >> 1) & 1;
    sFmvSingleAudioFrameSizeInSamples = sMasherInstance->field_2C_audio_header.field_C_single_audio_frame_size;

    if (DDV_StartAudio(sHasAudio) && sMasherInstance->ReadNextFrame() )
    {
       
    }


    return true;
}

u32 DDVAe::FrameWidth()
{
    return sMasherInstance->field_14_video_header.field_4_width;
}

u32 DDVAe::FrameHeight()
{
    return sMasherInstance->field_14_video_header.field_8_height;
}

u32 DDVAe::FrameRate()
{
    return sMasherInstance->field_14_video_header.field_14_key_frame_rate;
}

u32 DDVAe::TotalVideoFrames()
{
    return sMasherInstance->field_4_ddv_header.field_C_number_of_frames;
}

u32 DDVAe::AudioSampleRate()
{
    return sMasherInstance ? sMasherInstance->field_2C_audio_header.field_4_samples_per_second : 0u;
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
