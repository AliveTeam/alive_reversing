#include "stdafx.h"
#include "SDLSoundBuffer.hpp"

#define MAX_VOICE_COUNT 1024

SDLSoundBuffer* sAE_ActiveVoices[MAX_VOICE_COUNT] = {};

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

    LOG_ERROR("Failed to allocate voice! No space left");
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

    LOG_ERROR("Could not find voice to remove");
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
    mState.eStatus = SDLSoundBufferStatus::Stopped;

    AddVoiceToActiveList(this);
}

SDLSoundBuffer::SDLSoundBuffer(const DSBUFFERDESC& bufferDesc, int soundSysFreq)
    : mSoundSysFreq(soundSysFreq)
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
    mState.eStatus = SDLSoundBufferStatus::Stopped;


    mState.iSampleCount = bufferDesc.dwBufferBytes / 2;
    mBuffer = std::make_shared<std::vector<BYTE>>(bufferDesc.dwBufferBytes);
    mState.iBlockAlign = bufferDesc.lpwfxFormat->nBlockAlign;
    mState.iChannels = bufferDesc.lpwfxFormat->nChannels;

    AddVoiceToActiveList(this);
}

HRESULT SDLSoundBuffer::SetVolume(int volume)
{
    mState.iVolumeTarget = volume;

    if (!mState.bVolDirty)
    {
        mState.iVolume = mState.iVolumeTarget;
    }

    mState.bVolDirty = true;

    return S_OK;
}

HRESULT SDLSoundBuffer::Play(int, int, int flags)
{
    mState.fPlaybackPosition = 0;
    mState.eStatus = SDLSoundBufferStatus::Playing;

    if (flags & DSBPLAY_LOOPING)
    {
        mState.bLoop = true;
    }

    return S_OK;
}

HRESULT SDLSoundBuffer::Stop()
{
    mState.eStatus = SDLSoundBufferStatus::Stopped;
    return S_OK;
}

HRESULT SDLSoundBuffer::SetFrequency(int frequency)
{
    mState.fFrequency = frequency / static_cast<float>(mSoundSysFreq);
    return S_OK;
}

HRESULT SDLSoundBuffer::SetCurrentPosition(int position) // This offset is apparently in bytes
{
    mState.fPlaybackPosition = static_cast<float>(position / mState.iBlockAlign);
    return S_OK;
}

HRESULT SDLSoundBuffer::GetCurrentPosition(DWORD * readPos, DWORD * writePos)
{
    *readPos = static_cast<DWORD>(mState.fPlaybackPosition * mState.iBlockAlign);
    *writePos = 0;

    return S_OK;
}

HRESULT SDLSoundBuffer::GetFrequency(DWORD* freq)
{
    *freq = static_cast<DWORD>(mState.fFrequency * mSoundSysFreq);
    return S_OK;
}

HRESULT SDLSoundBuffer::SetPan(signed int pan)
{
    mState.iPan = pan;
    return S_OK;
}

void SDLSoundBuffer::Release()
{
    mState.bLoop = false; // because the SDL copy paste of SND_Free_4EFA30 was doing this
    mState.bIsReleased = true;
}

HRESULT SDLSoundBuffer::GetStatus(DWORD * r)
{
    if (mState.eStatus == SDLSoundBufferStatus::Playing)
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
    return S_OK;
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
    return mBuffer.get();
}

void SDLSoundBuffer::Duplicate(SDLSoundBuffer** dupePtr)
{
    *dupePtr = new SDLSoundBuffer(*this);
    AddVoiceToActiveList(*dupePtr);
}
