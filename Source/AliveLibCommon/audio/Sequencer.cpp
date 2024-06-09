#pragma once

#include "Sequencer.hpp"
#include "SDL.h"

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

void ProcessReverb(s16 left_in, s16 right_in, s32* left_out, s32* right_out);
static s16 ReverbRead(u32 address, s32 offset = 0);


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
        voices[i] = new Voice();
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

    s32 reverb_out_left = 0;
    s32 reverb_out_right = 0;

    // Prepare voices for every position SDL is expecting
    for (int i = 0; i < StreamLength; i += 2)
    {
        // set silence incase no voices are played
        AudioStream[i] = 0;
        AudioStream[i + 1] = 0;

        float leftSample = 0;
        float rightSample = 0;
        s32 reverb_in_left = 0;
        s32 reverb_in_right = 0;

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
            if (v->sample->reverb == 4)
            {
                reverb_in_left += (s32) std::get<0>(s);
                reverb_in_right += (s32) std::get<1>(s);
            }
        }

        // 2. Process and mix in the reverb
        ProcessReverb(
            static_cast<s16>(Clamp16((s32) (reverb_in_left))),
            static_cast<s16>(Clamp16((s32) (reverb_in_right))),
            &reverb_out_left,
            &reverb_out_right);

        leftSample += reverb_out_left;
        rightSample += reverb_out_right;

        // make value usable by SDL
        leftSample = leftSample / 32767.0f;
        rightSample = rightSample / 32767.0f;
        SDL_MixAudioFormat((Uint8*) (AudioStream + i), (const Uint8*) &leftSample, AUDIO_F32, sizeof(float), SDL_MIX_MAXVOLUME);
        SDL_MixAudioFormat((Uint8*) (AudioStream + i + 1), (const Uint8*) &rightSample, AUDIO_F32, sizeof(float), SDL_MIX_MAXVOLUME);
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
    vounter.bits = 0;
    complete = false;
    complete = false;
    hasLooped = false;
    inUse = false;
}

USHORT _svm_ptable[] = {
    4096, 4110, 4125, 4140, 4155, 4170, 4185, 4200,
    4216, 4231, 4246, 4261, 4277, 4292, 4308, 4323,
    4339, 4355, 4371, 4386, 4402, 4418, 4434, 4450,
    4466, 4482, 4499, 4515, 4531, 4548, 4564, 4581,
    4597, 4614, 4630, 4647, 4664, 4681, 4698, 4715,
    4732, 4749, 4766, 4783, 4801, 4818, 4835, 4853,
    4870, 4888, 4906, 4924, 4941, 4959, 4977, 4995,
    5013, 5031, 5050, 5068, 5086, 5105, 5123, 5142,
    5160, 5179, 5198, 5216, 5235, 5254, 5273, 5292,
    5311, 5331, 5350, 5369, 5389, 5408, 5428, 5447,
    5467, 5487, 5507, 5527, 5547, 5567, 5587, 5607,
    5627, 5648, 5668, 5688, 5709, 5730, 5750, 5771,
    5792, 5813, 5834, 5855, 5876, 5898, 5919, 5940,
    5962, 5983, 6005, 6027, 6049, 6070, 6092, 6114,
    6137, 6159, 6181, 6203, 6226, 6248, 6271, 6294,
    6316, 6339, 6362, 6385, 6408, 6431, 6455, 6478,
    6501, 6525, 6549, 6572, 6596, 6620, 6644, 6668,
    6692, 6716, 6741, 6765, 6789, 6814, 6839, 6863,
    6888, 6913, 6938, 6963, 6988, 7014, 7039, 7064,
    7090, 7116, 7141, 7167, 7193, 7219, 7245, 7271,
    7298, 7324, 7351, 7377, 7404, 7431, 7458, 7485,
    7512, 7539, 7566, 7593, 7621, 7648, 7676, 7704,
    7732, 7760, 7788, 7816, 7844, 7873, 7901, 7930,
    7958, 7987, 8016, 8045, 8074, 8103, 8133, 8162}; // 192 total (0-191)
// 8192}; This was originally here. Do we need?

