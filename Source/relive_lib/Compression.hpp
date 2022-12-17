#pragma once

#include "Types.hpp"

namespace AO {

void Decompress_Type_2(const u8* pInput, u8* pOutput, s32 decompressedLen);

void Decompress_Type_3(const u8* pInput, u8* pOutput, s32 len, s32 out_len);

void Decompress_Type_4Or5(const u8* pInput, u8* pOutput);

} // namespace AO
