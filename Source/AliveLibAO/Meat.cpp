#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "Meat.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/Shadow.hpp"
#include "Game.hpp"
#include "ThrowableArray.hpp"
#include "Abe.hpp"
#include "Sfx.hpp"
#include "Math.hpp"
#include "../relive_lib/Events.hpp"
#include "PlatformBase.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Throwable.hpp"
#include "CameraSwapper.hpp"
#include "../relive_lib/GameObjects/Particle.hpp"
#include "Grid.hpp"
#include "PathData.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "../relive_lib/FixedPoint.hpp"
#include "Path.hpp"

namespace AO {

void Meat::VScreenChanged()
{
    if (gMap.mCurrentPath != gMap.mNextPath || gMap.mCurrentLevel != gMap.mNextLevel)
    {
        SetDead(true);
    }
}

Meat::~Meat()
{
    if (!mBaseThrowableDead)
    {
        if (gThrowableArray)
        {
            gThrowableArray->Remove(mBaseThrowableCount >= 1 ? mBaseThrowableCount : 1);
        }
    }
}

void Meat::VThrow(FP velX, FP velY)
{
    GetAnimation().SetRender(true);

    mVelX = velX;
    mVelY = velY;

    if (mBaseThrowableCount == 0)
    {
        mState = 2;
    }
    else
    {
        mState = 1;
    }
}

bool Meat::VCanThrow()
{
    return mState == 2;
}

bool Meat::VCanEatMe()
{
    return mState != 0;
}

bool Meat::VIsFalling()
{
    return mState == 5;
}

void Meat::VTimeToExplodeRandom()
{
    // Empty
}

void Meat::InTheAir()
{
    mPreviousXPos = mXPos;
    mPreviousYPos = mYPos;

    if (mVelY < FP_FromInteger(18))
    {
        mVelY += FP_FromInteger(1);
    }

    mXPos += mVelX;
    mYPos += mVelY;

    u16 result = 0;
    const FP xVoidSkip = CamX_VoidSkipper(mXPos, mVelX, 8, &result);
    mXPos = xVoidSkip;

    if (result)
    {
        mPreviousXPos = xVoidSkip - mVelX;
    }

    const FP yVoidSkip = CamY_VoidSkipper(mYPos, mVelY, 8, &result);
    mYPos = yVoidSkip;

    if (result)
    {
        mPreviousYPos = yVoidSkip - mVelY;
    }

    FP hitX = {};
    FP hitY = {};

    const s16 CollisionRaycast = sCollisions->Raycast(
        mPreviousXPos,
        mPreviousYPos,
        xVoidSkip,
        yVoidSkip,
        &mPathLine,
        &hitX,
        &hitY,
        GetSpriteScale() == FP_FromInteger(1) ? kFgWallsOrFloor : kBgWallsOrFloor) ? 1 : 0;


    if (CollisionRaycast == 1)
    {
        switch (mPathLine->mLineType)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eDynamicCollision_32:
            case eLineTypes::eBackgroundDynamicCollision_36:
                if (mVelY > FP_FromInteger(0))
                {
                    mState = 3;

                    mXPos = FP_FromInteger(SnapToXGrid(GetSpriteScale(), FP_GetExponent(hitX)));
                    mYPos = hitY;

                    mVelY = FP_FromInteger(0);
                    mVelX = FP_FromInteger(0);

                    SFX_Play_Pitch(relive::SoundEffects::MeatBounce, 0, -650);
                    EventBroadcast(kEventNoise, this);
                    EventBroadcast(kEventSuspiciousNoise, this);
                    AddToPlatform();
                }
                break;

            case eLineTypes::eWallLeft_1:
            case eLineTypes::eBackgroundWallLeft_5:
                if (mVelX >= FP_FromInteger(0))
                {
                    mPathLine = nullptr;
                    break;
                }

                mVelX = (-mVelX / FP_FromInteger(2));

                SFX_Play_Pitch(relive::SoundEffects::MeatBounce, 0, -650);
                EventBroadcast(kEventNoise, this);
                EventBroadcast(kEventSuspiciousNoise, this);

                if (mVelY >= FP_FromInteger(0))
                {
                    mPathLine = nullptr;
                    break;
                }

                mVelY = FP_FromInteger(0);
                mPathLine = nullptr;
                break;

            case eLineTypes::eWallRight_2:
            case eLineTypes::eBackgroundWallRight_6:
                if (mVelX > FP_FromInteger(0))
                {
                    mVelX = (-mVelX / FP_FromInteger(4));

                    SFX_Play_Pitch(relive::SoundEffects::MeatBounce, 0, -650);
                    EventBroadcast(kEventNoise, this);
                    EventBroadcast(kEventSuspiciousNoise, this);

                    if (mVelY < FP_FromInteger(0))
                    {
                        mVelY = FP_FromInteger(0);
                    }
                }

                mPathLine = nullptr;
                break;

            default:
                return;
        }
    }
}

