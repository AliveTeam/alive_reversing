#include "stdafx.h"
#include "SoundSDL.hpp"
#include "SDLSoundBuffer.hpp"
#include "SDLSoundSystem.hpp"

#if USE_SDL2_SOUND

#include <cinder/audio/dsp/RingBuffer.h>

bool gReverbEnabled = false;
bool gAudioStereo = true;

void SND_InitVolumeTable_SDL()
{
    for (int i = 0; i < 127; i++)
    {
        sVolumeTable_BBBD38[i] = i;
    }
    sVolumeTable_BBBD38[0] = 0;
}

int SND_Clear_SDL(SoundEntry* pSoundEntry, unsigned int sampleOffset, unsigned int size)
{
    const DWORD alignedOffset = sampleOffset * pSoundEntry->field_1D_blockAlign;
    const DWORD alignedSize = size * pSoundEntry->field_1D_blockAlign;

    // TODO: Should only clear from offset to size ??
    memset(pSoundEntry->field_4_pDSoundBuffer->GetBuffer()->data(), 0, pSoundEntry->field_14_buffer_size_bytes);

    return 0;
}

signed int SND_LoadSamples_SDL(const SoundEntry* pSnd, DWORD sampleOffset, unsigned char* pSoundBuffer, unsigned int sampleCount)
{
    const int offsetBytes = sampleOffset * pSnd->field_1D_blockAlign;
    const unsigned int bufferSizeBytes = sampleCount * pSnd->field_1D_blockAlign;
    memcpy(reinterpret_cast<Uint8*>(pSnd->field_4_pDSoundBuffer->GetBuffer()->data()) + offsetBytes, pSoundBuffer, bufferSizeBytes);
    return 0;
}

signed int SND_CreateDS_SDL(unsigned int sampleRate, int bitsPerSample, int isStereo)
{
    sDSound_BBC344 = new SDLSoundSystem();
    sDSound_BBC344->Init(sampleRate, bitsPerSample, isStereo);
    return 0;
}

const char* SND_HR_Err_To_String_SDL(long)
{
    return "unknown error";
}

#endif
