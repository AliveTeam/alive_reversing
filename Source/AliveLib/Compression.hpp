#pragma once

#include "FunctionFwd.hpp"

void Compression_ForceLink();

EXPORT void CC CompressionType2_Decompress_40AA50(const BYTE* pSrc, BYTE* pDst, DWORD dataSize);
EXPORT void CC CompressionType_3Ae_Decompress_40A6A0(const BYTE* pData, BYTE* decompressedData);
EXPORT void CC CompressionType_4Or5_Decompress_4ABAB0(const BYTE* pData, BYTE* decompressedData);
EXPORT void CC CompressionType6Ae_Decompress_40A8A0(const BYTE* pSrc, BYTE* pDst);
