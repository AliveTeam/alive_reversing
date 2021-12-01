#include "stdafx.h"
#include "Compression.hpp"
#include "Function.hpp"
#include "PtrStream.hpp"
#include "CompressionType_4Or5.hpp"

void Compression_ForceLink()
{ }

static bool Expand3To4Bytes(s32& remainingCount, PtrStream& stream, u8* ret, u32& dstPos)
{
    if (!remainingCount)
    {
        return false;
    }

    const u32 src3Bytes = stream.ReadU8() | (stream.ReadU16() << 8);
    remainingCount--;

    u32 value;

    // decompress each byte of the input value from least to most significant
    value = (u8) src3Bytes & 0x3F;
    value |= ((u32) src3Bytes << 2) & 0x3F00;
    value |= (src3Bytes << 4) & 0x3F0000;
    value |= ((src3Bytes << 4) & 0x0FC00000) << 2;

    reinterpret_cast<u32*>(ret)[dstPos++] = value;

    return true;
}

EXPORT void CC CompressionType2_Decompress_40AA50(const u8* pSrc, u8* pDst, u32 dataSize)
{
    PtrStream stream(&pSrc);

    s32 dwords_left = dataSize / 4;
    s32 remainder = dataSize % 4;

    u32 dstPos = 0;
    while (dwords_left)
    {
        for (s32 i = 0; i < 4; i++)
        {
            if (!Expand3To4Bytes(dwords_left, stream, pDst, dstPos))
            {
                break;
            }
        }
    }

    // TODO: Branch not tested - copies remainder bytes directly into output
    while (remainder > 0)
    {
        remainder--;
        pDst[dstPos++] = stream.ReadU8();
    }
}

template <typename T>
static void ReadNextSource(PtrStream& stream, s32& control_byte, T& dstIndex)
{
    if (control_byte)
    {
        if (control_byte == 0xE) // Or 14
        {
            control_byte = 0x1Eu; // Or 30
            dstIndex |= stream.ReadU16() << 14;
        }
    }
    else
    {
        dstIndex = stream.ReadU32();
        control_byte = 0x20u; // 32
    }
    control_byte -= 6;
}

EXPORT void CC CompressionType_3Ae_Decompress_40A6A0(const u8* pData, u8* decompressedData)
{
    PtrStream stream(&pData);

    s32 dstPos = 0;
    s32 control_byte = 0;

    s32 width = stream.ReadU16();
    s32 height = stream.ReadU16();

    if (height > 0)
    {
        u32 dstIndex = 0;
        do
        {
            s32 columnNumber = 0;
            while (columnNumber < width)
            {
                ReadNextSource(stream, control_byte, dstIndex);

                const u8 blackBytes = dstIndex & 0x3F;
                u32 srcByte = dstIndex >> 6;

                const s32 bytesToWrite = blackBytes + columnNumber;

                for (s32 i = 0; i < blackBytes; i++)
                {
                    decompressedData[dstPos++] = 0;
                }

                ReadNextSource(stream, control_byte, srcByte);

                const u8 bytes = srcByte & 0x3F;
                dstIndex = srcByte >> 6;

                columnNumber = bytes + bytesToWrite;
                if (bytes > 0)
                {
                    s32 byteCount = bytes;
                    do
                    {
                        ReadNextSource(stream, control_byte, dstIndex);

                        const s8 dstByte = dstIndex & 0x3F;
                        dstIndex = dstIndex >> 6;

                        decompressedData[dstPos] = dstByte;
                        dstPos++;
                        --byteCount;
                    }
                    while (byteCount);
                }
            }

            while (columnNumber & 3)
            {
                ++dstPos;
                ++columnNumber;
            }
        }
        while (height-- != 1);
    }
}

EXPORT void CC CompressionType_4Or5_Decompress_4ABAB0(const u8* pData, u8* decompressedData)
{
    CompressionType_4Or5_Decompress(pData, decompressedData);
}

static u8 NextNibble(PtrStream& stream, bool& readLo, u8& srcByte)
{
    if (readLo)
    {
        readLo = !readLo;
        return srcByte >> 4;
    }
    else
    {
        srcByte = stream.ReadU8();
        readLo = !readLo;
        return srcByte & 0xF;
    }
}

EXPORT void CC CompressionType6Ae_Decompress_40A8A0(const u8* pSrc, u8* pDst)
{
    PtrStream stream(&pSrc);

    bool bNibbleToRead = false;
    bool bSkip = false;
    u32 dstPos = 0;

    const s32 w = stream.ReadU16();
    const s32 h = stream.ReadU16();

    if (h > 0)
    {
        u8 srcByte = 0;
        s32 heightCounter = h;
        do
        {
            s32 widthCounter = 0;
            while (widthCounter < w)
            {
                u8 nibble = NextNibble(stream, bNibbleToRead, srcByte);
                widthCounter += nibble;

                while (nibble)
                {
                    if (bSkip)
                    {
                        dstPos++;
                    }
                    else
                    {
                        pDst[dstPos] = 0;
                    }
                    bSkip = !bSkip;
                    --nibble;
                }

                nibble = NextNibble(stream, bNibbleToRead, srcByte);
                widthCounter += nibble;

                if (nibble > 0)
                {
                    do
                    {
                        const u8 data = NextNibble(stream, bNibbleToRead, srcByte);
                        if (bSkip)
                        {
                            pDst[dstPos++] |= 16 * data;
                            bSkip = 0;
                        }
                        else
                        {
                            pDst[dstPos] = data;
                            bSkip = 1;
                        }
                    }
                    while (--nibble);
                }
            }

            for (; widthCounter & 7; ++widthCounter)
            {
                if (bSkip)
                {
                    dstPos++;
                }
                else
                {
                    pDst[dstPos] = 0;
                }
                bSkip = !bSkip;
            }
        }
        while (heightCounter-- != 1);
    }
}
