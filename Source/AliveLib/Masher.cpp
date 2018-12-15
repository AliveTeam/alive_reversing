#include "stdafx.h"
#include "Masher.hpp"
#include "Function.hpp"
#include "masher_tables.hpp"
#include "Io.hpp"
#include <array>


AudioDecompressor::AudioDecompressor()
{
    init_Snd_tbl();
}

/*static*/ s32 AudioDecompressor::GetSoundTableValue(s16 tblIndex)
{
    const s32 positiveTblIdx = static_cast<s32>(abs(tblIndex));
    const u32 shiftedIdx = (positiveTblIdx >> 7) & 0xFF;
    s32 result = (u16)((s16)gSndTbl_byte_62EEB0[shiftedIdx] << 7) | (u16)(positiveTblIdx >> gSndTbl_byte_62EEB0[shiftedIdx]);
    if (tblIndex < 0)
    {
        result = -result;
    }
    return result;
}

s16 AudioDecompressor::sub_408F50(s16 sample)
{
    s32 absSample = static_cast<s32>(abs(sample));
    s32 sampleBits = absSample >> 7;
    s32 sampleMasked = absSample & 0x7F;

    s16 result = (u16)(sampleMasked << sampleBits);
    if (sampleBits >= 2)
    {
        result |= (u16)(1 << (sampleBits - 2));
    }

    if (sample < 0)
    {
        result = -result;
    }
    return result;
}

s32 AudioDecompressor::ReadNextAudioWord(s32 value)
{
    if (mUsedBits <= 16)
    {
        const int srcVal = *mAudioFrameDataPtr;
        ++mAudioFrameDataPtr;
        value |= srcVal << mUsedBits;
        mUsedBits += 16;
    }
    return value;
}

s32 AudioDecompressor::SndRelated_sub_409650()
{
    const s32 numBits = mUsedBits & 7;
    mUsedBits -= numBits;
    mWorkBits >>= numBits;
    mWorkBits = ReadNextAudioWord(mWorkBits);
    return mUsedBits;
}

s16 AudioDecompressor::NextSoundBits(u16 numBits)
{
    mUsedBits -= numBits;
    const s16 ret = static_cast<s16>(mWorkBits & ((1 << numBits) - 1));
    mWorkBits >>= numBits;
    mWorkBits = ReadNextAudioWord(mWorkBits);
    return ret;
}

bool AudioDecompressor::SampleMatches(s16& sample, s16 bitNum)
{
    const s32 bitMask = 1 << (bitNum - 1);
    if (sample != bitMask)
    {
        if (sample & bitMask)
        {
            sample = -(sample & ~bitMask);
        }
        return true;
    }
    return false;
}

template<class T>
void AudioDecompressor::decode_generic(T* outPtr, s32 numSamplesPerFrame, bool isLast)
{
    const s16 useTableFlag = NextSoundBits(16);
    const s16 firstWord = NextSoundBits(16);
    const s16 secondWord = NextSoundBits(16);
    const s16 thirdWord = NextSoundBits(16);

    const s16 previous1 = NextSoundBits(16);
    s32 previousValue1 = static_cast<s16>(previous1);

    *outPtr = static_cast<T>(previous1);
    outPtr += mAudioNumChannels;

    const s16 previous2 = NextSoundBits(16);
    s32 previousValue2 = static_cast<s16>(previous2);

    *outPtr = static_cast<T>(previous2);
    outPtr += mAudioNumChannels;

    const s16 previous3 = NextSoundBits(16);
    s32 previousValue3 = static_cast<s16>(previous3);

    *outPtr = static_cast<T>(previous3);
    outPtr += mAudioNumChannels;

    if (numSamplesPerFrame > 3)
    {
        for (s32 counter = 0; counter < numSamplesPerFrame - 3; counter++)
        {
            s16 samplePart = 0;
            do
            {
                samplePart = NextSoundBits(firstWord);
                if (SampleMatches(samplePart, firstWord))
                {
                    break;
                }

                samplePart = NextSoundBits(secondWord);
                if (SampleMatches(samplePart, secondWord))
                {
                    break;
                }

                samplePart = NextSoundBits(thirdWord);
                if (SampleMatches(samplePart, thirdWord))
                {
                    break;
                }

            } while (false);

            const s32 previous = (5 * previousValue3) - (4 * previousValue2);
            const s32 samplePartOrTableIndex = (previousValue1 + previous) >> 1;

            previousValue1 = previousValue2;
            previousValue2 = previousValue3;

            const bool bUseTbl = useTableFlag != 0;
            if (bUseTbl)
            {
                const s32 soundTableValue = GetSoundTableValue(static_cast<s16>(samplePartOrTableIndex));
                previousValue3 = sub_408F50(static_cast<s16>(samplePart + soundTableValue));
            }
            else
            {
                // TODO: Case never hit for any known data?
                previousValue3 = static_cast<s16>(samplePartOrTableIndex + samplePart);
            }

            *outPtr = static_cast<T>(previousValue3); // int to word
            outPtr += mAudioNumChannels;
        }
    }

    if (!isLast)
    {
        SndRelated_sub_409650();
    }
}

void AudioDecompressor::decode_8bit_audio_frame(u8* outPtr, s32 numSamplesPerFrame, bool isLast)
{
    decode_generic(outPtr, numSamplesPerFrame, isLast);
}

void AudioDecompressor::decode_16bit_audio_frame(u16* outPtr, s32 numSamplesPerFrame, bool isLast)
{
    decode_generic(outPtr, numSamplesPerFrame, isLast);
}

u16* AudioDecompressor::SetupAudioDecodePtrs(u16 *rawFrameBuffer)
{
    mAudioFrameDataPtr = rawFrameBuffer;
    mWorkBits = *(u32 *)mAudioFrameDataPtr;
    mAudioFrameDataPtr = mAudioFrameDataPtr + 2;
    mUsedBits = 32;
    return mAudioFrameDataPtr;
}

void AudioDecompressor::SetChannelCount(s32 numChannels)
{
    mAudioNumChannels = numChannels;
}

/*static*/ void AudioDecompressor::init_Snd_tbl()
{
    static bool done = false;
    if (!done)
    {
        done = true;
        int index = 0;
        do
        {
            int tableValue = 0;
            for (int i = index; i > 0; ++tableValue)
            {
                i >>= 1;
            }
            gSndTbl_byte_62EEB0[index++] = static_cast<u8>(tableValue);
        } while (index < 256);
    }
}

