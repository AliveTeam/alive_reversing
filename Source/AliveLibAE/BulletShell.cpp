#include "stdafx.h"
#include "BulletShell.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Sfx.hpp"
#include "Map.hpp"
#include "../relive_lib/FixedPoint.hpp"
#include "Math.hpp"

static s16 sShellCount = 0;

BulletShell::BulletShell(FP xpos, FP ypos, s16 direction, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    sShellCount++;

    if (sShellCount >= 11)
    {
        SetDrawable(false);
        SetDead(true);
    }
    else
    {
        SetType(ReliveTypes::eNone);
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Bullet_Shell));
        Animation_Init(GetAnimRes(AnimId::Bullet_Shell));

        SetSpriteScale(scale);

        if (scale == FP_FromInteger(1))
        {
            GetAnimation().SetRenderLayer(Layer::eLayer_Foreground_36);
        }
        else
        {
            GetAnimation().SetRenderLayer(Layer::eLayer_Foreground_Half_17);
        }

        SetApplyShadowZoneColour(false);
        GetAnimation().SetFlipX(direction & 1);

        mXPos = xpos;
        mYPos = ypos;

        if (direction)
        {
            mVelX = FP_FromInteger(Math_RandomRange(-6, -3));
        }
        else
        {
            mVelX = FP_FromInteger(Math_RandomRange(3, 6));
        }
        mVelY = FP_FromInteger(Math_RandomRange(-4, -1));
    }
}

BulletShell::~BulletShell()
{
    sShellCount--;
}

void BulletShell::VUpdate()
{
    if (GetDead())
    {
        return;
    }

    mXPos += mVelX;
    mYPos += mVelY;

    mVelY += mSpeed;

    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
            mXPos,
            mYPos - mVelY,
            mXPos,
            mYPos,
            &mLine,
            &hitX,
            &hitY,
            GetScale() == Scale::Fg ? kFgFloorCeilingOrWalls : kBgFloorCeilingOrWalls)
        == 1)
    {
        if (mLine->mLineType == eLineTypes::eFloor_0 ||
            mLine->mLineType == eLineTypes::eBackgroundFloor_4)
        {
            mYPos = hitY - FP_FromInteger(1);
            mVelY = -(mVelY * FP_FromDouble(0.3));
            mVelX = (mVelX * FP_FromDouble(0.3));

            if (mVelX < FP_FromInteger(0) && mVelX > FP_FromInteger(-1))
            {
                mVelX = FP_FromInteger(-1);
            }
            else if (mVelX > FP_FromInteger(0) && mVelX < FP_FromInteger(1))
            {
                mVelX = FP_FromInteger(1);
            }

            s16 volume = 19 * (3 - mFloorBounceCount);
            if (volume <= 19)
            {
                volume = 19;
            }

            SfxPlayMono(relive::SoundEffects::BulletShell, volume);
            mFloorBounceCount++;
        }
    }

    if (!gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
    {
        SetDead(true);
    }

    if (mFloorBounceCount >= 3)
    {
        SetDead(true);
    }
}
