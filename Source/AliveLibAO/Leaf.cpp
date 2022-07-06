#include "stdafx_ao.h"
#include "Function.hpp"
#include "Leaf.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Math.hpp"
#include "Sfx.hpp"
#include "Collisions.hpp"
#include "Map.hpp"

namespace AO {

ALIVE_VAR(1, 0x4D148C, u8, sLeafRandIdx_4D148C, 8);

void Leaf::VUpdate()
{
    mVelY += FP_FromDouble(0.5);

    mVelX = mVelX * FP_FromDouble(0.8);
    mVelY = mVelY * FP_FromDouble(0.8);

    const s32 randX = sRandomBytes_4BBE30[sLeafRandIdx_4D148C++] - 127;
    mVelX += mSpriteScale * (FP_FromInteger(randX) / FP_FromInteger(64));

    const s32 randY = sRandomBytes_4BBE30[sLeafRandIdx_4D148C++] - 127;
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
{
    mRGB.SetRGB(100, 100, 100);

    const AnimRecord& leafRec = AO::AnimRec(AnimId::Well_Leaf);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, leafRec.mResourceId, 1, 0);
    if (!ppRes)
    {
        return;
    }

    Animation_Init(AnimId::Well_Leaf, ppRes);

    mSpriteScale = scale;
    if (mSpriteScale == FP_FromInteger(1))
    {
        mAnim.mRenderLayer = Layer::eLayer_27;
        mScale = Scale::Fg;
    }
    else
    {
        mAnim.mRenderLayer = Layer::eLayer_8;
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
    SFX_Play_Pitch(SoundEffect::Leaf_27, (3 * randLeftVol) / 4, randRightVol, nullptr);
}

} // namespace AO
