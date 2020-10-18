#include "stdafx_ao.h"
#include "FG1.hpp"
#include "Function.hpp"
#include "Psx.hpp"
#include "Primitives.hpp"
#include "ScreenManager.hpp"
#include "ResourceManager.hpp"
#include "Map.hpp"
#include "Game.hpp"
#include "Sys_common.hpp"
#include "VRam.hpp"
#include "stdlib.hpp"

START_NS_AO

enum eChunkTypes
{
    ePartialChunk = 0,
    eFullChunk = 0xFFFE,
    eEndCompressedData = 0xFFFC,
    eStartCompressedData = 0xFFFD,
    eEndChunk = 0xFFFF
};

struct Fg1Chunk
{
    unsigned __int16 field_0_type;
    unsigned __int16 field_2_layer_or_decompressed_size;
    __int16 field_4_xpos_or_compressed_size;
    __int16 field_6_ypos;
    unsigned __int16 field_8_width;
    unsigned __int16 field_A_height;
};
ALIVE_ASSERT_SIZEOF(Fg1Chunk, 0xC);

struct FG1ResourceBlockHeader
{
    DWORD mCount;
    Fg1Chunk mChunks;
};

struct Fg1Block
{
    Poly_FT4 field_0_polys[2];
    PSX_RECT field_58_rect;
    int field_60_padding;
    __int16 field_64_padding;
    unsigned __int16 field_66_mapped_layer;
};
ALIVE_ASSERT_SIZEOF(Fg1Block, 0x68);

// TODO: Needs moving to compression.hpp
EXPORT void CC Decompress_Type_4_5_461770(const BYTE* /*pInput*/, BYTE* /*pOutput*/);

void FG1::Convert_Chunk_To_Render_Block_453BA0(const Fg1Chunk* /*pChunk*/, Fg1Block* /*pBlock*/)
{
    NOT_IMPLEMENTED();
}

BaseGameObject* FG1::dtor_453DF0()
{
    SetVTable(this, 0x4BC028);

    gObjList_drawables_504618->Remove_Item(this);

    for (int i = 0; i < field_18_render_block_count; i++)
    {
        if (field_20_chnk_res[i].field_58_rect.w > 0)
        {
            Vram_free_450CE0(
                { field_20_chnk_res[i].field_58_rect.x, field_20_chnk_res[i].field_58_rect.y },
                { field_20_chnk_res[i].field_58_rect.w, field_20_chnk_res[i].field_58_rect.h });
        }
    }

    ResourceManager::FreeResource_455550(field_1C_ptr);
    return dtor_487DF0();
}

