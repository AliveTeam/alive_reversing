#pragma once

#include "Sequencer.hpp"
#include <array>
#include "SDL.h"

namespace sean {

/////////////////////////////////
/// DuckStation

static void ProcessReverb(s16 left_in, s16 right_in, s32* left_out, s32* right_out);
static s16 ReverbRead(u32 address, s32 offset = 0);

u32 mask(u32 num)
{
    u32 res = 0;
    while (num-- > 0)
    {
        res = (res << 1) | 1;
    }
    return res;
}

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

ADSR parseADSR(u16 adsr1, u16 adsr2)
{
    ADSR adsr;
    adsr.sustainLevel = (adsr1) &mask(4);
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

s32 Voice::interpolate()
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

    // the interpolation index is based on the source files sample rate
    const u8 i = (u8)vounter.interpolation_index();
    const u32 s = ((u32)f_SampleOffset) + ZeroExtend32(vounter.sample_index());

    // interpolate on 4 most recent samples
    s32 out = 0;
    if (s == 0)
    {
        if (!isFirstBlock)
        {
            out += s32(gauss[0x0FF - i]) * s32(sample->buffer[sample->len - 3]); // oldest s16 - but should it be s8?
            out += s32(gauss[0x1FF - i]) * s32(sample->buffer[sample->len - 2]); // older
            out += s32(gauss[0x100 + i]) * s32(sample->buffer[sample->len - 1]); // old
        }
        out += s32(gauss[0x000 + i]) * s32(sample->buffer[s - 0]); // new

    }
    else if (s == 1)
    {
        if (!isFirstBlock)
        {
            out += s32(gauss[0x0FF - i]) * s32(sample->buffer[sample->len - 2]); // oldest s16 - but should it be s8?
            out += s32(gauss[0x1FF - i]) * s32(sample->buffer[sample->len - 1]); // older
        }
        out += s32(gauss[0x100 + i]) * s32(sample->buffer[s - 1]); // old
        out += s32(gauss[0x000 + i]) * s32(sample->buffer[s - 0]);           // new
    }
    else if (s == 2)
    {
        if (!isFirstBlock)
        {
            out += s32(gauss[0x0FF - i]) * s32(sample->buffer[sample->len - 1]); // oldest s16 - but should it be s8?
        }
        out += s32(gauss[0x1FF - i]) * s32(sample->buffer[s - 2]); // older
        out += s32(gauss[0x100 + i]) * s32(sample->buffer[s - 1]);           // old
        out += s32(gauss[0x000 + i]) * s32(sample->buffer[s - 0]);           // new
    }
    else
    {
        out += s32(gauss[0x0FF - i]) * s32(sample->buffer[(int) (s ) - 3]); // oldest s16 - but should it be s8?
        out += s32(gauss[0x1FF - i]) * s32(sample->buffer[(int) (s ) - 2]); // older
        out += s32(gauss[0x100 + i]) * s32(sample->buffer[(int) (s ) - 1]); // old
        out += s32(gauss[0x000 + i]) * s32(sample->buffer[(int) (s ) - 0]); // new
    }


    return out >> 15;
}

/// DuckStation END
/////////////////////////////////

Sequencer::Sequencer()
{
    gseq = this;

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
    for (int i = 0; i < voiceCount; i++)
    {
        voices[i] = new Voice();
        voices[i]->id = id;
        id = id << 1;
    }

    for (int i = 0; i < patchCount; i++)
    {
        patches[i] = NULL;
    }

    //running = true;
    //thread = new std::thread(&Sequencer::loop, this);
}

Sequencer::~Sequencer()
{
    //running = false;
    //thread->join();
    //delete thread;

    // Delete buffers (by killing the instrument)
    for (Patch* patch : patches)
    {
        delete patch;
    }
}

u64 timeSinceEpochMillisec()
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void SDLCallback(void* udata, Uint8* stream, int len)
{
    udata;
    stream;
    len;
    gseq->mutex.lock();

    // This runs at 44100hz
    // 1. tick voices
    //      a. interpolate
    //      b. adsr
    //      c. pitch
    // 2. run reverb

    // start/stop any sequence notes
    gseq->tickSequence();

    float* AudioStream = (float*) stream;
    int StreamLength = len / sizeof(float);

    s32 reverb_out_left = 0;
    s32 reverb_out_right = 0;

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
        for (Voice* v : gseq->voices)
        {
            if (!v->sample || v->complete || !v->inUse)
            {
                continue;
            }

            std::tuple<s32, s32> s = v->tick();
            leftSample += std::get<0>(s);
            rightSample += std::get<1>(s);

            // 0 Off
            // 1 Vibrate
            // 2 Portamento
            // 3 1 & 2(Portamento and Vibrate on)
            // 4 Reverb
            if (v->sample->reverb == 4)
            {
                reverb_in_left += std::get<0>(s);
                reverb_in_right += std::get<1>(s);
            }
        }

        // 2. Process and mix in the reverb
        ProcessReverb(
            static_cast<s16>(Clamp16((s32) (reverb_in_left))),
            static_cast<s16>(Clamp16((s32) (reverb_in_right))),
            &reverb_out_left,
            &reverb_out_right
        );

        leftSample += reverb_out_left;
        rightSample += reverb_out_right;

        // make value usable by SDL
        leftSample = leftSample / 32767.0f;
        rightSample = rightSample / 32767.0f;
        SDL_MixAudioFormat((Uint8*) (AudioStream + i), (const Uint8*) &leftSample, AUDIO_F32, sizeof(float), 100);      
        SDL_MixAudioFormat((Uint8*) (AudioStream + i + 1), (const Uint8*) &rightSample, AUDIO_F32, sizeof(float), 100);
    }

