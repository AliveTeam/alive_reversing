#include "stdafx.h"
#include "Compression.hpp"
#include "Function.hpp"

void Compression_ForceLink() { }

class IPtrStream
{
public:
    IPtrStream(const BYTE** ppStream)
        : mppStream(ppStream)
    {

    }

    void Read(DWORD& v)
    {
        memcpy(&v, *mppStream, sizeof(v));
        *mppStream += sizeof(v);
    }

    BYTE ReadU8()
    {
        return ReadT<BYTE>();
    }

    WORD ReadU16()
    {
        return ReadT<WORD>();
    }

    DWORD ReadU32()
    {
        return ReadT<DWORD>();
    }

    const BYTE** mppStream;

private:
    template<class T>
    T ReadT()
    {
        T v = 0;
        memcpy(&v, *mppStream, sizeof(v));
        *mppStream += sizeof(v);
        return v;
    }
};

// 0xxx xxxx = string of literals (1 to 128)
// 1xxx xxyy yyyy yyyy = copy from y bytes back, x bytes
EXPORT void CC CompressionType_4Or5_Decompress_4ABAB0(const BYTE* pData, BYTE* decompressedData)
{
    IPtrStream stream(&pData);

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
            for (int i = 0; i < c + 1; i++)
            {
                decompressedData[dstPos++] = stream.ReadU8();
            }
        }
    }
}

template<typename T>
static void ReadNextSource(IPtrStream& stream, int& control_byte, T& dstIndex)
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
    IPtrStream stream(&pData);

    int dstPos = 0;
    int control_byte = 0;

    int width = stream.ReadU16();
    int height = stream.ReadU16();

    if (height > 0)
    {
        unsigned int dstIndex = 0;
        do
        {
            int columnNumber = 0;
            while (columnNumber < width)
            {
                ReadNextSource(stream, control_byte, dstIndex);

                const unsigned char blackBytes = dstIndex & 0x3F;
                unsigned int srcByte = dstIndex >> 6;

                const int bytesToWrite = blackBytes + columnNumber;

                for (int i = 0; i < blackBytes; i++)
                {
                    decompressedData[dstPos++] = 0;
                }

                ReadNextSource(stream, control_byte, srcByte);

                const unsigned char bytes = srcByte & 0x3F;
                dstIndex = srcByte >> 6;

                columnNumber = bytes + bytesToWrite;
                if (bytes > 0)
                {
                    int byteCount = bytes;
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

static bool Expand3To4Bytes(int& remainingCount, IPtrStream& stream, BYTE** ret, DWORD& dstPos)
{
    if (!remainingCount)
    {
        return false;
    }
    const DWORD src3Bytes = stream.ReadU8() | (stream.ReadU16() << 8);
    remainingCount--;

    // TODO: Should write each byte by itself
    const DWORD value = (4 * (WORD)src3Bytes & 0x3F00) | (src3Bytes & 0x3F) | (16 * src3Bytes & 0x3F0000) | (4 * (16 * src3Bytes & 0xFC00000));
    *reinterpret_cast<DWORD*>(&ret[dstPos]) = value;
    dstPos += 4;

    return true;
}

EXPORT void CC CompressionType2_Decompress_40AA50(const BYTE* pSrc, BYTE* pDst, DWORD dataSize)
{
    IPtrStream stream(&pSrc);

    int dwords_left = dataSize / 4;
    int remainder = dataSize % 4;

    DWORD dstPos = 0;
    while (dwords_left)
    {
        for (int i = 0; i < 4; i++)
        {
            if (!Expand3To4Bytes(dwords_left, stream, &pDst, dstPos))
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
