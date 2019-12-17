#include "stdafx.h"
#include "SDLSoundBuffer.hpp"

#define MAX_VOICE_COUNT 1024

// TODO: This probably shouldn't be a atomic as it could result in samples being leaked, still better than having torn pointer values I guess
std::atomic<SDLSoundBuffer*> sAE_ActiveVoices[MAX_VOICE_COUNT] = {};

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


SDLSoundBuffer::SDLSoundBuffer(const SDLSoundBuffer& rhs)
{
    *this = rhs;
    AddVoiceToActiveList(this);
}

SDLSoundBuffer& SDLSoundBuffer::operator=(const SDLSoundBuffer& rhs)
{
    if (this != &rhs)
    {
        mSoundSysFreq = rhs.mSoundSysFreq;
        mState = rhs.mState;
        mBuffer = rhs.mBuffer;
    }
    return *this;
}

HRESULT SDLSoundBuffer::SetVolume(int volume)
{
    std::lock_guard<std::mutex> lock(mLock);

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
    std::lock_guard<std::mutex> lock(mLock);

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
    std::lock_guard<std::mutex> lock(mLock);

    mState.eStatus = SDLSoundBufferStatus::Stopped;
    return S_OK;
}

HRESULT SDLSoundBuffer::SetFrequency(int frequency)
{
    std::lock_guard<std::mutex> lock(mLock);

    mState.fFrequency = frequency / static_cast<float>(mSoundSysFreq);
    return S_OK;
}

HRESULT SDLSoundBuffer::SetCurrentPosition(int position) // This offset is apparently in bytes
{
    std::lock_guard<std::mutex> lock(mLock);

    mState.fPlaybackPosition = static_cast<float>(position / mState.iBlockAlign);
    return S_OK;
}

HRESULT SDLSoundBuffer::GetCurrentPosition(DWORD * readPos, DWORD * writePos)
{
    std::lock_guard<std::mutex> lock(mLock);

    *readPos = static_cast<DWORD>(mState.fPlaybackPosition * mState.iBlockAlign);
    *writePos = 0;

    return S_OK;
}

HRESULT SDLSoundBuffer::GetFrequency(DWORD* freq)
{
    std::lock_guard<std::mutex> lock(mLock);

    *freq = static_cast<DWORD>(mState.fFrequency * mSoundSysFreq);
    return S_OK;
}

HRESULT SDLSoundBuffer::SetPan(signed int pan)
{
    std::lock_guard<std::mutex> lock(mLock);

    mState.iPan = pan;
    return S_OK;
}

void SDLSoundBuffer::Release()
{
    std::lock_guard<std::mutex> lock(mLock);

    mState.bLoop = false; // because the SDL copy paste of SND_Free_4EFA30 was doing this
    mState.bIsReleased = true;
}

HRESULT SDLSoundBuffer::GetStatus(DWORD * r)
{
    std::lock_guard<std::mutex> lock(mLock);

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
    {
        std::lock_guard<std::mutex> lock(mLock);

        RemoveVoiceFromActiveList(this);
    }
    delete this;
}

std::vector<BYTE>* SDLSoundBuffer::GetBuffer()
{
    return mBuffer.get();
}

void SDLSoundBuffer::Duplicate(SDLSoundBuffer** dupePtr)
{
    std::lock_guard<std::mutex> lock(mLock);
    *dupePtr = new SDLSoundBuffer(*this);
}