    gseq->mutex.unlock();
}

//////////////////////////
// PRIVATE
void Sequencer::reset()
{
    mutex.lock();
    stopAll();
    
    for (s16 i = 0; i < patchCount; i++)
    {

        patches[i] = NULL;
    }
    
    for (Sequence* seq : sequences)
    {
        delete seq;
    }
    sequences.clear();
    mutex.unlock();
}

void Sequencer::stopAll()
{
    for (Sequence* seq : sequences)
    {
        seq->play = false;
    }

    for (s16 i = 0; i < voiceCount; i++)
    {
        releaseVoice(voices[i]);
    }
}

void Sequencer::playSeq(s32 seqId)
{
    mutex.lock();
    for (Sequence* seq : sequences)
    {
        if (seq->id == seqId)
        {
            seq->repeats = 0;
            seq->play = true;
        }
    }
    mutex.unlock();
}


void Sequencer::stopSeq(s32 seqId)
{
    mutex.lock();
    stopSeqSafe(seqId);
    mutex.unlock();
}

void Sequencer::stopSeqSafe(s32 seqId) 
{
    for (Sequence* seq : sequences)
    {
        if (seq->id == seqId)
        {
            seq->play = false;
            for (Voice* v : voices)
            {
                if (v && v->sequence == seq)
                {
                    v->offTime = timeSinceEpochMillisec();
                }
            }
        }
    }
}

void Sequencer::tickSequence()
{
    u64 now = timeSinceEpochMillisec();

    // Tick sequences
    for (Sequence* seq : sequences)
    {
        if (!seq || !seq->play)
        {
            continue;
        }

        if (seq->repeats > seq->repeatLimit)
        {
            stopSeqSafe(seq->id);
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

                    for (Sample* sample : seq->channels[message->channelId]->patch->samples)
                    {
                        if (!sample)
                        {
                            continue;
                        }

                        if (message->note > sample->maxNote || message->note < sample->minNote)
                        {
                            continue;
                        }

                        Voice* v = obtainVoice(message->note, message->patchId);
                        if (!v)
                        {
                            continue;
                        }

                        s16 right = (s16) (sample->volume);
                        s16 left = right;
                        s16 progPan = (s16) sample->pan;
                        if (progPan < 64)
                        {
                            right = (right * progPan) / 63;
                        }
                        else
                        {
                            left = (left * (127 - progPan)) / 63;
                        }

                        v->sequence = seq;
                        v->patchId = seq->channels[message->channelId]->patch->id;
                        v->channelId = message->channelId;
                        v->pitchMin = 0;
                        v->velocity = message->velocity;
                        v->note = message->note;
                        v->sample = sample;
                        v->voll = left;
                        v->reuse = true;
                        v->volr = right;
                        v->pan = 0; // divide by 63 for sample
                        v->loop = sample->adsr.attackRate > 0;  // attack is greater than sample length? sample->adsr.attackRate / 1000 > 1;
                    }

                    break;
                }
                case NOTE_OFF:
                {
                    for (Voice* v : voices)
                    {
                        if (v && v->sequence == seq && v->note == message->note)
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
                    // repeats are handled in the sequence when messages starts at position 0 again
                    break;
                }
                case PITCH_BEND:
                {
                    for (int i = 0; i < voiceCount; i++)
                    {
                        if (voices[i]->inUse && voices[i]->channelId == message->channelId)
                        {
                            voices[i]->pitch = message->bend;
                        }
                    }
                    break;
                }
            }
        }
    }
}