FG1* FG1::ctor_4539C0(unsigned __int8** ppRes)
{
    ctor_487E10(1);
     
    SetVTable(this, 0x4BC028);

    field_6_flags.Set(Options::eDrawable_Bit4);
    field_6_flags.Set(Options::eSurviveDeathReset_Bit9);
    field_6_flags.Set(Options::eUpdateDuringCamSwap_Bit10);

    field_4_typeId = Types::eFG1_67;

    field_10_cam_pos_x = FP_GetExponent(pScreenManager_4FF7C8->field_10_pCamPos->field_0_x);
    field_12_cam_pos_y = FP_GetExponent(pScreenManager_4FF7C8->field_10_pCamPos->field_4_y);
    
    field_16_current_path = gMap_507BA8.field_2_current_path;
    field_14_current_level = gMap_507BA8.field_0_current_level;

    gObjList_drawables_504618->Push_Back(this);
    
    // Cast to the actual FG1 resource block format
    FG1ResourceBlockHeader* pHeader = reinterpret_cast<FG1ResourceBlockHeader*>(*ppRes);

    field_18_render_block_count = static_cast<short>(pHeader->mCount);
    field_1C_ptr = ResourceManager::Allocate_New_Locked_Resource_454F80(ResourceManager::Resource_CHNK, 0, pHeader->mCount * sizeof(Fg1Block));
    field_20_chnk_res = reinterpret_cast<Fg1Block*>(*field_1C_ptr);

    // And take a pointer to the first chunk to iterate them
    Fg1Chunk* pChunkIter = &pHeader->mChunks;

    BYTE** pSavedChunkIter = nullptr;
    BYTE** pDecompressionBuffer = nullptr;
    int idx = 0;

    for (;;) // Exit when we hit the end chunk
    {
        switch (pChunkIter->field_0_type)
        {
            case ePartialChunk:
            {
                Fg1Block* pRenderBlock = &field_20_chnk_res[idx++];
                Convert_Chunk_To_Render_Block_453BA0(pChunkIter, pRenderBlock);

                // The pixel size is variable, calculate the size and move to the end of it to get the next block
                const int pixelSizeBytes = pChunkIter->field_A_height * pChunkIter->field_8_width * sizeof(short);
                pChunkIter = reinterpret_cast<Fg1Chunk*>(reinterpret_cast<BYTE*>(pChunkIter) + pixelSizeBytes + sizeof(Fg1Chunk));
            }
            break;

            case eFullChunk:
            {
                pScreenManager_4FF7C8->InvalidateRect_406D80(
                    pChunkIter->field_4_xpos_or_compressed_size,
                    pChunkIter->field_6_ypos,
                    pChunkIter->field_8_width + pChunkIter->field_4_xpos_or_compressed_size - 1,
                    pChunkIter->field_A_height + pChunkIter->field_6_ypos - 1,
                    pChunkIter->field_2_layer_or_decompressed_size);

                // Move to the next FG1 data from disk
                pChunkIter++;
            }
            break;

            case eStartCompressedData:
            {
                // Allocate buffer to decompress into
                pDecompressionBuffer = ResourceManager::Allocate_New_Locked_Resource_454F80(
                    ResourceManager::Resource_PBuf,
                    0,
                    pChunkIter->field_2_layer_or_decompressed_size);

                // Decompress into it
                Decompress_Type_4_5_461770(reinterpret_cast<const BYTE*>(pChunkIter) + sizeof(Fg1Chunk), *pDecompressionBuffer);

                // Compressed data size + header size
                const int sizeToSkipBytes = pChunkIter->field_4_xpos_or_compressed_size + sizeof(Fg1Chunk);
                
                // Goto next block and save ptr to it
                pSavedChunkIter = reinterpret_cast<BYTE**>((reinterpret_cast<BYTE*>(pChunkIter) + sizeToSkipBytes));

                // Set current ptr to decompressed data
                pChunkIter = reinterpret_cast<Fg1Chunk*>(*pDecompressionBuffer);
            }
            break;

            case eEndCompressedData:
            {
                PSX_DrawSync_496750(0);

                // Go back to the saved ptr
                pChunkIter = reinterpret_cast<Fg1Chunk*>(pSavedChunkIter);
                pSavedChunkIter = nullptr;

                // Free the decompression buffer
                ResourceManager::FreeResource_455550(pDecompressionBuffer);
                pDecompressionBuffer = nullptr;
            }
            break;

            case eEndChunk:
            {
                return this;
            }

            default:
                ALIVE_FATAL("Unknown FG1 block type");
        }
    }
}

BaseGameObject* FG1::VDestructor(signed int flags)
{
    return Vdtor_453E90(flags);
}

void FG1::VUpdate()
{
    // Empty
}

void FG1::VScreenChanged()
{
    VScreenChanged_453DE0();
}

void FG1::VScreenChanged_453DE0()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

void FG1::VRender(int** ppOt)
{
    VRender_453D50(ppOt);
}

void FG1::VRender_453D50(int** /*ppOt*/)
{
    NOT_IMPLEMENTED();
}

FG1* FG1::Vdtor_453E90(signed int flags)
{
    dtor_453DF0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

END_NS_AO
