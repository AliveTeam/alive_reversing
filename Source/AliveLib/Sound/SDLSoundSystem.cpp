#include "stdafx.h"
#include "SDLSoundSystem.hpp"
#include "SDLSoundBuffer.hpp"
#include "Reverb.hpp"
#include "Sys.hpp"

void SDLSoundSystem::Init(unsigned int /*sampleRate*/, int /*bitsPerSample*/, int /*isStereo*/)
{
    if (SDL_Init(SDL_INIT_AUDIO) != 0)
    {
        LOG_ERROR("SDL_Init(SDL_INIT_AUDIO) failed " << SDL_GetError());
        return;
    }

    for (int i = 0; i < SDL_GetNumAudioDrivers(); i++)
    {
        LOG_INFO("SDL Audio Driver " << i << " " << SDL_GetAudioDriver(i));
    }

    mAudioDeviceSpec.callback = SDLSoundSystem::AudioCallBackStatic;
    mAudioDeviceSpec.format = AUDIO_S16;
    mAudioDeviceSpec.channels = 2;
    mAudioDeviceSpec.freq = 44100;
    mAudioDeviceSpec.samples = 256;
    mAudioDeviceSpec.userdata = this;

    if (SDL_OpenAudio(&mAudioDeviceSpec, NULL) < 0)
    {
        LOG_ERROR("Couldn't open SDL audio: " << SDL_GetError());
        return;
    }

    LOG_INFO("-----------------------------");
    LOG_INFO("Audio Device opened, got specs:");
    LOG_INFO(
        "Channels: " << mAudioDeviceSpec.channels << " " <<
        "nFormat: " << mAudioDeviceSpec.format << " " <<
        "nFreq: " << mAudioDeviceSpec.freq << " " <<
        "nPadding: " << mAudioDeviceSpec.padding << " " <<
        "nSamples: " << mAudioDeviceSpec.samples << " " <<
        "nSize: " << mAudioDeviceSpec.size << " " <<
        "nSilence: " << mAudioDeviceSpec.silence);
    LOG_INFO("-----------------------------");

    Reverb_Init(mAudioDeviceSpec.freq);

    SDL_PauseAudio(0);

    SND_InitVolumeTable_4EEF60();

    if (sLoadedSoundsCount_BBC394)
    {
        for (int i = 0; i < 256; i++)
        {
            if (sSoundSamples_BBBF38[i])
            {
                SND_Renew_4EEDD0(sSoundSamples_BBBF38[i]);
                SND_LoadSamples_4EF1C0(sSoundSamples_BBBF38[i], 0, sSoundSamples_BBBF38[i]->field_8_pSoundBuffer, sSoundSamples_BBBF38[i]->field_C_buffer_size_bytes / (unsigned __int8)sSoundSamples_BBBF38[i]->field_1D_blockAlign);
                if ((i + 1) == sLoadedSoundsCount_BBC394)
                    break;
            }
        }
    }

    sLastNotePlayTime_BBC33C = SYS_GetTicks();
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
    // TODO: Free other buffers, safely end audio call back etc
    delete this;
    return S_OK;
}

void SDLSoundSystem::AudioCallBack(Uint8 *stream, int len)
{
    memset(stream, 0, len);
    RenderAudio(reinterpret_cast<StereoSample_S16 *>(stream), len / sizeof(StereoSample_S16));
}

void SDLSoundSystem::RenderAudio(StereoSample_S16* pSampleBuffer, int sampleBufferCount)
{
    // Check if our buffer size changes, and if its buffer, then resize the array
    if (sampleBufferCount > mCurrentSoundBufferSize)
    {
        if (mTempSoundBuffer != nullptr)
        {
            delete[] mTempSoundBuffer;
            mTempSoundBuffer = nullptr;
        }

        if (mNoReverbBuffer != nullptr)
        {
            delete[] mNoReverbBuffer;
            mNoReverbBuffer = nullptr;
        }

        mCurrentSoundBufferSize = sampleBufferCount;
    }

    if (mTempSoundBuffer == nullptr)
    {
        mTempSoundBuffer = new StereoSample_S16[sampleBufferCount];
    }
    if (mNoReverbBuffer == nullptr)
    {
        mNoReverbBuffer = new StereoSample_S16[sampleBufferCount];
    }

    memset(mNoReverbBuffer, 0, sampleBufferCount * sizeof(StereoSample_S16));

    for (int vi = 0; vi < MAX_VOICE_COUNT; vi++)
    {
        SDLSoundBuffer * pVoice = sAE_ActiveVoices[vi];
        if (pVoice)
        {
            RenderSoundBuffer(*pVoice, pSampleBuffer, sampleBufferCount);
        }
    }

    // Do Reverb Pass
    if (gReverbEnabled)
    {
        Reverb_Mix(pSampleBuffer, AUDIO_S16, sampleBufferCount * sizeof(StereoSample_S16), kMixVolume);

        // Mix our no reverb buffer
        SDL_MixAudioFormat(reinterpret_cast<Uint8 *>(pSampleBuffer), reinterpret_cast<Uint8 *>(mNoReverbBuffer), AUDIO_S16, sampleBufferCount * sizeof(StereoSample_S16), kMixVolume);
    }
}


