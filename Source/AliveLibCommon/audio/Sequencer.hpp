#pragma once

#include "SDL.h"
#include <thread>
#include <vector>
#include <mutex>

namespace sean {

const s16 MIN_VOLUME = 0;
const s16 MAX_VOLUME = 32767;

u32 mask(u32 num);

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
        delete[] buffer;
    }

    float volume;
    float pan;

    // reverb style - 0 is none
    s8 reverb;

    // Root Key
    u8 rootNote;
    u8 rootNotePitchShift;

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
    Patch()
    {
        for (int i = 0; i < 128; i++)
        {
            samples[i] = NULL;
        }
    }
    ~Patch()
    {
        for (Sample* sample : samples)
        {
            delete sample;
        }
    }

    u8 id;
    Sample* samples[128];
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
    s32 repeatLimit = 1;
    std::atomic_bool play = false;

    float volume = 1;
    float pan = 0;
    float tempoUs; // BPM - defined in in microseconds (us)
    float ticksPerBeat;

    s32 repeats = 0;

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

union VoiceCounter
{
    // promoted to u32 because of overflow
    u32 bits = 0;

    //BitField<u32, u8, 4, 8> interpolation_index;
    u32 interpolation_index()
    {
        return (bits >> 4) & mask(8);
    }

    //BitField<u32, u8, 12, 5> sample_index;
    u32 sample_index()
    {
        return (bits >> 12) & mask(5);
    }
};

class Voice
{
public:
    s32 id;

    double f_SampleOffset = 3;
    Sequence* sequence = NULL;
    u8 patchId;
    u8 channelId;
    u8 note;
    s32 pitch;
    s32 pitchMin = 0;
    s32 pitchMax = 127;
    float velocity = 1.0f;
    float pan = 0.0f;
    std::atomic_bool inUse = false;

    s32 sOldest = 0;
    s32 sOlder = 0;
    s32 sOld = 0;
    s32 sNew = 0;

    void pushSample(s32 s);

    bool complete = false;
    bool loop = false;
    u64 offTime = 0;  // when the note was released

    VoiceCounter vounter;

    ADSRPhase adsrPhase = NONE;
    s32 adsrCounter = 0; // decremented each midi tick
    u32 adsrRate;
    bool adsrDecreasing;
    bool adsrExponential;
    s16 adsrCurrentLevel = 0;
    s16 adsrTargetLevel = MAX_VOLUME; // attack we want to reach max

    Sample* sample;

    std::tuple<s32, s32> tick();

private:
    s32 interpolate();
};


/*
* Non blocking event ring for 
* cross thread actions (oddworld to midi player)
*/
class Event
{
    void (*func)();
};

class EventRing
{
public:
    void push(Event event);
    Event* pop();

private:
    static const int size = 256;
    Event events[size];
    std::atomic<u64> tail = 0;
    std::atomic<u64> head = 0;
};


/*
* Can play MIDI
*/
class Sequencer
{
public:
    Sequencer();
    ~Sequencer();

    void reset();
    void stopAll();

    Patch* createPatch(s16 id);

    Sequence* createSequence();
    Sequence* getSequence(s32 id);

    s32 playNote(s32 patchId, u8 note, float velocity, float pan, u8 pitch, s32 pitchMin, s32 pitchMax);
    void stopNote(s32 mask);

    void playSeq(s32 seqId);
    void stopSeq(s32 seqId);

    static const int voiceCount = 256;
    Voice* voices[voiceCount];

    void tickSequence();


private:
    std::thread* thread;
    bool running;
    void loop();
    EventRing eventRing;

    s32 uuid = 1;
    s32 nextUuid();
    
    Voice* obtainVoice();
    void releaseVoice(Voice* v);

    void tickVoice();
    void syncVoice();

    std::vector<Sequence*> sequences;


    static const int patchCount = 128;
    Patch* patches[patchCount];
};

static Sequencer* gseq;
void SDLCallback(void* udata, Uint8* stream, int len);

} // namespace