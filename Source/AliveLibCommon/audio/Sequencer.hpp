#pragma once

#include "SDL.h"
#include <thread>
#include <vector>
#include <mutex>
#include <array>

namespace SPU {

const u32 NUM_SAMPLES_FROM_LAST_ADPCM_BLOCK = 3;
const u32 NUM_SAMPLES_PER_ADPCM_BLOCK = 28;

const s16 MIN_VOLUME = 0;
const s16 MAX_VOLUME = 32767;

static u32 mask(u32 num)
{
    u32 res = 0;
    while (num-- > 0)
    {
        res = (res << 1) | 1;
    }
    return res;
}

struct ASDR
{
    // adsr1
    u32 sustainLevel;
    u32 decayRate;
    u32 attackRate;
    u32 attackExponential;

    // adsr2
    u32 releaseRate;
    u32 releaseExponential;
    u32 sustainRate;
    u32 sustainDirection;
    u32 sustainExponential;
} typedef ADSR;
ADSR parseADSR(u16 adsr1, u16 adsr2);


/*
* Defines an audio sample. This contains raw audio data and its information
*/
class Sample
{
public:
    Sample(s16* buf, u32 size, u32 sampleRate)
    {
        buffer = buf;
        len = size;
        SampleRate = sampleRate;
    }
    ~Sample()
    {

    }

    u8 priority;
    u8 volume;
    u8 pan;

    // reverb style - 0 is none
    s8 reverb;
    bool loop;

    // Root Key
    s32 rootNote;
    s32 rootNotePitchShift;

    // Key range
    u8 minNote;
    u8 maxNote;

    ADSR adsr;

    s16* buffer;
    u32 len;
    u32 SampleRate;
};


/*
* A patch is an instrument, think of a piano/keyboard. A piano may be configured
* to play drums on lower keys and guitar on upper keys. The instrument
* defines which sample(s) to play when a note is pressed.
*/
class Patch
{
public:
    Patch(const u8 _id, u8 _vol, u8 _pan)
        : id(_id)
        , vol(_vol)
        , pan(_pan)
    {
        for (int i = 0; i < SAMPLE_SIZE_LIMIT; i++)
        {
            samples[i] = nullptr;
        }
    }
    ~Patch()
    {
        for (int i = 0; i < SAMPLE_SIZE_LIMIT; i++)
        {
            delete samples[i];
            samples[i] = nullptr;
        }
    }

    const u8 id;
    const u8 vol;
    const u8 pan;

    static const int SAMPLE_SIZE_LIMIT = 128;
    std::array<Sample*, SAMPLE_SIZE_LIMIT> samples;
};


enum MIDIMessageType
{
    NOTE_ON,
    NOTE_OFF,
    PATCH_CHANGE,
    END_TRACK,
    PITCH_BEND
};


/*
* Defines what to do in a MIDI sequence (play note, stop note, etc)
*/
class MIDIMessage
{
public:
    MIDIMessageType type;
    u32 tick;
    u8 channelId = 0;
    u8 patchId = 0;
    u8 note = 0;
    u8 velocity = 0;
    s16 bend;
};


struct Channel
{   
    // channels may have volums and pans too, but not implemented
    Patch* patch;
    u8 vol = 127;
    u8 pan = 64;
};

/*
* A MIDI sequence (a song). Contains information on the tones (Notes/percussion)
* in the song.
*/
class Sequence
{
public:
    Sequence()
    {
        for (int i = 0; i < 16; i++)
        {
            channels[i] = new Channel();
        }
    }
    ~Sequence()
    {
        for (int i = 0; i < 16; i++)
        {
            delete channels[i];
        }
        for (auto msg : messages)
        {
            delete msg;
        }
    }

    s32 id;
    std::atomic_bool play = false;

    s16 voll = 127;
    s16 volr = 127;

    float tempoUs; // BPM - defined in in microseconds (us)
    float ticksPerBeat;

    s32 repeatLimit = 1;
    s32 repeats = 0;

    void Reset();
    MIDIMessage* createMIDIMessage();
    MIDIMessage* next(u64 now);
    Channel* channels[16];

private:
    std::vector<MIDIMessage*> messages;
    u32 actionPos = 0;
    u64 trackStartTime = 0;
};

enum ADSRPhase
{
    NONE,
    ATTACK,
    DECAY,
    SUSTAIN,
    RELEASE
};

class VoiceCounter2
{
public:
    u32 SampleIndex();
    u32 InterpolationIndex();
    bool NextSampleBlock(u32 sampleRate);
    void Reset();

private:
    static const u32 StartOffset;
    static const u32 InterpolationShift;
    static const u32 InterpolationAnd;

    u32 Counter = 0;
};

union VoiceCounter
{
    // promoted to u32 because of overflow
    u32 bits = 0;