void Voice::ConfigureNote(u8 _note, u32 _pitch)
{
    this->note = _note;
    this->pitch = _pitch;

    // NOTE CALC FROM VgmTrans
    // This code seems to be producing the same adpcm_sample_rate values
    // as duckstation. Believe it or not, this code was found on pastebin
    // by searching for "SsPitchFromNote" - I think this is from VGMTrans?
    // https://pastebin.com/aq7wxDdr

    // NOTE: This doesn't always produce correct pitches
    // It creates values that go beyond _svm_ptable bounds; Examples
    // 1. SecurityOrbs produce indexes >_svm_ptable.size (199, limit is 192)
    // 2. Leaves coming out of wells produce indexes below 0 (-104, limit is 0)

    u32 pitchA;
    s16 calc, type;
    s32 add, sfine; 
    s32 fine = pitch;
    sfine = fine + ((s32) sample->rootNotePitchShift);
    if (sfine < 0)
    {
        sfine += 7;
    }
    sfine >>= 3;

    add = 0;
    if (sfine > 15)
    {
        add = 1;
        sfine -= 16;
    }

    calc = (s16) (add + (note - (((s32) sample->rootNote) - 60))); //((center + 60) - note) + add;
    int pos = (16 * (calc % 12) + sfine);

    // START: ADDED
    // Security Orbs, Pause menu, and well leaves had the
    // wrong pitch. The values produced went beyond the
    // table range. Try to correct them. Seems to work ok...
    // I just messed around with what made sense and sounded right.
    if (pos >= 192)
    {
        pitchA = _svm_ptable[pos % 192] << ((pos / 192));
    }
    else if (pos < 0)
    {
        pitchA = _svm_ptable[192 + pos] >> 1;
    }
    else
    {
        pitchA = _svm_ptable[pos];
    }
    // END: Added

    type = calc / 12 - 5;

    // regular shift
    s32 ret = pitchA;
    if (type > 0)
    {
        ret = pitchA << type;
    }

    // negative shift
    if (type < 0)
    {
        ret = pitchA >> -type;
    }

    // freq = (ret * 44100) / 4096
    // step seems to be calculated for 8000hz samples hence the division
    noteStep = (u16) ((sample->SampleRate / 8000.0) * double(ret));
    // std::cout << ret << " " << (int) note << " " << noteStep << std::endl;
}

u8 Voice::GetNote()
{
    return note;
}