/*static*/ u8 AudioDecompressor::gSndTbl_byte_62EEB0[256];

bool IsPowerOf2(int i)
{
    return !(i & (i - 1));
}

DWORD RoundUpPowerOf2(DWORD numToRound, int multiple)
{
    assert(multiple && IsPowerOf2(multiple));
    return (numToRound + multiple - 1) & -multiple;
}

const int kMacroBlockWidth = 16;
const int kMacroBlockHeight = 16;


static u16 GetHiWord(u32 v)
{
    return static_cast<u16>((v >> 16) & 0xFFFF);
}

#ifndef MAKELONG
#define MAKELONG(a, b)      ((((u16)(((a)) & 0xffff)) | ((u32)((u16)(((b)) & 0xffff))) << 16))
#endif

static void SetLoWord(u32& v, u16 lo)
{
    u16 hiWord = GetHiWord(v);
    v = MAKELONG(lo, hiWord);
}

static void SetHiWord(u32& v, u16 hi)
{
    u16 loWord = v & 0xFFFF;
    v = MAKELONG(loWord, hi);
}

static u32 ExtractBits(u32 value, u32 numBits)
{
    return value >> (32 - numBits);
}

static void SkipBits(u32& value, char numBits, char& usedBitCount)
{
    value = value << numBits;
    usedBitCount += numBits;
}

static inline void GetBits(char& usedBitCount, u16*& rawBitStreamPtr, u32& rawWord4, u32& workBits)
{
    // I think this is used as an escape code?
    if (usedBitCount & 16)   // 0b10000 if bit 5 set
    {
        usedBitCount &= 15;
        rawWord4 = *rawBitStreamPtr++ << usedBitCount;
        workBits |= rawWord4;
    }
}

static inline void OutputWordAndAdvance(u16*& rawBitStreamPtr, u32& rawWord4, unsigned short int*& pOut, char& usedBitCount, u32& workBits)
{
    *pOut++ = workBits >> (32 - 16);

    rawWord4 = *rawBitStreamPtr++ << usedBitCount;
    workBits = rawWord4 | (workBits << 16);
}

#define MASK_11_BITS 0x7FF
#define MASK_10_BITS 0x3FF
#define MASK_13_BITS 0x1FFF
#define MDEC_END 0xFE00u

static int decode_bitstream(u16* pFrameData, unsigned short int* pOutput)
{

    unsigned int table_index_2 = 0;
    int ret = *pFrameData;


    u32 workBits = ((pFrameData[2] << 16) | (pFrameData[1]));
    workBits = (workBits << 16) | (workBits >> 16); // Swap words 0xff9a005f

    u32 rawWord4 = ExtractBits(workBits, 11);

    char usedBitCount = 0;
    SkipBits(workBits, 11, usedBitCount);

    *pOutput++ = static_cast<unsigned short>(rawWord4); // store in output 0x000007fc

    u16* rawBitStreamPtr = (pFrameData + 3); // 0x7f40
    while (1)
    {
        do
        {
            while (1)
            {
                do
                {
                    while (1)
                    {
                        do
                        {
                            while (1)
                            {
                                while (1)
                                {
                                    table_index_2 = ExtractBits(workBits, 13); // 0x1FFF / 8191 table size? 8192/8=1024 entries?
                                    if (table_index_2 >= 32)
                                    {
                                        break;
                                    }
                                    const int table_index_1 = ExtractBits(workBits, 17); // 0x1FFFF / 131072, 131072/4=32768 entries?

                                    SkipBits(workBits, 8, usedBitCount);

                                    GetBits(usedBitCount, rawBitStreamPtr, rawWord4, workBits);


                                    const char bitsToShiftFromTbl = gTbl1[table_index_1].mBitsToShift;

                                    SkipBits(workBits, bitsToShiftFromTbl, usedBitCount);

                                    GetBits(usedBitCount, rawBitStreamPtr, rawWord4, workBits);

                                    // Everything in the table is 0's after 4266 bytes 4266/2=2133 to perhaps 2048/4096 is max?
                                    *pOutput++ = gTbl1[table_index_1].mOutputWord;

                                } // End while


                                const char tblValueBits = gTbl2[table_index_2].mBitsToShift;

                                SkipBits(workBits, tblValueBits, usedBitCount);

                                GetBits(usedBitCount, rawBitStreamPtr, rawWord4, workBits);

                                SetLoWord(rawWord4, gTbl2[table_index_2].mOutputWord1);

                                if ((u16)rawWord4 != 0x7C1F) // 0b 11111 00000 11111
                                {
                                    break;
                                }

                                OutputWordAndAdvance(rawBitStreamPtr, rawWord4, pOutput, usedBitCount, workBits);
                            } // End while

                            *pOutput++ = static_cast<unsigned short>(rawWord4);

                            if ((u16)rawWord4 == MDEC_END)
                            {
                                const int v15 = ExtractBits(workBits, 11);
                                SkipBits(workBits, 11, usedBitCount);

                                if (v15 == MASK_10_BITS)
                                {
                                    return ret;
                                }

                                rawWord4 = v15 & MASK_11_BITS;
                                *pOutput++ = static_cast<unsigned short>(rawWord4);

                                GetBits(usedBitCount, rawBitStreamPtr, rawWord4, workBits);

                            }

                            SetLoWord(rawWord4, gTbl2[table_index_2].mOutputWord2);
                        } while (!(u16)rawWord4);


                        if ((u16)rawWord4 != 0x7C1F)
                        {
                            break;
                        }

                        OutputWordAndAdvance(rawBitStreamPtr, rawWord4, pOutput, usedBitCount, workBits);
                    } // End while

                    *pOutput++ = static_cast<unsigned short>(rawWord4);

                    if ((u16)rawWord4 == MDEC_END)
                    {
                        const int t11Bits = ExtractBits(workBits, 11);
                        SkipBits(workBits, 11, usedBitCount);

                        if (t11Bits == MASK_10_BITS)
                        {
                            return ret;
                        }

                        rawWord4 = t11Bits & MASK_11_BITS;
                        *pOutput++ = static_cast<unsigned short>(rawWord4);

                        GetBits(usedBitCount, rawBitStreamPtr, rawWord4, workBits);
                    }

                    SetLoWord(rawWord4, gTbl2[table_index_2].mOutputWord3);

                } while (!(u16)rawWord4);


                if ((u16)rawWord4 != 0x7C1F)
                {
                    break;
                }


                OutputWordAndAdvance(rawBitStreamPtr, rawWord4, pOutput, usedBitCount, workBits);
            } // End while

            *pOutput++ = static_cast<unsigned short>(rawWord4);

        } while ((u16)rawWord4 != MDEC_END);

        rawWord4 = ExtractBits(workBits, 11);
        SkipBits(workBits, 11, usedBitCount);

        if (rawWord4 == MASK_10_BITS)
        {
            return ret;
        }

        *pOutput++ = static_cast<unsigned short>(rawWord4);

        GetBits(usedBitCount, rawBitStreamPtr, rawWord4, workBits);

    }

    return ret;
}

