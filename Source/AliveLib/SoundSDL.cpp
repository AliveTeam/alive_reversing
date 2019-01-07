#include "stdafx.h"
#include "SoundSDL.hpp"

#if USE_SDL2_SOUND

#include "Function.hpp"
#include "Error.hpp"
#include "Midi.hpp"
#include <mutex>
#include <timeapi.h>

#include "Reverb.hpp"

static std::vector<AE_SDL_Voice*> sAE_ActiveVoices;
static std::vector<AE_SDL_Voice*> sAE_VoiceBuffer;

static std::mutex sVoiceBufferMutex;

static SDL_AudioSpec gAudioDeviceSpec;
static AudioFilterMode gAudioFilterMode = AudioFilterMode::Linear;

void AE_SDL_Audio_Callback(void * /*userdata*/, Uint8 *stream, int len)
{
    StereoSampleFloat * buffer = reinterpret_cast<StereoSampleFloat *>(stream);

    int bufferSamples = (len / sizeof(StereoSampleFloat));
    memset(stream, 0, len);

    if (!sAE_VoiceBuffer.empty())
    {
        std::unique_lock<std::mutex> sVoiceVectorLock(sVoiceBufferMutex);

        for (AE_SDL_Voice * pVoice : sAE_VoiceBuffer)
        {
            sAE_ActiveVoices.push_back(pVoice);
        }

        sAE_VoiceBuffer.clear();
    }

    // slow, store this somewhere permanantly.
    StereoSampleFloat * tempBuffer = new StereoSampleFloat[bufferSamples];

    for (AE_SDL_Voice * pVoice : sAE_ActiveVoices)
    {
        if (!pVoice->pBuffer || pVoice->State.eStatus != AE_SDL_Voice_Status::Playing)
            continue;

        // Clear Temp Sample Buffer
        memset(tempBuffer, 0, len);

        for (int i = 0; i < bufferSamples && pVoice->pBuffer; i += gAudioDeviceSpec.channels / 2)
        {
            if (!pVoice->pBuffer || pVoice->State.eStatus != AE_SDL_Voice_Status::Playing || pVoice->State.iSampleCount == 0)
                continue;

            if (pVoice->State.iChannels == 2)
            {
                // For Stereo buffers. The only time this is played is for FMV's.
                // Right now, unless the playback device is at 44100 hz, it sounds awful.
                // TODO: Resampling for stereo

                tempBuffer[i].left = ((reinterpret_cast<Sint16*>(pVoice->GetBuffer())[static_cast<int>(pVoice->State.fPlaybackPosition)]) / 65535.0f) * pVoice->State.fVolume;
                tempBuffer[i].right = ((reinterpret_cast<Sint16*>(pVoice->GetBuffer())[static_cast<int>(pVoice->State.fPlaybackPosition) + 1]) / 65535.0f) * pVoice->State.fVolume;

                pVoice->State.fPlaybackPosition += pVoice->State.fFrequency * 2;

                if (pVoice->State.fPlaybackPosition >= pVoice->State.iSampleCount)
                {
                    if (pVoice->State.bLoop)
                    {
                        // Restart playback for loop.
                        pVoice->State.fPlaybackPosition = 0;
                    }
                    else
                    {
                        pVoice->State.fPlaybackPosition = 0;
                        pVoice->State.eStatus = AE_SDL_Voice_Status::Stopped;
                    }
                }
            }
            else
            {
                float s = 0;

                switch (gAudioFilterMode)
                {
                case AudioFilterMode::NoFilter:
                    s = ((reinterpret_cast<Sint16*>(pVoice->GetBuffer())[static_cast<int>(pVoice->State.fPlaybackPosition)]) / 65535.0f) * pVoice->State.fVolume;
                    break;
                case AudioFilterMode::Linear:
                    const float s1 = reinterpret_cast<Sint16*>(pVoice->GetBuffer())[static_cast<int>(pVoice->State.fPlaybackPosition)] / 65535.0f;
                    const float s2 = reinterpret_cast<Sint16*>(pVoice->GetBuffer())[(static_cast<int>(pVoice->State.fPlaybackPosition) + 1) % pVoice->State.iSampleCount] / 65535.0f;

                    s = (s1 + ((s2 - s1) * (pVoice->State.fPlaybackPosition - floorf(pVoice->State.fPlaybackPosition)))) * pVoice->State.fVolume;
                    break;
                }

                float leftPan = 1.0f;
                float rightPan = 1.0f;

                if (pVoice->State.fPan < 0)
                {
                    rightPan = 1.0f - fabs(pVoice->State.fPan);
                }
                else if (pVoice->State.fPan > 0)
                {
                    leftPan = 1.0f - fabs(pVoice->State.fPan);
                }

                tempBuffer[i].left = s * leftPan;
                tempBuffer[i].right = s * rightPan;

                pVoice->State.fPlaybackPosition += pVoice->State.fFrequency;

                if (pVoice->State.fPlaybackPosition >= pVoice->State.iSampleCount)
                {
                    if (pVoice->State.bLoop)
                    {
                        // Restart playback for loop.
                        pVoice->State.fPlaybackPosition = 0;
                    }
                    else
                    {
                        pVoice->State.fPlaybackPosition = 0;
                        pVoice->State.eStatus = AE_SDL_Voice_Status::Stopped;
                    }
                }
            }


        }

        SDL_MixAudioFormat(reinterpret_cast<Uint8 *>(buffer), reinterpret_cast<Uint8 *>(tempBuffer), gAudioDeviceSpec.format, len, SDL_MIX_MAXVOLUME);
    }


    // Destroy and remove any released voices
    std::vector<AE_SDL_Voice *> voiceGarbage;
    for (unsigned int v = 0; v < sAE_ActiveVoices.size(); v++)
    {
        if (sAE_ActiveVoices[v]->State.bIsReleased)
        {
            voiceGarbage.push_back(sAE_ActiveVoices[v]);
        }
    }

    for (auto voice : voiceGarbage)
    {
        if (voice != nullptr)
        {
            voice->Destroy();
        }
    }

    delete[] tempBuffer;

    // Do Reverb Pass

    if (gReverbEnabled)
    {
        Reverb_Mix(buffer, gAudioDeviceSpec.format, len, SDL_MIX_MAXVOLUME);
    }

    // printf("Voice Count: %i\n", sAE_ActiveVoices.size());
}

