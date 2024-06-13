#pragma once

#include "Interpolation.hpp"
#include <array>

namespace SPU {

u32 mask(u32 num)
{
    u32 res = 0;
    while (num-- > 0)
    {
        res = (res << 1) | 1;
    }
    return res;
}

///////////////////////
// DUCKSTATION
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

u32 VoiceCounterDuckstation::CalculateNoteStep(s32 root, s32 rootFine, u32 srcRate, u8 note, u32 pitch)
{
    // NOTE CALC FROM VgmTrans
    // This code seems to be producing the same adpcm_sample_rate values
    // as duckstation. Believe it or not, this code was found on pastebin
    // by searching for "SsPitchFromNote" - I think this is from VGMTrans?
    // https://pastebin.com/aq7wxDdr

    // NOTE: The original does not always produce correct pitches
    // It creates values that go beyond _svm_ptable bounds; Examples
    // 1. SecurityOrbs produce indexes >_svm_ptable.size (199, limit is 192)
    // 2. Leaves coming out of wells produce indexes below 0 (-104, limit is 0)

    u32 pitchA;
    s16 calc, type;
    s32 add, sfine;
    s32 fine = pitch;
    sfine = fine + ((s32) rootFine);
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

    calc = (s16) (add + (note - (((s32) root) - 60))); //((center + 60) - note) + add;
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

    // step seems to be calculated for 8000hz samples hence the division
    // of the samples srcRate. Example, Abe's "Hello" is 22050hz instead of 8000hz.
    return (u16) ((srcRate / 8000.0) * double(ret));
}

u32 VoiceCounterDuckstation::InterpolationIndex()
{
    return (bits >> 4) & mask(8);
}

u32 VoiceCounterDuckstation::SampleIndex()
{
    return (bits >> 12) & mask(5);
}

bool VoiceCounterDuckstation::NextSampleBlock(u32 step)
{
    step = std::min<u32>(step, 0x3FFF);
    bits += step;
    if (this->SampleIndex() >= NUM_SAMPLES_PER_ADPCM_BLOCK)
    {
        bits -= (NUM_SAMPLES_PER_ADPCM_BLOCK << 12);
        return true;
    }

    return false;
}

f32 VoiceCounterDuckstation::Sample(s16* block)
{
    // the interpolation index is based on the source files sample rate
    const u32 i = InterpolationIndex();
    const u32 s = SampleIndex();

    // interpolate on the 4 most recent samples from current position
    s32 out = 0;
    out += s32(gauss[0x0FF - i]) * s32(block[(int) (s + NUM_SAMPLES_FROM_LAST_ADPCM_BLOCK - 3)]);
    out += s32(gauss[0x1FF - i]) * s32(block[(int) (s + NUM_SAMPLES_FROM_LAST_ADPCM_BLOCK - 2)]);
    out += s32(gauss[0x100 + i]) * s32(block[(int) (s + NUM_SAMPLES_FROM_LAST_ADPCM_BLOCK - 1)]);
    out += s32(gauss[0x000 + i]) * s32(block[(int) (s + NUM_SAMPLES_FROM_LAST_ADPCM_BLOCK - 0)]);
    out = out >> 15;

    // vounter.bits has two purposes
    // 1. change how samples are skipped to change the samples note
    // 2. maintain gauss table positioning for correct interpolation
    return (f32) out;
}

void VoiceCounterDuckstation::Reset()
{
    bits = 0;
}


///////////////////////
// Illeprih's magic

// Interpolation window is centered and first 3 samples are from the previous block,
// so the first block has to start at index 2.
static const u32 IlleprihFinePitchResolution = 128;
static const s32 IlleprihSampleRateCount = IlleprihFinePitchResolution * 12;

static const u32 IlleprihInterpolationBitDept = 9;
static const u32 IlleprihInterpolationWindowLen = 1 << IlleprihInterpolationBitDept;

static const u32 IlleprihSampleRateBitDepth = 26;
static const u32 IlleprihBaseSampleRate = 1 << IlleprihSampleRateBitDepth;

static const u32 IlleprihStartOffset = 2 << IlleprihInterpolationBitDept;
static const u32 IlleprihInterpolationAnd = (1 << IlleprihInterpolationBitDept) - 1;
static const u32 IlleprihInterpolationShift = IlleprihSampleRateBitDepth - IlleprihInterpolationBitDept;

// interpolation weights
constexpr std::array<std::array<f32, IlleprihInterpolationWindowLen + 1>, 2> GenerateInterpolationWeights()
{
    std::array<std::array<f32, IlleprihInterpolationWindowLen + 1>, 2> weights = {};
    for (size_t i = 0; i < IlleprihInterpolationWindowLen + 1; i++)
    {
        double pow1 = i / (double) IlleprihInterpolationWindowLen;
        double pow2 = pow1 * pow1;
        double pow3 = pow2 * pow1;

        weights[0][i] = (f32) (0.5f * (-pow3 + 2 * pow2 - pow1));
        weights[1][i] = (f32) (0.5f * (3 * pow3 - 5 * pow2 + 2));
    }
    return weights;
}
std::array<std::array<f32, IlleprihInterpolationWindowLen + 1>, 2> interpWeights = GenerateInterpolationWeights();

// sample rate
constexpr std::array<u32, IlleprihSampleRateCount> GenerateSampleRates()
{
    std::array<u32, IlleprihSampleRateCount> sampleRates = {};
    for (size_t i = 0; i < IlleprihSampleRateCount; i++)
    {
        sampleRates[i] = (u32) std::round(IlleprihBaseSampleRate * std::pow(2, i / (double) IlleprihSampleRateCount));
    }
    return sampleRates;
}
const std::array<u32, IlleprihSampleRateCount> SampleRates = GenerateSampleRates();

u32 VoiceCounterIlleprih::CalculateNoteStep(s32 root, s32 rootFine, u32 srcRate, u8 note, u32 pitch)
{
    f32 fix = srcRate / 8000.0f;
    fix; // TODO - this needs to be used somewhere. PC version has some non 8000hz samples

    s32 fineOffset = (note - root) * IlleprihFinePitchResolution + rootFine + (pitch - 64) * 2;

    u32 octaveOffset;
    u32 sampleRateOffset;
    if (fineOffset >= 0)
    {
        octaveOffset = fineOffset / IlleprihSampleRateCount;
        sampleRateOffset = fineOffset - octaveOffset * IlleprihSampleRateCount;
        return SampleRates[sampleRateOffset] << octaveOffset;
    }

    octaveOffset = (fineOffset + 1) / -IlleprihSampleRateCount + 1;
    sampleRateOffset = fineOffset + octaveOffset * IlleprihSampleRateCount;
    return SampleRates[sampleRateOffset] >> octaveOffset;

}

u32 VoiceCounterIlleprih::SampleIndex()
{
    return this->Counter >> IlleprihSampleRateBitDepth;
}

u32 VoiceCounterIlleprih::InterpolationIndex()
{
    return (this->Counter >> IlleprihInterpolationShift) & IlleprihInterpolationAnd;
}

bool VoiceCounterIlleprih::NextSampleBlock(u32 sampleRate)
{
    this->Counter += sampleRate;

    if (this->SampleIndex() >= 28)
    {
        this->Counter -= (28 << IlleprihSampleRateBitDepth);
        return true;
    }

    return false;
}

f32 VoiceCounterIlleprih::Sample(s16* block)
{
    const u32 i = InterpolationIndex();
    const u32 s = SampleIndex();
    return interpWeights[0][i] * block[NUM_SAMPLES_FROM_LAST_ADPCM_BLOCK - 3 + s]
            + interpWeights[1][i] * block[NUM_SAMPLES_FROM_LAST_ADPCM_BLOCK - 3 + s + 1]
            + interpWeights[1][IlleprihInterpolationWindowLen - i] * block[NUM_SAMPLES_FROM_LAST_ADPCM_BLOCK - 3 + s + 2]
            + interpWeights[0][IlleprihInterpolationWindowLen - i] * block[NUM_SAMPLES_FROM_LAST_ADPCM_BLOCK - 3 + s + 3];
}

void VoiceCounterIlleprih::Reset()
{
    this->Counter = IlleprihStartOffset;
}


} // namespace
