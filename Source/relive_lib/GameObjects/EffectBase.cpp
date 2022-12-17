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
    SetDrawable(true);
    mEffectBasePathId = GetMap().mCurrentPath;
    mEffectBaseLevelId = GetMap().mCurrentLevel;
    Init_SetTPage(&mEffectBaseTPage[0], PSX_getTPage(abr));
    Init_SetTPage(&mEffectBaseTPage[1], PSX_getTPage(abr));
    mEffectBaseLayer = layer;
    mSemiTrans = 1;
}

EffectBase::~EffectBase()
{
    gObjListDrawables->Remove_Item(this);
}

void EffectBase::VRender(PrimHeader** ppOt)
{
    Poly_G4* pTile = &mEffectBaseTile[gPsxDisplay.mBufferIndex];
    PolyG4_Init(pTile);
    SetRGB0(pTile, static_cast<u8>(mEffectBaseRed), static_cast<u8>(mEffectBaseGreen), static_cast<u8>(mEffectBaseBlue));
    SetRGB1(pTile, static_cast<u8>(mEffectBaseRed), static_cast<u8>(mEffectBaseGreen), static_cast<u8>(mEffectBaseBlue));
    SetRGB2(pTile, static_cast<u8>(mEffectBaseRed), static_cast<u8>(mEffectBaseGreen), static_cast<u8>(mEffectBaseBlue));
    SetRGB3(pTile, static_cast<u8>(mEffectBaseRed), static_cast<u8>(mEffectBaseGreen), static_cast<u8>(mEffectBaseBlue));  
    SetXYWH(pTile, 0, 0, gPsxDisplay.mWidth, gPsxDisplay.mHeight);

    if (mEffectBaseRed || mEffectBaseGreen || mEffectBaseBlue || !mSemiTrans)
    {
        Poly_Set_SemiTrans(&pTile->mBase.header, mSemiTrans);
        OrderingTable_Add(OtLayer(ppOt, mEffectBaseLayer), &pTile->mBase.header);
        OrderingTable_Add(OtLayer(ppOt, mEffectBaseLayer), &mEffectBaseTPage[gPsxDisplay.mBufferIndex].mBase);
    }
}