AE_SDL_Voice::AE_SDL_Voice()
{
    State.fVolume = 1.0f;
    State.fPan = 0;
    State.fFrequency = 1.0f;
    State.bIsReleased = false;
    State.bLoop = false;
    State.iChannels = 1;
    State.fPlaybackPosition = 0;
    State.eStatus = AE_SDL_Voice_Status::Stopped;

    std::unique_lock<std::mutex> sVoiceVectorLock(sVoiceBufferMutex);
    sAE_VoiceBuffer.push_back(this);
}

int AE_SDL_Voice::SetVolume(int volume)
{
    State.fVolume = volume / 127.0f;
    return 0;
}

int AE_SDL_Voice::SetFrequency(int frequency)
{
    State.fFrequency = frequency / static_cast<float>(gAudioDeviceSpec.freq);
    return 0;
}

int AE_SDL_Voice::SetCurrentPosition(int position) // This offset is apparently in bytes
{
    State.fPlaybackPosition = static_cast<float>(position / State.iBlockAlign * State.iChannels);
    return 0;
}

int AE_SDL_Voice::GetCurrentPosition(DWORD * readPos, DWORD * writePos)
{
    *readPos = static_cast<DWORD>(State.fPlaybackPosition * State.iBlockAlign / State.iChannels);
    *writePos = 0;

    return 0;
}

