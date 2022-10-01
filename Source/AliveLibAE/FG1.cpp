#include "stdafx.h"
#include "FG1.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "Map.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "../relive_lib/PsxDisplay.hpp"
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

    void OnPartialChunk(const Fg1Chunk& /*rChunk*/) override
    {
        //Fg1Block* pRenderBlock = &mFg1.field_30_chnk_res[mIdx++];
        //mFg1.Convert_Chunk_To_Render_Block(&rChunk, pRenderBlock);
    }

    void OnFullChunk(const Fg1Chunk& /*rChunk*/) override
    {
        /*
        pScreenManager->InvalidateRect_IdxPlus4(
            rChunk.field_4_xpos_or_compressed_size,
            rChunk.field_6_ypos,
            rChunk.field_8_width + rChunk.field_4_xpos_or_compressed_size - 1,
            rChunk.field_A_height + rChunk.field_6_ypos - 1,
            rChunk.field_2_layer_or_decompressed_size);*/
    }

    u8** Allocate(u32 len) override
    {
        // Shouldn't be called for AE
        return ResourceManager::Allocate_New_Locked_Resource(
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

        Poly_Set_SemiTrans(&rPoly.mBase.header, FALSE);
        Poly_Set_Blending(&rPoly.mBase.header, TRUE);

        SetTPage(&rPoly, static_cast<u16>(PSX_getTPage(TPageMode::e16Bit_2, TPageAbr::eBlend_0, 0, 0)));

        SetXYWH(&rPoly, pChunk->field_4_xpos_or_compressed_size, pChunk->field_6_ypos, pChunk->field_8_width, pChunk->field_A_height);

        SetPrimExtraPointerHack(&rPoly, pBlock->field_68_array_of_height);
    }
    return 1;
}

FG1::~FG1()
{
    gFG1List_5D1E28->Remove_Item(this);
    //ResourceManager::FreeResource_49C330(field_2C_ptr);
}

FG1::FG1(Fg1Resource& pFg1Res, CamResource& camRes)
    : BaseGameObject(TRUE, 0)
    , mFG1Res(pFg1Res)
    , mCamRes(camRes)
{
    mBaseGameObjectFlags.Set(Options::eDrawable_Bit4);
    mBaseGameObjectFlags.Set(Options::eSurviveDeathReset_Bit9);
    mBaseGameObjectFlags.Set(Options::eUpdateDuringCamSwap_Bit10);

    SetType(ReliveTypes::eFG1);

    field_24_level_id = gMap.mCurrentLevel;
    field_26_path_id = gMap.mCurrentPath;

    gFG1List_5D1E28->Push_Back(this);
    /*
    // TODO: render FG1 as 1 huge texture
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
    field_2C_ptr = ResourceManager::Allocate_New_Locked_Resource(ResourceManager::Resource_CHNK, 0, pHeader->mCount * sizeof(Fg1Block));
    field_30_chnk_res = reinterpret_cast<Fg1Block*>(*field_2C_ptr);

    FG1Reader loader(*this);
    loader.Iterate(pHeader);
    */
}




void FG1::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

void FG1::VRender(PrimHeader** ppOt)
{
    if (mFG1Res.Any())
    {
        if (mFG1Res.mBg.mImage.mPixels)
        {
            PolyFT4_Init(&mPolys[0]);
            mPolys[0].mFg1 = &mFG1Res.mBg;
            mPolys[0].mCam = &mCamRes;
            SetRGB0(&mPolys[0], 127, 127, 127);
            SetXYWH(&mPolys[0], 0, 0, 640, 240);
            OrderingTable_Add(OtLayer(ppOt, Layer::eLayer_FG1_Half_18), &mPolys[0].mBase.header);
        }

        if (mFG1Res.mFg.mImage.mPixels)
        {
            PolyFT4_Init(&mPolys[1]);
            mPolys[1].mFg1 = &mFG1Res.mFg;
            mPolys[1].mCam = &mCamRes;
            SetRGB0(&mPolys[1], 127, 127, 127);
            SetXYWH(&mPolys[1], 0, 0, 640, 240);
            OrderingTable_Add(OtLayer(ppOt, Layer::eLayer_FG1_37), &mPolys[1].mBase.header);
        }

        if (mFG1Res.mBgWell.mImage.mPixels)
        {
            PolyFT4_Init(&mPolys[2]);
            mPolys[2].mFg1 = &mFG1Res.mBgWell;
            mPolys[2].mCam = &mCamRes;
            SetRGB0(&mPolys[2], 127, 127, 127);
            SetXYWH(&mPolys[2], 0, 0, 640, 240);
            OrderingTable_Add(OtLayer(ppOt, Layer::eLayer_Well_Half_4), &mPolys[2].mBase.header);
        }

        if (mFG1Res.mFgWell.mImage.mPixels)
        {
            PolyFT4_Init(&mPolys[3]);
            mPolys[3].mFg1 = &mFG1Res.mFgWell;
            mPolys[3].mCam = &mCamRes;
            SetRGB0(&mPolys[3], 127, 127, 127);
            SetXYWH(&mPolys[3], 0, 0, 640, 240);
            OrderingTable_Add(OtLayer(ppOt, Layer::eLayer_Well_23), &mPolys[3].mBase.header);
        }
    }

    /*
    for (s32 i = 0; i < field_28_render_block_count; i++)
    {
        Poly_FT4* pPoly = &field_30_chnk_res[i].field_0_polys[gPsxDisplay.mBufferIndex];
        const s32 xpos = X0(pPoly);
        const s32 ypos = Y0(pPoly);
        if (pScreenManager->IsDirty(pScreenManager->Idx(), xpos, ypos) || pScreenManager->IsDirty(3, xpos, ypos))
        {
            OrderingTable_Add(OtLayer(ppOt, field_30_chnk_res[i].field_66_mapped_layer), &pPoly->mBase.header);
            // NOTE: Polygon has a pointer to the bit fields for which pixels should be skipped
            pScreenManager->InvalidateRectCurrentIdx(xpos, ypos, X3(pPoly), Y3(pPoly));
        }
    }*/
}