#include "stdafx.h"
#include "Meat.hpp"
#include "../relive_lib/Function.hpp"
#include "../relive_lib/GameObjects/ThrowableArray.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "../relive_lib/GameObjects/PlatformBase.hpp"
#include "Sfx.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "../relive_lib/Events.hpp"
#include "Abe.hpp"
#include "../relive_lib/GameObjects/Particle.hpp"
#include "../relive_lib/Grid.hpp"
#include "Map.hpp"
#include "Path.hpp"
#include "PathData.hpp"
#include "../relive_lib/FixedPoint.hpp"
#include "Math.hpp"
#include "QuikSave.hpp"
#include "../relive_lib/Collisions.hpp"

Meat::Meat(FP xpos, FP ypos, s16 count)
{
    mBaseThrowableDead = 0;

    SetType(ReliveTypes::eMeat);

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Meat));
    Animation_Init(GetAnimRes(AnimId::Meat));

    GetAnimation().SetSemiTrans(false);

    mXPos = xpos;
    mYPos = ypos;

    mPreviousXPos = xpos;
    mPreviousYPos = ypos;

    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);
    mShimmerTimer = 0;
    SetInteractive(false);

    GetAnimation().SetRender(false);

    mDeadTimer = MakeTimer(600);
    mPathLine = nullptr;
    mBaseThrowableCount = count;
    mState = MeatStates::eCreated_0;

    CreateShadow();
}

void Meat::CreateFromSaveState(SerializedObjectData& pBuffer)
{
    const auto pState = pBuffer.ReadTmpPtr<MeatSaveState>();

    auto pMeat = relive_new Meat(pState->mXPos, pState->mYPos, pState->mThrowableCount);

    pMeat->mBaseGameObjectTlvInfo = pState->mTlvId;

    pMeat->mXPos = pState->mXPos;
    pMeat->mYPos = pState->mYPos;

    pMeat->mCollectionRect.x = pMeat->mXPos - (ScaleToGridSize(pMeat->GetSpriteScale()) / FP_FromInteger(2));
    pMeat->mCollectionRect.y = pMeat->mYPos - ScaleToGridSize(pMeat->GetSpriteScale());
    pMeat->mCollectionRect.w = (ScaleToGridSize(pMeat->GetSpriteScale()) / FP_FromInteger(2)) + pMeat->mXPos;
    pMeat->mCollectionRect.h = pMeat->mYPos;

    pMeat->mVelX = pState->mVelX;
    pMeat->mVelY = pState->mVelY;

    pMeat->mCurrentPath = pState->mCurrentPath;
    pMeat->mCurrentLevel = pState->mCurrentLevel;

    pMeat->SetSpriteScale(pState->mSpriteScale);

    pMeat->GetAnimation().SetLoop(pState->mLoop);
    pMeat->GetAnimation().SetRender(pState->mRender);

    pMeat->SetDrawable(pState->mDrawable);
    pMeat->SetInteractive(pState->mInteractive);

    pMeat->SetRestoredFromQuickSave(true);

    pMeat->mShimmerTimer = sGnFrame;
    pMeat->BaseAliveGameObjectCollisionLineType = pState->mLineType;

    pMeat->mBaseThrowableCount = pState->mThrowableCount;
    pMeat->mState = pState->mState;

    pMeat->mPreviousXPos = pState->mPreviousXPos;
    pMeat->mPreviousYPos = pState->mPreviousYPos;

    pMeat->mDeadTimer = pState->mDeadTimer;
}

void Meat::VTimeToExplodeRandom()
{
    // TODO
}

void Meat::VScreenChanged()
{
    if (gMap.PathChanged() || gMap.LevelChanged())
    {
        SetDead(true);
    }
}

void Meat::AddToPlatform()
{
    BaseAddToPlatform();
}

bool Meat::VIsFalling()
{
    return mState == MeatStates::eFall_5;
}

bool Meat::VCanThrow()
{
    return mState == MeatStates::eBeingThrown_2;
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
        mState = MeatStates::eBeingThrown_2;
    }
    else
    {
        mState = MeatStates::eIdle_1;
    }
}

