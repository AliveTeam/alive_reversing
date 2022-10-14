#include "stdafx_ao.h"
#include "Function.hpp"
#include "Leaf.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Math.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Map.hpp"

namespace AO {

u8 sLeafRandIdx_4D148C = 8;

void Leaf::VUpdate()
{
    mVelY += FP_FromDouble(0.5);

    mVelX = mVelX * FP_FromDouble(0.8);
    mVelY = mVelY * FP_FromDouble(0.8);

    const s32 randX = gRandomBytes[sLeafRandIdx_4D148C++] - 127;
    mVelX += mSpriteScale * (FP_FromInteger(randX) / FP_FromInteger(64));

    const s32 randY = gRandomBytes[sLeafRandIdx_4D148C++] - 127;
    mVelY += (mSpriteScale * (FP_FromInteger(randY) / FP_FromInteger(64)));

    const FP x2 = mVelX + mXPos;
    const FP y2 = mVelY + mYPos;

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    const auto bCollision = sCollisions->Raycast(
        mXPos,
        mYPos,
        x2,
        y2,
        &pLine,
        &hitX,
        &hitY,
        kFgOrBgFloor);

    // Hit the floor, die but only if in background..
    if (bCollision && mSpriteScale == FP_FromDouble(0.5) && pLine->mLineType == eLineTypes::eFloor_0)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        return;
    }

    if (field_E4_bHitSomething & 1 || !bCollision || 
        (
        (mSpriteScale != FP_FromDouble(0.5) || pLine->mLineType != eLineTypes::eBackgroundFloor_4) &&
        (mSpriteScale != FP_FromInteger(1) || pLine->mLineType != eLineTypes::eFloor_0))
        )
    {
        mXPos = x2;
        mYPos = y2;
    }
    else
    {
        mVelX = FP_FromInteger(0);
        mVelY = FP_FromInteger(0);

        field_E4_bHitSomething |= 1;

        mXPos = hitX;
        mYPos = hitY;
    }

    // Out of the camera, die
    if (!gMap.Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
            0))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void Leaf::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

Leaf::Leaf(FP xpos, FP ypos, FP xVel, FP yVel, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    mRGB.SetRGB(100, 100, 100);

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Well_Leaf));
    Animation_Init(GetAnimRes(AnimId::Well_Leaf));

    mSpriteScale = scale;
    if (mSpriteScale == FP_FromInteger(1))
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_27);
        mScale = Scale::Fg;
    }
    else
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_8);
        mScale = Scale::Bg;
    }

    mXPos = xpos;
    mYPos = ypos;

    mVelX = xVel * mSpriteScale;
    mVelY = yVel * mSpriteScale;

    sLeafRandIdx_4D148C++;

    field_E4_bHitSomething &= ~1u;

    s16 randLeftVol = Math_RandomRange(19, 24);
    if (mSpriteScale == FP_FromDouble(0.4)) // ??
    {
        randLeftVol -= 7;
    }

    const s16 randRightVol = Math_RandomRange(-900, -700);
    SFX_Play_Pitch(relive::SoundEffects::Leaf, (3 * randLeftVol) / 4, randRightVol);
}

} // namespace AO