void Sequencer::tickVoice()
{
    for (int c = 0; c < voiceCount; c++)
    {
        Voice* voice = voices[c];
        if (!voice->inUse)
        {
            continue;
        }

         voice->tick();
    }
}

std::tuple<s32, s32> Voice::tick()
{
    if (!sample)
    {
        return std::make_tuple(0, 0);
    }

    // INTERPOLATION
    s32 notePitch = pitch < pitchMin ? pitchMin : pitch; // or sample?
    u8 rootNote = sample->rootNote;
    u8 rootPitch = sample->rootNotePitchShift;

    // frequencies are stable for duration of note? - move elsewhere to cut down on maths?
    float noteFreq = float(pow(2.0, float(note + (notePitch / 127.0f)) / 12.0f));
    float rootFreq = float(pow(2.0, float(rootNote + (rootPitch / 127.0f)) / 12.0f));
    //float freq = (float) ((noteFreq / rootFreq) * (float(sample->SampleRate) / 44100.0f));
    //f_SampleOffset += freq;


    if (vounter.sample_index() >= NUM_SAMPLES_PER_ADPCM_BLOCK)
    {
        // shift << 12 as sample_index is shifted over 12
        vounter.bits -= (NUM_SAMPLES_PER_ADPCM_BLOCK << 12);
        f_SampleOffset += NUM_SAMPLES_PER_ADPCM_BLOCK;
        isFirstBlock = false;
    }


    if ((!loop || !reuse) && f_SampleOffset + NUM_SAMPLES_PER_ADPCM_BLOCK >= sample->len - 1)
    {
        complete = true;
        return std::make_tuple(0, 0);
    }
    if (f_SampleOffset + NUM_SAMPLES_PER_ADPCM_BLOCK >= sample->len)
    {
        // NOTE: This seems like it might be wrong, but it fixes audio popping on loop
        f_SampleOffset = (((int) f_SampleOffset) + NUM_SAMPLES_PER_ADPCM_BLOCK) % sample->len;
    }

    s32 sampleData;
    sampleData = interpolate();

    // duckstation uses this to get the actual sample rate in hz
    // (float(v.regs.adpcm_sample_rate) / 4096.0f) * 44100.0f)

    u16 step = (u16) (((float(sample->SampleRate) * (noteFreq / rootFreq)) / 44100.0f) * 4096.0f);
    //const s32 factor = std::clamp<s32>(sampleData, -0x8000, 0x7FFF) + 0x8000;
    //step = Truncate16(static_cast<u32>((SignExtend32(step) * factor) >> 15));
    step = std::min<u16>(step, 0x3FFF);
    vounter.bits += step;

    //s32 volume;
    //volume = ApplyVolume(sampleData, 1); //  voice.regs.adsr_volume

    // UPDATE ADSR STATE - this probably doesn't need to be done every tick?
    if (adsrPhase == NONE)
    {
        //std::cout << sample->adsr.attackRate << " " << velocity << std::endl;
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
        return std::make_tuple(0, 0);
    }

    // UPDATE TICK STATE
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

    s32 vol = ApplyVolume(sampleData, adsrCurrentLevel);
    vol = ApplyVolume(vol, (s16) (sample->volume * 129 * 2));
    vol = ApplyVolume(vol, (s16) (velocity * 129 * 2));

    s32 left = ApplyVolume(vol, voll * 129 * 2);
    s32 right = ApplyVolume(vol, volr * 129 * 2);

    if (sequence)
    {
        left = ApplyVolume(left, sequence->voll * 129 * 2);
        right = ApplyVolume(right, sequence->volr * 129 * 2);
    }

    // TODO - apply volume sweeps.tick()

    return std::make_tuple(left, right);
}

