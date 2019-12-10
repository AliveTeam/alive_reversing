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

static int gSoundBufferSamples = 256;
static int gCurrentSoundBufferSize = 0;
const int gMixVolume = 127;

SDLSoundBuffer* sAE_ActiveVoices[MAX_VOICE_COUNT] = {};

static SDL_AudioSpec gAudioDeviceSpec = {};
static AudioFilterMode gAudioFilterMode = AudioFilterMode::Linear;
static StereoSample_S16 * pTempSoundBuffer;
static StereoSample_S16 * pNoReverbBuffer;

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

void AE_SDL_Audio_Generate(StereoSample_S16 * pSampleBuffer, int sampleBufferCount)
{
    // Check if our buffer size changes, and if its buffer, then resize the array
    if (sampleBufferCount > gCurrentSoundBufferSize)
    {
        if (pTempSoundBuffer != nullptr)
        {
            delete[] pTempSoundBuffer;
            pTempSoundBuffer = nullptr;
        }
        if (pNoReverbBuffer != nullptr)
        {
            delete[] pNoReverbBuffer;
            pNoReverbBuffer = nullptr;
        }
    }

    if (pTempSoundBuffer == nullptr)
    {
        pTempSoundBuffer = new StereoSample_S16[sampleBufferCount];
    }
    if (pNoReverbBuffer == nullptr)
    {
        pNoReverbBuffer = new StereoSample_S16[sampleBufferCount];
    }

    memset(pNoReverbBuffer, 0, sampleBufferCount * sizeof(StereoSample_S16));

    bool reverbPass = false;

    for (int vi = 0; vi < MAX_VOICE_COUNT; vi++)
    {
        SDLSoundBuffer * pVoice = sAE_ActiveVoices[vi];

        if (pVoice == nullptr || !pVoice->pBuffer)
        {
            continue;
        }

        if (pVoice->mState.bIsReleased)
        {
            pVoice->Destroy();
            continue;
        }

        // Clear Temp Sample Buffer
        memset(pTempSoundBuffer, 0, sampleBufferCount * sizeof(StereoSample_S16));

        Sint16* pVoiceBufferPtr = reinterpret_cast<Sint16*>(pVoice->GetBuffer()->data());

        bool loopActive = true;
        for (int i = 0; i < sampleBufferCount && pVoice->pBuffer && loopActive; i++)
        {
            if (!pVoice->pBuffer || pVoice->mState.eStatus != AE_SDL_Voice_Status::Playing || pVoice->mState.iSampleCount == 0)
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
                pTempSoundBuffer[i].left = static_cast<signed short>((pSample.left * pVoice->mState.iVolume) / 127);
                pTempSoundBuffer[i].right = static_cast<signed short>((pSample.right * pVoice->mState.iVolume) / 127);

                pVoice->mState.fPlaybackPosition += pVoice->mState.fFrequency;
            }
            else
            {
                reverbPass = true;

                int s = 0;

                switch (gAudioFilterMode)
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

                pTempSoundBuffer[i].left = static_cast<signed short>((((s * leftPan) / 10000) * pVoice->mState.iVolume) / 127);
                pTempSoundBuffer[i].right = static_cast<signed short>((((s * rightPan) / 10000) * pVoice->mState.iVolume) / 127);

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
            SDL_MixAudioFormat(reinterpret_cast<Uint8 *>(pSampleBuffer), reinterpret_cast<Uint8 *>(pTempSoundBuffer), AUDIO_S16, sampleBufferCount * sizeof(StereoSample_S16), 45);
        }
        else
        {
            SDL_MixAudioFormat(reinterpret_cast<Uint8 *>(pNoReverbBuffer), reinterpret_cast<Uint8 *>(pTempSoundBuffer), AUDIO_S16, sampleBufferCount * sizeof(StereoSample_S16), 45);
        }
    }

    // Do Reverb Pass

    if (gReverbEnabled)
    {
        Reverb_Mix(pSampleBuffer, AUDIO_S16, sampleBufferCount * sizeof(StereoSample_S16), gMixVolume);
    }

    // Mix our no reverb buffer
    SDL_MixAudioFormat(reinterpret_cast<Uint8 *>(pSampleBuffer), reinterpret_cast<Uint8 *>(pNoReverbBuffer), AUDIO_S16, sampleBufferCount * sizeof(StereoSample_S16), gMixVolume);

    //delete[] tempBuffer;
    //delete[] noReverbBuffer;
}

