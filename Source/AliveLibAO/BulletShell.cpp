#include "stdafx_ao.h"
#include "BulletShell.hpp"
#include "Math.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Map.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Sfx.hpp"
#include "../AliveLibAE/FixedPoint.hpp"

namespace AO {

BulletShell::BulletShell(FP xpos, FP ypos, s32 direction, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0)
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

    mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);
    GetAnimation().mFlags.Set(AnimFlags::eFlipX, direction & 1);

    mFloorBounceCount = 0;

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
    mSpeed = FP_FromInteger(1);
}

void BulletShell::VUpdate()
{
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
            GetSpriteScale() != FP_FromDouble(0.5) ? kFgWallsOrFloor : kBgWallsOrFloor)
        == 1)
    {
        if (mLine->mLineType == eLineTypes ::eFloor_0 ||
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

} // namespace AO
