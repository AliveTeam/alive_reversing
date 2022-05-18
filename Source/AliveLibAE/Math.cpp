#include "stdafx.h"
#include "Math.hpp"
#include "Function.hpp"
#include "FixedPoint.hpp"
#include "GameAutoPlayer.hpp"
#include <gmock/gmock.h>

void Math_ForceLink()
{ }

// clang-format off
ALIVE_ARY(1, 0x546744, u8, 256, sRandomBytes_546744,
{
    0x35, 0x85, 0x49, 0xE2, 0xA7, 0x42, 0xDF, 0x0B, 0x2D, 0x23, 0xDD, 0xDE, 0x1F, 0x17, 0xBB, 0xCF,
    0x4E, 0xA3, 0x19, 0x04, 0x71, 0x12, 0xB5, 0x50, 0x43, 0x64, 0xA0, 0x15, 0xDB, 0x22, 0xB0, 0x83,
    0x39, 0xEA, 0xAF, 0xC3, 0xD0, 0xCE, 0x77, 0x14, 0xAD, 0x56, 0x80, 0x5F, 0x6E, 0xD2, 0xD9, 0xC0,
    0xE6, 0xF6, 0x70, 0xF9, 0x05, 0x5A, 0x33, 0xC5, 0x8C, 0x73, 0xCB, 0xFA, 0x81, 0x3E, 0xD8, 0x9E,
    0x26, 0xD6, 0x0C, 0xBA, 0xAA, 0xCD, 0x7E, 0x9D, 0xFF, 0x1D, 0x06, 0xC4, 0xED, 0xF2, 0xF4, 0x5B,
    0x94, 0x9B, 0xA1, 0x5E, 0xB8, 0x37, 0xC1, 0xF1, 0x57, 0x7B, 0xD7, 0xFB, 0x25, 0xCC, 0x91, 0xF0,
    0x62, 0x7F, 0xFC, 0x1A, 0x96, 0x72, 0x2F, 0xDA, 0x38, 0xA2, 0x3A, 0xBF, 0xB4, 0xB1, 0xE8, 0xBD,
    0x0F, 0xF7, 0xAE, 0xA6, 0x88, 0x74, 0x2C, 0x7D, 0x01, 0xEC, 0x07, 0x24, 0x40, 0x34, 0x5D, 0x59,
    0x9C, 0x7A, 0x9A, 0xEE, 0xE7, 0x46, 0x9F, 0x61, 0x63, 0x30, 0xB2, 0x97, 0xEF, 0xAC, 0x76, 0x8E,
    0x75, 0xE4, 0xD3, 0xA9, 0x2A, 0x41, 0x00, 0xA5, 0xBC, 0x66, 0x51, 0xCA, 0x1B, 0xB7, 0x7C, 0x0E,
    0x18, 0x6B, 0xC7, 0x78, 0x84, 0x6A, 0x6C, 0x82, 0x60, 0xD5, 0x1C, 0x13, 0x55, 0x52, 0xB9, 0x53,
    0x32, 0x1E, 0xB6, 0x28, 0x4B, 0x8F, 0x11, 0x8D, 0x8B, 0xFD, 0x10, 0x67, 0x3F, 0xD1, 0x36, 0x45,
    0x86, 0xC9, 0x4A, 0x54, 0x4F, 0xF8, 0x79, 0x29, 0x69, 0x08, 0xE9, 0x89, 0x20, 0xAB, 0x6D, 0xE3,
    0xC6, 0x98, 0x99, 0xE5, 0x93, 0x48, 0x09, 0xE1, 0xF3, 0x47, 0x4C, 0xFE, 0x8A, 0x95, 0x3C, 0xEB,
    0x2B, 0x03, 0xF5, 0xA8, 0x58, 0x3D, 0xC2, 0x31, 0x65, 0xDC, 0x27, 0xBE, 0x21, 0x68, 0xE0, 0xB3,
    0xC8, 0xA4, 0x02, 0x2E, 0xD4, 0x3B, 0x6F, 0x5C, 0x87, 0x0A, 0x92, 0x0D, 0x4D, 0x16, 0x44, 0x90,
});
// clang-format on

