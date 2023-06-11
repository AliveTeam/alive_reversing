#include "stdafx.h"
#include "Rock.hpp"
#include "../relive_lib/Function.hpp"
#include "ThrowableArray.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "Sfx.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/GameObjects/Particle.hpp"
#include "Grid.hpp"
#include "Map.hpp"
#include <assert.h>
#include "../relive_lib/Collisions.hpp"
#include "../relive_lib/FixedPoint.hpp"
#include "QuikSave.hpp"

Rock::Rock(FP xpos, FP ypos, s16 count)
    : BaseThrowable(0)
{
    SetType(ReliveTypes::eRock);

    mBaseThrowableDead = 0;

    // Note: Loaded check removed
    LoadRockTypes(mCurrentLevel, mCurrentPath);

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Rock));
    Animation_Init(GetAnimRes(AnimId::Rock));

    SetInteractive(false);
    GetAnimation().SetRender(false);
    GetAnimation().SetSemiTrans(false);

    mXPos = xpos;
    mYPos = ypos;

    mPreviousXPos = xpos;
    mPreviousYPos = ypos;

    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);

    mBaseThrowableCount = count;
    mState = RockStates::eNone_0;

     /*
    mLoadedPals.push_back(ResourceManagerWrapper::LoadPal(PalId::BlueRock));

   
    // TODO: Don't know where this pal is, probably doesn't exist
    // maybe a stockyards hangover from AO
    //if (ppPal)
    {
        // TODO: I think this only existed in certain lvls, will need a way to know
        // which pal to use per lvl/path
        mAnim.LoadPal(GetPalRes(PalId::BlueRock));
    }
    //else
    {
        mAnim.ReloadPal();
    }*/

    mBounceCount = 0;

    CreateShadow();
}

Rock::~Rock()
{
    if (!gInfiniteThrowables && !mBaseThrowableDead)
    {
        if (gThrowableArray)
        {
            gThrowableArray->Remove(mBaseThrowableCount >= 1 ? mBaseThrowableCount : 1);
        }
    }
}

void Rock::VUpdate()
{
    auto pObj = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    if (GetRestoredFromQuickSave())
    {
        SetRestoredFromQuickSave(false);
        if (BaseAliveGameObjectCollisionLineType == -1)
        {
            BaseAliveGameObjectCollisionLine = nullptr;
        }
        else
        {
            gCollisions->Raycast(
                mXPos,
                mYPos - FP_FromInteger(10),
                mXPos,
                mYPos + FP_FromInteger(10),
                &BaseAliveGameObjectCollisionLine,
                &mXPos,
                &mYPos,
                CollisionMask(static_cast<eLineTypes>(BaseAliveGameObjectCollisionLineType)));
        }
        BaseAliveGameObjectCollisionLineType = 0;
    }

    switch (mState)
    {
        case RockStates::eNone_0:
            break;

        case RockStates::eFallingOutOfRockSack_1:
            InTheAir();
            return;

        case RockStates::eRolling_2:
            if (FP_Abs(mVelX) >= FP_FromInteger(1))
            {
                if (mVelX <= FP_FromInteger(0))
                {
                    mVelX += FP_FromDouble(0.01);
                }
                else
                {
                    mVelX -= FP_FromDouble(0.01);
                }
                BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mXPos, &mYPos, mVelX);
            }
            else
            {
                if (abs(SnapToXGrid(GetSpriteScale(), FP_GetExponent(mXPos)) - FP_GetExponent(mXPos)) <= 1)
                {
                    mVelX = FP_FromInteger(0);
                    mCollectionRect.x = mXPos - (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2));
                    mCollectionRect.w = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2)) + mXPos;
                    SetInteractive(true);
                    mCollectionRect.h = mYPos;
                    mCollectionRect.y = mYPos - ScaleToGridSize(GetSpriteScale());
                    mState = RockStates::eOnGround_3;
                    GetAnimation().SetLoop(false);
                    mShimmerTimer = sGnFrame;
                    return;
                }
                BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mXPos, &mYPos, mVelX);
            }

            if (BaseAliveGameObjectCollisionLine)
            {
                return;
            }

            GetAnimation().SetLoop(true);
            mState = RockStates::eBouncing_4;
            return;

        case RockStates::eOnGround_3:
            if (static_cast<s32>(sGnFrame) <= mShimmerTimer || pObj)
            {
                return;
            }
            // The strange shimmering that rocks give off.
            New_TintShiny_Particle(
                (GetSpriteScale() * FP_FromInteger(1)) + mXPos,
                (GetSpriteScale() * FP_FromInteger(-7)) + mYPos,
                FP_FromDouble(0.3),
                Layer::eLayer_Foreground_36);
            mShimmerTimer = (Math_NextRandom() % 16) + sGnFrame + 60;
            return;

        case RockStates::eBouncing_4:
        {
            InTheAir();
            const PSX_RECT bRect = VGetBoundingRect();
            const PSX_Point xy = {bRect.x, static_cast<s16>(bRect.y + 5)};
            const PSX_Point wh = {bRect.w, static_cast<s16>(bRect.h + 5)};
            OnCollisionWith(xy, wh, gBaseGameObjects);

            if (mVelY > FP_FromInteger(30))
            {
                mState = RockStates::eFallingOutOfWorld_5;
            }
        }
            return;

        case RockStates::eFallingOutOfWorld_5:
            mVelY += FP_FromDouble(1.01);
            mXPos += mVelX;
            mYPos += mVelY;
            if (!gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) && !gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos + FP_FromInteger(240), 0))
            {
                SetDead(true);
            }
            return;
    }
}

