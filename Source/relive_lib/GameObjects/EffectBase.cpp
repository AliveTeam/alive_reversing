#include "stdafx.h"
#include "EffectBase.hpp"
#include "MapWrapper.hpp"
#include "PsxDisplay.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

EffectBase::EffectBase(Layer layer, relive::TBlendModes blendMode)
    : BaseGameObject(true, 0)
    , mBlendMode(blendMode)
{
    SetType(ReliveTypes::eEffectBase);
    gObjListDrawables->Push_Back(this);
    SetDrawable(true);
    mEffectBasePathId = GetMap().mCurrentPath;
    mEffectBaseLevelId = GetMap().mCurrentLevel;
    mEffectBaseLayer = layer;
    mSemiTrans = 1;
}

EffectBase::~EffectBase()
{
    gObjListDrawables->Remove_Item(this);
}

void EffectBase::VRender(BasePrimitive** ppOt)
{
    Poly_G4* pTile = &mEffectBaseTile[gPsxDisplay.mBufferIndex];
    pTile->SetRGB0(static_cast<u8>(mEffectBaseRed), static_cast<u8>(mEffectBaseGreen), static_cast<u8>(mEffectBaseBlue));
    pTile->SetRGB1(static_cast<u8>(mEffectBaseRed), static_cast<u8>(mEffectBaseGreen), static_cast<u8>(mEffectBaseBlue));
    pTile->SetRGB2(static_cast<u8>(mEffectBaseRed), static_cast<u8>(mEffectBaseGreen), static_cast<u8>(mEffectBaseBlue));
    pTile->SetRGB3(static_cast<u8>(mEffectBaseRed), static_cast<u8>(mEffectBaseGreen), static_cast<u8>(mEffectBaseBlue));  
    pTile->SetXYWH(0, 0, gPsxDisplay.mWidth, gPsxDisplay.mHeight);
    pTile->SetBlendMode(mBlendMode);

    if (mEffectBaseRed || mEffectBaseGreen || mEffectBaseBlue || !mSemiTrans)
    {
        pTile->SetSemiTransparent(mSemiTrans);
        OrderingTable_Add(OtLayer(ppOt, mEffectBaseLayer), pTile);
    }
}
