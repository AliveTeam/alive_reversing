#include "stdafx.h"
#include "FG1.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "Map.hpp"
#include "ScreenManager.hpp"
#include "PsxDisplay.hpp"
#include "Sys_common.hpp"
#include "FG1Reader.hpp"

ALIVE_VAR(1, 0x5D1E28, DynamicArrayT<FG1>*, gFG1List_5D1E28, nullptr);

const static Layer sFg1_layer_to_bits_layer_5469BC[4] = {Layer::eLayer_Well_Half_4, Layer::eLayer_FG1_Half_18, Layer::eLayer_Well_23, Layer::eLayer_FG1_37};

class FG1Reader final : public BaseFG1Reader
{
public:
    explicit FG1Reader(FG1& fg1)
        : BaseFG1Reader(FG1Format::AE)
        , mFg1(fg1)
    {
    }

    void OnPartialChunk(const Fg1Chunk& rChunk) override
    {
        Fg1Block* pRenderBlock = &mFg1.field_30_chnk_res[mIdx++];
        mFg1.Convert_Chunk_To_Render_Block(&rChunk, pRenderBlock);
    }

    void OnFullChunk(const Fg1Chunk& rChunk) override
    {
        pScreenManager_5BB5F4->InvalidateRect_40EC50(
            rChunk.field_4_xpos_or_compressed_size,
            rChunk.field_6_ypos,
            rChunk.field_8_width + rChunk.field_4_xpos_or_compressed_size - 1,
            rChunk.field_A_height + rChunk.field_6_ypos - 1,
            rChunk.field_2_layer_or_decompressed_size);
    }

    u8** Allocate(u32 len) override
    {
        // Shouldn't be called for AE
        return ResourceManager::Allocate_New_Locked_Resource_49BF40(
            ResourceManager::Resource_PBuf,
            0,
            len);
    }

    void Deallocate(u8** ptr) override
    {
        // Shouldn't be called for AE
        ResourceManager::FreeResource_49C330(ptr);
    }

private:
    FG1& mFg1;
    u32 mIdx = 0;
};

FG1::FG1(u8** pFG1Res)
    : BaseGameObject(TRUE, 0)
{
    mFlags.Set(BaseGameObject::eUpdateDuringCamSwap_Bit10);
    mFlags.Set(BaseGameObject::eSurviveDeathReset_Bit9);
    mFlags.Set(BaseGameObject::eDrawable_Bit4);

    SetType(AETypes::eFG1_101);

    field_24_level_id = gMap.mCurrentLevel;
    field_26_path_id = gMap.mCurrentPath;

    gFG1List_5D1E28->Push_Back(this);

    // Cast to the actual FG1 resource block format
    FG1ResourceBlockHeader* pHeader = reinterpret_cast<FG1ResourceBlockHeader*>(*pFG1Res);
    
    // Check if its relive format FG1
    if (pHeader->mCount == ResourceManager::Resource_FG1)
    {
        // adjust past the new file magic
        pHeader = reinterpret_cast<FG1ResourceBlockHeader*>(*pFG1Res + sizeof(u32));
    }

    // So we can extract out the count of chunks and allocate a resource for it
    field_20_unused = 0;
    field_28_render_block_count = static_cast<s16>(pHeader->mCount);
    field_2C_ptr = ResourceManager::Allocate_New_Locked_Resource_49BF40(ResourceManager::Resource_CHNK, 0, pHeader->mCount * sizeof(Fg1Block));
    field_30_chnk_res = reinterpret_cast<Fg1Block*>(*field_2C_ptr);

    FG1Reader loader(*this);
    loader.Iterate(pHeader);
}


FG1::~FG1()
{
    gFG1List_5D1E28->Remove_Item(this);
    ResourceManager::FreeResource_49C330(field_2C_ptr);
}

s16 FG1::Convert_Chunk_To_Render_Block(const Fg1Chunk* pChunk, Fg1Block* pBlock)
{
    // Map the layer from FG1 internal to OT layer
    pBlock->field_66_mapped_layer = sFg1_layer_to_bits_layer_5469BC[pChunk->field_2_layer_or_decompressed_size];

    // Copy in the bits that represent the see through pixels
    memcpy(pBlock->field_68_array_of_height, &pChunk[1], pChunk->field_A_height * sizeof(u32));

    for (Poly_FT4& rPoly : pBlock->field_0_polys)
    {
        rPoly = {};

        PolyFT4_Init(&rPoly);

        Poly_Set_SemiTrans_4F8A60(&rPoly.mBase.header, FALSE);
        Poly_Set_Blending_4F8A20(&rPoly.mBase.header, TRUE);

        SetTPage(&rPoly, static_cast<u16>(PSX_getTPage_4F60E0(TPageMode::e16Bit_2, TPageAbr::eBlend_0, 0, 0)));

        SetXYWH(&rPoly, pChunk->field_4_xpos_or_compressed_size, pChunk->field_6_ypos, pChunk->field_8_width, pChunk->field_A_height);

        SetPrimExtraPointerHack(&rPoly, pBlock->field_68_array_of_height);
    }
    return 1;
}

void FG1::VRender(PrimHeader** ppOt)
{
    for (s32 i = 0; i < field_28_render_block_count; i++)
    {
        Poly_FT4* pPoly = &field_30_chnk_res[i].field_0_polys[gPsxDisplay_5C1130.field_C_buffer_index];
        const s32 xpos = X0(pPoly);
        const s32 ypos = Y0(pPoly);
        if (pScreenManager_5BB5F4->IsDirty_40EBC0(pScreenManager_5BB5F4->field_3A_idx, xpos, ypos) || pScreenManager_5BB5F4->IsDirty_40EBC0(3, xpos, ypos))
        {
            OrderingTable_Add_4F8AA0(OtLayer(ppOt, field_30_chnk_res[i].field_66_mapped_layer), &pPoly->mBase.header);
            // NOTE: Polygon has a pointer to the bit fields for which pixels should be skipped
            pScreenManager_5BB5F4->InvalidateRect_40EC90(xpos, ypos, X3(pPoly), Y3(pPoly), pScreenManager_5BB5F4->field_3A_idx);
        }
    }
}

void FG1::VScreenChanged()
{
    mFlags.Set(BaseGameObject::eDead);
}