void Meat::VUpdate()
{
    if (gNumCamSwappers == 0)
    {
        if (EventGet(kEventDeathReset))
        {
            SetDead(true);
        }

        // TODO: states enum
        switch (mState)
        {
            case 1:
                InTheAir();
                break;

            case 2:
            {
                InTheAir();
                const PSX_RECT bRect = VGetBoundingRect();

                const PSX_Point xy = {bRect.x, static_cast<s16>(bRect.y + 5)};
                const PSX_Point wh = {bRect.w, static_cast<s16>(bRect.h + 5)};

                OnCollisionWith(xy, wh, gBaseGameObjects);

                if (mYPos > FP_FromInteger(gMap.mPathData->field_A_bBottom))
                {
                    SetDead(true);
                }
                break;
            }

            case 3:
                if (FP_Abs(mVelX) < FP_FromInteger(1))
                {
                    GetAnimation().SetLoop(false);
                }

                if (FP_Abs(mVelX) >= FP_FromDouble(0.5))
                {
                    if (mVelX <= FP_FromInteger(0))
                    {
                        mVelX += FP_FromDouble(0.01);
                    }
                    else
                    {
                        mVelX -= FP_FromDouble(0.01);
                    }

                    mPathLine = mPathLine->MoveOnLine(&mXPos, &mYPos, mVelX);
                    if (!mPathLine)
                    {
                        mState = 2;
                        GetAnimation().SetLoop(true);
                    }
                }
                else
                {
                    mVelX = FP_FromInteger(0);
                    mCollectionRect.x = mXPos - ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2);
                    mCollectionRect.y = mYPos - ScaleToGridSize(GetSpriteScale());
                    mCollectionRect.w = mXPos + ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2);
                    mCollectionRect.h = mYPos;

                    SetInteractive(true);
                    mState = 4;
                }
                break;

            case 4:
                if (gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
                {
                    mDeadTimer = sGnFrame + 600;
                }

                if (static_cast<s32>(sGnFrame) > mShimmerTimer)
                {
                    New_TintShiny_Particle(
                        mXPos + GetSpriteScale(),
                        mYPos + (GetSpriteScale() * FP_FromInteger(-7)),
                        FP_FromDouble(0.3),
                        Layer::eLayer_Foreground_36);
                    mShimmerTimer = Math_NextRandom() % 16 + sGnFrame + 60;
                }
                if (mDeadTimer < static_cast<s32>(sGnFrame))
                {
                    SetDead(true);
                }
                break;

            case 5:
                mVelY += FP_FromInteger(1);
                mXPos += mVelX;
                mYPos += mVelY;

                if (!gMap.Is_Point_In_Current_Camera(
                        mCurrentLevel,
                        mCurrentPath,
                        mXPos,
                        mVelY + mYPos,
                        0))
                {
                    SetDead(true);
                }
                break;

            default:
                return;
        }
    }
}

bool Meat::OnCollision(BaseAnimatedWithPhysicsGameObject* pHit)
{
    if (!pHit->GetCanExplode())
    {
        return true;
    }

    if (pHit->Type() == ReliveTypes::eMine || pHit->Type() == ReliveTypes::eUXB || pHit->Type() == ReliveTypes::eTimedMine)
    {
        return true;
    }

    const PSX_RECT bRect = pHit->VGetBoundingRect();

    if (mPreviousXPos < FP_FromInteger(bRect.x) || mPreviousXPos > FP_FromInteger(bRect.w))
    {
        mXPos -= mVelX;
        mVelX = (-mVelX / FP_FromInteger(2));
    }
    else
    {
        mYPos -= mVelY;
        mVelY = (-mVelY / FP_FromInteger(2));
    }

    pHit->VOnThrowableHit(this);
    SFX_Play_Pitch(relive::SoundEffects::MeatBounce, 0, -650);
    return false;
}

void Meat::AddToPlatform()
{
    BaseAddToPlatform();
}


s16 Meat::VGetCount()
{
    if (mState == 4 && mBaseThrowableCount == 0)
    {
        return 1;
    }

    return mBaseThrowableCount;
}

void Meat::VOnTrapDoorOpen()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        BaseAliveGameObject_PlatformId = Guid{};
        if (mState == 3 || mState == 4)
        {
            mState = 1;
        }
    }
}

} // namespace AO