int AE_SDL_Voice::GetFrequency(DWORD * freq)
{
    *freq = static_cast<DWORD>(State.fFrequency * gAudioDeviceSpec.freq);
    return 0;
}

int AE_SDL_Voice::SetPan(int pan)
{
    State.fPan = pan / 10000.0f;
    return 0;
}

void AE_SDL_Voice::Release()
{
    State.bIsReleased = true;
}

int AE_SDL_Voice::GetStatus(DWORD * r)
{
    if (State.eStatus == AE_SDL_Voice_Status::Playing)
    {
        *r |= DSBSTATUS_PLAYING;
    }
    if (State.bLoop)
    {
        *r |= DSBSTATUS_LOOPING;
    }
    if (State.bIsReleased)
    {
        *r |= DSBSTATUS_TERMINATED;
    }
    return 0;
}

void AE_SDL_Voice::Destroy()
{
    // remove self from global list and
    // decrement shared mem ptr to audio buffer

    sAE_ActiveVoices.erase(std::remove(sAE_ActiveVoices.begin(), sAE_ActiveVoices.end(), this), sAE_ActiveVoices.end());
    delete this;
}

int AE_SDL_Voice::Play(int /*reserved*/, int /*priority*/, int flags)
{
    State.fPlaybackPosition = 0;
    State.eStatus = AE_SDL_Voice_Status::Playing;
    if (flags & DSBPLAY_LOOPING)
    {
        State.bLoop = true;
    }
    return 0;
}

int AE_SDL_Voice::Stop()
{
    State.eStatus = AE_SDL_Voice_Status::Stopped;
    return 0;
}

void * AE_SDL_Voice::GetBuffer()
{
    return *pBuffer.get();
}

int AE_SDL_Voice::Duplicate(AE_SDL_Voice ** dupePtr)
{
    AE_SDL_Voice * dupe = new AE_SDL_Voice();
    memcpy(&dupe->State, &this->State, sizeof(AE_SDL_Voice_State));
    dupe->pBuffer = this->pBuffer;
    *dupePtr = dupe;
    return 0;
}


// Exoddus Functions:

EXPORT void CC SND_SsQuit_4EFD50()
{
    for (int i = 0; i < 32; i++)
    {
        if (sSoundBuffers_BBBAB8[i].field_0_pDSoundBuffer)
        {
            sSoundBuffers_BBBAB8[i].field_0_pDSoundBuffer->Stop();
            sSoundBuffers_BBBAB8[i].field_0_pDSoundBuffer->Release();
            sSoundBuffers_BBBAB8[i].field_0_pDSoundBuffer = nullptr;
        }
    }

    for (int i = 0; i < sLoadedSoundsCount_BBC394; i++)
    {
        SoundEntry* pEntry = sSoundSamples_BBBF38[i];
        if (pEntry->field_4_pDSoundBuffer)
        {
            pEntry->field_4_pDSoundBuffer->Stop();
            pEntry->field_4_pDSoundBuffer->Release();
            pEntry->field_4_pDSoundBuffer = nullptr;
            pEntry->field_10 = 0;
        }
    }

    return;
}

EXPORT signed int CC SND_Free_4EFA30(SoundEntry* pSnd)
{
    pSnd->field_4_pDSoundBuffer->State.bLoop = false;

    pSnd->field_10 = 0;

    if (pSnd->field_8_pSoundBuffer)
    {
        mem_free_4F4EA0(pSnd->field_8_pSoundBuffer);
        pSnd->field_8_pSoundBuffer = 0;
    }

    if (pSnd->field_4_pDSoundBuffer)
    {
        pSnd->field_4_pDSoundBuffer->Release();
        pSnd->field_4_pDSoundBuffer = nullptr;
    }

    sSoundSamples_BBBF38[pSnd->field_0_tableIdx] = nullptr;
    sLoadedSoundsCount_BBC394--;
    return 0;
}

