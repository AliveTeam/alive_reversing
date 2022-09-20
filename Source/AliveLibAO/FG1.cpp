#include "stdafx_ao.h"
#include "FG1.hpp"
#include "Function.hpp"
#include "Psx.hpp"
#include "../relive_lib/Primitives.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "ResourceManager.hpp"
#include "Map.hpp"
#include "Game.hpp"
#include "Sys_common.hpp"
#include "../relive_lib/VRam.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../AliveLibAE/Compression.hpp"
#include "../AliveLibAE/Renderer/IRenderer.hpp"
#include "FG1Reader.hpp"

namespace AO {

struct Fg1Block
{
    Poly_FT4 field_0_polys[2];
    PSX_RECT field_58_rect;
    Layer field_66_mapped_layer;
    u32 field_68_array_of_height[16]; // Added for AE format interop
};
//ALIVE_ASSERT_SIZEOF(Fg1Block, 0x68);

class FG1Reader final : public BaseFG1Reader
{
public:
    explicit FG1Reader(FG1& fg1)
        : BaseFG1Reader(FG1Format::AO)
        , mFg1(fg1)
    {
    }

    void OnPartialChunk(const Fg1Chunk& rChunk) override
    {
        Fg1Block* pRenderBlock = &mFg1.field_20_chnk_res[mIdx++];
        mFg1.Convert_Chunk_To_Render_Block(&rChunk, pRenderBlock);
    }

    void OnFullChunk(const Fg1Chunk& rChunk) override
    {
        // For some reason the screen manager doesn't work the same as in AE and this won't
        // result in full blocks getting drawn. Therefore we should never see this get called
        // as all blocks are partial (full blocks are "fake" partial blocks).
        u16 remappedToAeLayer = 0;
        if (rChunk.field_2_layer_or_decompressed_size == 0)
        {
            // FG1
            remappedToAeLayer = ScreenManager::BitLayers::FG1_7 - ScreenManager::BitLayers::FG1_Half_Well_4;
        }
        else
        {
            // FG1 half
            remappedToAeLayer = ScreenManager::BitLayers::FG1_Half_5 - ScreenManager::BitLayers::FG1_Half_Well_4;
        }

        pScreenManager->InvalidateRect_IdxPlus4(
            rChunk.field_4_xpos_or_compressed_size,
            rChunk.field_6_ypos,
            rChunk.field_8_width + rChunk.field_4_xpos_or_compressed_size - 1,
            rChunk.field_A_height + rChunk.field_6_ypos - 1,
            remappedToAeLayer);
    }

    u8** Allocate(u32 len) override
    {
        return ResourceManager::Allocate_New_Locked_Resource(
            ResourceManager::Resource_PBuf,
            0,
            len);
    }

    void Deallocate(u8** ptr) override
    {
        ResourceManager::FreeResource_455550(ptr);
    }

private:
    FG1& mFg1;
    u32 mIdx = 0;
};

// Reads the tweaked AE format FG1
class FG1ReaderAE final : public BaseFG1Reader
{
public:
    explicit FG1ReaderAE(FG1& fg1)
        : BaseFG1Reader(FG1Format::AE)
        , mFg1(fg1)
    {
    }

    void OnPartialChunk(const Fg1Chunk& rChunk) override
    {
        Fg1Block* pRenderBlock = &mFg1.field_20_chnk_res[mIdx++];
        mFg1.Convert_Chunk_To_Render_Block_AE(&rChunk, pRenderBlock);
    }

    void OnFullChunk(const Fg1Chunk& rChunk) override
    {
        pScreenManager->InvalidateRect_IdxPlus4(
            rChunk.field_4_xpos_or_compressed_size,
            rChunk.field_6_ypos,
            rChunk.field_8_width + rChunk.field_4_xpos_or_compressed_size - 1,
            rChunk.field_A_height + rChunk.field_6_ypos - 1,
            rChunk.field_2_layer_or_decompressed_size);
    }

    u8** Allocate(u32 len) override
    {
        // Shouldn't be called for this format
        return ResourceManager::Allocate_New_Locked_Resource(
            ResourceManager::Resource_PBuf,
            0,
            len);
    }

