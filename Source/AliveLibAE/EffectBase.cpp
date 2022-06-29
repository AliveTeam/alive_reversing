#include "stdafx.h"
#include "EffectBase.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "ScreenManager.hpp"
#include "PsxDisplay.hpp"
#include "stdlib.hpp"

EffectBase::EffectBase(Layer layer, TPageAbr abr)
    : BaseGameObject(TRUE, 0)
{
    SetType(ReliveTypes::eEffectBase);
    gObjListDrawables->Push_Back(this);
    mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4);
    mEffectBasePathId = gMap.mCurrentPath;
    mEffectBaseLevelId = gMap.mCurrentLevel;
    Init_SetTPage(&mEffectBaseTPage[0], 0, 0, PSX_getTPage(TPageMode::e16Bit_2, abr, 0, 0));
    Init_SetTPage(&mEffectBaseTPage[1], 0, 0, PSX_getTPage(TPageMode::e16Bit_2, abr, 0, 0));
    mEffectBaseLayer = layer;
    mSemiTrans = 1;
}

EffectBase::~EffectBase()
{
    gObjListDrawables->Remove_Item(this);
}

void EffectBase::VRender(PrimHeader** ppOt)
{
    Prim_Tile* pTile = &mEffectBaseTile[gPsxDisplay.mBufferIndex];
    Init_Tile(pTile);
    SetRGB0(pTile,
            static_cast<u8>(mEffectBaseRed),
            static_cast<u8>(mEffectBaseGreen),
            static_cast<u8>(mEffectBaseBlue));
    SetXY0(pTile, 0, 0);
    pTile->field_14_w = 640;
    pTile->field_16_h = gPsxDisplay.mHeight;

    if (mEffectBaseRed || mEffectBaseGreen || mEffectBaseBlue || !mSemiTrans)
    {
        Poly_Set_SemiTrans(&pTile->mBase.header, mSemiTrans);
        OrderingTable_Add(OtLayer(ppOt, mEffectBaseLayer), &pTile->mBase.header);
        OrderingTable_Add(OtLayer(ppOt, mEffectBaseLayer), &mEffectBaseTPage[gPsxDisplay.mBufferIndex].mBase);
        if (!dword_5CA4D4 || !(PSX_getTPage(TPageMode::e4Bit_0, TPageAbr::eBlend_1, 0, 0) & mEffectBaseTPage[0].field_C_tpage))
        {
            pScreenManager->InvalidateRectCurrentIdx(
                0,
                0,
                640,
                gPsxDisplay.mHeight);
        }
    }
}