const u32 gQuant1_dword_42AEC8[64] =
{
    0x0000000C, 0x0000000B, 0x0000000A, 0x0000000C, 0x0000000E, 0x0000000E, 0x0000000D, 0x0000000E,
    0x00000010, 0x00000018, 0x00000013, 0x00000010, 0x00000011, 0x00000012, 0x00000018, 0x00000016,
    0x00000016, 0x00000018, 0x0000001A, 0x00000028, 0x00000033, 0x0000003A, 0x00000028, 0x0000001D,
    0x00000025, 0x00000023, 0x00000031, 0x00000048, 0x00000040, 0x00000037, 0x00000038, 0x00000033,
    0x00000039, 0x0000003C, 0x0000003D, 0x00000037, 0x00000045, 0x00000057, 0x00000044, 0x00000040,
    0x0000004E, 0x0000005C, 0x0000005F, 0x00000057, 0x00000051, 0x0000006D, 0x00000050, 0x00000038,
    0x0000003E, 0x00000067, 0x00000068, 0x00000067, 0x00000062, 0x00000070, 0x00000079, 0x00000071,
    0x0000004D, 0x0000005C, 0x00000078, 0x00000064, 0x00000067, 0x00000065, 0x00000063, 0x00000010
};

const u32 gQaunt2_dword_42AFC4[64] =
{
    0x00000010, 0x00000012, 0x00000012, 0x00000018, 0x00000015, 0x00000018, 0x0000002F, 0x0000001A,
    0x0000001A, 0x0000002F, 0x00000063, 0x00000042, 0x00000038, 0x00000042, 0x00000063, 0x00000063,
    0x00000063, 0x00000063, 0x00000063, 0x00000063, 0x00000063, 0x00000063, 0x00000063, 0x00000063,
    0x00000063, 0x00000063, 0x00000063, 0x00000063, 0x00000063, 0x00000063, 0x00000063, 0x00000063,
    0x00000063, 0x00000063, 0x00000063, 0x00000063, 0x00000063, 0x00000063, 0x00000063, 0x00000063,
    0x00000063, 0x00000063, 0x00000063, 0x00000063, 0x00000063, 0x00000063, 0x00000063, 0x00000063,
    0x00000063, 0x00000063, 0x00000063, 0x00000063, 0x00000063, 0x00000063, 0x00000063, 0x00000063,
    0x00000063, 0x00000063, 0x00000063, 0x00000063, 0x00000063, 0x00000063, 0x00000063, 0x00000063
};

const u32 g_index_look_up_table[64] =
{
    0x00000001, 0x00000008, 0x00000010, 0x00000009, 0x00000002, 0x00000003, 0x0000000A, 0x00000011,
    0x00000018, 0x00000020, 0x00000019, 0x00000012, 0x0000000B, 0x00000004, 0x00000005, 0x0000000C,
    0x00000013, 0x0000001A, 0x00000021, 0x00000028, 0x00000030, 0x00000029, 0x00000022, 0x0000001B,
    0x00000014, 0x0000000D, 0x00000006, 0x00000007, 0x0000000E, 0x00000015, 0x0000001C, 0x00000023,
    0x0000002A, 0x00000031, 0x00000038, 0x00000039, 0x00000032, 0x0000002B, 0x00000024, 0x0000001D,
    0x00000016, 0x0000000F, 0x00000017, 0x0000001E, 0x00000025, 0x0000002C, 0x00000033, 0x0000003A,
    0x0000003B, 0x00000034, 0x0000002D, 0x00000026, 0x0000001F, 0x00000027, 0x0000002E, 0x00000035,
    0x0000003C, 0x0000003D, 0x00000036, 0x0000002F, 0x00000037, 0x0000003E, 0x0000003F, 0x0000098E // TODO: Last value too large?
};

// Same as PSXMDECDecoder::RL_ZSCAN_MATRIX
const u32 RL_ZSCAN_MATRIX_2[64] =
{
    0x00000000, 0x00000001, 0x00000008, 0x00000010, 0x00000009, 0x00000002, 0x00000003, 0x0000000A,
    0x00000011, 0x00000018, 0x00000020, 0x00000019, 0x00000012, 0x0000000B, 0x00000004, 0x00000005,
    0x0000000C, 0x00000013, 0x0000001A, 0x00000021, 0x00000028, 0x00000030, 0x00000029, 0x00000022,
    0x0000001B, 0x00000014, 0x0000000D, 0x00000006, 0x00000007, 0x0000000E, 0x00000015, 0x0000001C,
    0x00000023, 0x0000002A, 0x00000031, 0x00000038, 0x00000039, 0x00000032, 0x0000002B, 0x00000024,
    0x0000001D, 0x00000016, 0x0000000F, 0x00000017, 0x0000001E, 0x00000025, 0x0000002C, 0x00000033,
    0x0000003A, 0x0000003B, 0x00000034, 0x0000002D, 0x00000026, 0x0000001F, 0x00000027, 0x0000002E,
    0x00000035, 0x0000003C, 0x0000003D, 0x00000036, 0x0000002F, 0x00000037, 0x0000003E, 0x0000003F
};

const u32 g_block_related_2_dword_42B0CC[64] =
{
    0x00000008, 0x00000010, 0x00000009, 0x00000002, 0x00000003, 0x0000000A, 0x00000011, 0x00000018,
    0x00000020, 0x00000019, 0x00000012, 0x0000000B, 0x00000004, 0x00000005, 0x0000000C, 0x00000013,
    0x0000001A, 0x00000021, 0x00000028, 0x00000030, 0x00000029, 0x00000022, 0x0000001B, 0x00000014,
    0x0000000D, 0x00000006, 0x00000007, 0x0000000E, 0x00000015, 0x0000001C, 0x00000023, 0x0000002A,
    0x00000031, 0x00000038, 0x00000039, 0x00000032, 0x0000002B, 0x00000024, 0x0000001D, 0x00000016,
    0x0000000F, 0x00000017, 0x0000001E, 0x00000025, 0x0000002C, 0x00000033, 0x0000003A, 0x0000003B,
    0x00000034, 0x0000002D, 0x00000026, 0x0000001F, 0x00000027, 0x0000002E, 0x00000035, 0x0000003C,
    0x0000003D, 0x00000036, 0x0000002F, 0x00000037, 0x0000003E, 0x0000003F, 0x0000098E, 0x0000098E
};

