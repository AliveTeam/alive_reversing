#pragma once

#include "PNGFile.hpp"
#include <fstream>
#include <deque>

namespace relive
{
class IDDVReaderCallBacks
{
public:
    virtual ~IDDVReaderCallBacks() { }
    virtual void OnVideoFrame(const u32* pixels, u32 width, u32 height) = 0;
    virtual void OnAudioFrame(const void* data, u32 sampleSizeBytes, u32 sampleCount) = 0;
};

class DDVDumper final : public IDDVReaderCallBacks
{
public:
    DDVDumper()
    {
        audio = fopen("fmv_debug/Audio.dat", "wb");
    }

    ~DDVDumper()
    {
        if (audio)
        {
            fclose(audio);
        }
    }

    void OnVideoFrame(const u32* pixels, u32 width, u32 height) override
    {
        frameIndex++;

        PNGFile png;
        const auto pngData = png.Encode((const u32*)pixels, width, height);
        const std::string path = "fmv_debug/masher_frame_" + std::to_string(frameIndex) + ".png";

        std::ofstream out(path, std::ios::binary);
        if (out)
        {
            out.write(reinterpret_cast<const char*>(pngData.data()), static_cast<std::streamsize>(pngData.size()));
            //LOG_INFO("Saved debug frame %u to '%s'", frameIndex, path.c_str());
        }
    }

    void OnAudioFrame(const void* data, u32 sampleSizeBytes, u32 sampleCount) override
    {
        if (audio)
        {
            fwrite(data, sampleSizeBytes, sampleCount, audio);
        }
    }

private:
    FILE* audio = nullptr;
    u32 frameIndex = 0;
};

class DDVAe final
{
public:
    DDVAe(const char_type* pDDVName, IDDVReaderCallBacks& callBacks);
    ~DDVAe();
    bool StepFrame();
    bool ReadInfo();
    u32 FrameWidth();
    u32 FrameHeight();
    u32 FrameRate();
    u32 TotalVideoFrames();
    u32 AudioSampleRate();
    u32 AudioChannels();
    u32 AudioBitsPerSample();
    const std::vector<u8>& GetPixels() const { return mPixels; }
    std::vector<u8> GetAudioFrames()
    { 
        if (!mInterleaveAudioFrames.empty())
        {
            auto tmp = mInterleaveAudioFrames.front();
            mInterleaveAudioFrames.pop_front();
            return tmp;
        }
        return mAudioFrames; 
    }
private:
    std::string mDDvName;
    IDDVReaderCallBacks& mCallBacks;
    std::vector<u8> mPixels;

    s8 DDV_StartAudio();
public:
    std::deque<std::vector<u8>> mInterleaveAudioFrames;
    std::vector<u8> mAudioFrames;
};
}
