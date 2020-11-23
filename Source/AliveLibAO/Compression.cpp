#include "stdafx.h"
#include "stdlib.hpp"
#include "Compression.hpp"
#include "Function.hpp"
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

EXPORT void CC Decompress_Type_3_4031E0(const unsigned __int16* pInput, BYTE* pOutput, int len, int out_len)
{
    unsigned int in_dwords; // edi
    const BYTE* pInputIter; // ebx
    signed int bits; // esi
    const BYTE* pInputIter_off; // ebp
    int total_len; // eax
    BYTE* pOutIter; // edx
    int tmp; // eax
    unsigned __int8 src_val; // cl
    int src_masked; // ecx
    int out_val_; // eax
    BYTE out_val; // cl
    bool bEndOut; // zf
    unsigned __int8 input_byte; // al
    int counter_val; // eax
    int counter; // ecx
    BYTE input_byte_direct; // al
    int in_remainder; // [esp+20h] [ebp+Ch]
    int out_couter; // [esp+24h] [ebp+10h]

    in_dwords = len & ~3u;
    in_remainder = len & 3;
    pInputIter = (const BYTE*)pInput;
    bits = 0;
    pInputIter_off = (const BYTE*)pInput + (signed int)(6 * in_dwords) / 8;
    total_len = (out_len + 3) / 4;
    if (total_len)
    {
        memset(pOutput, 0, 4 * total_len);
    }

    pOutIter = pOutput;
    if (in_dwords)
    {
        tmp = 0;// (int)pOutput;                     // ??
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
            src_val = tmp & 0x3F;
            tmp = (unsigned int)tmp >> 6;
            --in_dwords;
            if (src_val & 0x20)
            {
                src_masked = (src_val & 0x1F) + 1;
                if (src_masked)
                {
                    out_couter = src_masked;
                    do
                    {
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
                                out_val_ = *(DWORD*)pInputIter;
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
                        bEndOut = out_couter == 1;
                        pOutput = pOutIter;
                        --out_couter;
                    } while (!bEndOut);
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
        input_byte = *pInputIter_off++ & 0x3F;
        --in_remainder;
        if (input_byte & 0x20)
        {
            counter_val = (input_byte & 0x1F) + 1;
            if (counter_val)
            {
                counter = counter_val;
                do
                {
                    input_byte_direct = *pInputIter_off++ & 0x3F;
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
