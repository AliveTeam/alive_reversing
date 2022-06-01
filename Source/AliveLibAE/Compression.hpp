#pragma once

#include "../AliveLibCommon/Function.hpp"

void CompressionType2_Decompress_40AA50(const u8* pSrc, u8* pDst, u32 dataSize);
void CompressionType_3Ae_Decompress_40A6A0(const u8* pData, u8* decompressedData);
void CompressionType_4Or5_Decompress_4ABAB0(const u8* pData, u8* decompressedData);
void CompressionType6Ae_Decompress_40A8A0(const u8* pSrc, u8* pDst);
