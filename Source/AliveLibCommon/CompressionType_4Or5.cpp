#include "CompressionType_4Or5.hpp"

#include "PtrStream.hpp"

// 0xxx xxxx = string of literals (1 to 128)
// 1xxx xxyy yyyy yyyy = copy from y bytes back, x bytes
void CompressionType_4Or5_Decompress_Impl(const u8* pData, u8* decompressedData)
{
    PtrStream stream(&pData);

    // Get the length of the destination buffer
    u32 nDestinationLength = 0;
    stream.Read(nDestinationLength);

    u32 dstPos = 0;
    while (dstPos < nDestinationLength)
    {
        // get code byte
        const u8 c = stream.ReadU8();

        // 0x80 = 0b10000000 = RLE flag
        // 0xc7 = 0b01111100 = bytes to use for length
        // 0x03 = 0b00000011
        if (c & 0x80)
        {
            // Figure out how many bytes to copy.
            const u32 nCopyLength = ((c & 0x7C) >> 2) + 3;

            // The last 2 bits plus the next byte gives us the destination of the copy
            const u8 c1 = stream.ReadU8();
            const u32 nPosition = ((c & 0x03) << 8) + c1 + 1;
            const u32 startIndex = dstPos - nPosition;

            for (u32 i = 0; i < nCopyLength; i++)
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
