#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"

namespace AO {

EXPORT void CC Decompress_Type_1(const u8* pInput, u8* pOutput, u32 compressedLen, u32 decompressedLen);

EXPORT void CC Decompress_Type_2(const u8* pInput, u8* pOutput, s32 decompressedLen);

EXPORT void CC Decompress_Type_3(const u8* pInput, u8* pOutput, s32 len, s32 out_len);

EXPORT void CC Decompress_Type_4Or5(const u8* pInput, u8* pOutput);

} // namespace AO