EXPORT void CC SND_InitVolumeTable_4EEF60()
{
    for (int i = 0; i < 127; i++)
    {
        sVolumeTable_BBBD38[i] = i;
    }
    sVolumeTable_BBBD38[0] = 0;
}

EXPORT int CC SND_Reload_4EF350(SoundEntry* pSoundEntry, unsigned int sampleOffset, unsigned int size)
{
    const DWORD alignedOffset = sampleOffset * pSoundEntry->field_1D_blockAlign;
    const DWORD alignedSize = size * pSoundEntry->field_1D_blockAlign;

    memset(pSoundEntry->field_4_pDSoundBuffer->GetBuffer(), 0, pSoundEntry->field_14_buffer_size_bytes);

    return 0;
}

EXPORT signed int CC SND_Renew_4EEDD0(SoundEntry *pSnd)
{
    printf("SND_Renew_4EEDD0: %x\n", reinterpret_cast<int>(pSnd));
    return 0; // TODO
}

EXPORT signed int CC SND_Reload_4EF1C0(const SoundEntry* pSnd, DWORD sampleOffset, unsigned char* pSoundBuffer, unsigned int sampleCount)
{
    const int offsetBytes = sampleOffset * pSnd->field_1D_blockAlign;
    const unsigned int bufferSizeBytes = sampleCount * pSnd->field_1D_blockAlign;
    memcpy(reinterpret_cast<Uint8*>(pSnd->field_4_pDSoundBuffer->GetBuffer()) + offsetBytes, pSoundBuffer, bufferSizeBytes);
    return 0;
}