const u32 g_block_related_3_dword_42B0D0[64] =
{
    0x00000010, 0x00000009, 0x00000002, 0x00000003, 0x0000000A, 0x00000011, 0x00000018, 0x00000020,
    0x00000019, 0x00000012, 0x0000000B, 0x00000004, 0x00000005, 0x0000000C, 0x00000013, 0x0000001A,
    0x00000021, 0x00000028, 0x00000030, 0x00000029, 0x00000022, 0x0000001B, 0x00000014, 0x0000000D,
    0x00000006, 0x00000007, 0x0000000E, 0x00000015, 0x0000001C, 0x00000023, 0x0000002A, 0x00000031,
    0x00000038, 0x00000039, 0x00000032, 0x0000002B, 0x00000024, 0x0000001D, 0x00000016, 0x0000000F,
    0x00000017, 0x0000001E, 0x00000025, 0x0000002C, 0x00000033, 0x0000003A, 0x0000003B, 0x00000034,
    0x0000002D, 0x00000026, 0x0000001F, 0x00000027, 0x0000002E, 0x00000035, 0x0000003C, 0x0000003D,
    0x00000036, 0x0000002F, 0x00000037, 0x0000003E, 0x0000003F, 0x0000098E, 0x0000098E, 0x0000F384
};

u32 g_252_buffer_unk_635A0C[64] = {};
u32 g_252_buffer_unk_63580C[64] = {};

// Return val becomes param 1

// for Cr, Cb, Y1, Y2, Y3, Y4
int16_t* ddv_func7_DecodeMacroBlock_impl(int16_t* inPtr, int16_t* outputBlockPtr, bool isYBlock)
{
    const int v1 = isYBlock;
    const u32* pTable = isYBlock ? &g_252_buffer_unk_63580C[1] : &g_252_buffer_unk_635A0C[1];
    unsigned int counter = 0;
    u16* pInput = (u16*)inPtr;
    u32* pOutput = (u32*)outputBlockPtr;              // off 10 quantised coefficients

                                                      // 0xFE00 == END_OF_BLOCK, hence this loop moves past the EOB
    while (*pInput == 0xFE00u)
    {
        pInput++;
    }

    *pOutput = (v1 << 10) + 2 * (*pInput << 21 >> 22);
    pInput++;

    if ((*(pInput - 1)) & 1)
    {
        do
        {
            const unsigned int macroBlockWord = *pInput++;// bail if end
            if (macroBlockWord == 0xFE00)
            {
                break;
            }

            const u32 q_scale = (macroBlockWord >> 10);

            counter += q_scale;

            const int lookedUpIndex = g_index_look_up_table[counter];
            signed int v24 = pOutput[lookedUpIndex] + (macroBlockWord << 22);

            u32 v25 = 0;
            SetHiWord(v25, GetHiWord(v24));
            SetLoWord(v25, static_cast<u16>((pTable[q_scale] * (v24 >> 22) + 4) >> 3));
            pTable += q_scale + 1;

            pOutput[lookedUpIndex] = v25;


            counter++;
        } while (counter < 63);                     // 63 AC values?

    }
    else
    {

        while (1)
        {
            const unsigned int macroBlockWord = *pInput++;// bail if end
            if (macroBlockWord == 0xFE00)
            {
                break;
            }
            const u32 q_scale = (macroBlockWord >> 10);

            const signed int v24 = macroBlockWord << 22;
            int k = q_scale + 1;
            int idx = 0;
            while (1)
            {
                --k;
                idx = g_index_look_up_table[counter];
                if (!k)
                {
                    break;
                }
                pOutput[idx] = 0;
                ++counter;
            }

            u32 outVal = 0;
            SetHiWord(outVal, GetHiWord(v24));
            SetLoWord(outVal, static_cast<u16>((pTable[q_scale] * (v24 >> 22) + 4) >> 3));

            pTable += q_scale + 1;
            pOutput[idx] = outVal;

            ++counter;
            if (counter >= 63)                      // 63 AC values?
            {
                return (int16_t*)pInput;
            }
        }

        if (counter)
        {
            int counter3 = counter + 1;

            if (counter3 & 3)
            {
                pOutput[RL_ZSCAN_MATRIX_2[counter3++]] = 0;
                if (counter3 & 3)
                {
                    pOutput[RL_ZSCAN_MATRIX_2[counter3++]] = 0;
                    if (counter3 & 3)
                    {
                        pOutput[RL_ZSCAN_MATRIX_2[counter3++]] = 0;
                    }
                }
            }

            while (counter3 != 64)              // 63 AC values?
            {
                pOutput[RL_ZSCAN_MATRIX_2[counter3]] = 0;
                pOutput[g_index_look_up_table[counter3]] = 0;
                pOutput[g_block_related_2_dword_42B0CC[counter3]] = 0;
                pOutput[g_block_related_3_dword_42B0D0[counter3]] = 0;
                counter3 += 4;
            }
        }
        else
        {
            memset(pOutput + 1, 0, 0xFCu);            // 63 dwords buffer
        }

    }
    return (int16_t*)pInput;
}

// TODO: Should probably just be 64? Making this bigger fixes a sound glitch which is probably caused
// by an out of bounds write somewhere.
typedef std::array<int32_t, 64 * 4> T64IntsArray;

static T64IntsArray Cr_block = {};
static T64IntsArray Cb_block = {};
static T64IntsArray Y1_block = {};
static T64IntsArray Y2_block = {};
static T64IntsArray Y3_block = {};
static T64IntsArray Y4_block = {};


