#pragma once

#include "SDL.h"

#include <stdio.h>
#include <iostream>
#include <vector>
#include <mutex>
#include <algorithm>
#include "../Soundbank.hpp"
#include "Voice.hpp"
#include "biquad.hpp"

namespace psx {

void AliveInitAudio();
void AliveAudioSDLCallback(void* udata, Uint8* stream, int len);

const int AliveAudioSampleRate = 44100;

class AliveAudio
{
public:
    static Soundbank* m_CurrentSoundbank;
    static std::mutex voiceListMutex;
    static std::vector<Voice*> m_Voices;
    static bool AliveAudio::voiceListLocked;

    static void PlayOneShot(s32 playId, s32 program, s32 note, s32 volume, float pitch, s32 pitch_min, s32 pitch_max);
    static void PlayOneShot(s32 playId, s32 program, s32 note, s32 volLeft, s32 volRight, float pitch, s32 pitch_min, s32 pitch_max);

    static void NoteOn(int program, int note, char velocity, float pitch = 0, int trackID = 0, float trackDelay = 0);
    static void NoteOn(int program, int note, char velocity, int trackID = 0, float trackDelay = 0);

    static void NoteOff(int program, int note, int trackID = 0);
    static void NoteOffDelay(int program, int note, int trackID = 0, float trackDelay = 0);
    static void SetVolume(int trackID, s32 volLeft, s32 volRight);

    static void DebugPlayFirstToneSample(int program, int tone);

    static void LockNotes();
    static void UnlockNotes();

    static void CleanVoices();
    static void ClearAllVoices(bool forceKill = true);
    static void ClearAllTrackVoices(int trackID, bool forceKill = false);

    static long long currentSampleIndex;
    static biquad* AliveAudioEQBiQuad;

private:
    static void AliveAudioSetEQ(float cutoff);
    static void AliveEQEffect(float* stream, int len);
};

} // namespace psx