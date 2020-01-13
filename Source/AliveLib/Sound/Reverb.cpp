#include "stdafx.h"
#include "Reverb.hpp"

#if USE_SDL2_SOUND

#include <math.h>
#include <iostream>
#include <memory>
#include <vector>

// Reference https://www.vegardno.net/2016/05/writing-reverb-filter-from-first.html

const int ReverbEchos = 24;
static StereoSample_S16 sReverbBuffer[1024 * 32] = {};
const float gReverbMix = 1.0f / ReverbEchos;

class FeedbackBuffer
{
public:
    explicit FeedbackBuffer(int samples)
        : mSamples(samples)
    {
        mBuffer.resize(samples);
        for (auto& sample : mBuffer)
        {
            sample.left = 0;
            sample.right = 0;
        }
    }

    void PushSample(StereoSample_S16 s)
    {
        mBuffer[mIdx].left += s.left;
        mBuffer[mIdx].right += s.right;
        
        mIdx++;

        if (mIdx >= mSamples)
        {
            mIdx = 0;
        }
    }

    StereoSample_S32 GetSample()
    {
        const StereoSample_S32 f = mBuffer[mIdx];
        mBuffer[mIdx].left -= mBuffer[mIdx].left / 10;
        mBuffer[mIdx].right -= mBuffer[mIdx].right / 10;
        return f;
    }

    int mIdx = 0;
    int mSamples;
    std::vector<StereoSample_S32> mBuffer;
};

static std::vector<std::unique_ptr<FeedbackBuffer>> gFeedbackBuffers;

void Reverb_Init(int sampleRate)
{
    const int sampleGap = sampleRate / 800;
    for (int i = 1; i <= ReverbEchos; i++)
    {
        gFeedbackBuffers.emplace_back(new FeedbackBuffer((sampleGap + (i * 2)) * i)); // make_unique not in all supports cpp stdlibs
    }
}

void Reverb_DeInit()
{
    gFeedbackBuffers.clear();
}

void Reverb_PushSample(StereoSample_S16 v)
{
    for (auto& buffer : gFeedbackBuffers)
    {
        buffer->PushSample(v);
    }
}

inline void Reverb_Update(int index)
{
    sReverbBuffer[index].left = 0;
    sReverbBuffer[index].right = 0;

    for (auto& buffer : gFeedbackBuffers)
    {
        const StereoSample_S32 v = buffer->GetSample();

        sReverbBuffer[index].left += static_cast<signed short>(v.left * gReverbMix);
        sReverbBuffer[index].right += static_cast<signed short>(v.right * gReverbMix);
    }
}

void Reverb_Mix(StereoSample_S16 * dst, SDL_AudioFormat format, Uint32 len, int volume)
{
    for (unsigned int i = 0; i < len / sizeof(StereoSample_S16); i++)
    {
        Reverb_PushSample(dst[i]);
        Reverb_Update(i);
    }

    SDL_MixAudioFormat(reinterpret_cast<Uint8 *>(dst), reinterpret_cast<Uint8 *>(sReverbBuffer), format, len, volume);
    // memcpy(dst, sReverbBuffer, len); // Uncomment to hear only reverb
}

#endif
