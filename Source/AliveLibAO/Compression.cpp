#include "stdafx.h"
#include "stdlib.hpp"
#include "Compression.hpp"
#include "Function.hpp"
#include "PtrStream.hpp"
#include "../AliveLibAE/Compression.hpp"

namespace AO {

EXPORT void CC Decompress_Type_1_403150(const BYTE* /*pInput*/, BYTE* /*pOutput*/, unsigned int /*compressedLen*/, unsigned int /*decompressedLen*/)
{
    NOT_IMPLEMENTED();
}

EXPORT void CC Decompress_Type_2_403390(const BYTE* pInput, BYTE* pOutput, int decompressedLen)
{
    // Exactly the same as AE
    CompressionType2_Decompress_40AA50(pInput, pOutput, decompressedLen);
}


template<typename T>
static void ReadNextSource(PtrStream& stream, int& control_byte, T& dstIndex)
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

EXPORT void CC Decompress_Type_3_4031E0(const BYTE* pInput, BYTE* pOutput, int len, int out_len)
{
    unsigned int in_dwords = len & ~3u;
    int in_remainder = len & 3;
 
    PtrStream inStream(&pInput);

    const BYTE* pInputIter_off = pInput + (6 * in_dwords) / 8;

    const int total_len = (out_len + 3) / 4;
    if (total_len)
    {
        memset(pOutput, 0, 4 * total_len);
    }

    int control_byte = 0;

    BYTE* pOutIter = pOutput;
    unsigned int dstIndex = 0;
    while (in_dwords)
    {
        ReadNextSource(inStream, control_byte, dstIndex);

        const BYTE input_byte = dstIndex & 0x3F;
        dstIndex = dstIndex >> 6;
        in_dwords--;

        if (input_byte & 0x20)
        {
            const int src_masked = (input_byte & 0x1F) + 1;
            for (int i = 0; i < src_masked; i++)
            {
                if (in_dwords)
                {
                    if (control_byte)
                    {
                        if (control_byte == 14)
                        {
                            control_byte = 30;
                            dstIndex |= inStream.ReadU16() << 14;
                        }
                        control_byte -= 6;

                        *pOutIter++ = dstIndex & 0x3F;
                        dstIndex = dstIndex >> 6;
                        in_dwords--;
                    }
                    else
                    {
                        dstIndex = inStream.ReadU32();
                        control_byte = 26;

                        *pOutIter++ = dstIndex & 0x3F;
                        dstIndex = dstIndex >> 6;
                        in_dwords--;
                    }
                }
                else
                {
                    const BYTE copyByte = *pInputIter_off++ & 0x3F;
                    in_remainder--;
                    *pOutIter++ = copyByte;
                }
            }
        }
        else
        {
            // Near direct copy byte
            pOutIter += input_byte + 1;
        }
    }

    while (in_remainder)
    {
        const BYTE input_byte = *pInputIter_off++ & 0x3F;
        in_remainder--;

        if (input_byte & 0x20)
        {
            const int numBytesToCopy = (input_byte & 0x1F) + 1;
            for (int i = 0; i < numBytesToCopy; i++)
            {
                const BYTE copyByte = *pInputIter_off++ & 0x3F;
                in_remainder--;
                *pOutIter++ = copyByte;
            }
        }
        else
        {
            // Skip length of zeros
            pOutIter += input_byte + 1;
        }
    }
}

EXPORT void CC Decompress_Type_4_5_461770(const BYTE* pInput, BYTE* pOutput)
{
    // Exactly the same as AE
    CompressionType_4Or5_Decompress_4ABAB0(pInput, pOutput);
}

}
