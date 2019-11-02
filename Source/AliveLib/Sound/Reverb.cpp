#include "stdafx.h"
#include "Reverb.hpp"

#if USE_SDL2_SOUND

#include <math.h>
#include <iostream>

// Reference https://www.vegardno.net/2016/05/writing-reverb-filter-from-first.html

const int ReverbEchos = 24;
static StereoSample_S16 sReverbBuffer[1024 * 32];
const float gReverbMix = 1.0f / ReverbEchos;

class FeedbackBuffer
{
public:
    FeedbackBuffer(int samples)
    {
        iSamples = samples;
        pBuffer = new StereoSample_S32[samples];
        for (int i = 0; i < samples; i++)
        {
            pBuffer[i].left = 0;
            pBuffer[i].right = 0;
        }
    }

    ~FeedbackBuffer()
    {
        delete[] pBuffer;
    }

    int idx = 0;
    int iSamples;
    StereoSample_S32 * pBuffer;

    inline void PushSample(StereoSample_S16 s)
    {
        pBuffer[idx].left += s.left;
        pBuffer[idx].right += s.right;
        
        idx++;

        if (idx >= iSamples)
        {
            idx = 0;
        }
    }

    inline StereoSample_S32 GetSample()
    {
        const StereoSample_S32 f = pBuffer[idx];
        pBuffer[idx].left -= pBuffer[idx].left / 10;
        pBuffer[idx].right -= pBuffer[idx].right / 10;
        return f;
    }
};

std::vector<FeedbackBuffer*> feedbackBuffers;

void Reverb_Init(int sampleRate)
{
    int sampleGap = sampleRate / 800;
    sampleGap = sampleGap;
    for (int i = 1; i <= ReverbEchos; i++)
    {
        feedbackBuffers.push_back(new FeedbackBuffer((sampleGap + (i * 2)) * i));
    }
}

inline void Reverb_PushSample(StereoSample_S16 v)
{
    const size_t feedbackArySize = feedbackBuffers.size();
    FeedbackBuffer ** feedbackAry = feedbackBuffers.data();
    for (unsigned int i = 0; i < feedbackArySize; i++)
    {
        feedbackAry[i]->PushSample(v);
    }
}

inline void Reverb_Update(int index)
{
    sReverbBuffer[index].left = 0;
    sReverbBuffer[index].right = 0;

    const size_t feedbackArySize = feedbackBuffers.size();
    FeedbackBuffer ** feedbackAry = feedbackBuffers.data();

    for (unsigned int i = 0; i < feedbackArySize; i++)
    {
        StereoSample_S32 v = feedbackAry[i]->GetSample();

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