#include "stdafx_ao.h"
#include "EffectBase.hpp"
#include "Function.hpp"
#include "DynamicArray.hpp"
#include "Map.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "ScreenManager.hpp"
#include "PsxDisplay.hpp"

namespace AO {

EffectBase::EffectBase(Layer layer, TPageAbr abr)
    : BaseGameObject(TRUE)
{
    mBaseGameObjectTypeId = ReliveTypes::eEffectBase;
    gObjListDrawables->Push_Back(this);
    mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4);
    mEffectBasePathId = gMap.mCurrentPath;
    mEffectBaseLevelId = gMap.mCurrentLevel;
    for (s32 i = 0; i < 2; i++)
    {
        Init_SetTPage_495FB0(&mEffectBaseTPage[i], 0, 0, static_cast<s16>(PSX_getTPage_4965D0(TPageMode::e16Bit_2, abr, 0, 0)));
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
    Prim_Tile* pTile = &mEffectBaseTile[gPsxDisplay_504C78.field_A_buffer_index];
    Init_Tile(pTile);

    SetRGB0(pTile,
            static_cast<u8>(mEffectBaseRed),
            static_cast<u8>(mEffectBaseGreen),
            static_cast<u8>(mEffectBaseBlue));
    SetXY0(pTile, 0, 0);

    pTile->field_14_w = 640;
    pTile->field_16_h = gPsxDisplay_504C78.field_2_height;

    Poly_Set_SemiTrans_498A40(&pTile->mBase.header, mSemiTrans);
    OrderingTable_Add_498A80(OtLayer(ppOt, mEffectBaseLayer), &pTile->mBase.header);
    OrderingTable_Add_498A80(OtLayer(ppOt, mEffectBaseLayer), &mEffectBaseTPage[gPsxDisplay_504C78.field_A_buffer_index].mBase);
    pScreenManager_4FF7C8->InvalidateRectCurrentIdx(0, 0, 640, gPsxDisplay_504C78.field_2_height);
}

} // namespace AO
