#pragma once

#include <chrono>
#include "AliveAudio.hpp"

namespace psx {

Soundbank* AliveAudio::m_CurrentSoundbank = nullptr;
std::mutex AliveAudio::voiceListMutex;
std::vector<Voice*> AliveAudio::m_Voices;
long long AliveAudio::currentSampleIndex = 20;
bool AliveAudio::voiceListLocked = false;

void AliveInitAudio()
{
    SDL_Init(SDL_INIT_AUDIO);

    SDL_AudioSpec waveSpec;
    waveSpec.callback = AliveAudioSDLCallback;
    waveSpec.userdata = nullptr;
    waveSpec.channels = 2;
    waveSpec.freq = AliveAudioSampleRate;
    waveSpec.samples = 512;
    waveSpec.format = AUDIO_F32;

    /* Open the audio device */
    if (SDL_OpenAudio(&waveSpec, NULL) < 0)
    {
        fprintf(stderr, "Failed to initialize audio: %s\n", SDL_GetError());
        exit(-1);
    }

    SDL_PauseAudio(0);
}

void AliveAudio::PlayOneShot(int programId, int note, s32 volume, float pitch)
{
    for (auto program : m_CurrentSoundbank->m_Programs)
    {
        if (program->prog_id != programId)
        {
            continue;
        }

        for (auto tone : program->m_Tones)
        {
            if (note >= tone->Min && note <= tone->Max)
            {
                Voice* voice = new Voice();
                voice->i_Note = note;
                voice->f_Velocity = float(volume == 0 ? 127 : volume) / 127;
                voice->m_Tone = tone;
                voice->f_Pitch = pitch;
                m_Voices.push_back(voice);
            }
        }
    }
}

void AliveAudio::PlayOneShot(std::string soundID)
{
    soundID;
}

void AliveAudio::NoteOn(int programId, int note, char velocity, float pitch , int trackID , float trackDelay )
{
    for (auto program : m_CurrentSoundbank->m_Programs)
    {
        if (program->prog_id != programId)
        {
            continue;
        }
        for (auto tone : program->m_Tones)
        {
            if (note >= tone->Min && note <= tone->Max)
            {
                Voice* voice = new Voice();
                voice->i_Note = note;
                voice->m_Tone = tone;
                voice->i_Program = programId;
                voice->f_Velocity = velocity == 0 ? 127 : velocity / 127.0f;
                voice->i_TrackID = trackID;
                voice->f_Pitch = pitch;
                voice->f_TrackDelay = trackDelay;
                m_Voices.push_back(voice);
            }
        }
    }
}
void AliveAudio::NoteOn(int program, int note, char velocity, int trackID , float trackDelay)
{
    NoteOn(program, note, velocity, 0, trackID, trackDelay);
} 

void AliveAudio::NoteOff(int program, int note, int trackID )
{
    for (auto voice : m_Voices)
    {
        if (voice->i_Note == note && voice->i_Program == program && voice->i_TrackID == trackID)
        {
            voice->b_NoteOn = false;
        }
    }
}
void AliveAudio::NoteOffDelay(int program, int note, int trackID , float trackDelay)
{
    for (auto voice : m_Voices)
    {
        if (voice->i_Note == note && voice->i_Program == program && voice->i_TrackID == trackID && voice->f_TrackDelay < trackDelay && voice->f_NoteOffDelay <= 0)
        {
            voice->m_UsesNoteOffDelay = true;
            voice->f_NoteOffDelay = trackDelay;
        }
    }
} 
void AliveAudio::DebugPlayFirstToneSample(int program, int tone)
{
    program;
    tone;
} 
void AliveAudio::LockNotes()
{
    voiceListMutex.lock();
}
void AliveAudio::UnlockNotes()
{
    voiceListMutex.unlock();
}

void AliveAudio::ClearAllVoices(bool forceKill)
{
    std::vector<Voice*> deadVoices;

    for (auto voice : AliveAudio::m_Voices)
    {
        if (forceKill)
        {
            deadVoices.push_back(voice);
        }
        else
        {
            voice->b_NoteOn = false;        // Send a note off to all of the notes though.
            if (voice->f_SampleOffset == 0) // Let the voices that are CURRENTLY playing play.
            {
                deadVoices.push_back(voice);
            }
        }
    }

    for (auto obj : deadVoices)
    {
        delete obj;

        AliveAudio::m_Voices.erase(std::remove(AliveAudio::m_Voices.begin(), AliveAudio::m_Voices.end(), obj), AliveAudio::m_Voices.end());
    }
}

void AliveAudio::ClearAllTrackVoices(int trackID, bool forceKill)
{
    std::vector<Voice*> deadVoices;

    for (auto voice : AliveAudio::m_Voices)
    {
        if (forceKill)
        {
            if (voice->i_TrackID == trackID) // Kill the voices no matter what. Cuts of any sounds = Ugly sound
            {
                deadVoices.push_back(voice);
            }
        }
        else
        {
            voice->b_NoteOn = false;                                       // Send a note off to all of the notes though.
            if (voice->i_TrackID == trackID && voice->f_SampleOffset == 0) // Let the voices that are CURRENTLY playing play.
            {
                deadVoices.push_back(voice);
            }
        }
    }

    for (auto obj : deadVoices)
    {
        delete obj;

        AliveAudio::m_Voices.erase(std::remove(AliveAudio::m_Voices.begin(), AliveAudio::m_Voices.end(), obj), AliveAudio::m_Voices.end());
    }
}

void CleanVoices()
{
    std::vector<Voice*> deadVoices;

    for (auto voice : AliveAudio::m_Voices)
    {
        if (voice->b_Dead)
        {
            deadVoices.push_back(voice);
        }
    }

    for (auto obj : deadVoices)
    {
        delete obj;

        AliveAudio::m_Voices.erase(std::remove(AliveAudio::m_Voices.begin(), AliveAudio::m_Voices.end(), obj), AliveAudio::m_Voices.end());
    }
}

void AliveRenderAudio(float* AudioStream, int StreamLength)
{
    static float tick = 0;
    static int note = 0;

    int voiceCount = AliveAudio::m_Voices.size();
    Voice** rawPointer = AliveAudio::m_Voices.data(); // Real nice speed boost here.

    for (int i = 0; i < StreamLength; i += 2)
    {
        for (int v = 0; v < voiceCount; v++)
        {
            Voice* voice = rawPointer[v]; // Raw pointer skips all that vector bottleneck crap

            voice->f_TrackDelay--;

            if (voice->m_UsesNoteOffDelay)
            {
                voice->f_NoteOffDelay--;
            }

            if (voice->m_UsesNoteOffDelay && voice->f_NoteOffDelay <= 0 && voice->b_NoteOn == true)
            {
                voice->b_NoteOn = false;
            }

            if (voice->b_Dead || voice->f_TrackDelay > 0) {
                continue;
            }
            
            float centerPan = voice->m_Tone->f_Pan;
            float leftPan = 1.0f;
            float rightPan = 1.0f;

            if (centerPan > 0)
            {
                leftPan = 1.0f - abs(centerPan);
            }
            if (centerPan < 0)
            {
                rightPan = 1.0f - abs(centerPan);
            }

            float s = voice->GetSample();

            float leftSample = s * leftPan;
            float rightSample = s * rightPan;

            SDL_MixAudioFormat((Uint8*) (AudioStream + i), (const Uint8*) &leftSample, AUDIO_F32, sizeof(float), 37);      // Left Channel
            SDL_MixAudioFormat((Uint8*) (AudioStream + i + 1), (const Uint8*) &rightSample, AUDIO_F32, sizeof(float), 37); // Right Channel
        }

        AliveAudio::currentSampleIndex++;
    }

    CleanVoices();
}

void AliveAudioSDLCallback(void* udata, Uint8* stream, int len)
{
    udata;
    memset(stream, 0, len);

    AliveAudio::LockNotes();
    AliveRenderAudio((float*) stream, len / sizeof(float));
    AliveAudio::UnlockNotes();

    //if (AliveAudio::EQEnabled)
    //    AliveEQEffect((float*) stream, len / sizeof(float));
}

} // namespace psx