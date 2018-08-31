#include "stdafx.h"
#include "Compression.hpp"
#include "Function.hpp"

class PtrStream
{
public:
    PtrStream(const BYTE** ppStream)
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
        BYTE v = 0;
        memcpy(&v, *mppStream, sizeof(v));
        *mppStream += sizeof(v);
        return v;
    }

    const BYTE** mppStream;
};

// 0xxx xxxx = string of literals (1 to 128)
// 1xxx xxyy yyyy yyyy = copy from y bytes back, x bytes
EXPORT void CompressionType4Or5_Decompress_4ABAB0(const BYTE* pData, BYTE* decompressedData)
{
    PtrStream stream(&pData);

    //stream.Seek(stream.Pos() - 4);

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