void half_idct(T64IntsArray& pSource, T64IntsArray& pDestination, int nPitch, int nIncrement, int nShift)
{
    std::array<int32_t, 8> pTemp;

    size_t sourceIdx = 0;
    size_t destinationIdx = 0;

    for (int i = 0; i < 8; i++)
    {
        pTemp[4] = pSource[(0 * nPitch) + sourceIdx] * 8192 + pSource[(2 * nPitch) + sourceIdx] * 10703 + pSource[(4 * nPitch) + sourceIdx] * 8192 + pSource[(6 * nPitch) + sourceIdx] * 4433;
        pTemp[5] = pSource[(0 * nPitch) + sourceIdx] * 8192 + pSource[(2 * nPitch) + sourceIdx] * 4433 - pSource[(4 * nPitch) + sourceIdx] * 8192 - pSource[(6 * nPitch) + sourceIdx] * 10704;
        pTemp[6] = pSource[(0 * nPitch) + sourceIdx] * 8192 - pSource[(2 * nPitch) + sourceIdx] * 4433 - pSource[(4 * nPitch) + sourceIdx] * 8192 + pSource[(6 * nPitch) + sourceIdx] * 10704;
        pTemp[7] = pSource[(0 * nPitch) + sourceIdx] * 8192 - pSource[(2 * nPitch) + sourceIdx] * 10703 + pSource[(4 * nPitch) + sourceIdx] * 8192 - pSource[(6 * nPitch) + sourceIdx] * 4433;

        pTemp[0] = pSource[(1 * nPitch) + sourceIdx] * 11363 + pSource[(3 * nPitch) + sourceIdx] * 9633 + pSource[(5 * nPitch) + sourceIdx] * 6437 + pSource[(7 * nPitch) + sourceIdx] * 2260;
        pTemp[1] = pSource[(1 * nPitch) + sourceIdx] * 9633 - pSource[(3 * nPitch) + sourceIdx] * 2259 - pSource[(5 * nPitch) + sourceIdx] * 11362 - pSource[(7 * nPitch) + sourceIdx] * 6436;
        pTemp[2] = pSource[(1 * nPitch) + sourceIdx] * 6437 - pSource[(3 * nPitch) + sourceIdx] * 11362 + pSource[(5 * nPitch) + sourceIdx] * 2261 + pSource[(7 * nPitch) + sourceIdx] * 9633;
        pTemp[3] = pSource[(1 * nPitch) + sourceIdx] * 2260 - pSource[(3 * nPitch) + sourceIdx] * 6436 + pSource[(5 * nPitch) + sourceIdx] * 9633 - pSource[(7 * nPitch) + sourceIdx] * 11363;

        pDestination[(0 * nPitch) + destinationIdx] = (pTemp[4] + pTemp[0]) >> nShift;
        pDestination[(1 * nPitch) + destinationIdx] = (pTemp[5] + pTemp[1]) >> nShift;
        pDestination[(2 * nPitch) + destinationIdx] = (pTemp[6] + pTemp[2]) >> nShift;
        pDestination[(3 * nPitch) + destinationIdx] = (pTemp[7] + pTemp[3]) >> nShift;
        pDestination[(4 * nPitch) + destinationIdx] = (pTemp[7] - pTemp[3]) >> nShift;
        pDestination[(5 * nPitch) + destinationIdx] = (pTemp[6] - pTemp[2]) >> nShift;
        pDestination[(6 * nPitch) + destinationIdx] = (pTemp[5] - pTemp[1]) >> nShift;
        pDestination[(7 * nPitch) + destinationIdx] = (pTemp[4] - pTemp[0]) >> nShift;

        sourceIdx += nIncrement;
        destinationIdx += nIncrement;
    }
}

// 0x40ED90
void idct(int16_t* input, T64IntsArray& pDestination) // dst is 64 dwords
{
    T64IntsArray pTemp;
    T64IntsArray pExtendedSource;

    // Source is passed as signed 16 bits stored every 32 bits
    // We sign extend it at the beginning like Masher does
    for (int i = 0; i < 64; i++)
    {
        pExtendedSource[i] = input[i * 2];
    }

    half_idct(pExtendedSource, pTemp, 8, 1, 11);
    half_idct(pTemp, pDestination, 1, 8, 18);
}


static void after_block_decode_no_effect_q_impl(int quantScale)
{
    g_252_buffer_unk_63580C[0] = 16;
    g_252_buffer_unk_635A0C[0] = 16;
    if (quantScale > 0)
    {
        signed int result = 0;
        do
        {
            auto val = gQuant1_dword_42AEC8[result];
            result++;
            g_252_buffer_unk_63580C[result] = quantScale * val;
            g_252_buffer_unk_635A0C[result] = quantScale * gQaunt2_dword_42AFC4[result];


        } while (result < 63);                   // 252/4=63
    }
    else
    {
        // These are simply null buffers to start with
        for (int i = 0; i < 64; i++)
        {
            g_252_buffer_unk_635A0C[i] = 16;
            g_252_buffer_unk_63580C[i] = 16;
        }
        // memset(&g_252_buffer_unk_635A0C[1], 16, 252  /*sizeof(g_252_buffer_unk_635A0C)*/); // u32[63]
        // memset(&g_252_buffer_unk_63580C[1], 16, 252 /*sizeof(g_252_buffer_unk_63580C)*/);
    }

}

