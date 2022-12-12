#pragma once

void CompressionType2_Decompress(const u8* pSrc, u8* pDst, u32 dataSize);
void CompressionType_3Ae_Decompress(const u8* pData, u8* decompressedData);
void CompressionType_4Or5_Decompress(const u8* pData, u8* decompressedData);
void CompressionType6Ae_Decompress(const u8* pSrc, u8* pDst);
