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

EXPORT void CC Decompress_Type_3_4031E0(const BYTE* pInput, BYTE* pOutput, int len, int out_len)
{
    unsigned int in_dwords = len & ~3u;
    int in_remainder = len & 3;
    
    const BYTE* pInputIter = pInput;
    const BYTE* pInputIter_off = pInput + (6 * in_dwords) / 8;
    
    int total_len = (out_len + 3) / 4;
    if (total_len)
    {
        memset(pOutput, 0, 4 * total_len);
    }

    int bits = 0;

    BYTE* pOutIter = pOutput;
    if (in_dwords)
    {
        int tmp = 0;
        do
        {
            if (bits)
            {
                if (bits == 14)
                {
                    bits = 30;
                    tmp |= *(unsigned __int16*)pInputIter << 14;
                    pInputIter += 2;
                }
            }
            else
            {
                tmp = *(DWORD*)pInputIter;
                bits = 32;
                pInputIter += 4;
            }
            bits -= 6;
            const BYTE src_val = tmp & 0x3F;
            tmp = (unsigned int)tmp >> 6;
            --in_dwords;
            if (src_val & 0x20)
            {
                const int src_masked = (src_val & 0x1F) + 1;
                if (src_masked)
                {
                    int out_couter = src_masked;
                    do
                    {
                        BYTE out_val = 0;
                        if (in_dwords)
                        {
                            if (bits)
                            {
                                if (bits == 14)
                                {
                                    bits = 30;
                                    tmp |= *(unsigned __int16*)pInputIter << 14;
                                    pInputIter += 2;
                                }
                                bits -= 6;
                                out_val = tmp & 0x3F;
                                tmp = (unsigned int)tmp >> 6;
                                --in_dwords;
                            }
                            else
                            {
                                const int out_val_ = *(DWORD*)pInputIter;
                                pInputIter += 4;
                                out_val = out_val_ & 0x3F;
                                bits = 26;
                                tmp = (unsigned int)out_val_ >> 6;
                                --in_dwords;
                            }
                        }
                        else
                        {
                            out_val = *pInputIter_off++ & 0x3F;
                            --in_remainder;
                            pOutIter = pOutput;
                        }
                        *pOutIter++ = out_val;
                        pOutput = pOutIter;
                        --out_couter;
                    } while (out_couter != 0);
                }
            }
            else
            {
                pOutIter += src_val + 1;
                pOutput = pOutIter;
            }
        } while (in_dwords);
    }
    while (in_remainder)
    {
        const BYTE input_byte = *pInputIter_off++ & 0x3F;
        --in_remainder;
        if (input_byte & 0x20)
        {
            const int counter_val = (input_byte & 0x1F) + 1;
            if (counter_val)
            {
                int counter = counter_val;
                do
                {
                    const BYTE input_byte_direct = *pInputIter_off++ & 0x3F;
                    *pOutIter++ = input_byte_direct;
                    --counter;
                    --in_remainder;
                } while (counter);
            }
        }
        else
        {
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