s16 Meat::VGetCount()
{
    if (mState == MeatStates::eWaitForPickUp_4 && mBaseThrowableCount == 0)
    {
        return 1;
    }

    return mBaseThrowableCount;
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

    FP hitX = {};
    FP hitY = {};
    if (gCollisions->Raycast(mPreviousXPos, mPreviousYPos, mXPos, mYPos, &mPathLine, &hitX, &hitY, GetScale() == Scale::Fg ? kFgFloorCeilingOrWalls : kBgFloorCeilingOrWalls) == 1)
    {
        switch (mPathLine->mLineType)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eDynamicCollision_32:
            case eLineTypes::eBackgroundDynamicCollision_36:
                if (mVelY > FP_FromInteger(0))
                {
                    mXPos = FP_FromInteger(SnapToXGrid_AE(GetSpriteScale(), FP_GetExponent(hitX)));
                    mYPos = hitY;
                    mState = MeatStates::eBecomeAPickUp_3;
                    mVelY = FP_FromInteger(0);
                    mVelX = FP_FromInteger(0);
                    SFX_Play_Pitch(relive::SoundEffects::MeatBounce, 0, -650);
                    EventBroadcast(Event::kEventNoise, this);
                    EventBroadcast(Event::kEventSuspiciousNoise, this);
                    AddToPlatform();
                }
                break;

            case eLineTypes::eWallLeft_1:
            case eLineTypes::eBackgroundWallLeft_5:
                if (mVelX < FP_FromInteger(0))
                {
                    mXPos = hitX;
                    mYPos = hitY;
                    mVelX = (-mVelX / FP_FromInteger(4));
                    SFX_Play_Pitch(relive::SoundEffects::MeatBounce, 0, -650);
                    EventBroadcast(Event::kEventNoise, this);
                    EventBroadcast(Event::kEventSuspiciousNoise, this);
                    if (mVelY < FP_FromInteger(0))
                    {
                        mVelY = FP_FromInteger(0);
                    }
                }
                mPathLine = nullptr;
                break;

            case eLineTypes::eWallRight_2:
            case eLineTypes::eBackgroundWallRight_6:
                if (mVelX > FP_FromInteger(0))
                {
                    mXPos = hitX;
                    mYPos = hitY;
                    mVelX = (-mVelX / FP_FromInteger(4));
                    SFX_Play_Pitch(relive::SoundEffects::MeatBounce, 0, -650);
                    EventBroadcast(Event::kEventNoise, this);
                    EventBroadcast(Event::kEventSuspiciousNoise, this);
                    if (mVelY < FP_FromInteger(0))
                    {
                        mVelY = FP_FromInteger(0);
                    }
                }
                mPathLine = nullptr;
                break;

            case eLineTypes::eCeiling_3:
            case eLineTypes::eBackgroundCeiling_7:
                if (mVelY < FP_FromInteger(0))
                {
                    mXPos = hitX;
                    mYPos = hitY + FP_FromInteger(1);
                    mVelY = FP_FromInteger(0);
                    SFX_Play_Pitch(relive::SoundEffects::MeatBounce, 0, -650);
                    EventBroadcast(Event::kEventNoise, this);
                    EventBroadcast(Event::kEventSuspiciousNoise, this);
                }
                break;
        }
    }
}

bool Meat::OnCollision(BaseGameObject* pHit)
{
    // TODO: Check if pHit type is correct for all throwables

    if (!pHit->GetCanExplode())
    {
        return true;
    }

    if (pHit->Type() == ReliveTypes::eMine || pHit->Type() == ReliveTypes::eUXB || pHit->Type() == ReliveTypes::eTimedMine_or_MovingBomb)
    {
        return true;
    }

    const PSX_RECT bRect = static_cast<BaseAliveGameObject*>(pHit)->VGetBoundingRect();

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

    static_cast<BaseAliveGameObject*>(pHit)->VOnThrowableHit(this);

    SFX_Play_Pitch(relive::SoundEffects::MeatBounce, 0, -650);

    return false;
}