void AE_SDL_Audio_Callback(void * /*userdata*/, Uint8 *stream, int len)
{
    memset(stream, 0, len);
    AE_SDL_Audio_Generate(reinterpret_cast<StereoSample_S16 *>(stream), len / sizeof(StereoSample_S16));

    // Frag code    
    /*const int lenStereo = len / sizeof(StereoSample_S16);
    const int fragSize = 128;
    const int fragments = lenStereo / fragSize;

    for (int i = 0; i < fragments; i++)
    {
        AE_SDL_Audio_Generate(reinterpret_cast<StereoSample_S16 *>(stream + (i * fragSize * sizeof(StereoSample_S16))), fragSize);
    }*/
}

void AE_SDL_AudioShutdown()
{
    // Todo: clean up audio + reverb buffers
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

int SDLSoundBuffer::Duplicate(SDLSoundBuffer ** dupePtr)
{
    SDLSoundBuffer * dupe = new SDLSoundBuffer();
    memcpy(&dupe->mState, &this->mState, sizeof(AE_SDL_Voice_State));
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
    pSnd->field_4_pDSoundBuffer->mState.bLoop = false;

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

signed int CC SND_CreateDS_SDL(unsigned int /*sampleRate*/, int /*bitsPerSample*/, int /*isStereo*/)
{
    if (!SDL_Init(SDL_INIT_AUDIO))
    {
        for (int i = 0; i < SDL_GetNumAudioDrivers(); i++)
        {
            printf("SDL Audio Driver %i: %s\n", i, SDL_GetAudioDriver(i));
        }

        gAudioDeviceSpec.callback = AE_SDL_Audio_Callback;
        gAudioDeviceSpec.format = AUDIO_S16;
        gAudioDeviceSpec.channels = 2;
        gAudioDeviceSpec.freq = 44100;
        gAudioDeviceSpec.samples = static_cast<Uint16>(gSoundBufferSamples);
        gAudioDeviceSpec.userdata = NULL;

        if (SDL_OpenAudio(&gAudioDeviceSpec, NULL) < 0) {
            fprintf(stderr, "Couldn't open SDL audio: %s\n", SDL_GetError());
        }
        else
        {
            printf("-----------------------------\n");
            printf("Audio Device opened, got specs:\n");
            printf("Channels: %i\nFormat: %X\nFreq: %i\nPadding: %i\nSamples: %i\nSize: %i\nSilence: %i\n",
                gAudioDeviceSpec.channels, gAudioDeviceSpec.format, gAudioDeviceSpec.freq, gAudioDeviceSpec.padding, gAudioDeviceSpec.samples, gAudioDeviceSpec.size, gAudioDeviceSpec.silence);
            printf("-----------------------------\n");

            gSoundBufferSamples = gAudioDeviceSpec.samples;

            Reverb_Init(gAudioDeviceSpec.freq);

            memset(&sAE_ActiveVoices, 0, sizeof(sAE_ActiveVoices));

            SDL_PauseAudio(0);

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
            sLastNotePlayTime_BBC33C = SYS_GetTicks();

            return 0;
        }
    }

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

EXPORT int CC SND_PlayEx_4EF740(const SoundEntry* pSnd, int panLeft, int panRight, float freq, MIDI_Struct1* pMidiStru, int playFlags, int priority)
{
    if (!pSnd)
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\SND.C", 845, -1, "SND_PlayEx: NULL SAMPLE !!!");
        return -1;
    }

    SDLSoundBuffer * pDSoundBuffer = pSnd->field_4_pDSoundBuffer;

    if (!pDSoundBuffer)
    {
        return -1;
    }

    sLastNotePlayTime_BBC33C = SYS_GetTicks();

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
    pDSoundBuffer->SetVolume(sVolumeTable_BBBD38[panRightConverted]);

    const int panConverted = (DSBPAN_RIGHT * (panLeft2 - panRight)) / 127; // From PSX pan range to DSound pan range
    pDSoundBuffer->SetPan(-panConverted); // Fix Inverted Stereo

    if (playFlags & DSBPLAY_LOOPING)
    {
        playFlags = DSBPLAY_LOOPING;
    }


    pDSoundBuffer->Play(0, 0, playFlags);

    return 0;
}

#endif