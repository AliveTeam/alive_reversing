#pragma once

#include "Types.hpp"
#include "CompressionType_4Or5.hpp"
#include "Function.hpp"

// Forward declare here to avoid SDL.h dep (todo: probably move to its own header)
[[noreturn]] void ALIVE_FATAL(const char_type* errMsg);

enum eChunkTypes
{
    ePartialChunk = 0,
    eFullChunk = 0xFFFE,
    eEndCompressedData = 0xFFFC,
    eStartCompressedData = 0xFFFD,
    eEndChunk = 0xFFFF
};

struct Fg1Chunk final
{
    u16 field_0_type;
    u16 field_2_layer_or_decompressed_size;
    s16 field_4_xpos_or_compressed_size;
    s16 field_6_ypos;
    u16 field_8_width;
    u16 field_A_height;
};
ALIVE_ASSERT_SIZEOF(Fg1Chunk, 0xC);


struct FG1ResourceBlockHeader final
{
    u32 mCount;
    Fg1Chunk mChunks;
};

class BaseFG1Reader
{
public:
    enum class FG1Format
    {
        AO,
        AE,
    };

    explicit BaseFG1Reader(FG1Format format)
        : mFormat(format)
    {

    }

    virtual ~BaseFG1Reader() { }

    virtual void OnPartialChunk(const Fg1Chunk& rChunk) = 0;
    virtual void OnFullChunk(const Fg1Chunk& rChunk) = 0;

    virtual u8** Allocate(u32 len) = 0;
    virtual void Deallocate(u8** ptr) = 0;

    static bool IsReliveFG1(const FG1ResourceBlockHeader* pHeader)
    {
        return pHeader->mCount == 0x20314746; // The "FG1 " constant
    }

    void Iterate(const FG1ResourceBlockHeader* pHeader)
    {
        const Fg1Chunk* pChunkIter = &pHeader->mChunks;

        const u8** pSavedChunkIter = nullptr;
        u8** pDecompressionBuffer = nullptr;

        for (;;) // Exit when we hit the end chunk
        {
            switch (pChunkIter->field_0_type)
            {
                case ePartialChunk:
                {
                    OnPartialChunk(*pChunkIter);

                    if (mFormat == FG1Format::AO)
                    {
                        // The pixel size is variable, calculate the size and move to the end of it to get the next block
                        const s32 pixelSizeBytes = pChunkIter->field_A_height * pChunkIter->field_8_width * sizeof(s16);
                        pChunkIter = reinterpret_cast<const Fg1Chunk*>(reinterpret_cast<const u8*>(pChunkIter) + pixelSizeBytes + sizeof(Fg1Chunk));
                    }
                    else
                    {
                        // Skip to the next block - a bit more tricky as we must skip the bit field array thats used for the transparent pixels
                        const u8* pNextChunk = reinterpret_cast<const u8*>(pChunkIter) + ((pChunkIter->field_A_height * sizeof(u32)) + sizeof(Fg1Chunk));
                        pChunkIter = reinterpret_cast<const Fg1Chunk*>(pNextChunk);
                    }
                }
                break;

                case eFullChunk:
                {
                    OnFullChunk(*pChunkIter);

                    // Move to the next FG1 data from disk
                    pChunkIter++;
                }
                break;

                case eStartCompressedData:
                {
                    if (mFormat == FG1Format::AO)
                    {
                        // Allocate buffer to decompress into
                        pDecompressionBuffer = Allocate(pChunkIter->field_2_layer_or_decompressed_size);

                        // Decompress into it
                        CompressionType_4Or5_Decompress(reinterpret_cast<const u8*>(pChunkIter) + sizeof(Fg1Chunk), *pDecompressionBuffer);

                        // Compressed data size + header size
                        const s32 sizeToSkipBytes = pChunkIter->field_4_xpos_or_compressed_size + sizeof(Fg1Chunk);

                        // Goto next block and save ptr to it
                        const u8* pNextBlockStart = reinterpret_cast<const u8*>(pChunkIter) + sizeToSkipBytes;

                        pSavedChunkIter = reinterpret_cast<const u8**>(const_cast<u8*>(pNextBlockStart));

                        // Set current ptr to decompressed data
                        pChunkIter = reinterpret_cast<const Fg1Chunk*>(*pDecompressionBuffer);
                    }
                    else
                    {
                        // This block type should never appear in AE
                        ALIVE_FATAL("eStartCompressedData is not in any AE data, impossible!");
                    }
                }
                break;

                case eEndCompressedData:
                {
                    if (mFormat == FG1Format::AO)
                    {
                        // Does nothing on PC
                        //PSX_DrawSync_496750(0);

                        // Go back to the saved ptr
                        pChunkIter = reinterpret_cast<Fg1Chunk*>(pSavedChunkIter);
                        pSavedChunkIter = nullptr;

                        // Free the decompression buffer
                        Deallocate(pDecompressionBuffer);

                        pDecompressionBuffer = nullptr;
                    }
                    else
                    {
                        // This block type should never appear in AE
                        ALIVE_FATAL("eEndCompressedData is not in any AE data, impossible!");
                    }
                }
                break;

                case eEndChunk:
                {
                    return;
                }

                default:
                    ALIVE_FATAL("Unknown FG1 block type");
            }
        }
    }

protected:
    FG1Format mFormat = {};
};