    void Deallocate(u8** ptr) override
    {
        // Shouldn't be called for this format
        ResourceManager::FreeResource_455550(ptr);
    }

private:
    FG1& mFg1;
    u32 mIdx = 0;
};

static const Layer sFg1_layer_to_bits_layer_4BC024[] = {Layer::eLayer_FG1_37, Layer::eLayer_FG1_Half_18};

void FG1::Convert_Chunk_To_Render_Block(const Fg1Chunk* pChunk, Fg1Block* pBlock)
{
    const s16 width_rounded = (pChunk->field_8_width + 1) & ~1u;
    if (Vram_alloc(pChunk->field_8_width, pChunk->field_A_height, 16, &pBlock->field_58_rect))
    {
        pBlock->field_66_mapped_layer = sFg1_layer_to_bits_layer_4BC024[pChunk->field_2_layer_or_decompressed_size];

        PSX_RECT rect = {};
        rect.x = pBlock->field_58_rect.x;
        rect.y = pBlock->field_58_rect.y;
        rect.w = width_rounded;
        rect.h = pChunk->field_A_height;
        IRenderer::GetRenderer()->Upload(IRenderer::BitDepth::e16Bit, rect, (u8*) &pChunk[1]);

        const s16 tPage = static_cast<s16>(PSX_getTPage(TPageMode::e16Bit_2, TPageAbr::eBlend_0, rect.x /*& 0xFFC0*/, rect.y));

        const u8 u0 = rect.x & 63;
        const u8 v0 = static_cast<u8>(rect.y);
        const u8 u1 = static_cast<u8>(u0 + pChunk->field_8_width - 1);
        const u8 v1 = static_cast<u8>(v0 + pChunk->field_A_height - 1);

        const s16 x1 = pChunk->field_4_xpos_or_compressed_size + pChunk->field_8_width;
        const s16 y2 = pChunk->field_6_ypos + pChunk->field_A_height;

        for (Poly_FT4& rPoly : pBlock->field_0_polys)
        {
            rPoly = {};

            PolyFT4_Init(&rPoly);
            Poly_Set_SemiTrans(&rPoly.mBase.header, FALSE);
            Poly_Set_Blending(&rPoly.mBase.header, TRUE);

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
static const Layer sFg1_layer_to_bits_layer[] = {Layer::eLayer_Well_Half_4, Layer::eLayer_FG1_Half_18, Layer::eLayer_Well_23, Layer::eLayer_FG1_37};

void FG1::Convert_Chunk_To_Render_Block_AE(const Fg1Chunk* pChunk, Fg1Block* pBlock)
{
    // Map the layer from FG1 internal to OT layer
    pBlock->field_66_mapped_layer = sFg1_layer_to_bits_layer[pChunk->field_2_layer_or_decompressed_size];

    // Copy in the bits that represent the see through pixels
    memcpy(pBlock->field_68_array_of_height, &pChunk[1], pChunk->field_A_height * sizeof(u32));

    for (Poly_FT4& rPoly : pBlock->field_0_polys)
    {
        rPoly = {};

        PolyFT4_Init(&rPoly);

        Poly_Set_SemiTrans(&rPoly.mBase.header, FALSE);
        Poly_Set_Blending(&rPoly.mBase.header, TRUE);

        SetTPage(&rPoly, static_cast<u16>(PSX_getTPage(TPageMode::e16Bit_2, TPageAbr::eBlend_0, 0, 0))); // 16bit hits psx renderer FG1 path ?

        SetXYWH(&rPoly, pChunk->field_4_xpos_or_compressed_size, pChunk->field_6_ypos, pChunk->field_8_width, pChunk->field_A_height);

        SetPrimExtraPointerHack(&rPoly, pBlock->field_68_array_of_height);
    }
}

FG1::~FG1()
{
    gObjListDrawables->Remove_Item(this);
    /*
    for (s32 i = 0; i < field_18_render_block_count; i++)
    {
        if (field_20_chnk_res[i].field_58_rect.w > 0)
        {
            Vram_free(
                {field_20_chnk_res[i].field_58_rect.x, field_20_chnk_res[i].field_58_rect.y},
                {field_20_chnk_res[i].field_58_rect.w, field_20_chnk_res[i].field_58_rect.h});
        }
    }

    ResourceManager::FreeResource_455550(field_1C_ptr);
    */
}

FG1::FG1(Fg1Resource& /*ppRes*/)
    : BaseGameObject(TRUE, 0)
{
    mBaseGameObjectFlags.Set(Options::eDrawable_Bit4);
    mBaseGameObjectFlags.Set(Options::eSurviveDeathReset_Bit9);
    mBaseGameObjectFlags.Set(Options::eUpdateDuringCamSwap_Bit10);

    SetType(ReliveTypes::eFG1);

    field_10_cam_pos_x = FP_GetExponent(pScreenManager->mCamPos->x);
    field_12_cam_pos_y = FP_GetExponent(pScreenManager->mCamPos->y);

    field_16_current_path = gMap.mCurrentPath;
    field_14_current_level = gMap.mCurrentLevel;

    gObjListDrawables->Push_Back(this);

    /*
    // TODO: Render as 1 texture

    // Cast to the actual FG1 resource block format
    FG1ResourceBlockHeader* pHeader = reinterpret_cast<FG1ResourceBlockHeader*>(*ppRes);

    // Check if its relive format FG1
    bool isReliveFG1 = false;
    if (pHeader->mCount == ResourceManager::Resource_FG1)
    {
        // adjust past the new file magic
        pHeader = reinterpret_cast<FG1ResourceBlockHeader*>(*ppRes + sizeof(u32));

        isReliveFG1 = true;
    }

    field_18_render_block_count = static_cast<s16>(pHeader->mCount);
    field_1C_ptr = ResourceManager::Allocate_New_Locked_Resource(ResourceManager::Resource_CHNK, 0, pHeader->mCount * sizeof(Fg1Block));
    field_20_chnk_res = reinterpret_cast<Fg1Block*>(*field_1C_ptr);

    if (isReliveFG1)
    {
        FG1ReaderAE loader(*this);
        loader.Iterate(pHeader);
    }
    else
    {
        FG1Reader loader(*this);
        loader.Iterate(pHeader);
    }*/
}

void FG1::VUpdate()
{
    // Empty
}

void FG1::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

void FG1::VRender(PrimHeader** /*ppOt*/)
{
    /*
    for (s32 i = 0; i < field_18_render_block_count; i++)
    {
        Poly_FT4* pPoly = &field_20_chnk_res[i].field_0_polys[gPsxDisplay.mBufferIndex];
        const s32 xpos = X0(pPoly);
        const s32 ypos = Y0(pPoly);
        //if (pScreenManager->IsDirty(pScreenManager->Idx(), xpos, ypos) || pScreenManager->IsDirty(ScreenManager::BitLayers::Unknown_3, xpos, ypos))
        {
            OrderingTable_Add(OtLayer(ppOt, field_20_chnk_res[i].field_66_mapped_layer), &pPoly->mBase.header);

            pScreenManager->InvalidateRectCurrentIdx(xpos, ypos, X3(pPoly), Y3(pPoly));
        }
    }*/
}

} // namespace AO