void Rock::VTimeToExplodeRandom()
{
    // Calls actual implementation of 0x411490 which is empty.
}

//TODO Identical to AO - merge
void Rock::VScreenChanged()
{
    if (gMap.mCurrentPath != gMap.mNextPath
        || gMap.mCurrentLevel != gMap.mNextLevel)
    {
        SetDead(true);
    }
}

bool Rock::VIsFalling()
{
    return mState == RockStates::eFallingOutOfWorld_5;
}

bool Rock::VCanThrow()
{
    return mState == RockStates::eBouncing_4;
}

//TODO Identical to AO - merge
void Rock::VThrow(FP velX, FP velY)
{
    mVelX = velX;
    mVelY = velY;

    GetAnimation().SetRender(true);

    if (mBaseThrowableCount == 0)
    {
        mState = RockStates::eBouncing_4;
    }
    else
    {
        mState = RockStates::eFallingOutOfRockSack_1;
    }
}

void Rock::InTheAir()
{
    mPreviousXPos = mXPos;
    mPreviousYPos = mYPos;

    if (mVelY > FP_FromInteger(30))
    {
        SetDead(true);
    }

    mVelY += FP_FromDouble(1.01);

    mXPos += mVelX;
    mYPos += mVelY;

    FP hitX = {};
    FP hitY = {};
    if (gCollisions->Raycast(
            mPreviousXPos,
            mPreviousYPos,
            mXPos,
            mYPos,
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            GetScale() == Scale::Fg ? kFgFloorOrCeiling : kBgFloorOrCeiling)
        == 1)
    {
        switch (BaseAliveGameObjectCollisionLine->mLineType)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eDynamicCollision_32:
            case eLineTypes::eBackgroundDynamicCollision_36:
                if (mVelY <= FP_FromInteger(0))
                {
                    break;
                }

                if (!gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
                {
                    return;
                }

                if (mState == RockStates::eBouncing_4 && mVelY < FP_FromInteger(5))
                {
                    mState = RockStates::eFallingOutOfWorld_5;
                    return;
                }

                if (mState == RockStates::eFallingOutOfRockSack_1 && mVelY < FP_FromInteger(1))
                {
                    mState = RockStates::eRolling_2;

                    if (mVelX >= FP_FromInteger(0) && mVelX < FP_FromInteger(1))
                    {
                        mVelX = FP_FromInteger(1);
                    }

                    if (mVelX > FP_FromInteger(-1) && mVelX < FP_FromInteger(0))
                    {
                        mVelX = FP_FromInteger(-1);
                    }

                    mYPos = hitY;
                    return;
                }
                else
                {
                    mYPos = hitY;
                    mVelX = (mVelX / FP_FromInteger(2));
                    mVelY = (-mVelY / FP_FromInteger(2));

                    s16 vol = 20 * (4 - mBounceCount);
                    if (vol < 40)
                    {
                        vol = 40;
                    }

                    SfxPlayMono(relive::SoundEffects::RockBounce, vol);
                    EventBroadcast(kEventNoise, this);
                    EventBroadcast(kEventSuspiciousNoise, this);
                    mBounceCount++;
                }
                break;

            case eLineTypes::eCeiling_3:
            case eLineTypes::eBackgroundCeiling_7:
                if (mVelY < FP_FromInteger(0))
                {
                    mYPos = hitY;
                    mVelY = (-mVelY / FP_FromInteger(2));
                    s16 vol = 20 * (4 - mBounceCount);
                    if (vol < 40)
                    {
                        vol = 40;
                    }
                    SfxPlayMono(relive::SoundEffects::RockBounce, vol);
                    EventBroadcast(kEventNoise, this);
                    EventBroadcast(kEventSuspiciousNoise, this);
                }
                break;
        }
    }

    if (gCollisions->Raycast(mPreviousXPos, mPreviousYPos, mXPos, mYPos, &BaseAliveGameObjectCollisionLine, &hitX, &hitY, GetScale() == Scale::Fg ? kFgWalls : kBgWalls) == 1)
    {
        switch (BaseAliveGameObjectCollisionLine->mLineType)
        {
            case eLineTypes::eWallLeft_1:
            case eLineTypes::eBackgroundWallLeft_5:
                if (mVelX < FP_FromInteger(0))
                {
                    BounceHorizontally( hitX, hitY );
                }
                break;

            case eLineTypes::eWallRight_2:
            case eLineTypes::eBackgroundWallRight_6:
                if (mVelX > FP_FromInteger(0))
                {
                    BounceHorizontally( hitX, hitY );
                }
                break;
        }
    }
}

