#include "stdafx.h"
#include "SoundSDL.hpp"

#if USE_SDL2_SOUND

#include "Function.hpp"
#include "Error.hpp"
#include "Sound/Midi.hpp"
#include <mutex>
#include "Reverb.hpp"
#include "Sys.hpp"

#include <cinder/audio/dsp/RingBuffer.h>

#define MAX_VOICE_COUNT 1024

bool gReverbEnabled = false;
bool gAudioStereo = true;


SDLSoundBuffer* sAE_ActiveVoices[MAX_VOICE_COUNT] = {};

static SDL_AudioSpec gAudioDeviceSpec = {};

void AddVoiceToActiveList(SDLSoundBuffer * pVoice)
{
    for (int i = 0; i < MAX_VOICE_COUNT; i++)
    {
        if (sAE_ActiveVoices[i] == nullptr)
        {
            sAE_ActiveVoices[i] = pVoice;
            return;
        }
    }

    printf("WARNING !!: Failed to allocate voice! No space left!\n");
}

void RemoveVoiceFromActiveList(SDLSoundBuffer * pVoice)
{
    for (int i = 0; i < MAX_VOICE_COUNT; i++)
    {
        if (sAE_ActiveVoices[i] == pVoice)
        {
            sAE_ActiveVoices[i] = nullptr;
            return;
        }
    }

    printf("WARNING !!: Could not find voice to remove!?!\n");
}

SDLSoundBuffer::SDLSoundBuffer()
{
    mState.iVolume = 0;
    mState.iVolumeTarget = 127;
    mState.bVolDirty = false;
    mState.iPan = 0;
    mState.fFrequency = 1.0f;
    mState.bIsReleased = false;
    mState.bLoop = false;
    mState.iChannels = 1;
    mState.fPlaybackPosition = 0;
    mState.eStatus = AE_SDL_Voice_Status::Stopped;

    AddVoiceToActiveList(this);
}

int SDLSoundBuffer::SetVolume(int volume)
{
    mState.iVolumeTarget = volume;

    if (!mState.bVolDirty)
    {
        mState.iVolume = mState.iVolumeTarget;
    }

    mState.bVolDirty = true;

    return 0;
}

int SDLSoundBuffer::Play(int, int, int flags)
{
    mState.fPlaybackPosition = 0;
    mState.eStatus = AE_SDL_Voice_Status::Playing;

    if (flags & DSBPLAY_LOOPING)
    {
        mState.bLoop = true;
    }

    return 0;
}

int SDLSoundBuffer::Stop()
{
    mState.eStatus = AE_SDL_Voice_Status::Stopped;

    return 0;
}

int SDLSoundBuffer::SetFrequency(int frequency)
{
    mState.fFrequency = frequency / static_cast<float>(gAudioDeviceSpec.freq);
    return 0;
}

int SDLSoundBuffer::SetCurrentPosition(int position) // This offset is apparently in bytes
{
    mState.fPlaybackPosition = static_cast<float>(position / mState.iBlockAlign);
    return 0;
}

int SDLSoundBuffer::GetCurrentPosition(DWORD * readPos, DWORD * writePos)
{
    *readPos = static_cast<DWORD>(mState.fPlaybackPosition * mState.iBlockAlign);
    *writePos = 0;

    return 0;
}

int SDLSoundBuffer::GetFrequency(DWORD * freq)
{
    *freq = static_cast<DWORD>(mState.fFrequency * gAudioDeviceSpec.freq);
    return 0;
}

int SDLSoundBuffer::SetPan(signed int pan)
{
    mState.iPan = pan;
    return 0;
}

void SDLSoundBuffer::Release()
{
    mState.bLoop = false; // because the SDL copy paste of SND_Free_4EFA30 was doing this
    mState.bIsReleased = true;
}

int SDLSoundBuffer::GetStatus(DWORD * r)
{
    if (mState.eStatus == AE_SDL_Voice_Status::Playing)
    {
        *r |= DSBSTATUS_PLAYING;
    }
    if (mState.bLoop)
    {
        *r |= DSBSTATUS_LOOPING;
    }
    if (mState.bIsReleased)
    {
        *r |= DSBSTATUS_TERMINATED;
    }
    return 0;
}

void SDLSoundBuffer::Destroy()
{
    // remove self from global list and
    // decrement shared mem ptr to audio buffer

    RemoveVoiceFromActiveList(this);
    delete this;
}

std::vector<BYTE>* SDLSoundBuffer::GetBuffer()
{
    return pBuffer.get();
}

int SDLSoundBuffer::Duplicate(SDLSoundBuffer** dupePtr)
{
    *dupePtr = new SDLSoundBuffer(*this);
    AddVoiceToActiveList(*dupePtr);
    return 0;
}

// Exoddus Functions:


EXPORT void CC SND_InitVolumeTable_4EEF60()
{
    for (int i = 0; i < 127; i++)
    {
        sVolumeTable_BBBD38[i] = i;
    }
    sVolumeTable_BBBD38[0] = 0;
}

