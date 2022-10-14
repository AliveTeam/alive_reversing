#include "stdafx.h"
#include "BulletShell.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Sfx.hpp"
#include "Map.hpp"

s16 sShellCount_BAF7E0 = 0;

BulletShell::BulletShell(FP xpos, FP ypos, s16 direction, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    sShellCount_BAF7E0++;

    if (sShellCount_BAF7E0 >= 11)
    {
        mBaseGameObjectFlags.Clear(BaseGameObject::eDrawable_Bit4);
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
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

        mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);
        GetAnimation().mFlags.Set(AnimFlags::eFlipX, direction & 1);

        field_FC_hitCount = 0;

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
        field_100_speed = FP_FromInteger(1);
    }
}

BulletShell::~BulletShell()
{
    sShellCount_BAF7E0--;
}

void BulletShell::VUpdate()
{
    if (mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        return;
    }

    mXPos += mVelX;
    mYPos += mVelY;

    mVelY += field_100_speed;

    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
            mXPos,
            mYPos - mVelY,
            mXPos,
            mYPos,
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            GetScale() == Scale::Fg ? kFgFloorCeilingOrWalls : kBgFloorCeilingOrWalls)
        == 1)
    {
        if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eFloor_0 ||
            BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundFloor_4)
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

            s16 volume = 19 * (3 - field_FC_hitCount);
            if (volume <= 19)
            {
                volume = 19;
            }

            SfxPlayMono(relive::SoundEffects::BulletShell, volume);
            field_FC_hitCount++;
        }
    }

    if (!gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (field_FC_hitCount >= 3)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}
