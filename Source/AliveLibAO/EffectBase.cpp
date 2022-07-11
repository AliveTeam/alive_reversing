#include "stdafx_ao.h"
#include "EffectBase.hpp"
#include "Function.hpp"
#include "../relive_lib/DynamicArray.hpp"
#include "Map.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "../relive_lib/PsxDisplay.hpp"

namespace AO {

EffectBase::EffectBase(Layer layer, TPageAbr abr)
    : BaseGameObject(TRUE, 0)
{
    mBaseGameObjectTypeId = ReliveTypes::eEffectBase;
    gObjListDrawables->Push_Back(this);
    mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4);
    mEffectBasePathId = gMap.mCurrentPath;
    mEffectBaseLevelId = gMap.mCurrentLevel;
    for (s32 i = 0; i < 2; i++)
    {
        Init_SetTPage(&mEffectBaseTPage[i], 0, 0, static_cast<s16>(PSX_getTPage(TPageMode::e16Bit_2, abr, 0, 0)));
    }
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

    Poly_Set_SemiTrans(&pTile->mBase.header, mSemiTrans);
    OrderingTable_Add(OtLayer(ppOt, mEffectBaseLayer), &pTile->mBase.header);
    OrderingTable_Add(OtLayer(ppOt, mEffectBaseLayer), &mEffectBaseTPage[gPsxDisplay.mBufferIndex].mBase);
    pScreenManager->InvalidateRectCurrentIdx(0, 0, 640, gPsxDisplay.mHeight);
}

} // namespace AO
