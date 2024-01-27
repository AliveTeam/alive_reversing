#include "stdafx.h"
#include "Leaf.hpp"
#include "Sfx.hpp"
#include "../Collisions.hpp"
#include "../FixedPoint.hpp"
#include "../GameType.hpp"

static u8 sLeafRandIdx = 8;

Leaf::Leaf(FP xpos, FP ypos, FP xVel, FP yVel, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    mRGB.SetRGB(100, 100, 100);

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Well_Leaf));
    Animation_Init(GetAnimRes(AnimId::Well_Leaf));

    SetSpriteScale(scale);

    if (GetSpriteScale() == FP_FromInteger(1))
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_27);
        SetScale(Scale::Fg);
    }
    else
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_8);
        SetScale(Scale::Bg);
    }

    mXPos = xpos;
    mYPos = ypos;

    mVelX = xVel * GetSpriteScale();
    mVelY = yVel * GetSpriteScale();

    sLeafRandIdx++;

    s16 randLeftVol = Math_RandomRange(19, 24);
    if (GetSpriteScale() == FP_FromDouble(0.5))
    {
        randLeftVol -= 7;
    }

    const s16 randRightVol = Math_RandomRange(-900, -700);
    SFX_Play_Pitch(relive::SoundEffects::Leaf, (3 * randLeftVol) / 4, randRightVol);

    if (GetGameType() == GameType::eAe)
    {
        SetUpdateDelay(1);
    }
}

void Leaf::VUpdate()
{
    mVelY += FP_FromDouble(0.5);

    mVelX = mVelX * FP_FromDouble(0.8);
    mVelY = mVelY * FP_FromDouble(0.8);

    const s32 randX = gRandomBytes[sLeafRandIdx++] - 127;
    mVelX += GetSpriteScale() * (FP_FromInteger(randX) / FP_FromInteger(64));

    const s32 randY = gRandomBytes[sLeafRandIdx++] - 127;
    mVelY += GetSpriteScale() * (FP_FromInteger(randY) / FP_FromInteger(64));

    const FP x2 = mVelX + mXPos;
    const FP y2 = mVelY + mYPos;

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    const auto bCollision = gCollisions->Raycast(
        mXPos,
        mYPos,
        x2,
        y2,
        &pLine,
        &hitX,
        &hitY,
        kFgOrBgFloor);

    // Hit the floor, die but only if in background..
    if (bCollision && GetSpriteScale() == FP_FromDouble(0.5) && pLine->mLineType == eLineTypes::eFloor_0)
    {
        SetDead(true);
        return;
    }

    if (mHitSomething || !bCollision || 
        (
        (GetSpriteScale() != FP_FromDouble(0.5) || pLine->mLineType != eLineTypes::eBackgroundFloor_4) &&
        (GetSpriteScale() != FP_FromInteger(1) || pLine->mLineType != eLineTypes::eFloor_0))
        )
    {
        mXPos = x2;
        mYPos = y2;
    }
    else
    {
        mVelX = FP_FromInteger(0);
        mVelY = FP_FromInteger(0);

        mHitSomething = true;

        mXPos = hitX;
        mYPos = hitY;
    }

    // Out of the camera, die
    if (!GetMap().Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
            0))
    {
        SetDead(true);
    }
}

void Leaf::VScreenChanged()
{
    SetDead(true);
}