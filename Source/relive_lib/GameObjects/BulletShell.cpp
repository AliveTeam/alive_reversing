#include "stdafx.h"
#include "BulletShell.hpp"
#include "Math.hpp"
#include "../../AliveLibAE/stdlib.hpp"
#include "../MapWrapper.hpp"
#include "../Collisions.hpp"
#include "Sfx.hpp"
#include "../FixedPoint.hpp"
#include "../GameType.hpp"

static s16 sShellCount = 0;

BulletShell::BulletShell(FP xpos, FP ypos, bool flipX, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    sShellCount++;

    if (sShellCount >= 11 && GetGameType() == GameType::eAe)
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
        GetAnimation().SetFlipX(flipX);

        mXPos = xpos;
        mYPos = ypos;

        if (flipX)
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
    if (GetDead() && GetGameType() == GameType::eAe)
    {
        return;
    }

    mXPos += mVelX;
    mYPos += mVelY;

    mVelY += mSpeed;

    const CollisionMask aeMask = GetScale() == Scale::Fg ? kFgFloorCeilingOrWalls : kBgFloorCeilingOrWalls;
    const CollisionMask aoMask = GetSpriteScale() != FP_FromDouble(0.5) ? kFgWallsOrFloor : kBgWallsOrFloor;

    const CollisionMask usedMask = GetGameType() == GameType::eAe ? aeMask : aoMask;

    FP hitX = {};
    FP hitY = {};
    if (gCollisions->Raycast(
            mXPos,
            mYPos - mVelY,
            mXPos,
            mYPos,
            &mLine,
            &hitX,
            &hitY,
            usedMask)
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

    if (!GetMap().Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
    {
        SetDead(true);
    }

    if (mFloorBounceCount >= 3)
    {
        SetDead(true);
    }
}
