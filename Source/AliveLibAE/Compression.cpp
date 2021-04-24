#include "stdafx.h"
#include "Compression.hpp"
#include "Function.hpp"
#include "PtrStream.hpp"

void Compression_ForceLink() { }

static bool Expand3To4Bytes(s32& remainingCount, PtrStream& stream, BYTE* ret, DWORD& dstPos)
{
    if (!remainingCount)
    {
        return false;
    }

    const DWORD src3Bytes = stream.ReadU8() | (stream.ReadU16() << 8);
    remainingCount--;

    DWORD value;

    // decompress each byte of the input value from least to most significant
    value =  (BYTE)src3Bytes & 0x3F;
    value |= ((DWORD)src3Bytes << 2) & 0x3F00;
    value |= (src3Bytes << 4) & 0x3F0000;
    value |= ((src3Bytes << 4) & 0x0FC00000) << 2;

    reinterpret_cast<DWORD*>(ret)[dstPos++] = value;

    return true;
}

EXPORT void CC CompressionType2_Decompress_40AA50(const BYTE* pSrc, BYTE* pDst, DWORD dataSize)
{
    PtrStream stream(&pSrc);

    s32 dwords_left = dataSize / 4;
    s32 remainder = dataSize % 4;

    DWORD dstPos = 0;
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

template<typename T>
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

EXPORT void CC CompressionType_3Ae_Decompress_40A6A0(const BYTE* pData, BYTE* decompressedData)
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

                        const char dstByte = dstIndex & 0x3F;
                        dstIndex = dstIndex >> 6;

                        decompressedData[dstPos] = dstByte;
                        dstPos++;
                        --byteCount;
                    } while (byteCount);
                }
            }

            while (columnNumber & 3)
            {
                ++dstPos;
                ++columnNumber;
            }
        } while (height-- != 1);
    }

}

// 0xxx xxxx = string of literals (1 to 128)
// 1xxx xxyy yyyy yyyy = copy from y bytes back, x bytes
EXPORT void CC CompressionType_4Or5_Decompress_4ABAB0(const BYTE* pData, BYTE* decompressedData)
{
    PtrStream stream(&pData);

    // Get the length of the destination buffer
    DWORD nDestinationLength = 0;
    stream.Read(nDestinationLength);

    DWORD dstPos = 0;
    while (dstPos < nDestinationLength)
    {
        // get code byte
        const BYTE c = stream.ReadU8();

        // 0x80 = 0b10000000 = RLE flag
        // 0xc7 = 0b01111100 = bytes to use for length
        // 0x03 = 0b00000011
        if (c & 0x80)
        {
            // Figure out how many bytes to copy.
            const DWORD nCopyLength = ((c & 0x7C) >> 2) + 3;

            // The last 2 bits plus the next byte gives us the destination of the copy
            const BYTE c1 = stream.ReadU8();
            const DWORD nPosition = ((c & 0x03) << 8) + c1 + 1;
            const DWORD startIndex = dstPos - nPosition;

            for (DWORD i = 0; i < nCopyLength; i++)
            {
                decompressedData[dstPos++] = decompressedData[startIndex + i];
            }
        }
        else
        {
            // Here the value is the number of literals to copy
            for (s32 i = 0; i < c + 1; i++)
            {
                decompressedData[dstPos++] = stream.ReadU8();
            }
        }
    }
}

static BYTE NextNibble(PtrStream& stream, bool& readLo, BYTE& srcByte)
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

EXPORT void CC CompressionType6Ae_Decompress_40A8A0(const BYTE* pSrc, BYTE* pDst)
{
    PtrStream stream(&pSrc);

    bool bNibbleToRead = false;
    bool bSkip = false;
    DWORD dstPos = 0;

    const s32 w = stream.ReadU16();
    const s32 h = stream.ReadU16();

    if (h > 0)
    {
        BYTE srcByte = 0;
        s32 heightCounter = h;
        do
        {
            s32 widthCounter = 0;
            while (widthCounter < w)
            {
                BYTE nibble = NextNibble(stream, bNibbleToRead, srcByte);
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
                        const BYTE data = NextNibble(stream, bNibbleToRead, srcByte);
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
                    } while (--nibble);
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

        } while (heightCounter-- != 1);
    }
}