void SDLSoundSystem::RenderSoundBuffer(SDLSoundBuffer& entry, StereoSample_S16* pSampleBuffer, int sampleBufferCount)
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
    memset(mTempSoundBuffer, 0, sampleBufferCount * sizeof(StereoSample_S16));

    Sint16* pVoiceBufferPtr = reinterpret_cast<Sint16*>(pVoice->GetBuffer()->data());

    for (int i = 0; i < sampleBufferCount; i++)
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
            reverbPass = false; // Todo: determine this with flags in the sound object itself.
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
        SDL_MixAudioFormat(reinterpret_cast<Uint8 *>(pSampleBuffer), reinterpret_cast<Uint8 *>(mTempSoundBuffer), AUDIO_S16, sampleBufferCount * sizeof(StereoSample_S16), 45);
    }
    else
    {
        SDL_MixAudioFormat(reinterpret_cast<Uint8 *>(mNoReverbBuffer), reinterpret_cast<Uint8 *>(mTempSoundBuffer), AUDIO_S16, sampleBufferCount * sizeof(StereoSample_S16), 45);
    }
}


void SDLSoundSystem::RenderMonoSample(Sint16* pVoiceBufferPtr, SDLSoundBuffer* pVoice, int i)
{
    int s = 0;

    switch (mAudioFilterMode)
    {
    case AudioFilterMode::NoFilter:
        s = pVoiceBufferPtr[static_cast<int>(pVoice->mState.fPlaybackPosition)];
        break;
    case AudioFilterMode::Linear:
        const signed short s1 = pVoiceBufferPtr[static_cast<int>(pVoice->mState.fPlaybackPosition)];
        const signed short s2 = pVoiceBufferPtr[(static_cast<int>(pVoice->mState.fPlaybackPosition) + 1) % pVoice->mState.iSampleCount];

        s = static_cast<int>((s1 + ((s2 - s1) * (pVoice->mState.fPlaybackPosition - floorf(pVoice->mState.fPlaybackPosition)))));
        break;
    }

    signed int leftPan = 10000;
    signed int rightPan = 10000;

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
        signed int r = (leftPan + rightPan) / 2;
        leftPan = r;
        rightPan = r;
    }

    mTempSoundBuffer[i].left = static_cast<signed short>((((s * leftPan) / 10000) * pVoice->mState.iVolume) / 127);
    mTempSoundBuffer[i].right = static_cast<signed short>((((s * rightPan) / 10000) * pVoice->mState.iVolume) / 127);

    pVoice->mState.fPlaybackPosition += pVoice->mState.fFrequency;
}

void SDLSoundSystem::RenderStereoSample(Sint16* pVoiceBufferPtr, SDLSoundBuffer* pVoice, int i)
{
    StereoSample_S16 pSample = reinterpret_cast<StereoSample_S16*>(pVoiceBufferPtr)[static_cast<int>(pVoice->mState.fPlaybackPosition)];
    mTempSoundBuffer[i].left = static_cast<signed short>((pSample.left * pVoice->mState.iVolume) / 127);
    mTempSoundBuffer[i].right = static_cast<signed short>((pSample.right * pVoice->mState.iVolume) / 127);

    pVoice->mState.fPlaybackPosition += pVoice->mState.fFrequency;
}

void SDLSoundSystem::AudioCallBackStatic(void* userdata, Uint8 *stream, int len)
{
    static_cast<SDLSoundSystem*>(userdata)->AudioCallBack(stream, len);
}