int Masher::Init_4E6770(const char* movieFileName)
{
    field_40_video_frame_to_decode = nullptr;
    field_44_decoded_frame_data_buffer = nullptr;
    field_48_sound_frame_to_decode = nullptr;
    field_4C_decoded_audio_buffer = nullptr;
    field_8C_macro_block_buffer = nullptr;
    field_80_raw_frame_data = nullptr;
    field_70_frame_sizes_array = nullptr;
    field_90_64_or_0 = 0;
    field_84_max_frame_size = 8;
    field_88_audio_data_offset = 0;

    // Open the file
    field_0_file_handle = sMovie_IO_BBB314.mIO_Open(movieFileName);

    // Read file magic
    DWORD fileMagic = 0;
    if (!field_0_file_handle ||
        !sMovie_IO_BBB314.mIO_Read(field_0_file_handle, (BYTE*)&fileMagic, sizeof(DWORD)) ||
        !sMovie_IO_BBB314.mIO_Wait(field_0_file_handle))
    {
        return 1;
    }

    // Verify magic
    const DWORD kDDV_dword_68EE70 = 0x564444;
    if (memcmp(&fileMagic, &kDDV_dword_68EE70, sizeof(DWORD)))
    {
        return 3;
    }

    // Read DDV header
    if (!sMovie_IO_BBB314.mIO_Read(field_0_file_handle, (BYTE*)&field_4_ddv_header, sizeof(Masher_Header)) ||
        !sMovie_IO_BBB314.mIO_Wait(field_0_file_handle))
    {
        return 1;
    }

    // Verify version is correct
    if (field_4_ddv_header.field_0_ddv_version != 1)
    {
        return 4;
    }

    // Does the file contain a video stream?
    field_61_bHasVideo = field_4_ddv_header.field_4_contains & 1;
    if (field_61_bHasVideo)
    {
        // Read the video header
        if (!sMovie_IO_BBB314.mIO_Read(field_0_file_handle, (BYTE*)&field_14_video_header, sizeof(Masher_VideoHeader)) ||
            !sMovie_IO_BBB314.mIO_Wait(field_0_file_handle))
        {
            return 1;
        }

        // Add on to the max frame size
        field_84_max_frame_size += field_14_video_header.field_C_max_audio_frame_size;

        // Allocate buffer for decoding frame data
        field_44_decoded_frame_data_buffer = (WORD*)malloc(sizeof(WORD) * field_14_video_header.field_10_max_video_frame_size);
        if (!field_44_decoded_frame_data_buffer)
        {
            return 2;
        }

        // Calculate blocks X/Y
        field_58_macro_blocks_x = (field_14_video_header.field_4_width + 15) / kMacroBlockWidth;
        field_5C_macro_blocks_y = (field_14_video_header.field_8_height + 15) / kMacroBlockHeight;

        // Alloc macro block temp buffer (6 blocks for Cr, Cb, Y1, Y2, Y3, Y4)
        field_8C_macro_block_buffer = malloc(kMacroBlockWidth * kMacroBlockHeight * 6 * field_58_macro_blocks_x * field_5C_macro_blocks_y);
        if (!field_8C_macro_block_buffer)
        {
            return 2;
        }

        field_90_64_or_0 = 64;

        // NOTE: Pruned dead branches here
    }

    field_2C_audio_header.field_10_num_frames_interleave = 0;

    // Does the file contain an audio stream?
    field_60_bHasAudio = (field_4_ddv_header.field_4_contains >> 1) & 1;
    if (field_60_bHasAudio)
    {
        // Read audio header
        if (!sMovie_IO_BBB314.mIO_Read(field_0_file_handle, (BYTE*)&field_2C_audio_header, sizeof(Masher_AudioHeader)) ||
            !sMovie_IO_BBB314.mIO_Wait(field_0_file_handle))
        {
            return 1;
        }

        field_50_num_channels = 1;
        field_54_bits_per_sample = 8;

        if (field_2C_audio_header.field_0_audio_format & 1)
        {
            field_50_num_channels = 2;
        }

        if (field_2C_audio_header.field_0_audio_format & 2)
        {
            field_54_bits_per_sample = 16;
        }

        field_84_max_frame_size += field_2C_audio_header.field_8_max_audio_frame_size;

        field_4C_decoded_audio_buffer = (BYTE*)malloc(field_2C_audio_header.field_C_single_audio_frame_size
            * (field_50_num_channels * field_54_bits_per_sample / 8));

        if (!field_4C_decoded_audio_buffer)
        {
            return 2;
        }
    }

    // Align the size
    field_84_max_frame_size = RoundUpPowerOf2(field_84_max_frame_size, 2);
    field_80_raw_frame_data = (int*)malloc(2 * field_84_max_frame_size);
    if (!field_80_raw_frame_data)
    {
        return 2;
    }

    // Allocate buffer for frame sizes
    const DWORD frameSizeArrayInBytes = sizeof(DWORD) * (field_2C_audio_header.field_10_num_frames_interleave + field_4_ddv_header.field_C_number_of_frames);
    field_70_frame_sizes_array = (int*)malloc(frameSizeArrayInBytes);
    if (!field_70_frame_sizes_array)
    {
        return 2;
    }

    // Populate frame sizes array from disk
    if (!sMovie_IO_BBB314.mIO_Read(field_0_file_handle, (BYTE*)field_70_frame_sizes_array, frameSizeArrayInBytes) ||
        !sMovie_IO_BBB314.mIO_Wait(field_0_file_handle))
    {
        return 1;
    }

    field_64_audio_frame_idx = 0;
    field_74_pCurrentFrameSize = field_70_frame_sizes_array;
    field_68_frame_number = 0;
    field_6C_frame_num = 0;

    return 0;
}

void Masher::dtor_4E6AB0()
{
    if (field_0_file_handle)
    {
        sMovie_IO_BBB314.mIO_Close(field_0_file_handle);
    }

    if (field_70_frame_sizes_array)
    {
        free(field_70_frame_sizes_array);
    }

    if (field_80_raw_frame_data)
    {
        free(field_80_raw_frame_data);
    }

    if (field_44_decoded_frame_data_buffer)
    {
        free(field_44_decoded_frame_data_buffer);
    }

    if (field_4C_decoded_audio_buffer)
    {
        free(field_4C_decoded_audio_buffer);
    }

    if (field_8C_macro_block_buffer)
    {
        if (field_90_64_or_0)
        {
            free(field_8C_macro_block_buffer);
        }
    }
}

int Masher::sub_4E6B30()
{
    // Read next frame data if we are not at the end
    if (field_68_frame_number < field_4_ddv_header.field_C_number_of_frames)
    {
        int frameSizeToRead = *field_74_pCurrentFrameSize;
        field_74_pCurrentFrameSize++;

        if (field_60_bHasAudio && field_61_bHasVideo)
        {
            // Contains offset to audio in the buffer
            frameSizeToRead += sizeof(DWORD);
        }

        if (frameSizeToRead > 0
            && (!sMovie_IO_BBB314.mIO_Wait(field_0_file_handle) ||
                !sMovie_IO_BBB314.mIO_Read(field_0_file_handle,
                (BYTE*)field_80_raw_frame_data + field_88_audio_data_offset,
                    frameSizeToRead)))
        {
            return 0;
        }
    }

    const int frameOffset = field_84_max_frame_size - field_88_audio_data_offset;
    field_88_audio_data_offset = frameOffset;

    // Audio with no video
    if (field_60_bHasAudio && !field_61_bHasVideo)
    {
        field_48_sound_frame_to_decode = (int *)((char *)field_80_raw_frame_data + frameOffset);
    }
    // Video with no audio
    else if (!field_60_bHasAudio && field_61_bHasVideo)
    {
        field_40_video_frame_to_decode = field_80_raw_frame_data + frameOffset;
    }
    // Audio and video
    else
    {
        BYTE* pFrameData = (BYTE *)field_80_raw_frame_data;
        field_40_video_frame_to_decode = (void*)&pFrameData[frameOffset + sizeof(DWORD)];

        // Skip video data + video data len to get start of sound data
        DWORD videoDataSize = *(DWORD *)&pFrameData[frameOffset];
        field_48_sound_frame_to_decode = (int *)&pFrameData[frameOffset + sizeof(DWORD) + videoDataSize];
    }
    return ++field_68_frame_number < field_4_ddv_header.field_C_number_of_frames + 2;
}