    //BitField<u32, u8, 4, 8> interpolation_index;
    u32 InterpolationIndex()
    {
        return (bits >> 4) & mask(8);
    }

    //BitField<u32, u8, 12, 5> sample_index;
    u32 SampleIndex()
    {
        return (bits >> 12) & mask(5);
    }

    bool NextSampleBlock(u32 step)
    {
        step = std::min<u32>(step, 0x3FFF);
        bits += step;
        if (this->SampleIndex() >= 28)
        {
            bits -= (NUM_SAMPLES_PER_ADPCM_BLOCK << 12);
            return true;
        }
        
        return false;
    }

    void Reset()
    {
        bits = 0;
    }
};

class VolumeEnvelope
{
public:
    s16 current_level;
    s16 target_level;
    s32 counter;
    bool decreasing;
    void Tick();
};

class Voice
{
public:
    static const u32 InterpolationBitDept = 9;
    static const u32 InterpolationWindowLen = 1 << InterpolationBitDept;
    static const f32 InterpolationWeights[][4];
    static const u32 SampleRateBitDepth = 26;
    static const u32 BaseSampleRate = 1 << Voice::SampleRateBitDepth;
    f32 interpWeights[2][InterpolationWindowLen + 1];

    static const s32 SampleRateCount = 256 * 12;
    u32 SampleRates[SampleRateCount];

    Voice()
    { 
        for (size_t i = 0; i < InterpolationWindowLen + 1; i++)
        {
            double pow1 = i / (double) InterpolationWindowLen;
            double pow2 = pow1 * pow1;
            double pow3 = pow2 * pow1;

            interpWeights[0][i] = (f32) (0.5f * (-pow3 + 2 * pow2 - pow1));
            interpWeights[1][i] = (f32) (0.5f * (3 * pow3 - 5 * pow2 + 2));
        }

       
        for (size_t i = 0; i < SampleRateCount; i++)
        {
            SampleRates[i] = (u32) std::round(Voice::BaseSampleRate * std::pow(2, i / (double) SampleRateCount));
        }
    }

    s32 id;

    Sequence* sequence = nullptr;
    Channel* channel = nullptr;
    Patch* patch = nullptr;
    Sample* sample = nullptr;
    MIDIMessage* message = nullptr;

    bool inUse = false;
    bool complete = false;
    u64 offTime = 0; // when the note was released

    std::tuple<f32, f32> Tick();
    void Configure(MIDIMessage* message, Sequence* seq, Channel* channel, Patch* patch, Sample* sample);
    void Configure(Patch* patch, Sample* sample);
    void ConfigureNote(u8 note, u32 pitch);
    void ConfigureVolume(u8 volLeft, u8 volRight);
    void Reset();
    u8 GetNote();

    bool IsMatch(Sequence* seq, u8 channelId);
    bool IsMatch(Sequence* seq, u8 channelId, u8 note);

private:
    // Volume modifer (multiple it by sample data)
    f32 volumeLeftModifier;
    f32 volumeRightModifier;

    // ADSR calculations
    ADSRPhase adsrPhase = NONE;
    s32 adsrCounter = 0; // decremented each midi tick
    u32 adsrRate;
    bool adsrDecreasing;
    bool adsrExponential;
    s16 adsrCurrentLevel = 0;
    s16 adsrTargetLevel = MAX_VOLUME; // attack we want to reach max

    // Interpolation/pitch Calculations
    VoiceCounter vounter;
    s16 currSamples[31];
    u8 note;
    s32 pitch;
    u32 noteStep;
    u32 f_SampleOffset = 0;
    bool HasSamples;

    std::tuple<f32, f32> CalculateVolume();
    f32 Interpolate();
    s16 TickAdsr();
};


void Init();
void DeInit();

void Reset();
void StopAll();

// returns an 'id' mask - more than one note may play during a oneshot.
// Each of the 24 voices have an id of (1 << x) where x is the voice num
s32 OneShotPlay(s32 patchId, u8 note, s16 voll, s16 volr, s32 pitch, s32 pitchMin, s32 pitchMax);
void OneShotStop(s32 mask);

// Patches are instruments that contain samples.
// Once a patch is added it will be managed by the SPU
// do not modify or delete it.
void PatchAdd(Patch* patch);

// Sequences define what notes to play "in a sequence" - a song.
// Once a sequence is added it will be managed by the SPU
// do not modify or delete it.
void SeqAdd(Sequence* seq);
bool SeqPlay(s32 seqId, s32 repeats, bool stopDuplicateSeq);
bool SeqPlay(s32 seqId, s32 repeats, s16 voll, s16 volr);
void SeqSetVolume(s32 seqId, s16 voll, s16 volr);
void SeqStop(s32 seqId);
bool SeqIsDone(s32 seqId);


} // namespace