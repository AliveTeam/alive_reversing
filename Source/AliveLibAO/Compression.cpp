#include "stdafx.h"
#include "stdlib.hpp"
#include "Compression.hpp"
#include "Function.hpp"
#include "PtrStream.hpp"
#include "../AliveLibAE/Compression.hpp"
#include "Sys_common.hpp"

namespace AO {

EXPORT void CC Decompress_Type_1_403150(const BYTE* /*pInput*/, BYTE* /*pOutput*/, unsigned int /*compressedLen*/, unsigned int /*decompressedLen*/)
{
    ALIVE_FATAL("Decompress_Type_1_403150 never expected be called");
}

EXPORT void CC Decompress_Type_2_403390(const BYTE* pInput, BYTE* pOutput, int decompressedLen)
{
    // Exactly the same as AE
    CompressionType2_Decompress_40AA50(pInput, pOutput, decompressedLen);
}


template<typename T>
static void ReadNextSource(PtrStream& stream, int& control_byte, T& workBits)
{
    if (control_byte)
    {
        if (control_byte == 0xE) // Or 14
        {
            control_byte = 0x1Eu; // Or 30
            workBits |= stream.ReadU16() << 14;
        }
    }
    else
    {
        workBits = stream.ReadU32();
        control_byte = 0x20u; // 32
    }
    control_byte -= 6;
}

EXPORT void CC Decompress_Type_3_4031E0(const BYTE* pInput, BYTE* pOutput, int totalLen, int out_len)
{
    unsigned int inStreamLen = totalLen & ~3u;
    unsigned int inStreamDirectBytesLen = totalLen & 3;
 
    PtrStream inStream(&pInput);

    const BYTE* pDirectBytes = pInput + (6 * inStreamLen) / 8;
    PtrStream inStreamDirectBytes(&pDirectBytes);

    const int total_out_len = (out_len + 3) / 4;
    if (total_out_len)
    {
        memset(pOutput, 0, 4 * total_out_len);
    }

    int control_byte = 0;
    unsigned int workBits = 0;

    BYTE* pOutIter = pOutput;

    while (inStreamLen)
    {
        ReadNextSource(inStream, control_byte, workBits);
        inStreamLen--;

        const BYTE input_byte = workBits & 0x3F;
        workBits = workBits >> 6;

        if (input_byte & 0x20)
        {
            const int src_masked = (input_byte & 0x1F) + 1;
            for (int i = 0; i < src_masked; i++)
            {
                if (inStreamLen)
                {
                    ReadNextSource(inStream, control_byte, workBits);
                    inStreamLen--;

                    *pOutIter++ = workBits & 0x3F;
                    workBits = workBits >> 6;
                }
                else
                {
                    *pOutIter++ = (inStreamDirectBytes.ReadU8() & 0x3F);
                    inStreamDirectBytesLen--;
                }
            }
        }
        else
        {
            // Near direct copy byte
            pOutIter += input_byte + 1;
        }
    }

    while (inStreamDirectBytesLen)
    {
        const BYTE input_byte = (inStreamDirectBytes.ReadU8() & 0x3F);
        inStreamDirectBytesLen--;

        if (input_byte & 0x20)
        {
            const int numBytesToCopy = (input_byte & 0x1F) + 1;
            for (int i = 0; i < numBytesToCopy; i++)
            {
                const BYTE copyByte = (inStreamDirectBytes.ReadU8() & 0x3F);
                inStreamDirectBytesLen--;
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
