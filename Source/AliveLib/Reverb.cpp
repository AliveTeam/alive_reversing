#include "stdafx.h"
#include "Reverb.hpp"

#if USE_SDL2_SOUND

#include <math.h>
#include <iostream>

Echo mEchos[REVERB_ECHO_COUNT];
StereoSampleFloat mLastReverbSample[REVERB_ECHO_COUNT];

static StereoSampleFloat mReverbGrains[REVERB_GRAINS_SIZE];
static int mReverbBufferIndex = 0;
static StereoSampleFloat sReverbBuffer[1024 * 32];
static float gReverbMix = 0.05f;

void Reverb_Init(int sampleRate)
{
    for (int i = 1; i < REVERB_ECHO_COUNT + 1; i++)
    {
        Echo echo;
        echo.delay = static_cast<int>((i * ((REVERB_ROOM_SIZE * 100) / REVERB_ECHO_COUNT) + (rand() % (REVERB_ROOM_SIZE / 2))) * sampleRate / 44100.0f);
        echo.volume = (powf(static_cast<float>(REVERB_ECHO_COUNT - i), 4.0f) / powf(static_cast<float>(REVERB_ECHO_COUNT), 4.0f)) * (REVERB_ECHO_COUNT / 100.0f);

        mEchos[i] = echo;
    }
}

void Reverb_PushSample(StereoSampleFloat v)
{
    mReverbBufferIndex++;
    if (mReverbBufferIndex >= REVERB_GRAINS_SIZE)
        mReverbBufferIndex = 0;

    mReverbGrains[mReverbBufferIndex] = v;
}

inline void Reverb_Update(int index)
{
    sReverbBuffer[index].left = 0;
    sReverbBuffer[index].right = 0;

    for (unsigned int i = 0; i < REVERB_ECHO_COUNT; i++)
    {
        const Echo e = mEchos[i];

        if (e.volume <= 0)
            continue;

        int reverbBufferSampleIndex = ((mReverbBufferIndex - e.delay) + (REVERB_GRAINS_SIZE * 24)) % REVERB_GRAINS_SIZE;

        StereoSampleFloat r;

        StereoSampleFloat v = mReverbGrains[reverbBufferSampleIndex];

        const float reverbLowPassAmount = (REVERB_LOW_PASS * (static_cast<float>(i) / REVERB_ECHO_COUNT));
        const float reverbLowPass = 1.0f - reverbLowPassAmount;

        // Low Pass
        // Slowly become more and more low passed as we go through our reverb echos
        r.left = mLastReverbSample[i].left + ((v.left - mLastReverbSample[i].left) * reverbLowPass);
        r.right = mLastReverbSample[i].right + ((v.right - mLastReverbSample[i].right) * reverbLowPass);
        mLastReverbSample[i] = r;
        v = r;

        // High Pass - Take our normal audio and negate the low pass.
        // This creates a high pass.
        /*v.left -= r.left;
        v.right -= r.right;*/

        sReverbBuffer[index].left += v.left * e.volume;
        sReverbBuffer[index].right += v.right * e.volume;
    }
}

void Reverb_Mix(StereoSampleFloat * dst, SDL_AudioFormat format, Uint32 len, int volume, int channels)
{
    for (unsigned int i = 0; i < len / sizeof(StereoSampleFloat); i+= channels / 2)
    {
        Reverb_PushSample(dst[i]);

        Reverb_Update(i);
    }

    for (unsigned int i = 0; i < len / sizeof(StereoSampleFloat); i++)
    {
        sReverbBuffer[i].left *= gReverbMix;
        sReverbBuffer[i].right *= gReverbMix;
    }

    SDL_MixAudioFormat(reinterpret_cast<Uint8 *>(dst), reinterpret_cast<Uint8 *>(sReverbBuffer), format, len, volume);
    //memcpy(dst, sReverbStream, len); // Uncomment to hear only reverb
}

#endif