int CC Masher::sub_4EAC30(Masher* pMasher)
{
    int* pFrameSize = pMasher->field_74_pCurrentFrameSize;
    int sizeToRead = *pFrameSize;
    pMasher->field_74_pCurrentFrameSize = pFrameSize + 1;
    if (!sMovie_IO_BBB314.mIO_Read(pMasher->field_0_file_handle, (BYTE*)pMasher->field_80_raw_frame_data, sizeToRead) ||
        !sMovie_IO_BBB314.mIO_Wait(pMasher->field_0_file_handle))
    {
        return 0;
    }
    pMasher->field_48_sound_frame_to_decode = pMasher->field_80_raw_frame_data;
    return 1;
}

void Masher::Decode_4EA670()
{
    // This seems to be used to just skip data without rendering ??
    MMX_Decode_4E6C60(nullptr);
}

void Masher::MMX_Decode_4E6C60(BYTE* pPixelBuffer)
{
    if (!field_61_bHasVideo)
    {
        return;
    }

    ++field_6C_frame_num;

    //  if (!(field_6C_frame_num % field_14_video_header.field_14_key_frame_rate))
    {
        // return;
    }

    //if (field_6C_frame_num < field_2C_audio_header.field_10_num_frames_interleave)
    {
        // return;
    }

    // if (field_68_frame_number < field_2C_audio_header.field_10_num_frames_interleave)
    {
        //  return;
    }

    const int blocksX = field_58_macro_blocks_x;
    const int blocksY = field_5C_macro_blocks_y;
    if (blocksX <= 0 || field_5C_macro_blocks_y <= 0)
    {
        return;
    }

    const int quantScale = decode_bitstream((u16*)field_40_video_frame_to_decode, field_44_decoded_frame_data_buffer);

    after_block_decode_no_effect_q_impl(quantScale);

    int16_t* bitstreamCurPos = (int16_t*)field_44_decoded_frame_data_buffer;
    int16_t* block1Output = (int16_t*)field_8C_macro_block_buffer;

    int xoff = 0;
    for (int xBlock = 0; xBlock < blocksX; xBlock++)
    {
        int yoff = 0;
        for (int yBlock = 0; yBlock < blocksY; yBlock++)
        {
            const int dataSizeBytes = field_90_64_or_0 * 2; // Convert to byte count 64*4=256

            int16_t* afterBlock1Ptr = ddv_func7_DecodeMacroBlock_impl(bitstreamCurPos, block1Output, 0);
            idct(block1Output, Cr_block);
            int16_t* block2Output = dataSizeBytes + block1Output;

            int16_t* afterBlock2Ptr = ddv_func7_DecodeMacroBlock_impl(afterBlock1Ptr, block2Output, 0);
            idct(block2Output, Cb_block);
            int16_t* block3Output = dataSizeBytes + block2Output;

            int16_t* afterBlock3Ptr = ddv_func7_DecodeMacroBlock_impl(afterBlock2Ptr, block3Output, 1);
            idct(block3Output, Y1_block);
            int16_t* block4Output = dataSizeBytes + block3Output;

            int16_t* afterBlock4Ptr = ddv_func7_DecodeMacroBlock_impl(afterBlock3Ptr, block4Output, 1);
            idct(block4Output, Y2_block);
            int16_t* block5Output = dataSizeBytes + block4Output;

            int16_t* afterBlock5Ptr = ddv_func7_DecodeMacroBlock_impl(afterBlock4Ptr, block5Output, 1);
            idct(block5Output, Y3_block);
            int16_t* block6Output = dataSizeBytes + block5Output;

            bitstreamCurPos = ddv_func7_DecodeMacroBlock_impl(afterBlock5Ptr, block6Output, 1);
            idct(block6Output, Y4_block);
            block1Output = dataSizeBytes + block6Output;

            if (pPixelBuffer)
            {
                // TODO: Should probably be using gMasher_pitch_bytes_BB4AF8 ??
                ConvertYuvToRgbAndBlit((u16*)pPixelBuffer, xoff, yoff,
                    640,
                    480,
                    true,
                    true);

                // pPixelBuffer += gMasher_pitch_bytes_BB4AF8;
            }

            yoff += kMacroBlockHeight;
        }
        xoff += kMacroBlockWidth;
    }
}

ALIVE_VAR(1, 0xbbb9b4, int, gMasher_num_channels_BBB9B4, 0);
ALIVE_VAR(1, 0xbbb9a8, int, gMasher_bits_per_sample_BBB9A8, 0);

void CC Masher::DDV_SND_4ECFD0(int numChannels, int bitsPerSample)
{
    gMasher_num_channels_BBB9B4 = numChannels;
    gMasher_bits_per_sample_BBB9A8 = bitsPerSample;
}

void CC Masher::DDV_SND_4ECFF0(int* pMasherFrame, BYTE* pDecodedFrame, int frameSize)
{
    AudioDecompressor decompressor;
    const int bytesPerSample = gMasher_bits_per_sample_BBB9A8 / 8;
    decompressor.SetChannelCount(bytesPerSample);
    decompressor.SetupAudioDecodePtrs((u16*)pMasherFrame);
    memset(pDecodedFrame, 0, frameSize * bytesPerSample * gMasher_num_channels_BBB9B4);

    if (gMasher_bits_per_sample_BBB9A8 == 8)
    {
        BYTE* pAsByte = (BYTE*)pDecodedFrame;
        decompressor.decode_8bit_audio_frame(pAsByte, frameSize, false);
        if (gMasher_num_channels_BBB9B4 == 2)
        {
            decompressor.decode_8bit_audio_frame(pAsByte + 1, frameSize, true);
        }
    }

    if (gMasher_bits_per_sample_BBB9A8 == 16)
    {
        decompressor.decode_16bit_audio_frame((u16*)pDecodedFrame, frameSize, false);
        if (gMasher_num_channels_BBB9B4 == 2)
        {
            decompressor.decode_16bit_audio_frame((u16*)pDecodedFrame + 1, frameSize, true);
        }
    }
}