Voice* Sequencer::obtainVoice(u8 note, u8 patchId)
{
    note;
    patchId;

    Voice* available;
    available = NULL;
    for (int i = 0; i < voiceCount; i++)
    {
        Voice* v = voices[i];
        if (v->reuse && v->note == note && v->patchId == patchId)
        {
            std::cout << "got prev " << (int) note << " " << v->sample << std::endl;
            v->f_SampleOffset = 0;
            v->vounter.bits = 0;
            v->inUse = true;
            return v;
            //mutex.unlock();
        }

        if (v->complete)
        {
            releaseVoice(v);
        }

        if (!v->inUse)
        {
            v->inUse = true;
            std::cout << "return " << (int) note << " " << (int) patchId << std::endl;
            return v;
        }
    }
    return NULL;
}

void Sequencer::releaseVoice(Voice* v)
{
    v->offTime = 0;
    v->pitch = 0;
    v->adsrPhase = NONE;
    v->adsrCounter = 0;
    v->adsrCurrentLevel = 0;
    v->adsrTargetLevel = MAX_VOLUME;
    v->sequence = NULL;
    v->loop = false;
    v->pan = 0;
    v->f_SampleOffset = 0;
    v->vounter.bits = 0;
    v->complete = false;
    v->velocity = 127;
    v->voll = 127;
    v->volr = 127;
    v->complete = false;
    v->isFirstBlock = true;
    v->inUse = false;
}

//////////////////////////
// PUBLIC
Patch* Sequencer::createPatch(s16 id)
{
    if (patches[id])
    {
        return patches[id];
    }

    patches[id] = new Patch();
    patches[id]->id = (u8)id;
    return patches[id];
}

Sequence* Sequencer::createSequence()
{
    mutex.lock();
    Sequence* seq = new Sequence();
    sequences.push_back(seq);
    mutex.unlock();
    return seq;
}

Sequence* Sequencer::getSequence(s32 id)
{
    //mutex.lock();
    Sequence* s;
    s = NULL;
    for (Sequence* seq : sequences)
    {
        if (seq->id == id)
        {
            s = seq;
            break;
        }
    }
    //mutex.unlock();
    return s;
}

s32 Sequencer::playNote(s32 patchId, u8 note, s16 voll, s16 volr, u8 pitch, s32 pitchMin, s32 pitchMax, bool reuse)
{    

    // TODO - 
    // - SoundEfx: apparently use sweeps and reuse the voice register (slig walking offscreen)
    // - OneShots: do not reuse voice regester (slig turning) 
    // - Notes   : reuese register if possible (chanting)

    mutex.lock();
    Patch* patch = patches[patchId];
    if (!patch)
    {
        mutex.unlock();
        return 0;
    }

    int ids = 0;
    for (Sample* s : patch->samples)
    {
        if (!s)
        {
            continue;
        }

        if (note > s->maxNote || note < s->minNote)
        {
            continue;
        }

        Voice* v = obtainVoice(note, (u8) patchId);
        if (!v)
        {
            mutex.unlock();
            return 0;
        }

        v->patchId = (u8) patchId;
        v->note = note;
        v->velocity = 127;
        //v->pan = pan;
        v->voll = voll;
        v->volr = volr;
        v->reuse = reuse;
        v->pitch = pitch;
        v->pitchMin = pitchMin;
        v->pitchMax = pitchMax;
        v->sample = s;
        ids |= v->id;
    }

    mutex.unlock();
    return ids;
}

void Sequencer::stopNote(s32 mask)
{
    mutex.lock();
    for (int i = 0; i < voiceCount; i++)
    {
        if ((voices[i]->id & mask) != 0)
        {
            // TODO - not sure if this is right.
            // Maybe it should trigger a release?
            releaseVoice(voices[i]);
        }
    }
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


////////////////////////////
// REVERB
////////////////////////////

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
    
    // these values seem to be stable in duckstation
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