int CC SND_Reload_SDL(SoundEntry* pSoundEntry, unsigned int sampleOffset, unsigned int size)
{
    const DWORD alignedOffset = sampleOffset * pSoundEntry->field_1D_blockAlign;
    const DWORD alignedSize = size * pSoundEntry->field_1D_blockAlign;

    // TODO: Should only clear from offset to size ??
    memset(pSoundEntry->field_4_pDSoundBuffer->GetBuffer()->data(), 0, pSoundEntry->field_14_buffer_size_bytes);

    return 0;
}

EXPORT signed int CC SND_Renew_4EEDD0(SoundEntry *pSnd)
{
    printf("SND_Renew_4EEDD0: %p\n", pSnd);
    return 0; // TODO
}

EXPORT signed int CC SND_Reload_4EF1C0(const SoundEntry* pSnd, DWORD sampleOffset, unsigned char* pSoundBuffer, unsigned int sampleCount)
{
    const int offsetBytes = sampleOffset * pSnd->field_1D_blockAlign;
    const unsigned int bufferSizeBytes = sampleCount * pSnd->field_1D_blockAlign;
    memcpy(reinterpret_cast<Uint8*>(pSnd->field_4_pDSoundBuffer->GetBuffer()->data()) + offsetBytes, pSoundBuffer, bufferSizeBytes);
    return 0;
}

EXPORT signed int CC SND_New_4EEFF0(SoundEntry *pSnd, int sampleLength, int sampleRate, int bitsPerSample, int isStereo)
{
    if (sLoadedSoundsCount_BBC394 < 256)
    {
        pSnd->field_1D_blockAlign = static_cast<unsigned char>(bitsPerSample * ((isStereo != 0) + 1) / 8);
        int sampleByteSize = sampleLength * pSnd->field_1D_blockAlign;

        SDLSoundBuffer * pDSoundBuffer = new SDLSoundBuffer();
        pDSoundBuffer->SetFrequency(sampleRate);
        pDSoundBuffer->mState.iSampleCount = sampleByteSize / 2;
        pDSoundBuffer->pBuffer = std::make_shared<std::vector<BYTE>>(std::vector<BYTE>(sampleByteSize));
        pDSoundBuffer->mState.iBlockAlign = pSnd->field_1D_blockAlign;
        pDSoundBuffer->mState.iChannels = (isStereo & 1) ? 2 : 1;
        pSnd->field_4_pDSoundBuffer = pDSoundBuffer;

        pSnd->field_10 = 0;
        unsigned char * bufferData = static_cast<unsigned char *>(malloc_4F4E60(sampleByteSize));
        pSnd->field_8_pSoundBuffer = bufferData;
        if (bufferData)
        {
            pSnd->field_18_sampleRate = sampleRate;
            pSnd->field_1C_bitsPerSample = static_cast<char>(bitsPerSample);
            pSnd->field_C_buffer_size_bytes = sampleByteSize;
            pSnd->field_14_buffer_size_bytes = sampleByteSize;
            pSnd->field_20_isStereo = isStereo;

            for (int i = 0; i < 256; i++)
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
    }
    else
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\SND.C", 568, -1, "SND_New: out of samples");
        return -1;
    }

    return -1;
}

signed int CC SND_CreateDS_SDL(unsigned int sampleRate, int bitsPerSample, int isStereo)
{
    sDSound_BBC344 = new SDLSoundSystem();
    sDSound_BBC344->Init(sampleRate, bitsPerSample, isStereo);
    return 0;
}

EXPORT char CC SND_CreatePrimarySoundBuffer_4EEEC0(int /*sampleRate*/, int /*bitsPerSample*/, int /*isStereo*/)
{
    return 0;
}