ALIVE_VAR(1, 0x5D1E10, u8, sRandomSeed_5D1E10, 0);

EXPORT u32 CC Math_FixedPoint_Multiply_496C50(s32 op1, s32 op2)
{
    u32 op1a;   // ecx
    u32 op2a;   // esi
    s32 result; // eax

    op1a = op1;
    if (op1 < 0)
        op1a = -op1;
    op2a = op2;
    if (op2 < 0)
        op2a = -op2;
    result = op2a * (op1a >> 16)
           + (u16) op1a * (op2a >> 16)
           + ((u16) op1a * (u32)(u16) op2a >> 16);
    if ((op1 < 0) != (op2 < 0))
        result = -result;
    return result;
}

EXPORT u32 CC Math_FixedPoint_Divide_496B70(s32 op1, s32 op2)
{
    s32 op1a;   // ebx
    u32 op1b;   // edi
    u32 op2a;   // ecx
    u32 v5;     // esi
    s32 v6;     // edi
    s32 result; // eax

    op1a = op1;
    op1b = op1;
    if (op1 < 0)
        op1b = -op1;
    op2a = op2;
    if (op2 < 0)
        op2a = -op2;
    v5 = op1b % op2a;
    v6 = op1b / op2a << 16;
    if (op2a >= 0x1000000)
    {
        result = v5 % (op2a >> 8) / (op2a >> 16) + v6 + (v5 / (op2a >> 8) << 8);
        op1a = op1;
    }
    else
    {
        result = ((v5 << 8) % op2a << 8) / op2a + v6 + ((v5 << 8) / op2a << 8);
    }
    if ((op1a < 0) != (op2 < 0))
        result = -result;
    return result;
}

EXPORT s16 CC Math_RandomRange_496AB0(s16 min, s16 max)
{
    s16 tempMax = max;
    s16 tempMin = min;

    if (max >= min)
    {
        if (max == min)
        {
            // No need for random if min and max are the same.
            return max;
        }
    }
    else
    {
        // Whoops, silly developer, you've swapped the min and max values! Let me fix that for you!
        tempMax = min;
        tempMin = max;
    }

    s16 result = tempMin;
    const s32 rangeSize = tempMax - tempMin;

    if (rangeSize >= 256)
    {
        const s32 randByte = (257 * Math_NextRandom());
        sRandomSeed_5D1E10 += 1;
        result = static_cast<s16>(result + randByte % (rangeSize + 1));
    }
    else
    {
        result += Math_NextRandom() % (rangeSize + 1);
    }

    return result;
}

// This seems to have been inlined a lot
EXPORT u8 Math_NextRandom()
{
    const u8 random = sRandomBytes_546744[sRandomSeed_5D1E10++];
    return static_cast<u8>(gGameAutoPlayer.Rng(random));
}

const u16 sSineTable_5466C4[64] = {
    0, 1633, 3266, 4897, 6525, 8148, 9767, 11380,
    12985, 14582, 16171, 17749, 19316, 20872, 22414, 23942,
    25456, 26953, 28434, 29897, 31342, 32767, 34172, 35555,
    36917, 38255, 39570, 40860, 42125, 43363, 44575, 45758,
    46914, 48040, 49136, 50202, 51237, 52240, 53210, 54147,
    55051, 55920, 56754, 57554, 58317, 59044, 59735, 60388,
    61004, 61582, 62122, 62623, 63085, 63508, 63891, 64235,
    64539, 64803, 65026, 65209, 65351, 65453, 65514, 65535};

EXPORT FP CC Math_Cosine_496CD0(u8 v)
{
    if (v < 64u)
    {
        return FP_FromRaw(-sSineTable_5466C4[v]);
    }

    if (v < 128u)
    {
        return FP_FromRaw(-sSineTable_5466C4[127 - v]);
    }

    if (v < 192u)
    {
        return FP_FromRaw(sSineTable_5466C4[v - 128]);
    }

    return FP_FromRaw(sSineTable_5466C4[255 - v]);
}

