#pragma once

#include "FunctionFwd.hpp"

void Compression_ForceLink();

EXPORT void CC CompressionType2_Decompress_40AA50(const u8* pSrc, u8* pDst, u32 dataSize);
EXPORT void CC CompressionType_3Ae_Decompress_40A6A0(const u8* pData, u8* decompressedData);
EXPORT void CC CompressionType_4Or5_Decompress_4ABAB0(const u8* pData, u8* decompressedData);
EXPORT void CC CompressionType6Ae_Decompress_40A8A0(const u8* pSrc, u8* pDst);
