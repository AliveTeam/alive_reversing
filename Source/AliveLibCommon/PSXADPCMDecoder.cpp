#include "PSXADPCMDecoder.h"
#include "logger.hpp"
#include <algorithm>
#include "Types.hpp"

// Pos / neg Tables
const short pos_adpcm_table[5] = { 0, +60, +115, +98, +122 };
const short neg_adpcm_table[5] = { 0, 0, -52, -55, -60 };

static s8 Signed4bit(u8 number)
{
    if ((number & 0x8) == 0x8)
    {
        return (number & 0x7) - 8;
    }
    else
    {
        return number;
    }
}

static int MinMax(int number, int min, int max)
{
    if (number < min)
    {
        return min;
    }

    if (number > max)
    {
        return max;
    }

    return number;
}

template<class T>
static void DecodeBlock(
    T& out,
    const u8(&samples)[112],
    const u8(&parameters)[16],
    u8 block,
    u8 nibble,
    short& dst,
    f64& old,
    f64& older)
{
    // 4 blocks for each nibble, so 8 blocks total
    const int shift = (u8)(12 - (parameters[4 + block * 2 + nibble] & 0xF));
    const int filter = (s8)((parameters[4 + block * 2 + nibble] & 0x30) >> 4);

    const int f0 = pos_adpcm_table[filter];
    const int f1 = neg_adpcm_table[filter];

    for (int d = 0; d < 28; d++)
    {
        const u8 value = samples[block + d * 4];
        const int t = Signed4bit((u8)((value >> (nibble * 4)) & 0xF));
        int s = (short)((t << shift) + ((old * f0 + older * f1 + 32) / 64));
        s = static_cast<u16>(MinMax(s, -32768, 32767));


        out[dst] = static_cast<short>(s);
        dst += 2;

        older = old;
        old = static_cast<short>(s);
    }
}

template<class T>
static void Decode(const PSXADPCMDecoder::SoundFrame& sf, T& out)
{
    short dstLeft = 0;
    static f64 oldLeft = 0;
    static f64 olderLeft = 0;

    short dstRight = 1;
    static f64 oldRight = 0;
    static f64 olderRight = 0;

    for (int i = 0; i < 18; i++)
    {
        const PSXADPCMDecoder::SoundFrame::SoundGroup& sg = sf.sound_groups[i];
        for (u8 b = 0; b < 4; b++)
        {
            DecodeBlock(out, sg.audio_sample_bytes, sg.sound_parameters, b, 1, dstLeft, oldLeft, olderLeft);
            DecodeBlock(out, sg.audio_sample_bytes, sg.sound_parameters, b, 0, dstRight, oldRight, olderRight);
        }
    }
}

void PSXADPCMDecoder::DecodeFrameToPCM(std::vector<s16>& out, uint8_t* arg_adpcm_frame)
{
    const PSXADPCMDecoder::SoundFrame* sf = reinterpret_cast<const PSXADPCMDecoder::SoundFrame*>(arg_adpcm_frame);
    Decode(*sf, out);
}

void PSXADPCMDecoder::DecodeFrameToPCM(std::array<s16, 4032>& out, uint8_t* arg_adpcm_frame)
{
    const PSXADPCMDecoder::SoundFrame* sf = reinterpret_cast<const PSXADPCMDecoder::SoundFrame*>(arg_adpcm_frame);
    Decode(*sf, out);
}