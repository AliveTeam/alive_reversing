#include "../stdafx.h"
#include "SDLSoundSystem.hpp"
#include "SDLSoundBuffer.hpp"
#include "Reverb.hpp"
#include "../../relive_lib/Sys.hpp"
#include <functional>
#include <cmath>

extern bool gLatencyHack;

void SDLSoundSystem::Init(u32 /*sampleRate*/, s32 /*bitsPerSample*/, s32 /*isStereo*/)
{
    mCreated = false;

    if (!SDL_InitSubSystem(SDL_INIT_AUDIO))
    {
        LOG_ERROR("SDL_Init(SDL_INIT_AUDIO) failed %s", SDL_GetError());
        return;
    }

    for (s32 i = 0; i < SDL_GetNumAudioDrivers(); i++)
    {
        LOG_INFO("SDL Audio Driver %d %s", i, SDL_GetAudioDriver(i));
    }

    mAudioDeviceSpec.format = SDL_AUDIO_S16;
    mAudioDeviceSpec.channels = 2;
    mAudioDeviceSpec.freq = 44100;

    mAudioStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &mAudioDeviceSpec, SDLSoundSystem::AudioCallBackStatic, this);
    if (!mAudioStream)
    {
        LOG_ERROR("Couldn't open SDL audio: %d", SDL_GetError());
        return;
    }

    mAudioDevice = SDL_GetAudioStreamDevice(mAudioStream);

    //SDL_SetAudioStreamGain(mAudioStream, 0.0f);

    LOG_INFO("-----------------------------");
    LOG_INFO("Audio Device opened, got specs:");
    LOG_INFO("Channels: %d nFormat: %d nFreq: %d", static_cast<s32>(mAudioDeviceSpec.channels), mAudioDeviceSpec.format, mAudioDeviceSpec.freq);
    LOG_INFO("Driver: %s", SDL_GetCurrentAudioDriver());
    LOG_INFO("-----------------------------");

    Reverb_Init(mAudioDeviceSpec.freq);

    GetSoundAPI().SND_InitVolumeTable();

    if (sLoadedSoundsCount_BBC394)
    {
        for (s32 i = 0; i < 256; i++)
        {
            if (sSoundSamples_BBBF38[i])
            {
                GetSoundAPI().mSND_Renew(sSoundSamples_BBBF38[i]);
                GetSoundAPI().mSND_LoadSamples(sSoundSamples_BBBF38[i], 0, sSoundSamples_BBBF38[i]->field_8_pSoundBuffer, sSoundSamples_BBBF38[i]->field_C_buffer_size_bytes / (u8) sSoundSamples_BBBF38[i]->field_1D_blockAlign);
                if ((i + 1) == sLoadedSoundsCount_BBC394)
                    break;
            }
        }
    }

    sLastNotePlayTime_BBC33C = SYS_GetTicks();
    mCreated = true;

    // Correctly size the lock free buffer on the main thread before any other threads start
    mAudioRingBuffer.resize(2048 * 2);

    // TODO: Test just running this on the main thread
    if (!gLatencyHack)
    {
        mRenderAudioThread.reset(new std::thread(std::bind(&SDLSoundSystem::RenderAudioThread, this)));
    }

    SDL_ResumeAudioDevice(mAudioDevice);
}


HRESULT SDLSoundSystem::DuplicateSoundBuffer(TSoundBufferType* pDSBufferOriginal, TSoundBufferType** ppDSBufferDuplicate)
{
    pDSBufferOriginal->Duplicate(ppDSBufferDuplicate);
    return S_OK;
}

HRESULT SDLSoundSystem::CreateSoundBuffer(LPCDSBUFFERDESC pcDSBufferDesc, TSoundBufferType** ppDSBuffer, void* /*pUnkOuter*/)
{
    *ppDSBuffer = new SDLSoundBuffer(*pcDSBufferDesc, mAudioDeviceSpec.freq);
    return S_OK;
}


HRESULT SDLSoundSystem::Release()
{
    TRACE_ENTRYEXIT;

    if (mCreated)
    {
        // Stop the audio call back
        SDL_PauseAudioStreamDevice(mAudioStream);

        // Stop audio rendering thread
        mRenderAudioThreadQuit = true;
        if (mRenderAudioThread && mRenderAudioThread->joinable())
        {
            mRenderAudioThread->join();
        }
    }

    if (mAudioStream)
    {
        SDL_DestroyAudioStream(mAudioStream);
        mAudioStream = nullptr;
        mAudioDevice = 0;
    }

    // Shutdown the sound system
    SDL_QuitSubSystem(SDL_INIT_AUDIO);

    delete this;
    return S_OK;
}