void Meat::VUpdate()
{
    const auto pPlatform = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
    if (gNumCamSwappers == 0)
    {
        if (EventGet(Event::kEventDeathReset))
        {
            SetDead(true);
        }

        switch (mState)
        {
            case MeatStates::eIdle_1:
                InTheAir();
                break;

            case MeatStates::eBeingThrown_2:
            {
                InTheAir();
                const PSX_RECT bRect = VGetBoundingRect();

                const PSX_Point xy = {bRect.x, static_cast<s16>(bRect.y + 5)};
                const PSX_Point wh = {bRect.w, static_cast<s16>(bRect.h + 5)};
                OnCollisionWith(
                    xy,
                    wh,
                    gBaseGameObjects);

                // TODO: OG bug - why only checking for out of the bottom of the map?? Nades check for death object - probably should check both
                if (mYPos > FP_FromInteger(gMap.mPathData->field_6_bBottom))
                {
                    SetDead(true);
                }
            }
            break;

            case MeatStates::eBecomeAPickUp_3:
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
                        GetAnimation().SetLoop(true);
                        mState = MeatStates::eBeingThrown_2;
                    }
                }
                else
                {
                    mVelX = FP_FromInteger(0);

                    mCollectionRect.x = mXPos - (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2));
                    mCollectionRect.y = mYPos - ScaleToGridSize(GetSpriteScale());
                    mCollectionRect.w = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2)) + mXPos;
                    mCollectionRect.h = mYPos;

                    SetInteractive(true);
                    mState = MeatStates::eWaitForPickUp_4;
                }
                break;

            case MeatStates::eWaitForPickUp_4:
                if (gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
                {
                    mDeadTimer = MakeTimer(600);
                }

                if (static_cast<s32>(sGnFrame) > mShimmerTimer && !pPlatform)
                {
                    // That strange "shimmer" the meat gives off
                    New_TintShiny_Particle(
                        (GetSpriteScale() * FP_FromInteger(1)) + mXPos,
                        mYPos + (GetSpriteScale() * FP_FromInteger(-7)),
                        FP_FromDouble(0.3),
                        Layer::eLayer_Foreground_36);
                    mShimmerTimer = Math_NextRandom() % 16 + MakeTimer(60);
                }
                if (mDeadTimer < static_cast<s32>(sGnFrame))
                {
                    SetDead(true);
                }
                break;

            case MeatStates::eFall_5:
                mVelY += FP_FromInteger(1);
                mXPos += mVelX;
                mYPos = mVelY + mYPos;
                if (!gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
                {
                    SetDead(true);
                }
                break;

            default:
                return;
        }
    }
}

void Meat::VGetSaveState(SerializedObjectData& pSaveBuffer)
{
    MeatSaveState data = {};

    data.mType = ReliveTypes::eMeat;
    data.mTlvId = mBaseGameObjectTlvInfo;

    data.mXPos = mXPos;
    data.mYPos = mYPos;

    data.mVelX = mVelX;
    data.mVelY = mVelY;

    data.mCurrentPath = mCurrentPath;
    data.mCurrentLevel = mCurrentLevel;

    data.mSpriteScale = GetSpriteScale();

    data.mLoop = GetAnimation().GetLoop();
    data.mRender = GetAnimation().GetRender();

    data.mDrawable = GetDrawable();
    data.mInteractive = GetInteractive();

    if (mPathLine)
    {
        data.mLineType = mPathLine->mLineType;
    }
    else
    {
        data.mLineType = eLineTypes::eNone_m1;
    }

    data.mPlatformId = BaseAliveGameObject_PlatformId;
    data.mThrowableCount = mBaseThrowableCount;
    data.mState = mState;

    data.mPreviousXPos = mPreviousXPos;
    data.mPreviousYPos = mPreviousYPos;

    data.mDeadTimer = mDeadTimer;

    pSaveBuffer.Write(data);
}

bool Meat::VCanEatMe()
{
    return mState != MeatStates::eCreated_0;
}

void Meat::VOnTrapDoorOpen()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        BaseAliveGameObject_PlatformId = Guid{};
        if (mState == MeatStates::eBecomeAPickUp_3 || mState == MeatStates::eWaitForPickUp_4)
        {
            mState = MeatStates::eIdle_1;
        }
    }
}
