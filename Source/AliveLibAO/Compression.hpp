#pragma once

#include "FunctionFwd.hpp"

namespace AO {

EXPORT void CC Decompress_Type_1_403150(const u8* pInput, u8* pOutput, u32 compressedLen, u32 decompressedLen);

EXPORT void CC Decompress_Type_2_403390(const u8* pInput, u8* pOutput, s32 decompressedLen);

EXPORT void CC Decompress_Type_3_4031E0(const u8* pInput, u8* pOutput, s32 len, s32 out_len);

EXPORT void CC Decompress_Type_4_5_461770(const u8* pInput, u8* pOutput);

}
