#pragma once

#include "FunctionFwd.hpp"

namespace AO {

EXPORT void CC Decompress_Type_1_403150(const BYTE* pInput, BYTE* pOutput, unsigned int compressedLen, unsigned int decompressedLen);

EXPORT void CC Decompress_Type_2_403390(const BYTE* pInput, BYTE* pOutput, int decompressedLen);

EXPORT void CC Decompress_Type_3_4031E0(const BYTE* pInput, BYTE* pOutput, int len, int out_len);

EXPORT void CC Decompress_Type_4_5_461770(const BYTE* pInput, BYTE* pOutput);

}
