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
    SetType(AETypes::eEffectBase_115);
    gObjList_drawables_5C1124->Push_Back(this);
    mGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4);
    mPathId = gMap.mCurrentPath;
    mLevelId = gMap.mCurrentLevel;
    Init_SetTPage_4F5B60(&mTPage[0], 0, 0, PSX_getTPage_4F60E0(TPageMode::e16Bit_2, abr, 0, 0));
    Init_SetTPage_4F5B60(&mTPage[1], 0, 0, PSX_getTPage_4F60E0(TPageMode::e16Bit_2, abr, 0, 0));
    mLayer = layer;
    mSemiTrans = 1;
}

EffectBase::~EffectBase()
{
    gObjList_drawables_5C1124->Remove_Item(this);
}

void EffectBase::VRender(PrimHeader** ppOt)
{
    Prim_Tile* pTile = &mTile[gPsxDisplay_5C1130.field_C_buffer_index];
    Init_Tile(pTile);
    SetRGB0(pTile,
            static_cast<u8>(mRed),
            static_cast<u8>(mGreen),
            static_cast<u8>(mBlue));
    SetXY0(pTile, 0, 0);
    pTile->field_14_w = 640;
    pTile->field_16_h = gPsxDisplay_5C1130.field_2_height;

    if (mRed || mGreen || mBlue || !mSemiTrans)
    {
        Poly_Set_SemiTrans_4F8A60(&pTile->mBase.header, mSemiTrans);
        OrderingTable_Add_4F8AA0(OtLayer(ppOt, mLayer), &pTile->mBase.header);
        OrderingTable_Add_4F8AA0(OtLayer(ppOt, mLayer), &mTPage[gPsxDisplay_5C1130.field_C_buffer_index].mBase);
        if (!dword_5CA4D4 || !(PSX_getTPage_4F60E0(TPageMode::e4Bit_0, TPageAbr::eBlend_1, 0, 0) & mTPage[0].field_C_tpage))
        {
            pScreenManager_5BB5F4->InvalidateRect_40EC10(
                0,
                0,
                640,
                gPsxDisplay_5C1130.field_2_height);
        }
    }
}
