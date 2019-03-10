#include "stdafx.h"
#include "Reverb.hpp"

#if USE_SDL2_SOUND

#include <math.h>
#include <iostream>

// Reference https://www.vegardno.net/2016/05/writing-reverb-filter-from-first.html

const int ReverbEchos = 24;
static StereoSampleFloat sReverbBuffer[1024 * 32];
static float gReverbMix = 0.05f;

class FeedbackBuffer
{
public:
    FeedbackBuffer(int samples)
    {
        iSamples = samples;
        pBuffer = new StereoSampleFloat[samples];
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
    StereoSampleFloat * pBuffer;

    inline void PushSample(StereoSampleFloat s)
    {
        pBuffer[idx].left += s.left;
        pBuffer[idx].right += s.right;
        
        idx++;

        if (idx >= iSamples)
        {
            idx = 0;
        }
    }

    inline StereoSampleFloat GetSample()
    {
        const StereoSampleFloat f = pBuffer[idx];
        pBuffer[idx].left *= 0.88f;
        pBuffer[idx].right *= 0.88f;
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

inline void Reverb_PushSample(StereoSampleFloat v)
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
        float volume = 1.0f;

        StereoSampleFloat v = feedbackAry[i]->GetSample();

        sReverbBuffer[index].left += v.left * volume * gReverbMix;
        sReverbBuffer[index].right += v.right * volume * gReverbMix;
    }
}

void Reverb_Mix(StereoSampleFloat * dst, SDL_AudioFormat format, Uint32 len, int volume)
{
    for (unsigned int i = 0; i < len / sizeof(StereoSampleFloat); i++)
    {
        Reverb_PushSample(dst[i]);
        Reverb_Update(i);
    }

    SDL_MixAudioFormat(reinterpret_cast<Uint8 *>(dst), reinterpret_cast<Uint8 *>(sReverbBuffer), format, len, volume);
    //memcpy(dst, sReverbBuffer, len); // Uncomment to hear only reverb
}

#endif