int SND_Play_SDL(const SoundEntry* pSnd, int volume, signed int pan, float freq, MIDI_Struct1* pMidiStru, int playFlags, int priority)
{
    if (!pSnd)
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\SND.C", 845, -1, "SND_PlayEx: NULL SAMPLE !!!");
        return -1;
    }

    SDLSoundBuffer* pBufferToUse = pSnd->field_4_pDSoundBuffer;
    if (!pBufferToUse)
    {
        return -1;
    }

    sLastNotePlayTime_BBC33C = SYS_GetTicks();

    if (!(pSnd->field_20_isStereo & 2))
    {
        SoundBuffer* pFreeBuffer = SND_Get_Sound_Buffer_4EF970(pSnd->field_0_tableIdx, priority);
        if (!pFreeBuffer)
        {
            return -1;
        }

        pBufferToUse->Duplicate(&pFreeBuffer->field_0_pDSoundBuffer);
        pBufferToUse = pFreeBuffer->field_0_pDSoundBuffer;

        if (pMidiStru)
        {
            pMidiStru->field_0_sound_buffer_field_4 = pFreeBuffer->field_4;
        }
    }

    pBufferToUse->SetCurrentPosition(0);

    DWORD freqHz = static_cast<DWORD>((pSnd->field_18_sampleRate * freq) + 0.5);
    if (freqHz < DSBFREQUENCY_MIN)
    {
        freqHz = DSBFREQUENCY_MIN;
    }
    else if (freqHz >= DSBFREQUENCY_MAX)
    {
        freqHz = DSBFREQUENCY_MAX;
    }

    pBufferToUse->SetFrequency(freqHz);
    pBufferToUse->SetVolume(volume);
    pBufferToUse->SetPan(pan);

    if (playFlags & DSBPLAY_LOOPING)
    {
        playFlags = DSBPLAY_LOOPING;
    }


    pBufferToUse->Play(0, 0, playFlags);

    return 0;
}

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

    gAudioDeviceSpec.callback = SDLSoundSystem::AudioCallBackStatic;
    gAudioDeviceSpec.format = AUDIO_S16;
    gAudioDeviceSpec.channels = 2;
    gAudioDeviceSpec.freq = 44100;
    gAudioDeviceSpec.samples = 256;
    gAudioDeviceSpec.userdata = this;

    if (SDL_OpenAudio(&gAudioDeviceSpec, NULL) < 0)
    {
        LOG_ERROR("Couldn't open SDL audio: " << SDL_GetError());
        return;
    }

    LOG_INFO("-----------------------------");
    LOG_INFO("Audio Device opened, got specs:");
    LOG_INFO(
        "Channels: " << gAudioDeviceSpec.channels << " " <<
        "nFormat: " << gAudioDeviceSpec.format << " " <<
        "nFreq: " << gAudioDeviceSpec.freq << " " <<
        "nPadding: " << gAudioDeviceSpec.padding << " " <<
        "nSamples: " << gAudioDeviceSpec.samples << " " <<
        "nSize: " << gAudioDeviceSpec.size << " " <<
        "nSilence: " << gAudioDeviceSpec.silence);
    LOG_INFO("-----------------------------");

    Reverb_Init(gAudioDeviceSpec.freq);

    SDL_PauseAudio(0);

    SND_InitVolumeTable_4EEF60();

    if (sLoadedSoundsCount_BBC394)
    {
        for (int i = 0; i < 256; i++)
        {
            if (sSoundSamples_BBBF38[i])
            {
                SND_Renew_4EEDD0(sSoundSamples_BBBF38[i]);
                //SND_LoadSamples_4EF1C0
                    SND_Reload_4EF1C0(sSoundSamples_BBBF38[i], 0, sSoundSamples_BBBF38[i]->field_8_pSoundBuffer, sSoundSamples_BBBF38[i]->field_C_buffer_size_bytes / (unsigned __int8)sSoundSamples_BBBF38[i]->field_1D_blockAlign);
                if ((i + 1) == sLoadedSoundsCount_BBC394)
                    break;
            }
        }
    }

    sLastNotePlayTime_BBC33C = SYS_GetTicks();
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
    }

    // Mix our no reverb buffer
    SDL_MixAudioFormat(reinterpret_cast<Uint8 *>(pSampleBuffer), reinterpret_cast<Uint8 *>(mNoReverbBuffer), AUDIO_S16, sampleBufferCount * sizeof(StereoSample_S16), kMixVolume);

    //delete[] tempBuffer;
    //delete[] noReverbBuffer;
}


void SDLSoundSystem::RenderSoundBuffer(SDLSoundBuffer& entry, StereoSample_S16* pSampleBuffer, int sampleBufferCount)
{
    bool reverbPass = false;

    SDLSoundBuffer* pVoice = &entry;

    if (pVoice == nullptr || !pVoice->pBuffer || pVoice->pBuffer->empty())
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

    bool loopActive = true;
    for (int i = 0; i < sampleBufferCount && pVoice->pBuffer && loopActive; i++)
    {
        if (pVoice->pBuffer->empty() || pVoice->mState.eStatus != AE_SDL_Voice_Status::Playing || pVoice->mState.iSampleCount == 0)
        {
            loopActive = false;
            continue;
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

            StereoSample_S16 pSample = reinterpret_cast<StereoSample_S16*>(pVoiceBufferPtr)[static_cast<int>(pVoice->mState.fPlaybackPosition)];
            mTempSoundBuffer[i].left = static_cast<signed short>((pSample.left * pVoice->mState.iVolume) / 127);
            mTempSoundBuffer[i].right = static_cast<signed short>((pSample.right * pVoice->mState.iVolume) / 127);

            pVoice->mState.fPlaybackPosition += pVoice->mState.fFrequency;
        }
        else
        {
            reverbPass = true;

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

        if (pVoice->mState.fPlaybackPosition >= pVoice->mState.iSampleCount / pVoice->mState.iChannels)
        {
            if (pVoice->mState.bLoop)
            {
                // Restart playback for loop.
                pVoice->mState.fPlaybackPosition = 0;
            }
            else
            {
                pVoice->mState.fPlaybackPosition = 0;
                pVoice->mState.eStatus = AE_SDL_Voice_Status::Stopped;
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

void SDLSoundSystem::AudioCallBackStatic(void* userdata, Uint8 *stream, int len)
{
    static_cast<SDLSoundSystem*>(userdata)->AudioCallBack(stream, len);
}


// TODO: Consolidate this
EXPORT char * CC SND_HR_Err_To_String_4EEC70(long)
{
    return "unknown error";
}

#endif