EXPORT FP CC Math_Cosine_496D60(FP fp)
{
    if (fp < FP_FromInteger(0))
    {
        // TODO: Refactor to something sane
        fp.fpValue = ((u32)(0xFFFFFF - fp.fpValue) >> 24 << 24) + fp.fpValue;
    }

    if (fp > FP_FromInteger(255))
    {
        // TODO: Refactor to something sane
        fp.fpValue += 0xFF000000 * ((u32)(fp.fpValue + 0xFFFF) >> 24);
    }

    return Math_Cosine_496CD0(static_cast<u8>(FP_GetExponent(fp)));
}

EXPORT FP CC Math_Sine_496DF0(FP fp)
{
    // May actually be Cosine -> Sine here ?? Damn maths :(
    return Math_Cosine_496D60(fp + FP_FromInteger(64));
}

EXPORT FP CC Math_Sine_496DD0(u8 v)
{
    // TODO: Relies on underflow
    return Math_Cosine_496CD0(v - 64);
}

s32 CC Math_Distance_496EB0(s32 x1, s32 y1, s32 x2, s32 y2)
{
    s32 dx = x1 - x2;
    if (dx < 0)
    {
        dx = x2 - x1;
    }

    s32 dy = y1 - y2;
    if (y1 - y2 < 0)
    {
        dy = y2 - y1;
    }

    if (dx <= dy)
    {
        return dy + dx / 2;
    }
    else
    {
        return dx + dy / 2;
    }
}

EXPORT FP CC Math_Tan_496F70(const FP value1, const FP value2)
{
    FP value1abs = FP_Abs(value1);
    FP value2abs = FP_Abs(value2);
    s32 switchScenario = 0;
    if (value1 < FP_FromInteger(0))
    {
        switchScenario += 4;
    }
    if (value2 < FP_FromInteger(0))
    {
        switchScenario += 2;
    }
    switchScenario += value2abs < value1abs ? 1 : 0;
    FP aux = {};
    switch (switchScenario)
    {
        case 0:
            if (value2 != FP_FromInteger(0))
            {
                aux = value1abs / value2abs;
                return aux * FP_FromInteger(32);
            }
            return FP_FromInteger(0);
        case 1:
            aux = value2abs / value1abs;
            return (FP_FromInteger(2) - aux) * FP_FromInteger(32);
        case 2:
            aux = value1abs / value2abs;
            return (FP_FromInteger(4) - aux) * FP_FromInteger(32);
        case 3:
            aux = value2abs / value1abs;
            return (FP_FromInteger(2) + aux) * FP_FromInteger(32);
        case 4:
            aux = value1abs / value2abs;
            return (FP_FromInteger(8) - aux) * FP_FromInteger(32);
        case 5:
            aux = value2abs / value1abs;
            return (FP_FromInteger(6) + aux) * FP_FromInteger(32);
        case 6:
            aux = value1abs / value2abs;
            return (FP_FromInteger(4) + aux) * FP_FromInteger(32);
            break;
        case 7:
            aux = value2abs / value1abs;
            return (FP_FromInteger(6) - aux) * FP_FromInteger(32);
            break;
        default:
            break;
    }
    return FP_FromInteger(0);
}

namespace AETest::TestsMath {
void Math_Tests()
{
    for (u8 i = 0; i < 64; i++)
    {
        ASSERT_EQ(FP_FromRaw(-sSineTable_5466C4[i]), Math_Cosine_496CD0(i));
    }

    for (u8 i = 65; i < 128; i++)
    {
        ASSERT_EQ(FP_FromRaw(-sSineTable_5466C4[127 - i]), Math_Cosine_496CD0(i));
    }

    for (u8 i = 128; i < 192; i++)
    {
        ASSERT_EQ(FP_FromRaw(sSineTable_5466C4[i - 128]), Math_Cosine_496CD0(i));
    }

    for (u8 i = 192; i < 255; i++)
    {
        ASSERT_EQ(FP_FromRaw(sSineTable_5466C4[255 - i]), Math_Cosine_496CD0(i));
    }
}
} // namespace AETest::TestsMath