//TODO Identical to AO - merge
void Rock::BounceHorizontally( FP hitX, FP hitY )
{
    mVelX = (-mVelX / FP_FromInteger(2));
    mXPos = hitX;
    mYPos = hitY;
    s16 vol = 20 * (4 - mBounceCount);
    if (vol < 40)
    {
        vol = 40;
    }
    SfxPlayMono(relive::SoundEffects::RockBounce, vol);
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);
}

//TODO Identical to AO - merge
bool Rock::OnCollision(BaseAnimatedWithPhysicsGameObject* pObj)
{
    if (!pObj->GetCanExplode())
    {
        return true;
    }

    const PSX_RECT bRect = pObj->VGetBoundingRect();

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

    pObj->VOnThrowableHit(this);

    SfxPlayMono(relive::SoundEffects::RockBounceOnMine, 80);
    return false;
}

void Rock::VGetSaveState(SerializedObjectData& pSaveBuffer)
{
    RockSaveState data = {};

    data.mType = ReliveTypes::eRock;
    data.mTlvId = mBaseGameObjectTlvInfo;

    data.mXPos = mXPos;
    data.mYPos = mYPos;

    data.mVelX = mVelX;
    data.mVelY = mVelY;

    data.mCurrentPath = mCurrentPath;
    data.mCurrentLevel = mCurrentLevel;

    data.mSpriteScale = GetSpriteScale();

    data.mRender = GetAnimation().GetRender();
    data.mDrawable = GetDrawable();

    data.mLoop = GetAnimation().GetLoop();
    data.mInteractive = GetInteractive();

    if (BaseAliveGameObjectCollisionLine)
    {
        data.mCollisionLineType = BaseAliveGameObjectCollisionLine->mLineType;
    }
    else
    {
        data.mCollisionLineType = eLineTypes::eNone_m1;
    }
    data.mPlatformId = BaseAliveGameObject_PlatformId;
    data.mThrowableCount = mBaseThrowableCount;
    data.mState = mState;
    data.mBounceCount = mBounceCount;
    data.mPreviousXPos = mPreviousXPos;
    data.mPreviousYPos = mPreviousYPos;

    pSaveBuffer.Write(data);
}

void Rock::CreateFromSaveState(SerializedObjectData& pData)
{
    const auto pState = pData.ReadTmpPtr<RockSaveState>();

    auto pRock = relive_new Rock(pState->mXPos, pState->mYPos, pState->mThrowableCount);

    pRock->mBaseGameObjectTlvInfo = pState->mTlvId;

    pRock->mXPos = pState->mXPos;
    pRock->mYPos = pState->mYPos;

    pRock->mCollectionRect.x = pRock->mXPos - (ScaleToGridSize(pRock->GetSpriteScale()) / FP_FromInteger(2));
    pRock->mCollectionRect.w = pRock->mXPos + (ScaleToGridSize(pRock->GetSpriteScale()) / FP_FromInteger(2));
    pRock->mCollectionRect.h = pRock->mYPos;
    pRock->mCollectionRect.y = pRock->mYPos - ScaleToGridSize(pRock->GetSpriteScale());

    pRock->mVelX = pState->mVelX;
    pRock->mVelY = pState->mVelY;

    pRock->mCurrentPath = pState->mCurrentPath;
    pRock->mCurrentLevel = pState->mCurrentLevel;

    pRock->SetSpriteScale(pState->mSpriteScale);
    pRock->SetScale(pState->mSpriteScale > FP_FromDouble(0.75) ? Scale::Fg : Scale::Bg);

    pRock->GetAnimation().SetRender(pState->mRender);
    pRock->GetAnimation().SetLoop(pState->mLoop);

    pRock->SetDrawable(pState->mDrawable);
    pRock->SetInteractive(pState->mInteractive);

    pRock->SetRestoredFromQuickSave(true);

    pRock->mShimmerTimer = sGnFrame;

    pRock->BaseAliveGameObjectCollisionLineType = pState->mCollisionLineType;

    pRock->mBaseThrowableCount = pState->mThrowableCount;
    pRock->mState = pState->mState;

    pRock->mBounceCount = pState->mBounceCount;

    pRock->mPreviousXPos = pState->mPreviousXPos;
    pRock->mPreviousYPos = pState->mPreviousYPos;
}
