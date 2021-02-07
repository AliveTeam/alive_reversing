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
#include "PsxDisplay.hpp"
#include "Compression.hpp"
#include "../AliveLibAE/Renderer/IRenderer.hpp"

namespace AO {

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
    Layer field_66_mapped_layer;
};
ALIVE_ASSERT_SIZEOF(Fg1Block, 0x68);

const Layer sFg1_layer_to_bits_layer_4BC024[] = { Layer::eLayer_37, Layer::eLayer_18 };

void FG1::Convert_Chunk_To_Render_Block_453BA0(const Fg1Chunk* pChunk, Fg1Block* pBlock)
{
    const short width_rounded = (pChunk->field_8_width + 1) & ~1u;
    if (vram_alloc_450860(pChunk->field_8_width, pChunk->field_A_height, &pBlock->field_58_rect))
    {
        pBlock->field_66_mapped_layer = sFg1_layer_to_bits_layer_4BC024[pChunk->field_2_layer_or_decompressed_size];

        PSX_RECT rect = {};
        rect.x = pBlock->field_58_rect.x;
        rect.y = pBlock->field_58_rect.y;
        rect.w = width_rounded;
        rect.h = pChunk->field_A_height;
        IRenderer::GetRenderer()->Upload(IRenderer::BitDepth::e16Bit, rect, (BYTE*)&pChunk[1]);

        const short tPage = static_cast<short>(PSX_getTPage_4965D0(TPageMode::e16Bit_2, TPageAbr::eBlend_0, rect.x /*& 0xFFC0*/, rect.y));

        const BYTE u0 = rect.x & 63;
        const BYTE v0 = static_cast<BYTE>(rect.y);
        const BYTE u1 = static_cast<BYTE>(u0 + pChunk->field_8_width - 1);
        const BYTE v1 = static_cast<BYTE>(v0 + pChunk->field_A_height - 1);

        const short x1 = pChunk->field_4_xpos_or_compressed_size + pChunk->field_8_width;
        const short y2 = pChunk->field_6_ypos + pChunk->field_A_height;

        for (Poly_FT4& rPoly : pBlock->field_0_polys)
        {
            PolyFT4_Init(&rPoly);
            Poly_Set_SemiTrans_498A40(&rPoly.mBase.header, FALSE);
            Poly_Set_Blending_498A00(&rPoly.mBase.header, TRUE);

            SetTPage(&rPoly, tPage);

            SetXY0(&rPoly, pChunk->field_4_xpos_or_compressed_size, pChunk->field_6_ypos);
            SetXY1(&rPoly, x1, pChunk->field_6_ypos);
            SetXY2(&rPoly, pChunk->field_4_xpos_or_compressed_size, y2);
            SetXY3(&rPoly, x1, y2);

            SetUV0(&rPoly, u0, v0);
            SetUV1(&rPoly, u1, v0);
            SetUV2(&rPoly, u0, v1);
            SetUV3(&rPoly, u1, v1);

            SetRGB0(&rPoly, 128, 128, 128);
        }
    }
    else
    {
        pBlock->field_58_rect.w = 0;
    }
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

void FG1::VRender(PrimHeader** ppOt)
{
    VRender_453D50(ppOt);
}

void FG1::VRender_453D50(PrimHeader** ppOt)
{
    if (field_18_render_block_count > 0)
    {
        for (int i = 0; i < field_18_render_block_count; i++)
        {
            Fg1Block* pBlock = &field_20_chnk_res[i];
            if (pBlock->field_58_rect.w > 0)
            {
                Poly_FT4* pPoly = &pBlock->field_0_polys[gPsxDisplay_504C78.field_A_buffer_index];

                SetPrimExtraPointerHack(pPoly, nullptr);

                OrderingTable_Add_498A80(OtLayer(ppOt, pBlock->field_66_mapped_layer), &pPoly->mBase.header);
                
                pScreenManager_4FF7C8->InvalidateRect_406E40(
                    X0(pPoly),
                    Y0(pPoly),
                    X3(pPoly),
                    Y3(pPoly),
                    pScreenManager_4FF7C8->field_2E_idx);
            }
        }
    }
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

}