EXPORT signed int CC SND_New_4EEFF0(SoundEntry *pSnd, int sampleLength, int sampleRate, int bitsPerSample, int isStereo)
{
    if (sLoadedSoundsCount_BBC394 < 256)
    {
        pSnd->field_1D_blockAlign = static_cast<unsigned char>(bitsPerSample * ((isStereo != 0) + 1) / 8);
        int sampleByteSize = sampleLength * pSnd->field_1D_blockAlign;

        AE_SDL_Voice * pDSoundBuffer = new AE_SDL_Voice();
        pDSoundBuffer->SetFrequency(sampleRate);
        pDSoundBuffer->State.iSampleCount = sampleByteSize / 2;
        pDSoundBuffer->pBuffer = std::make_shared<void*>(new Sint8[sampleByteSize]);
        pDSoundBuffer->State.iBlockAlign = pSnd->field_1D_blockAlign;
        pDSoundBuffer->State.iChannels = (isStereo & 1) ? 2 : 1;
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

EXPORT signed int CC SND_CreateDS_4EEAA0(unsigned int /*sampleRate*/, int /*bitsPerSample*/, int /*isStereo*/)
{
    if (!SDL_Init(SDL_INIT_AUDIO))
    {
        for (int i = 0; i < SDL_GetNumAudioDrivers(); i++)
        {
            printf("SDL Audio Driver %i: %s\n", i, SDL_GetAudioDriver(i));
        }

        gAudioDeviceSpec.callback = AE_SDL_Audio_Callback;
        gAudioDeviceSpec.format = AUDIO_F32;
        gAudioDeviceSpec.channels = 2;
        gAudioDeviceSpec.freq = 44100;
        gAudioDeviceSpec.samples = 256;
        gAudioDeviceSpec.userdata = NULL;

        if (SDL_OpenAudio(&gAudioDeviceSpec, &gAudioDeviceSpec) < 0) {
            fprintf(stderr, "Couldn't open SDL audio: %s\n", SDL_GetError());
        }
        else
        {
            SDL_PauseAudio(0);

            Reverb_Init(gAudioDeviceSpec.freq);

            SND_InitVolumeTable_4EEF60();

            if (sLoadedSoundsCount_BBC394)
            {
                for (int i = 0; i < 256; i++)
                {
                    if (sSoundSamples_BBBF38[i])
                    {
                        SND_Renew_4EEDD0(sSoundSamples_BBBF38[i]);
                        SND_Reload_4EF1C0(sSoundSamples_BBBF38[i], 0, sSoundSamples_BBBF38[i]->field_8_pSoundBuffer, sSoundSamples_BBBF38[i]->field_C_buffer_size_bytes / (unsigned __int8)sSoundSamples_BBBF38[i]->field_1D_blockAlign);
                        if ((i + 1) == sLoadedSoundsCount_BBC394)
                            break;
                    }
                }
            }
            sLastNotePlayTime_BBC33C = timeGetTime();

            return 0;
        }
    }

    return 0;
}

EXPORT char CC SND_CreatePrimarySoundBuffer_4EEEC0(int /*sampleRate*/, int /*bitsPerSample*/, int /*isStereo*/)
{
    return 0;
}

EXPORT int CC SND_PlayEx_4EF740(const SoundEntry* pSnd, int panLeft, int panRight, float freq, MIDI_Struct1* pMidiStru, int playFlags, int priority)
{
    if (!pSnd)
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\SND.C", 845, -1, "SND_PlayEx: NULL SAMPLE !!!");
        return -1;
    }

    AE_SDL_Voice * pDSoundBuffer = pSnd->field_4_pDSoundBuffer;

    if (!pDSoundBuffer)
    {
        return -1;
    }

    sLastNotePlayTime_BBC33C = timeGetTime();

    int panLeft2 = panLeft;
    int panRight2 = panRight;
    if (panLeft > panRight)
    {
        panRight2 = panLeft;
    }

    int panRightConverted = 120 * panRight2 * 127UL >> 14;// >> 14 = 16384
    if (panRightConverted < 0)
    {
        panRightConverted = 0;
    }
    else if (panRightConverted > 127)
    {
        panRightConverted = 127;
    }

    if (pSnd->field_20_isStereo & 2)
    {
        pDSoundBuffer->SetFrequency(static_cast<DWORD>((pSnd->field_18_sampleRate * freq) + 0.5)); // This freq don't get clamped for some reason
        pDSoundBuffer->SetVolume(static_cast<int>(sVolumeTable_BBBD38[panRightConverted] / 10000.0f));
        pDSoundBuffer->SetCurrentPosition(0);
    }
    else
    {
        SoundBuffer* pSoundBuffer = SND_Get_Sound_Buffer_4EF970(pSnd->field_0_tableIdx, panRightConverted + priority);
        if (!pSoundBuffer)
        {
            return -1;
        }


        pDSoundBuffer->Duplicate(&pSoundBuffer->field_0_pDSoundBuffer);
        pSoundBuffer->field_0_pDSoundBuffer->SetCurrentPosition(0);
        pDSoundBuffer = pSoundBuffer->field_0_pDSoundBuffer;

        if (pMidiStru)
        {
            pMidiStru->field_0_sound_buffer_field_4 = pSoundBuffer->field_4;
        }
    }

    DWORD freqHz = static_cast<DWORD>((pSnd->field_18_sampleRate * freq) + 0.5);
    if (freqHz < DSBFREQUENCY_MIN)
    {
        freqHz = DSBFREQUENCY_MIN;
    }
    else if (freqHz >= DSBFREQUENCY_MAX)
    {
        freqHz = DSBFREQUENCY_MAX;
    }

    pDSoundBuffer->SetFrequency(freqHz);
    pDSoundBuffer->SetVolume(sVolumeTable_BBBD38[panRightConverted]);;

    const int panConverted = (DSBPAN_RIGHT * (panLeft2 - panRight)) / 127; // From PSX pan range to DSound pan range
    pDSoundBuffer->SetPan(panConverted);

    if (playFlags & DSBPLAY_LOOPING)
    {
        playFlags = DSBPLAY_LOOPING;
    }


    pDSoundBuffer->Play(0, 0, playFlags);

    return 0;
}

#endif