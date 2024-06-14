#pragma once

#include "Sequencer.hpp"
#include "SDL.h"
#include "Reverb.hpp"

namespace SPU {

// modified version of:
// https://github.com/stenzek/duckstation/blob/master/src/core/spu.cpp


//////////////////////////
// SPU state
std::mutex mutex;

const int VOICE_SIZE_LIMIT = 32; // Can be increased to 32, but 24 is what PSX has
std::array<Voice*, VOICE_SIZE_LIMIT> voices;

const int SEQUENCE_SIZE_LIMIT = 256;
std::vector<Sequence*> sequences;

const int PATCH_SIZE_LIMIT = 128;
std::array<Patch*, PATCH_SIZE_LIMIT> patches;

// Can swap these out for testing.
// We could make them dependencies and configurable in game
// 
//Reverb* reverb = new ReverbDuckstation();
Reverb* reverb = new ReverbIlleprih();
//InterpolationProvider* interpolation = new InterpolationProviderDuckstation();
InterpolationProvider* interpolation = new InterpolationProviderIlleprih();

//////////////////////////
// SPU Internal mangement methods
void SPUInit();
void SPUStopAll();
void SPUReset();

Voice* SPUObtainVoice(s8 priority, u8 note, u8 patchId);
void SPUReleaseVoice(Voice* v);

void SPUPatchAdd(Patch* patch);

void SPUSeqAdd(Sequence* seq);
bool SPUSeqPlay(s32 seqId, s32 repeats);
void SPUSeqStop(s32 seqId);
void SPUSeqSetVolume(s32 seqId, s16 voll, s16 volr);
bool SPUSeqIsDone(s32 seqId);

s32 SPUOneShotPlay(s32 patchId, u8 note, s16 voll, s16 volr, s32 pitch, s32 pitchMin, s32 pitchMax);
void SPUOneShotStop(s32 mask);

void SPUTick(void* udata, Uint8* stream, int len);
void SPUTickSequences();


//////////////////////////
// Public SPU methods
// Basically just a wrapper for mutex lock
void SPU::Init()
{
    mutex.lock();
    SPUInit();
    mutex.unlock();
}

void SPU::DeInit()
{
    mutex.lock();
    // This is called on AE quit and exit to desktop.
    // We could gracefully stop SDL audio rendering,
    // but AO never calls this on app exit, ignoring for now
    mutex.unlock();
}

void SPU::Reset()
{
    mutex.lock();
    SPUReset();
    mutex.unlock();
}

void SPU::StopAll()
{
    mutex.lock();
    SPUStopAll();
    mutex.unlock();
}

void PatchAdd(Patch* patch)
{
    if (patch->id >= PATCH_SIZE_LIMIT)
    {
        throw std::runtime_error("PatchId is above PATCH_SIZE_LIMIT");
    }
    mutex.lock();
    SPUPatchAdd(patch);
    mutex.unlock();
}

void SPU::SeqAdd(Sequence* seq)
{
    mutex.lock();
    SPUSeqAdd(seq);
    mutex.unlock();
}

bool SeqPlay(s32 seqId, s32 repeats, bool stopDuplicateSeq)
{
    mutex.lock();
    if (stopDuplicateSeq)
    {
        SPUSeqStop(seqId);
    }
    bool res = SPUSeqPlay(seqId, repeats);
    mutex.unlock();
    return res;
}

bool SeqPlay(s32 seqId, s32 repeats, s16 voll, s16 volr)
{
    mutex.lock();
    SPUSeqSetVolume(seqId, voll, volr);
    bool res = SPUSeqPlay(seqId, repeats);
    mutex.unlock();
    return res;
}

void SeqStop(s32 seqId)
{
    mutex.lock();
    SPUSeqStop(seqId);
    mutex.unlock();
}

void SeqSetVolume(s32 seqId, s16 voll, s16 volr)
{
    mutex.lock();
    SPUSeqSetVolume(seqId, voll, volr);
    mutex.unlock();
}

bool SeqIsDone(s32 seqId)
{
    mutex.lock();
    bool res = SPUSeqIsDone(seqId);
    mutex.unlock();
    return res;
}

s32 SPU::OneShotPlay(s32 patchId, u8 note, s16 voll, s16 volr, s32 pitch, s32 pitchMin, s32 pitchMax)
{
    mutex.lock();
    s32 res = SPUOneShotPlay(patchId, note, voll, volr, pitch, pitchMin, pitchMax);
    mutex.unlock();
    return res;
}

void SPU::OneShotStop(s32 mask)
{
    mutex.lock();
    SPUOneShotStop(mask);
    mutex.unlock();
}

void SDLCallback(void* udata, Uint8* stream, int len)
{
    mutex.lock();
    SPUTick(udata, stream, len);
    mutex.unlock();
}


//////////////////////////
// SEQUENCE
MIDIMessage* Sequence::createMIDIMessage()
{
    MIDIMessage* msg = new MIDIMessage();
    messages.push_back(msg);
    return msg;
}

void Sequence::Reset()
{
    play = false;
    actionPos = 0;
    trackStartTime = 0;
    voll = 127;
    volr = 127;
    repeats = 0;
    repeatLimit = 1;
}

MIDIMessage* Sequence::next(u64 now)
{
    if (messages.size() <= 0)
    {
        return NULL;
    }

    if (actionPos == 0)
    {
        // the track is just starting if we are at pos 0
        trackStartTime = now;
    }

    u64 runtimeUs = std::max((u64) 0, now - trackStartTime) * 1000; // x1000 to conver to microseconds
    float midiTickUs = tempoUs / ticksPerBeat;
    u64 trackTick = u64(runtimeUs / midiTickUs);

    MIDIMessage* msg = messages.at(actionPos);
    if (msg->tick <= trackTick)
    {
        actionPos++;
        if (actionPos == messages.size())
        {
            actionPos = 0;
            repeats++; // we've looped the sequence
        }
        return msg;
    }
    return NULL;
}


//////////////////////////
// Duckstation helpers
template <typename TValue>
constexpr u16 Truncate16(TValue value)
{
    return static_cast<u16>(static_cast<typename std::make_unsigned<decltype(value)>::type>(value));
}

template <typename TReturn, typename TValue>
constexpr TReturn SignExtend(TValue value)
{
    return static_cast<TReturn>(
        static_cast<typename std::make_signed<TReturn>::type>(static_cast<typename std::make_signed<TValue>::type>(value)));
}

template <typename TValue>
constexpr u32 SignExtend32(TValue value)
{
    return SignExtend<u32, TValue>(value);
}

template <typename TReturn, typename TValue>
constexpr TReturn ZeroExtend(TValue value)
{
    return static_cast<TReturn>(static_cast<typename std::make_unsigned<TReturn>::type>(
        static_cast<typename std::make_unsigned<TValue>::type>(value)));
}

template <typename TValue>
constexpr u32 ZeroExtend32(TValue value)
{
    return ZeroExtend<u32, TValue>(value);
}

static constexpr s32 Clamp16(s32 value)
{
    return (value < -0x8000) ? -0x8000 : (value > 0x7FFF) ? 0x7FFF
                                                          : value;
}

static constexpr s32 ApplyVolume(s32 sample, s16 volume)
{
    return (sample * s32(volume)) >> 15;
}


/////////////////////////
// SPU management
u64 timeSinceEpochMillisec()
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void SPUInit()
{
    // Open SDL
    SDL_Init(SDL_INIT_AUDIO);

    SDL_AudioSpec waveSpec;
    waveSpec.callback = SDLCallback;
    waveSpec.userdata = nullptr;
    waveSpec.channels = 2;
    waveSpec.freq = 44100;
    waveSpec.samples = 512;
    waveSpec.format = AUDIO_F32;

    /* Open the audio device */
    if (SDL_OpenAudio(&waveSpec, NULL) < 0)
    {
        fprintf(stderr, "Failed to initialize audio: %s\n", SDL_GetError());
        exit(-1);
    }

    SDL_PauseAudio(0);

    int id = 1;
    for (int i = 0; i < VOICE_SIZE_LIMIT; i++)
    {
        voices[i] = new Voice(interpolation);
        voices[i]->id = id;
        id = id << 1;
    }

    for (int i = 0; i < PATCH_SIZE_LIMIT; i++)
    {
        patches[i] = nullptr;
    }
}

void SPUReset()
{
    SPUStopAll();
    
    for (s16 i = 0; i < PATCH_SIZE_LIMIT; i++)
    {
        delete patches[i];
        patches[i] = nullptr;
    }
    
    for (Sequence* seq : sequences)
    {
        delete seq;
    }
    sequences.clear();
}

void SPUStopAll()
{
    for (Sequence* seq : sequences)
    {
        seq->play = false;
    }

    for (s16 i = 0; i < VOICE_SIZE_LIMIT; i++)
    {
        SPUReleaseVoice(voices[i]);
    }
}

Voice* SPUObtainVoice(s8 priority, u8 note, u8 patchId)
{
    note;
    patchId;

    // 1. Always try to use a free voice
    // 2. If no voice can be found - try using a repeated note that has the furthest offset
    // 3. Reap voices that have 'x' many playing? Shooting with a slig non-stop uses too many voices

    Voice* available = NULL;
    for (int i = 0; i < VOICE_SIZE_LIMIT; i++)
    {
        Voice* v = voices[i];
        if (v->complete)
        {
            SPUReleaseVoice(v);
        }

        if (!v->inUse)
        {
            v->inUse = true;
            return v;
        }

        // Overlapping voices are a problem when sligs shoot continously.
        // This is a simple implementation to overwrite some lower priority sound.
        if (v->sample->priority < priority)
        {
            available = v;
        }
    }

    if (!available)
    {
        return NULL;
    }

    // this is voice in use that we can reuse
    SPUReleaseVoice(available);
    available->inUse = true;
    return available;
}

void SPUReleaseVoice(Voice* v)
{
    v->Reset();
}

void SPUPatchAdd(Patch* patch)
{
    if (patches[patch->id])
    {
        //delete patches[patch->_id];
    }
    patches[patch->id] = patch;
}

void SPUSeqAdd(Sequence* seq)
{
    sequences.push_back(seq);
}

bool SPUSeqPlay(s32 seqId, s32 repeats)
{
    bool res = false;
    for (Sequence* seq : sequences)
    {
        if (seq->id == seqId)
        {
            seq->repeatLimit = repeats;
            seq->play = true;
            res |= true;
        }
    }
    return res;
}

void SPUSeqStop(s32 seqId) 
{
    for (Sequence* seq : sequences)
    {
        if (seq->id == seqId)
        {
            seq->Reset();
            for (Voice* v : voices)
            {
                if (v && v->sequence == seq)
                {
                    v->offTime = timeSinceEpochMillisec();
                    
                    // TODO - If I add volume sweeps this should use
                    // that instead of a hard stop which can cause popping
                    // SPUReleaseVoice(v);
                }
            }
        }
    }
}

void SPUSeqSetVolume(s32 seqId, s16 voll, s16 volr)
{
    for (Sequence* seq : sequences)
    {
        if (seq->id == seqId)
        {
            seq->voll = voll;
            seq->volr = volr;
        }
    }
    for (Voice* v : voices)
    {
        if (v->sequence && v->sequence->id == seqId)
        {
            v->ConfigureVolume(127, 127); // max volume, just calling configure to calc again
        }
    }
}

bool SPUSeqIsDone(s32 seqId)
{
    bool res = false;
    for (Sequence* seq : sequences)
    {
        if (seq->id == seqId)
        {
            res |= seq->repeats >= seq->repeatLimit || !seq->play;
        }
    }
    return res;
}

s32 SPUOneShotPlay(s32 patchId, u8 note, s16 voll, s16 volr, s32 pitch, s32 pitchMin, s32 pitchMax)
{
    // - SoundEfx: apparently use sweeps and reuse the voice register (slig walking offscreen)
    // - OneShots: do not reuse voice regester (slig turning)
    // - Notes   : reuese register if possible (chanting)
    Patch* patch = patches[patchId];
    if (!patch)
    {
        return 0;
    }

    int ids = 0;
    for (Sample* sample : patch->samples)
    {
        if (!sample)
        {
            continue;
        }

        if (note > sample->maxNote || note < sample->minNote)
        {
            continue;
        }

        Voice* v = SPUObtainVoice(sample->priority, note, (u8) patchId);
        if (!v)
        {
            return 0;
        }

        pitchMax; // Not needed?
        v->Configure(patch, sample);
        v->ConfigureNote(note, pitch < pitchMin ? pitchMin : pitch);
        v->ConfigureVolume((u8) voll, (u8) volr);
        ids |= v->id;
    }

    return ids;
}

void SPUOneShotStop(s32 mask)
{
    for (int i = 0; i < VOICE_SIZE_LIMIT; i++)
    {
        if ((voices[i]->id & mask) != 0)
        {
            // TODO - not sure if this is right.
            // Maybe it should trigger a release?
            SPUReleaseVoice(voices[i]);
        }
    }
} 

void SPUTick(void* udata, Uint8* stream, int len)
{
    udata;

    // This runs at 44100hz
    // 1. tick voices
    //      a. interpolate
    //      b. adsr
    //      c. pitch
    // 2. run reverb

    // start/stop any sequence notes
    SPUTickSequences();

    float* AudioStream = (float*) stream;
    int StreamLength = len / sizeof(float);

    f32 reverb_out_left = 0;
    f32 reverb_out_right = 0;

    // Prepare voices for every position SDL is expecting
    for (int i = 0; i < StreamLength; i += 2)
    {
        // set silence incase no voices are played
        AudioStream[i] = 0;
        AudioStream[i + 1] = 0;

        f32 leftSample = 0;
        f32 rightSample = 0;
        f32 reverb_in_left = 0;
        f32 reverb_in_right = 0;

        // 1. Prepare all voices with reverb
        for (Voice* v : voices)
        {
            if (!v->sample || v->complete || !v->inUse)
            {
                continue;
            }

            std::tuple<f32, f32> s = v->Tick();
            leftSample += std::get<0>(s);
            rightSample += std::get<1>(s);

            // 0 Off
            // 1 Vibrate
            // 2 Portamento
            // 3 1 & 2(Portamento and Vibrate on)
            // 4 Reverb
            if (v->sample->reverb != 0)
            {
                reverb->ChangeSetting(v->sample->reverb);
                reverb_in_left += std::get<0>(s);
                reverb_in_right += std::get<1>(s);
            }
        }

        // make value usable by SDL
        leftSample = leftSample / 32768.0f;
        rightSample = rightSample / 32768.0f;

        // 2. Process and mix in the reverb
        std::tuple<f32, f32> r = reverb->Process(reverb_in_left, reverb_in_right);
        reverb_out_left = std::get<0>(r);
        reverb_out_right = std::get<1>(r);

        //leftSample = reverb_out_left / 32768.0f;
        //rightSample = reverb_out_right / 32768.0f;
        leftSample += reverb_out_left / 32768.0f;
        rightSample += reverb_out_right / 32768.0f;

        SDL_MixAudioFormat((Uint8*) (AudioStream + i), (const Uint8*) &leftSample, AUDIO_F32, sizeof(float), 80);
        SDL_MixAudioFormat((Uint8*) (AudioStream + i + 1), (const Uint8*) &rightSample, AUDIO_F32, sizeof(float), 80);
    }
}

void SPUTickSequences()
{
    // TODO - convert this to be based on 44100hz instead of ms timestamp 

    u64 now = timeSinceEpochMillisec();

    // Tick sequences
    for (Sequence* seq : sequences)
    {
        if (!seq || !seq->play)
        {
            continue;
        }

        if (seq->repeats >= seq->repeatLimit && seq->repeatLimit > 0)
        {
            SPUSeqStop(seq->id);
            continue;
        }

        MIDIMessage* message;
        while ((message = seq->next(now)) != NULL)
        {
            switch (message->type)
            {
                case NOTE_ON:
                {
                    if (!seq->channels[message->channelId]->patch)
                    {
                        break;
                    }

                    Channel* channel = seq->channels[message->channelId];
                    Patch* patch = channel->patch;
                    for (Sample* sample : patch->samples)
                    {
                        if (!sample)
                        {
                            continue;
                        }

                        if (message->note > sample->maxNote || message->note < sample->minNote)
                        {
                            continue;
                        }

                        Voice* v = SPUObtainVoice(sample->priority, message->note, message->patchId);
                        if (!v)
                        {
                            continue;
                        }

                        v->Configure(message, seq, channel, patch, sample);
                        v->ConfigureVolume(127, 127); // max volume, seq message data has calculated volume data
                        v->ConfigureNote(message->note, 0);
                    }

                    break;
                }
                case NOTE_OFF:
                {
                    for (Voice* v : voices)
                    {
                        if (v->inUse && v->IsMatch(seq, message->channelId, message->note))
                        {
                            v->offTime = now;
                        }
                    }
                    break;
                }
                case PATCH_CHANGE:
                {
                    seq->channels[message->channelId]->patch = patches[message->patchId];
                    break;
                }
                case END_TRACK:
                {
                    // repeats are handled in the sequence when messages restart at position 0
                    break;
                }
                case PITCH_BEND:
                {
                    for (Voice* v : voices)
                    {
                        if (v->inUse && v->IsMatch(seq, message->channelId))
                        {
                            v->ConfigureNote(v->GetNote(), message->bend);
                        }
                    }
                    break;
                }
            }
        }
    }
}


//////////////////////////
// ADSR
ADSR parseADSR(u16 adsr1, u16 adsr2)
{
    ADSR adsr;
    adsr.sustainLevel = (adsr1) & mask(4);
    adsr.decayRate = (adsr1 >> 4) & mask(4);
    adsr.attackRate = (adsr1 >> 8) & mask(7);
    adsr.attackExponential = (adsr1 >> 15) & mask(1);

    adsr.releaseRate = adsr2 & mask(5);
    adsr.releaseExponential = (adsr2 >> 5) & mask(1);
    adsr.sustainRate = (adsr2 >> 6) & mask(7);
    adsr.sustainDirection = (adsr2 >> 14) & mask(1);
    adsr.sustainExponential = (adsr2 >> 15) & mask(1);
    return adsr;
}

struct ADSRTableEntry
{
    s32 ticks;
    s32 step;
};
enum : u32
{
    NUM_ADSR_TABLE_ENTRIES = 128,
    NUM_ADSR_DIRECTIONS = 2 // increasing, decreasing
};
using ADSRTableEntries = std::array<std::array<ADSRTableEntry, NUM_ADSR_TABLE_ENTRIES>, NUM_ADSR_DIRECTIONS>;

static constexpr ADSRTableEntries ComputeADSRTableEntries()
{
    ADSRTableEntries entries = {};
    for (u32 decreasing = 0; decreasing < 2; decreasing++)
    {
        for (u32 rate = 0; rate < NUM_ADSR_TABLE_ENTRIES; rate++)
        {
            if (rate < 48)
            {
                entries[decreasing][rate].ticks = 1;
                if (decreasing != 0)
                    entries[decreasing][rate].step = static_cast<s32>(static_cast<u32>(-8 + static_cast<s32>(rate & 3)) << (11 - (rate >> 2)));
                else
                    entries[decreasing][rate].step = (7 - static_cast<s32>(rate & 3)) << (11 - (rate >> 2));
            }
            else
            {
                entries[decreasing][rate].ticks = 1 << (static_cast<s32>(rate >> 2) - 11);
                if (decreasing != 0)
                    entries[decreasing][rate].step = (-8 + static_cast<s32>(rate & 3));
                else
                    entries[decreasing][rate].step = (7 - static_cast<s32>(rate & 3));
            }
        }
    }

    return entries;
}

static constexpr ADSRTableEntries s_adsr_table = ComputeADSRTableEntries();


//////////////////////////
// Voice
USHORT panMergeTable[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03,
    0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13,
    0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23,
    0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33,
    0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x41, 0x42, 0x43,
    0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53,
    0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60, 0x61, 0x62, 0x63,
    0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x5B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73,
    0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
    0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
    0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
    0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
    0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F};


static u16 MergePan(u16 pan1, u16 pan2)
{
    return panMergeTable[pan1 + pan2];
}

void Voice::Configure(MIDIMessage* msg, Sequence* seq, Channel* chan, Patch* pat, Sample* samp)
{
    this->message = msg;
    this->sequence = seq;
    this->channel = chan;
    Configure(pat, samp);
}

void Voice::Configure(Patch* pat, Sample* samp)
{
    this->patch = pat;
    this->sample = samp;

    // current sample block is 28 samples in this block + 3 from the next block
    // to achieve interpolation without going out of bounds
    for (int i = 0; i < NUM_SAMPLES_FROM_LAST_ADPCM_BLOCK; i++)
    {
        currSamples[i] = 0;
    }
    memcpy(&currSamples[NUM_SAMPLES_FROM_LAST_ADPCM_BLOCK], sample->buffer, NUM_SAMPLES_PER_ADPCM_BLOCK * sizeof(s16));
}

void Voice::Reset()
{
    message = nullptr;
    sequence = nullptr;
    channel = nullptr;
    patch = nullptr;
    sample = nullptr;

    offTime = 0;
    pitch = 0;
    note = 0;

    adsrPhase = NONE;
    adsrCounter = 0;
    adsrCurrentLevel = 0;
    adsrTargetLevel = MAX_VOLUME;
    f_SampleOffset = 0;
    vounter->Reset();
    HasSamples = true;
    complete = false;
    inUse = false;
}

void Voice::ConfigureNote(u8 _note, u32 _pitch)
{
    this->note = _note;
    this->pitch = _pitch;
    noteStep = vounter->CalculateNoteStep(sample->rootNote, sample->rootNotePitchShift, sample->SampleRate, note, pitch);
}

u8 Voice::GetNote()
{
    return note;
}

// PSX table with lerped inbeatween values
f32 panTable[0x80] = {
    0x00, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E,
    0x10, 0x12, 0x14, 0x16, 0x18, 0x1A, 0x1C, 0x1E,
    0x20, 0x22, 0x24, 0x26, 0x28, 0x2A, 0x2C, 0x2E,
    0x30, 0x32, 0x34, 0x36, 0x38, 0x3A, 0x3C, 0x3E,
    0x40, 0x42, 0x44, 0x46, 0x48, 0x4A, 0x4C, 0x4E,
    0x50, 0x52, 0x54, 0x56, 0x58, 0x5A, 0x5C, 0x5E,
    0x60, 0x62, 0x64, 0x66, 0x68, 0x6A, 0x6C, 0x6E,
    0x70, 0x72, 0x74, 0x76, 0x78, 0x78, 0x78, 0x78,
    0x78, 0x7A, 0x7C, 0x7E, 0x80, 0x80, 0x80, 0x80,
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80};

void Voice::ConfigureVolume(u8 volLeft, u8 volRight)
{
    f32 panVolumes[0x80];

    for (size_t i = 0; i < 0x80; i++)
    {
        panVolumes[i] = panTable[i] / 128.0f;
    }

    u8 channelPan = 64;
    u8 patchPan = patch->pan;
    u8 tonePan = sample->pan;

    f32 volumeLeft = 1.0f;
    f32 volumeRight = 1.0f;

    if (sequence)
    {
        channelPan = channel->pan;
        f32 volume = (channel->vol / 128.0f) * (message->velocity / 128.0f);
        volumeLeft = volume * (sequence->voll / 128.0f);
        volumeRight = volume * (sequence->volr / 128.0f);
    }
    
    u16 panIndex = MergePan(channelPan, MergePan(patchPan, tonePan));
    volumeLeft = volumeLeft * panVolumes[127 - panIndex];
    volumeRight = volumeRight * panVolumes[panIndex];

    volumeLeftModifier = volumeLeft * (volLeft / 128.0f) * (patch->vol / 128.0f) * (sample->volume / 128.0f);
    volumeRightModifier = volumeRight * (volRight / 128.0f) * (patch->vol / 128.0f) * (sample->volume / 128.0f);
}

bool Voice::IsMatch(Sequence* seq, u8 channelId)
{
    if (!this->sequence || !this->channel)
    {
        return false;
    }
    return this->sequence->id == seq->id && this->sequence->channels[channelId] == this->channel;
}

bool Voice::IsMatch(Sequence* seq, u8 channelId, u8 _note)
{
    if (!IsMatch(seq, channelId))
    {
        return false;
    }
    return this->note == _note;
}

float Voice::Interpolate()
{
    // vounter (gauss interpolation table) runs at 28 byte blocks (NUM_SAMPLES_PER_ADPCM_BLOCK).
    // move the sample offset forward every 28 bytes
    u32 sampOffset = NUM_SAMPLES_FROM_LAST_ADPCM_BLOCK;
    if (!this->HasSamples)
    {
        f_SampleOffset += NUM_SAMPLES_PER_ADPCM_BLOCK;

        // If it doesn't loop and we don't have enough in the last block, complete the voice
        if (!sample->loop && f_SampleOffset + NUM_SAMPLES_PER_ADPCM_BLOCK >= sample->len)
        {
            complete = true;
            return 0;
        }

        // if the new offset is greater than the audio data we are looping back to the front
        if (f_SampleOffset >= sample->len)
        {
            f_SampleOffset = f_SampleOffset - sample->len;
        }

        // Copy the last 3 samples from the previous block to the front
        memcpy(&currSamples[0], &currSamples[NUM_SAMPLES_PER_ADPCM_BLOCK], sampOffset * sizeof(u16));

        s32 diff = f_SampleOffset + NUM_SAMPLES_PER_ADPCM_BLOCK - sample->len;
        if (diff >= 0)
        {
            // the next block overflows, essentially the same as a for loop doing
            // sample->buffer[(f_SampleOffset + i) % sample->len];
            memcpy(&currSamples[sampOffset], &sample->buffer[f_SampleOffset], (NUM_SAMPLES_PER_ADPCM_BLOCK - diff) * sizeof(s16));
            memcpy(&currSamples[sampOffset + NUM_SAMPLES_PER_ADPCM_BLOCK - diff], &sample->buffer[0], (diff) * sizeof(s16));
        }
        else
        {
            memcpy(&currSamples[sampOffset], &sample->buffer[f_SampleOffset], NUM_SAMPLES_PER_ADPCM_BLOCK * sizeof(s16));
        }
    }

    f32 out = vounter->Sample(currSamples);

    // noteStep is called in ConfigureNote() to save CPU cycles as this is called at 44100hz
    this->HasSamples = !this->vounter->NextSampleBlock(noteStep);
    return out;
}

s16 Voice::TickAdsr()
{
    // UPDATE ADSR STATE - done at 441000hz
    if (adsrPhase == NONE)
    {
        adsrPhase = ATTACK;
        adsrDecreasing = false;
        adsrRate = sample->adsr.attackRate;
        adsrExponential = sample->adsr.attackExponential;
        adsrTargetLevel = MAX_VOLUME;
        adsrCounter = s_adsr_table[adsrDecreasing][adsrRate].ticks;
        //adsrCurrentLevel = MAX_VOLUME;
    }
    else if (adsrPhase == ATTACK && adsrCurrentLevel >= adsrTargetLevel)
    {
        adsrPhase = DECAY;
        adsrDecreasing = true;
        adsrRate = sample->adsr.decayRate << 2;
        adsrExponential = true;
        adsrTargetLevel = static_cast<s16>(std::min<s32>((u32(sample->adsr.sustainLevel) + 1) * 0x800, MAX_VOLUME));
        adsrCounter = s_adsr_table[adsrDecreasing][adsrRate].ticks;
    }
    else if (adsrPhase == DECAY && adsrCurrentLevel <= adsrTargetLevel)
    {
        adsrPhase = SUSTAIN;
        adsrDecreasing = sample->adsr.sustainDirection;
        adsrRate = sample->adsr.sustainRate;
        adsrExponential = sample->adsr.sustainExponential;
        adsrTargetLevel = 0;
        adsrCounter = s_adsr_table[adsrDecreasing][adsrRate].ticks;
    }
    else if (offTime != 0 && adsrPhase != RELEASE)
    {
        adsrPhase = RELEASE;
        adsrDecreasing = true;
        adsrRate = sample->adsr.releaseRate << 2;
        adsrExponential = sample->adsr.releaseExponential;
        adsrTargetLevel = 0;
        adsrCounter = s_adsr_table[adsrDecreasing][adsrRate].ticks;
        //adsrCurrentLevel = MIN_VOLUME;
    }
    else if (adsrPhase == RELEASE && adsrCurrentLevel <= adsrTargetLevel)
    {
        complete = true;
        return 0;
    }

    // UPDATE ADSR TICK STATE
    adsrCounter--;
    if (adsrCounter <= 0)
    {
        const ADSRTableEntry& table_entry = s_adsr_table[adsrDecreasing][adsrRate];
        s32 this_step = table_entry.step;
        adsrCounter = table_entry.ticks;

        if (adsrExponential)
        {
            if (adsrDecreasing)
            {
                this_step = (this_step * adsrCurrentLevel) >> 15;
            }
            else
            {
                if (adsrCurrentLevel >= 0x6000)
                {
                    if (adsrRate < 40)
                    {
                        this_step >>= 2;
                    }
                    else if (adsrRate >= 44)
                    {
                        adsrCounter >>= 2;
                    }
                    else
                    {
                        this_step >>= 1;
                        adsrCounter >>= 1;
                    }
                }
            }
        }

        adsrCurrentLevel = static_cast<s16>(
            std::clamp<s32>(static_cast<s32>(adsrCurrentLevel) + this_step, MIN_VOLUME, MAX_VOLUME));
    }

    return adsrCurrentLevel;
}

std::tuple<f32, f32> Voice::Tick()
{
    if (!sample)
    {
        return std::make_tuple(0.0f, 0.0f);
    }

    // INTERPOLATION
    s32 sampleData = (s32) Interpolate();

    // Set the volume of the sample
    s32 vol = ApplyVolume(sampleData, TickAdsr());

    // TODO - apply voll and volr as sweeps.tick()? (VolumeEnvelope)
    //  it would be similar to the ADSR tick
    //  I believe sligs walking offscreen use a vol sweep
    f32 leftA = vol * volumeLeftModifier;
    f32 rightA = vol * volumeRightModifier; 
    return std::make_tuple(leftA, rightA);
}

} // namespace