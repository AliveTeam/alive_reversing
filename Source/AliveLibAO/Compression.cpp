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

EXPORT void CC Decompress_Type_3_4031E0(unsigned __int16* /*pInput*/, BYTE* /*pOutput*/, int /*len*/, int /*out_len*/)
{
    NOT_IMPLEMENTED();
}

EXPORT void CC Decompress_Type_4_5_461770(const BYTE* pInput, BYTE* pOutput)
{
    // Exactly the same as AE
    CompressionType_4Or5_Decompress_4ABAB0(pInput, pOutput);
}

}