SDLSoundSystem::~SDLSoundSystem()
{
    TRACE_ENTRYEXIT;

    Reverb_DeInit();

    // TODO: Clean up outstanding samples in sAE_ActiveVoices
}

void SDLSoundSystem::AudioCallBack(SDL_AudioStream* stream, s32 additionalAmount)
{
    if (additionalAmount <= 0)
    {
        return;
    }

    const s32 bufferLenSamples = additionalAmount / sizeof(StereoSample_S16);
    std::vector<StereoSample_S16> buffer(bufferLenSamples);
    
    if (gLatencyHack)
    {
        // Calculate the audio in the callback instead of another thread with a busy loop to reduce CPU usage
        // this will probably cause audio glitching in a lot of cases
        RenderAudio(buffer.data(), bufferLenSamples);
    }
    else
    {
        const s32 readAvilSamples = static_cast<s32>(mAudioRingBuffer.getAvailableRead());
        if (readAvilSamples > 0 && readAvilSamples < bufferLenSamples)
        {
            LOG_WARNING("Audio buffer underflow!");
        }

        if (!mAudioRingBuffer.read(buffer.data(), bufferLenSamples))
        {
            LOG_ERROR("Ring buffer read failure!");
        }
    }

    SDL_PutAudioStreamData(stream, buffer.data(), additionalAmount);
}


void SDLSoundSystem::RenderAudioThread()
{
    std::vector<StereoSample_S16> tmpBuffer;
    while (!mRenderAudioThreadQuit)
    {
        const size_t bufferSize = mAudioRingBuffer.getAvailableWrite();
        if (bufferSize > 0)
        {
            tmpBuffer.resize(bufferSize);
            memset(tmpBuffer.data(), 0, tmpBuffer.size() * sizeof(StereoSample_S16));
            RenderAudio(tmpBuffer.data(), static_cast<s32>(tmpBuffer.size()));
            if (!mAudioRingBuffer.write(tmpBuffer.data(), tmpBuffer.size()))
            {
                // Couldn't write all the data, should never happen ??
                LOG_ERROR("Ring buffer write failed");
            }
        }
    }

}

void SDLSoundSystem::RenderAudio(StereoSample_S16* pSampleBuffer, s32 sampleBufferCount)
{
    // Check if our buffer size changes, and if its buffer, then resize the array
    if (sampleBufferCount > mCurrentSoundBufferSize)
    {
        mTempSoundBuffer.resize(sampleBufferCount);
        mNoReverbBuffer.resize(sampleBufferCount);
        mCurrentSoundBufferSize = sampleBufferCount;
    }

    if (mTempSoundBuffer.empty())
    {
        mTempSoundBuffer.resize(sampleBufferCount);
        mNoReverbBuffer.resize(sampleBufferCount);
    }

    memset(mNoReverbBuffer.data(), 0, sampleBufferCount * sizeof(StereoSample_S16));

    for (s32 vi = 0; vi < MAX_VOICE_COUNT; vi++)
    {
        SDLSoundBuffer* pVoice = sAE_ActiveVoices[vi];
        if (pVoice)
        {
            RenderSoundBuffer(*pVoice, pSampleBuffer, sampleBufferCount);
        }
    }

    // Do Reverb Pass
    if (gReverbEnabled)
    {
        Reverb_Mix(pSampleBuffer, SDL_AUDIO_S16, sampleBufferCount * sizeof(StereoSample_S16), kMixVolume);

        // Mix our no reverb buffer
        SDL_MixAudio(reinterpret_cast<Uint8*>(pSampleBuffer), reinterpret_cast<Uint8*>(mNoReverbBuffer.data()), SDL_AUDIO_S16, sampleBufferCount * sizeof(StereoSample_S16), kMixVolume);
    }
}