void* CC Masher::GetDecompressedAudioFrame_4EAC60(Masher* pMasher)
{
    void* result = nullptr;
    if (pMasher->field_60_bHasAudio
        && pMasher->field_64_audio_frame_idx < pMasher->field_4_ddv_header.field_C_number_of_frames)
    {
        DDV_SND_4ECFD0(pMasher->field_50_num_channels, pMasher->field_54_bits_per_sample);
        DDV_SND_4ECFF0(
            pMasher->field_48_sound_frame_to_decode,
            (BYTE*)pMasher->field_4C_decoded_audio_buffer,
            pMasher->field_2C_audio_header.field_C_single_audio_frame_size);
        result = pMasher->field_4C_decoded_audio_buffer;
        ++pMasher->field_64_audio_frame_idx;
    }
    else
    {
        ++pMasher->field_64_audio_frame_idx;
        result = nullptr;
    }
    return result;
}

int Masher::To1d(int x, int y)
{
    // 8x8 index to x64 index
    return y * 8 + x;
}

unsigned char Masher::Clamp(f32 v)
{
    if (v < 0.0f) v = 0.0f;
    if (v > 255.0f) v = 255.0f;
    return (unsigned char)v;
}

void Masher::SetElement(int x, int y, int width, int height, u16* ptr, u16 value, bool doubleWidth, bool doubleHeight)
{
    if (doubleWidth)
    {
        x *= 2;
    }

    if (doubleHeight)
    {
        y *= 2;
    }

    ptr[(width * y) + x] = value;

    if (doubleWidth)
    {
        if (x + 1 < width)
        {
            ptr[(width * y) + x + 1] = value;
        }
    }

    if (doubleHeight)
    {
        if (y + 1 < height)
        {
            ptr[(width * (y + 1)) + x] = value;

            if (doubleWidth)
            {
                if (x + 1 < width)
                {
                    ptr[(width * (y + 1)) + x + 1] = value;
                }
            }
        }
    }
}

uint16_t Masher::rgb888torgb565(Macroblock_RGB_Struct& rgb888Pixel)
{
    uint8_t red = rgb888Pixel.Red;
    uint8_t green = rgb888Pixel.Green;
    uint8_t blue = rgb888Pixel.Blue;

    uint16_t b = (blue >> 3) & 0x1f;
    uint16_t g = ((green >> 2) & 0x3f) << 5;
    uint16_t r = ((red >> 3) & 0x1f) << 11;

    return (uint16_t)(r | g | b);
}

void Masher::ConvertYuvToRgbAndBlit(u16* pixelBuffer, int xoff, int yoff, int width, int height, bool doubleWidth, bool doubleHeight)
{
    // convert the Y1 Y2 Y3 Y4 and Cb and Cr blocks into a 16x16 array of (Y, Cb, Cr) pixels
    struct Macroblock_YCbCr_Struct
    {
        f32 Y;
        f32 Cb;
        f32 Cr;
    };

    std::array< std::array<Macroblock_YCbCr_Struct, 16>, 16> Macroblock_YCbCr = {};

    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            Macroblock_YCbCr[x][y].Y = static_cast<f32>(Y1_block[To1d(x, y)]);
            Macroblock_YCbCr[x + 8][y].Y = static_cast<f32>(Y2_block[To1d(x, y)]);
            Macroblock_YCbCr[x][y + 8].Y = static_cast<f32>(Y3_block[To1d(x, y)]);
            Macroblock_YCbCr[x + 8][y + 8].Y = static_cast<f32>(Y4_block[To1d(x, y)]);

            Macroblock_YCbCr[x * 2][y * 2].Cb = static_cast<f32>(Cb_block[To1d(x, y)]);
            Macroblock_YCbCr[x * 2 + 1][y * 2].Cb = static_cast<f32>(Cb_block[To1d(x, y)]);
            Macroblock_YCbCr[x * 2][y * 2 + 1].Cb = static_cast<f32>(Cb_block[To1d(x, y)]);
            Macroblock_YCbCr[x * 2 + 1][y * 2 + 1].Cb = static_cast<f32>(Cb_block[To1d(x, y)]);

            Macroblock_YCbCr[x * 2][y * 2].Cr = static_cast<f32>(Cr_block[To1d(x, y)]);
            Macroblock_YCbCr[x * 2 + 1][y * 2].Cr = static_cast<f32>(Cr_block[To1d(x, y)]);
            Macroblock_YCbCr[x * 2][y * 2 + 1].Cr = static_cast<f32>(Cr_block[To1d(x, y)]);
            Macroblock_YCbCr[x * 2 + 1][y * 2 + 1].Cr = static_cast<f32>(Cr_block[To1d(x, y)]);
        }
    }

    // Convert the (Y, Cb, Cr) pixels into RGB pixels
    std::array< std::array<Macroblock_RGB_Struct, 16>, 16> Macroblock_RGB = {};

    for (u32 x = 0; x < kMacroBlockWidth; x++)
    {
        for (u32 y = 0; y < kMacroBlockHeight; y++)
        {
            const f32 r = (Macroblock_YCbCr[x][y].Y) + 1.402f *  Macroblock_YCbCr[x][y].Cb;
            const f32 g = (Macroblock_YCbCr[x][y].Y) - 0.3437f * Macroblock_YCbCr[x][y].Cr - 0.7143f * Macroblock_YCbCr[x][y].Cb;
            const f32 b = (Macroblock_YCbCr[x][y].Y) + 1.772f *  Macroblock_YCbCr[x][y].Cr;

            Macroblock_RGB[x][y].Red = Clamp(r);
            Macroblock_RGB[x][y].Green = Clamp(g);
            Macroblock_RGB[x][y].Blue = Clamp(b);

            // Due to macro block padding this can be out of bounds
            int xpos = x + xoff;
            int ypos = y + yoff;
            if (xpos < width && ypos < height)
            {

                u16 pixel16Value = rgb888torgb565(Macroblock_RGB[x][y]);
                // Actually is no alpha in FMVs
                // pixelValue = (pixelValue << 8) + Macroblock_RGB[x][y].A
                SetElement(xpos, ypos, width, height, pixelBuffer, pixel16Value, doubleWidth, doubleHeight);
            }
        }
    }
}
