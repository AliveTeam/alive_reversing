#include "stdafx.h"
#include "EffectBase.hpp"
#include "MapWrapper.hpp"
#include "PsxDisplay.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

EffectBase::EffectBase(Layer layer, TPageAbr abr)
    : BaseGameObject(true, 0)
{
    SetType(ReliveTypes::eEffectBase);
    gObjListDrawables->Push_Back(this);
    mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4);
    mEffectBasePathId = GetMap().mCurrentPath;
    mEffectBaseLevelId = GetMap().mCurrentLevel;
    Init_SetTPage(&mEffectBaseTPage[0], 0, 0, PSX_getTPage(abr));
    Init_SetTPage(&mEffectBaseTPage[1], 0, 0, PSX_getTPage(abr));
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
    }
}