void SDLSoundSystem::RenderSoundBuffer(SDLSoundBuffer& entry, StereoSample_S16* pSampleBuffer, s32 sampleBufferCount)
{
    bool reverbPass = false;

    SDLSoundBuffer* pVoice = &entry;

    if (pVoice == nullptr || !pVoice->mBuffer || pVoice->mBuffer->empty())
    {
        return;
    }

    if (pVoice->mState.bIsReleased)
    {
        pVoice->Destroy(); // TODO: Still correct ??
        return;
    }

    // Clear Temp Sample Buffer
    memset(mTempSoundBuffer.data(), 0, sampleBufferCount * sizeof(StereoSample_S16));

    Sint16* pVoiceBufferPtr = reinterpret_cast<Sint16*>(pVoice->GetBuffer()->data());

    for (s32 i = 0; i < sampleBufferCount; i++)
    {
        if (pVoice->mBuffer->empty() || pVoice->mState.eStatus != SDLSoundBufferStatus::Playing || pVoice->mState.iSampleCount == 0)
        {
            break;
        }

        if (pVoice->mState.iVolume < pVoice->mState.iVolumeTarget)
        {
            pVoice->mState.iVolume++;
        }
        else if (pVoice->mState.iVolume > pVoice->mState.iVolumeTarget)
        {
            pVoice->mState.iVolume--;
        }

        if (pVoice->mState.iChannels == 2)
        {
            reverbPass = true; // Todo: determine this with flags in the sound object itself.
                               // For Stereo buffers. The only time this is played is for FMV's.
                               // Right now, unless the playback device is at 44100 hz, it sounds awful.
                               // TODO: Resampling for stereo

            RenderStereoSample(pVoiceBufferPtr, pVoice, i);
        }
        else
        {
            reverbPass = true;

            RenderMonoSample(pVoiceBufferPtr, pVoice, i);
        }

        if (pVoice->mState.fPlaybackPosition >= pVoice->mState.iSampleCount / pVoice->mState.iChannels)
        {
            pVoice->mState.fPlaybackPosition = 0;
            if (!pVoice->mState.bLoop)
            {
                pVoice->mState.eStatus = SDLSoundBufferStatus::Stopped;
            }
        }
    }

    if (reverbPass)
    {
        SDL_MixAudio(reinterpret_cast<Uint8*>(pSampleBuffer), reinterpret_cast<Uint8*>(mTempSoundBuffer.data()), SDL_AUDIO_S16, sampleBufferCount * sizeof(StereoSample_S16), 0.35f);
    }
    else
    {
        SDL_MixAudio(reinterpret_cast<Uint8*>(mNoReverbBuffer.data()), reinterpret_cast<Uint8*>(mTempSoundBuffer.data()), SDL_AUDIO_S16, sampleBufferCount * sizeof(StereoSample_S16), 0.35f);
    }
}


void SDLSoundSystem::RenderMonoSample(Sint16* pVoiceBufferPtr, SDLSoundBuffer* pVoice, s32 i)
{
    s32 s = 0;

    switch (mAudioFilterMode)
    {
        case AudioFilterMode::NoFilter:
            s = pVoiceBufferPtr[static_cast<s32>(pVoice->mState.fPlaybackPosition)];
            break;
        case AudioFilterMode::Linear:
            const s16 s1 = pVoiceBufferPtr[static_cast<s32>(pVoice->mState.fPlaybackPosition)];
            const s16 s2 = pVoiceBufferPtr[(static_cast<s32>(pVoice->mState.fPlaybackPosition) + 1) % pVoice->mState.iSampleCount];

            s = static_cast<s32>((s1 + ((s2 - s1) * (pVoice->mState.fPlaybackPosition - floorf(pVoice->mState.fPlaybackPosition)))));
            break;
    }

    s32 leftPan = 10000;
    s32 rightPan = 10000;

    if (gAudioStereo)
    {
        if (pVoice->mState.iPan < 0)
        {
            rightPan = 10000 - abs(pVoice->mState.iPan);
        }
        else if (pVoice->mState.iPan > 0)
        {
            leftPan = 10000 - abs(pVoice->mState.iPan);
        }
    }
    else
    {
        s32 r = (leftPan + rightPan) / 2;
        leftPan = r;
        rightPan = r;
    }

    mTempSoundBuffer[i].left = static_cast<s16>((((s * leftPan) / 10000) * pVoice->mState.iVolume) / 127);
    mTempSoundBuffer[i].right = static_cast<s16>((((s * rightPan) / 10000) * pVoice->mState.iVolume) / 127);

    pVoice->mState.fPlaybackPosition += pVoice->mState.fFrequency;
}

void SDLSoundSystem::RenderStereoSample(Sint16* pVoiceBufferPtr, SDLSoundBuffer* pVoice, s32 i)
{
    StereoSample_S16 pSample = reinterpret_cast<StereoSample_S16*>(pVoiceBufferPtr)[static_cast<s32>(pVoice->mState.fPlaybackPosition)];
    mTempSoundBuffer[i].left = static_cast<s16>((pSample.left * pVoice->mState.iVolume) / 127);
    mTempSoundBuffer[i].right = static_cast<s16>((pSample.right * pVoice->mState.iVolume) / 127);

    pVoice->mState.fPlaybackPosition += pVoice->mState.fFrequency;
}

void SDLSoundSystem::AudioCallBackStatic(void* userdata, SDL_AudioStream* stream, s32 additionalAmount, s32 /*totalAmount*/)
{
    static_cast<SDLSoundSystem*>(userdata)->AudioCallBack(stream, additionalAmount);
}