void Voice::ConfigureVolume(u8 volLeft, u8 volRight)
{
    f32 panVolumes[0x80];

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
    static constexpr std::array<s16, 0x200> gauss = {{
        -0x001, -0x001, -0x001, -0x001, -0x001, -0x001, -0x001, -0x001, //
        -0x001, -0x001, -0x001, -0x001, -0x001, -0x001, -0x001, -0x001, //
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0001, //
        0x0001, 0x0001, 0x0001, 0x0002, 0x0002, 0x0002, 0x0003, 0x0003, //
        0x0003, 0x0004, 0x0004, 0x0005, 0x0005, 0x0006, 0x0007, 0x0007, //
        0x0008, 0x0009, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E, //
        0x000F, 0x0010, 0x0011, 0x0012, 0x0013, 0x0015, 0x0016, 0x0018, // entry
        0x0019, 0x001B, 0x001C, 0x001E, 0x0020, 0x0021, 0x0023, 0x0025, // 000..07F
        0x0027, 0x0029, 0x002C, 0x002E, 0x0030, 0x0033, 0x0035, 0x0038, //
        0x003A, 0x003D, 0x0040, 0x0043, 0x0046, 0x0049, 0x004D, 0x0050, //
        0x0054, 0x0057, 0x005B, 0x005F, 0x0063, 0x0067, 0x006B, 0x006F, //
        0x0074, 0x0078, 0x007D, 0x0082, 0x0087, 0x008C, 0x0091, 0x0096, //
        0x009C, 0x00A1, 0x00A7, 0x00AD, 0x00B3, 0x00BA, 0x00C0, 0x00C7, //
        0x00CD, 0x00D4, 0x00DB, 0x00E3, 0x00EA, 0x00F2, 0x00FA, 0x0101, //
        0x010A, 0x0112, 0x011B, 0x0123, 0x012C, 0x0135, 0x013F, 0x0148, //
        0x0152, 0x015C, 0x0166, 0x0171, 0x017B, 0x0186, 0x0191, 0x019C, //
        0x01A8, 0x01B4, 0x01C0, 0x01CC, 0x01D9, 0x01E5, 0x01F2, 0x0200, //
        0x020D, 0x021B, 0x0229, 0x0237, 0x0246, 0x0255, 0x0264, 0x0273, //
        0x0283, 0x0293, 0x02A3, 0x02B4, 0x02C4, 0x02D6, 0x02E7, 0x02F9, //
        0x030B, 0x031D, 0x0330, 0x0343, 0x0356, 0x036A, 0x037E, 0x0392, //
        0x03A7, 0x03BC, 0x03D1, 0x03E7, 0x03FC, 0x0413, 0x042A, 0x0441, //
        0x0458, 0x0470, 0x0488, 0x04A0, 0x04B9, 0x04D2, 0x04EC, 0x0506, //
        0x0520, 0x053B, 0x0556, 0x0572, 0x058E, 0x05AA, 0x05C7, 0x05E4, // entry
        0x0601, 0x061F, 0x063E, 0x065C, 0x067C, 0x069B, 0x06BB, 0x06DC, // 080..0FF
        0x06FD, 0x071E, 0x0740, 0x0762, 0x0784, 0x07A7, 0x07CB, 0x07EF, //
        0x0813, 0x0838, 0x085D, 0x0883, 0x08A9, 0x08D0, 0x08F7, 0x091E, //
        0x0946, 0x096F, 0x0998, 0x09C1, 0x09EB, 0x0A16, 0x0A40, 0x0A6C, //
        0x0A98, 0x0AC4, 0x0AF1, 0x0B1E, 0x0B4C, 0x0B7A, 0x0BA9, 0x0BD8, //
        0x0C07, 0x0C38, 0x0C68, 0x0C99, 0x0CCB, 0x0CFD, 0x0D30, 0x0D63, //
        0x0D97, 0x0DCB, 0x0E00, 0x0E35, 0x0E6B, 0x0EA1, 0x0ED7, 0x0F0F, //
        0x0F46, 0x0F7F, 0x0FB7, 0x0FF1, 0x102A, 0x1065, 0x109F, 0x10DB, //
        0x1116, 0x1153, 0x118F, 0x11CD, 0x120B, 0x1249, 0x1288, 0x12C7, //
        0x1307, 0x1347, 0x1388, 0x13C9, 0x140B, 0x144D, 0x1490, 0x14D4, //
        0x1517, 0x155C, 0x15A0, 0x15E6, 0x162C, 0x1672, 0x16B9, 0x1700, //
        0x1747, 0x1790, 0x17D8, 0x1821, 0x186B, 0x18B5, 0x1900, 0x194B, //
        0x1996, 0x19E2, 0x1A2E, 0x1A7B, 0x1AC8, 0x1B16, 0x1B64, 0x1BB3, //
        0x1C02, 0x1C51, 0x1CA1, 0x1CF1, 0x1D42, 0x1D93, 0x1DE5, 0x1E37, //
        0x1E89, 0x1EDC, 0x1F2F, 0x1F82, 0x1FD6, 0x202A, 0x207F, 0x20D4, //
        0x2129, 0x217F, 0x21D5, 0x222C, 0x2282, 0x22DA, 0x2331, 0x2389, // entry
        0x23E1, 0x2439, 0x2492, 0x24EB, 0x2545, 0x259E, 0x25F8, 0x2653, // 100..17F
        0x26AD, 0x2708, 0x2763, 0x27BE, 0x281A, 0x2876, 0x28D2, 0x292E, //
        0x298B, 0x29E7, 0x2A44, 0x2AA1, 0x2AFF, 0x2B5C, 0x2BBA, 0x2C18, //
        0x2C76, 0x2CD4, 0x2D33, 0x2D91, 0x2DF0, 0x2E4F, 0x2EAE, 0x2F0D, //
        0x2F6C, 0x2FCC, 0x302B, 0x308B, 0x30EA, 0x314A, 0x31AA, 0x3209, //
        0x3269, 0x32C9, 0x3329, 0x3389, 0x33E9, 0x3449, 0x34A9, 0x3509, //
        0x3569, 0x35C9, 0x3629, 0x3689, 0x36E8, 0x3748, 0x37A8, 0x3807, //
        0x3867, 0x38C6, 0x3926, 0x3985, 0x39E4, 0x3A43, 0x3AA2, 0x3B00, //
        0x3B5F, 0x3BBD, 0x3C1B, 0x3C79, 0x3CD7, 0x3D35, 0x3D92, 0x3DEF, //
        0x3E4C, 0x3EA9, 0x3F05, 0x3F62, 0x3FBD, 0x4019, 0x4074, 0x40D0, //
        0x412A, 0x4185, 0x41DF, 0x4239, 0x4292, 0x42EB, 0x4344, 0x439C, //
        0x43F4, 0x444C, 0x44A3, 0x44FA, 0x4550, 0x45A6, 0x45FC, 0x4651, //
        0x46A6, 0x46FA, 0x474E, 0x47A1, 0x47F4, 0x4846, 0x4898, 0x48E9, //
        0x493A, 0x498A, 0x49D9, 0x4A29, 0x4A77, 0x4AC5, 0x4B13, 0x4B5F, //
        0x4BAC, 0x4BF7, 0x4C42, 0x4C8D, 0x4CD7, 0x4D20, 0x4D68, 0x4DB0, //
        0x4DF7, 0x4E3E, 0x4E84, 0x4EC9, 0x4F0E, 0x4F52, 0x4F95, 0x4FD7, // entry
        0x5019, 0x505A, 0x509A, 0x50DA, 0x5118, 0x5156, 0x5194, 0x51D0, // 180..1FF
        0x520C, 0x5247, 0x5281, 0x52BA, 0x52F3, 0x532A, 0x5361, 0x5397, //
        0x53CC, 0x5401, 0x5434, 0x5467, 0x5499, 0x54CA, 0x54FA, 0x5529, //
        0x5558, 0x5585, 0x55B2, 0x55DE, 0x5609, 0x5632, 0x565B, 0x5684, //
        0x56AB, 0x56D1, 0x56F6, 0x571B, 0x573E, 0x5761, 0x5782, 0x57A3, //
        0x57C3, 0x57E2, 0x57FF, 0x581C, 0x5838, 0x5853, 0x586D, 0x5886, //
        0x589E, 0x58B5, 0x58CB, 0x58E0, 0x58F4, 0x5907, 0x5919, 0x592A, //
        0x593A, 0x5949, 0x5958, 0x5965, 0x5971, 0x597C, 0x5986, 0x598F, //
        0x5997, 0x599E, 0x59A4, 0x59A9, 0x59AD, 0x59B0, 0x59B2, 0x59B3  //
    }};

    // vounter (gauss interpolation table) runs at 28 byte blocks.
    // move the sample offset forward every 28 bytes
    if (vounter.sample_index() >= NUM_SAMPLES_PER_ADPCM_BLOCK)
    {
        // vounter holds sample position shifted << 12
        vounter.bits -= (NUM_SAMPLES_PER_ADPCM_BLOCK << 12);
        f_SampleOffset += NUM_SAMPLES_PER_ADPCM_BLOCK;
    }

    // Are we at the end of the sample?
    if (f_SampleOffset + vounter.sample_index() >= sample->len)
    {
        if (!sample->loop)
        {
            complete = true;
            return 0;
        }

        // we can loop this sample
        hasLooped = true;
        vounter.bits = 0;
        f_SampleOffset = 0;
    }

    // the interpolation index is based on the source files sample rate
    const u8 i = (u8) vounter.interpolation_index();
    const u32 s = ((u32) f_SampleOffset) + ZeroExtend32(vounter.sample_index());

    // interpolate on the 4 most recent samples from current position
    s32 out = 0;
    out += s32(gauss[0x0FF - i]) * s32(sample->buffer[(int) (s)]); 
    out += s32(gauss[0x1FF - i]) * s32(sample->buffer[((int) (s) + 1) % sample->len]); 
    out += s32(gauss[0x100 + i]) * s32(sample->buffer[((int) (s) + 2) % sample->len]);
    out += s32(gauss[0x000 + i]) * s32(sample->buffer[((int) (s) + 3) % sample->len]);

    // vounter.bits has two purposes
    // 1. change how samples are skipped to change the samples note
    // 2. maintain gauss table positioning for correct interpolation
    // noteStep is called in ConfigureNote() to save CPU cycles as this is called at 44100hz
    u16 step = noteStep;
    // if (v->isPitchModulated)
    // {
    //     const s32 factor = std::clamp<s32>(sampleData, -0x8000, 0x7FFF) + 0x8000;
    //     step = Truncate16(static_cast<u32>((SignExtend32(step) * factor) >> 15));
    // }
    step = std::min<u16>(step, 0x3FFF);
    vounter.bits += step;

    return (f32) (out >> 15);
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


//////////////////////////
// REVERB - duckstation
static const u32 NUM_REVERB_REGS = 32;
struct ReverbRegisters
{
    s16 vLOUT;
    s16 vROUT;
    u16 mBASE;

    union
    {
        struct
        {
            u16 FB_SRC_A;
            u16 FB_SRC_B;
            s16 IIR_ALPHA;
            s16 ACC_COEF_A;
            s16 ACC_COEF_B;
            s16 ACC_COEF_C;
            s16 ACC_COEF_D;
            s16 IIR_COEF;
            s16 FB_ALPHA;
            s16 FB_X;
            u16 IIR_DEST_A[2];
            u16 ACC_SRC_A[2];
            u16 ACC_SRC_B[2];
            u16 IIR_SRC_A[2];
            u16 IIR_DEST_B[2];
            u16 ACC_SRC_C[2];
            u16 ACC_SRC_D[2];
            u16 IIR_SRC_B[2];
            u16 MIX_DEST_A[2];
            u16 MIX_DEST_B[2];
            s16 IN_COEF[2];
        }a;

        u16 rev[NUM_REVERB_REGS];
    };
};

// bit mask of voices with reverb
static const u32 RAM_SIZE = 512 * 1024;
static std::array<u8, RAM_SIZE> s_ram{};
//static u32 s_reverb_on_register = 0;
static u32 s_reverb_base_address = 0;
static u32 s_reverb_current_address = 0;
static ReverbRegisters s_reverb_registers{};
static std::array<std::array<s16, 128>, 2> s_reverb_downsample_buffer;
static std::array<std::array<s16, 64>, 2> s_reverb_upsample_buffer;
static s32 s_reverb_resample_buffer_position = 0;


// Zeroes optimized out; middle removed too(it's 16384)
static constexpr std::array<s16, 20> s_reverb_resample_coefficients = {
    -1,
    2,
    -10,
    35,
    -103,
    266,
    -616,
    1332,
    -2960,
    10246,
    10246,
    -2960,
    1332,
    -616,
    266,
    -103,
    35,
    -10,
    2,
    -1,
};
static s16 s_last_reverb_input[2];
static s32 s_last_reverb_output[2];

static s32 Reverb4422(const s16* src)
{
    s32 out = 0; // 32-bits is adequate(it won't overflow)
    for (u32 i = 0; i < 20; i++)
        out += s_reverb_resample_coefficients[i] * src[i * 2];

    // Middle non-zero
    out += 0x4000 * src[19];
    out >>= 15;
    return std::clamp<s32>(out, -32768, 32767);
}

template <bool phase>
static s32 Reverb2244(const s16* src)
{
    s32 out; // 32-bits is adequate(it won't overflow)
    if (phase)
    {
        // Middle non-zero
        out = src[9];
    }
    else
    {
        out = 0;
        for (u32 i = 0; i < 20; i++)
            out += s_reverb_resample_coefficients[i] * src[i];

        out >>= 14;
        out = std::clamp<s32>(out, -32768, 32767);
    }

    return out;
}

static s16 ReverbSat(s32 val)
{
    return static_cast<s16>(std::clamp<s32>(val, -0x8000, 0x7FFF));
}

static s16 ReverbNeg(s16 samp)
{
    if (samp == -32768)
        return 0x7FFF;

    return -samp;
}

static s32 IIASM(const s16 IIR_ALPHA, const s16 insamp)
{
    if (IIR_ALPHA == -32768)
    {
        if (insamp == -32768)
            return 0;
        else
            return insamp * -65536;
    }
    else
        return insamp * (32768 - IIR_ALPHA);
}

u32 ReverbMemoryAddress(u32 address)
{
    // Ensures address does not leave the reverb work area.
    static constexpr u32 MASK = (RAM_SIZE - 1) / 2;
    u32 offset = s_reverb_current_address + (address & MASK);
    offset += s_reverb_base_address & ((s32) (offset << 13) >> 31);

    // We address RAM in bytes. TODO: Change this to words.
    return (offset & MASK) * 2u;
}

s16 ReverbRead(u32 address, s32 offset)
{
    // TODO: This should check interrupts.
    const u32 real_address = ReverbMemoryAddress((address << 2) + offset);

    s16 data;
    std::memcpy(&data, &s_ram[real_address], sizeof(data));
    return data;
}

void ReverbWrite(u32 address, s16 data)
{
    // TODO: This should check interrupts.
    const u32 real_address = ReverbMemoryAddress(address << 2);
    std::memcpy(&s_ram[real_address], &data, sizeof(data));
}

void ProcessReverb(s16 left_in, s16 right_in, s32* left_out, s32* right_out)
{
    s_last_reverb_input[0] = left_in;
    s_last_reverb_input[1] = right_in;
    s_reverb_downsample_buffer[0][s_reverb_resample_buffer_position | 0x00] = left_in;
    s_reverb_downsample_buffer[0][s_reverb_resample_buffer_position | 0x40] = left_in;
    s_reverb_downsample_buffer[1][s_reverb_resample_buffer_position | 0x00] = right_in;
    s_reverb_downsample_buffer[1][s_reverb_resample_buffer_position | 0x40] = right_in;
    std::array<std::array<s16, 128>, 2> test;
    
    // these values seem to be stable in duckstation - move them to be set once
    s_reverb_registers.vLOUT = 4128;
    s_reverb_registers.vROUT = 4128;
    s_reverb_registers.mBASE = 61956;
    s_reverb_registers.a.IIR_SRC_A[0] = 2905;
    s_reverb_registers.a.IIR_SRC_A[1] = 2266;
    s_reverb_registers.a.IIR_COEF = -22912;
    s_reverb_registers.a.IN_COEF[0] = -32768;
    s_reverb_registers.a.IN_COEF[1] = -32768;
    s_reverb_registers.a.IIR_SRC_B[0] = 1514;
    s_reverb_registers.a.IIR_SRC_B[1] = 797;
    s_reverb_registers.a.IIR_ALPHA = 28512;
    s_reverb_registers.a.IIR_DEST_A[0] = 3579;
    s_reverb_registers.a.IIR_DEST_A[1] = 2904;
    s_reverb_registers.a.IIR_DEST_B[0] = 2265;
    s_reverb_registers.a.IIR_DEST_B[1] = 1513;
    s_reverb_registers.a.ACC_SRC_A[0] = 3337;
    s_reverb_registers.a.ACC_SRC_A[1] = 2620;
    s_reverb_registers.a.ACC_SRC_B[0] = 3033;
    s_reverb_registers.a.ACC_SRC_B[1] = 2419;
    s_reverb_registers.a.ACC_SRC_C[0] = 2028;
    s_reverb_registers.a.ACC_SRC_C[1] = 1200;
    s_reverb_registers.a.ACC_SRC_D[0] = 1775;
    s_reverb_registers.a.ACC_SRC_D[1] = 978;
    s_reverb_registers.a.ACC_COEF_A = 20392;
    s_reverb_registers.a.ACC_COEF_B = -17184;
    s_reverb_registers.a.ACC_COEF_C = 17680;
    s_reverb_registers.a.ACC_COEF_D = -16656;
    s_reverb_registers.a.MIX_DEST_A[0] = 796;
    s_reverb_registers.a.MIX_DEST_A[1] = 568;
    s_reverb_registers.a.MIX_DEST_B[0] = 340;
    s_reverb_registers.a.MIX_DEST_B[1] = 170;
    s_reverb_registers.a.FB_SRC_A = 227;
    s_reverb_registers.a.FB_SRC_B = 169;
    s_reverb_registers.a.FB_ALPHA = 22144;
    s_reverb_registers.a.FB_X = 21184;
       
     
    test = s_reverb_downsample_buffer;
    //std::cout << "L:" << left_in << " R:" << right_in << std::endl;
    //std::cout << s_reverb_registers.a.IIR_SRC_A[0] << std::endl;

    s32 out[2];
    if (s_reverb_resample_buffer_position & 1u)
    {
        //std::cout << "here1" << std::endl;
        std::array<s32, 2> downsampled;
        for (unsigned lr = 0; lr < 2; lr++)
            downsampled[lr] = Reverb4422(&s_reverb_downsample_buffer[lr][(s_reverb_resample_buffer_position - 38) & 0x3F]);

        for (unsigned lr = 0; lr < 2; lr++)
        {
            //if (s_SPUCNT.a.reverb_master_enable)
            {
                const s16 IIR_INPUT_A = ReverbSat((((ReverbRead(s_reverb_registers.a.IIR_SRC_A[lr ^ 0]) * s_reverb_registers.a.IIR_COEF) >> 14) + ((downsampled[lr] * s_reverb_registers.a.IN_COEF[lr]) >> 14)) >> 1);
                const s16 IIR_INPUT_B = ReverbSat((((ReverbRead(s_reverb_registers.a.IIR_SRC_B[lr ^ 1]) * s_reverb_registers.a.IIR_COEF) >> 14) + ((downsampled[lr] * s_reverb_registers.a.IN_COEF[lr]) >> 14)) >> 1);
                const s16 IIR_A = ReverbSat((((IIR_INPUT_A * s_reverb_registers.a.IIR_ALPHA) >> 14) + (IIASM(s_reverb_registers.a.IIR_ALPHA, ReverbRead(s_reverb_registers.a.IIR_DEST_A[lr], -1)) >> 14)) >> 1);
                const s16 IIR_B = ReverbSat((((IIR_INPUT_B * s_reverb_registers.a.IIR_ALPHA) >> 14) + (IIASM(s_reverb_registers.a.IIR_ALPHA, ReverbRead(s_reverb_registers.a.IIR_DEST_B[lr], -1)) >> 14)) >> 1);
                ReverbWrite(s_reverb_registers.a.IIR_DEST_A[lr], IIR_A);
                ReverbWrite(s_reverb_registers.a.IIR_DEST_B[lr], IIR_B);
            }

            const s32 ACC = ((ReverbRead(s_reverb_registers.a.ACC_SRC_A[lr]) * s_reverb_registers.a.ACC_COEF_A) >> 14) + ((ReverbRead(s_reverb_registers.a.ACC_SRC_B[lr]) * s_reverb_registers.a.ACC_COEF_B) >> 14) + ((ReverbRead(s_reverb_registers.a.ACC_SRC_C[lr]) * s_reverb_registers.a.ACC_COEF_C) >> 14) + ((ReverbRead(s_reverb_registers.a.ACC_SRC_D[lr]) * s_reverb_registers.a.ACC_COEF_D) >> 14);

            const s16 FB_A = ReverbRead(s_reverb_registers.a.MIX_DEST_A[lr] - s_reverb_registers.a.FB_SRC_A);
            const s16 FB_B = ReverbRead(s_reverb_registers.a.MIX_DEST_B[lr] - s_reverb_registers.a.FB_SRC_B);
            const s16 MDA = ReverbSat((ACC + ((FB_A * ReverbNeg(s_reverb_registers.a.FB_ALPHA)) >> 14)) >> 1);
            const s16 MDB = ReverbSat(
                FB_A + ((((MDA * s_reverb_registers.a.FB_ALPHA) >> 14) + ((FB_B * ReverbNeg(s_reverb_registers.a.FB_X)) >> 14)) >> 1));
            const s16 IVB = ReverbSat(FB_B + ((MDB * s_reverb_registers.a.FB_X) >> 15));

            //if (s_SPUCNT.reverb_master_enable)
            {
                ReverbWrite(s_reverb_registers.a.MIX_DEST_A[lr], MDA);
                ReverbWrite(s_reverb_registers.a.MIX_DEST_B[lr], MDB);
            }

            s_reverb_upsample_buffer[lr][(s_reverb_resample_buffer_position >> 1) | 0x20] = s_reverb_upsample_buffer[lr][s_reverb_resample_buffer_position >> 1] = IVB;
        }

        s_reverb_current_address = (s_reverb_current_address + 1) & 0x3FFFFu;
        if (s_reverb_current_address == 0)
            s_reverb_current_address = s_reverb_base_address;

        for (unsigned lr = 0; lr < 2; lr++)
            out[lr] = Reverb2244<false>(&s_reverb_upsample_buffer[lr][((s_reverb_resample_buffer_position >> 1) - 19) & 0x1F]);
    }
    else
    {
        //std::cout << "here2" << std::endl;
        for (unsigned lr = 0; lr < 2; lr++)
            out[lr] = Reverb2244<true>(&s_reverb_upsample_buffer[lr][((s_reverb_resample_buffer_position >> 1) - 19) & 0x1F]);
    }

    s_reverb_resample_buffer_position = (s_reverb_resample_buffer_position + 1) & 0x3F;

    s_last_reverb_output[0] = *left_out = ApplyVolume(out[0], s_reverb_registers.vLOUT);
    s_last_reverb_output[1] = *right_out = ApplyVolume(out[1], s_reverb_registers.vROUT);
}

} // namespace