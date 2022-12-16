#include "stdafx.h"
#include "Bone.hpp"
#include "../relive_lib/Function.hpp"
#include "ThrowableArray.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "PlatformBase.hpp"
#include "Sfx.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "../relive_lib/Events.hpp"
#include "Abe.hpp"
#include "../relive_lib/Particle.hpp"
#include "Grid.hpp"
#include "Map.hpp"
#include "Path.hpp"
#include "PathData.hpp"
#include "../relive_lib/Collisions.hpp"
#include "FixedPoint.hpp"
#include "Math.hpp"

Bone::Bone(FP xpos, FP ypos, s16 countId)
    : BaseThrowable(0)
{
    mBaseThrowableDead = 0;
    SetType(ReliveTypes::eBone);

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Bone));
    Animation_Init(GetAnimRes(AnimId::Bone));

    GetAnimation().SetSemiTrans(false);

    mXPos = xpos;
    mYPos = ypos;
    mPreviousXPos = xpos;
    mPreviousYPos = ypos;
    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);
    SetInteractive(false);
    mHitObject = false;

    GetAnimation().SetRender(false);

    mDeadTimer = sGnFrame + 300;
    mBaseThrowableCount = countId;
    mState = BoneStates::eSpawned_0;
    mBounceCount = 0;

    CreateShadow();
}

void Bone::VTimeToExplodeRandom()
{
    // Empty
}

s32 Bone::CreateFromSaveState(const u8* pData)
{
    auto pState = reinterpret_cast<const BoneSaveState*>(pData);

    auto pBone = relive_new Bone(pState->mXPos, pState->mYPos, pState->mThrowableCount);

    pBone->mBaseGameObjectTlvInfo = pState->mBaseTlvId;

    pBone->mXPos = pState->mXPos;
    pBone->mYPos = pState->mYPos;

    pBone->mCollectionRect.x = pBone->mXPos - (ScaleToGridSize(pBone->GetSpriteScale()) / FP_FromInteger(2));
    pBone->mCollectionRect.y = pBone->mYPos - ScaleToGridSize(pBone->GetSpriteScale());
    pBone->mCollectionRect.w = (ScaleToGridSize(pBone->GetSpriteScale()) / FP_FromInteger(2)) + pBone->mXPos;
    pBone->mCollectionRect.h = pBone->mYPos;

    pBone->mVelX = pState->mVelX;
    pBone->mVelY = pState->mVelY;

    pBone->mCurrentPath = pState->mCurrentPath;
    pBone->mCurrentLevel = pState->mCurrentLevel;
    pBone->SetSpriteScale(pState->mSpriteScale);

    pBone->SetScale(pState->mSpriteScale > FP_FromDouble(0.75) ? Scale::Fg : Scale::Bg);

    pBone->GetAnimation().SetLoop(pState->mLoop);
    pBone->GetAnimation().SetRender(pState->mRender);

    pBone->SetDrawable(pState->mDrawable);
    pBone->SetInteractive(pState->mInteractive);

    pBone->SetRestoredFromQuickSave(true);

    pBone->mShimmerTimer = sGnFrame;

    pBone->BaseAliveGameObjectCollisionLineType = pState->mCollisionLineType;
    pBone->mBaseThrowableCount = pState->mThrowableCount;
    pBone->mState = pState->mState;

    pBone->mBounceCount = pState->mBounceCount;
    pBone->mPreviousXPos = pState->mPreviousXPos;
    pBone->mPreviousYPos = pState->mPreviousYPos;

    pBone->mDeadTimer = pState->mTimeToLiveTimer;

    pBone->mHitObject = false;
    if (pState->mHitObject)
    {
        pBone->mHitObject = true;
    }

    return sizeof(BoneSaveState);
}

Bone::~Bone()
{
    if (!gInfiniteThrowables && !mBaseThrowableDead)
    {
        if (gpThrowableArray)
        {
            gpThrowableArray->Remove(mBaseThrowableCount >= 1 ? mBaseThrowableCount : 1);
        }
    }
}

void Bone::AddToPlatform()
{
    BaseAddToPlatform([](ReliveTypes type)
                      { return type == ReliveTypes::eLiftPoint || type == ReliveTypes::eTrapDoor; });
}

void Bone::VThrow(FP velX, FP velY)
{
    mVelX = velX;
    mVelY = velY;

    GetAnimation().SetRender(true);

    if (mBaseThrowableCount == 0)
    {
        mState = BoneStates::eEdible_4;
    }
    else
    {
        mState = BoneStates::eAirborne_1;
    }
}

void Bone::VOnTrapDoorOpen()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        BaseAliveGameObject_PlatformId = Guid{};
        if (mState == BoneStates::eCollided_2 || mState == BoneStates::eOnGround_3)
        {
            mState = BoneStates::eAirborne_1;
        }
    }
}

bool Bone::VIsFalling()
{
    return mState == BoneStates::eFalling_5;
}

bool Bone::VCanThrow()
{
    return mState != BoneStates::eSpawned_0 && mState != BoneStates::eAirborne_1;
}

bool Bone::OnCollision(BaseAnimatedWithPhysicsGameObject* pObj)
{
    if (!pObj->GetCanExplode())
    {
        return true;
    }

    if (pObj->Type() != ReliveTypes::eMine && pObj->Type() != ReliveTypes::eUXB && mHitObject)
    {
        return true;
    }

    if (pObj->Type() == ReliveTypes::eSecurityOrb && sControlledCharacter->GetScale() != pObj->GetScale())
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
        if (mVelY > FP_FromInteger(0))
        {
            const FP slowerVelY = (-mVelY / FP_FromInteger(2));
            mVelY = slowerVelY;
            mYPos += slowerVelY;
        }
    }

    pObj->VOnThrowableHit(this);

    mHitObject = true;
    SfxPlayMono(relive::SoundEffects::RockBounceOnMine, 80);

    if (pObj->Type() == ReliveTypes::eMine || pObj->Type() == ReliveTypes::eUXB)
    {
        SetDead(true);
    }

    return false;
}

void Bone::VScreenChanged()
{
    if (gMap.mCurrentPath != gMap.mNextPath || gMap.mCurrentLevel != gMap.mNextLevel)
    {
        SetDead(true);
    }
}

bool Bone::VCanBeEaten()
{
    return mState == BoneStates::eEdible_4;
}

s32 Bone::VGetSaveState(u8* pSaveBuffer)
{
    auto pState = reinterpret_cast<BoneSaveState*>(pSaveBuffer);

    pState->mType = ReliveTypes::eBone;
    pState->mBaseTlvId = mBaseGameObjectTlvInfo;

    pState->mXPos = mXPos;
    pState->mYPos = mYPos;

    pState->mVelX = mVelX;
    pState->mVelY = mVelY;

    pState->mCurrentPath = mCurrentPath;
    pState->mCurrentLevel = mCurrentLevel;

    pState->mSpriteScale = GetSpriteScale();

    pState->mLoop = GetAnimation().GetLoop();
    pState->mRender = GetAnimation().GetRender();

    pState->mDrawable = GetDrawable();
    pState->mInteractive = GetInteractive();

    pState->mHitObject = mHitObject;

    if (BaseAliveGameObjectCollisionLine)
    {
        pState->mCollisionLineType = BaseAliveGameObjectCollisionLine->mLineType;
    }
    else
    {
        pState->mCollisionLineType = -1;
    }

    pState->mPlatformId = BaseAliveGameObject_PlatformId;
    pState->mThrowableCount = mBaseThrowableCount;
    pState->mState = mState;

    pState->mBounceCount = mBounceCount;
    pState->mPreviousXPos = mPreviousXPos;

    pState->mPreviousYPos = mPreviousYPos;
    pState->mTimeToLiveTimer = mDeadTimer;

    return sizeof(BoneSaveState);
}

void Bone::InTheAir()
{
    mPreviousXPos = mXPos;
    mPreviousYPos = mYPos;

    if (mVelY > FP_FromInteger(30))
    {
        SetDead(true);
    }

    mVelY += FP_FromInteger(1);
    mXPos += mVelX;
    mYPos += mVelY;

    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
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

                mYPos = hitY;

                if (mVelY < FP_FromInteger(1))
                {
                    mState = BoneStates::eCollided_2;

                    mYPos = FP_FromInteger(BaseAliveGameObjectCollisionLine->mRect.y);
                    mVelY = FP_FromInteger(0);
                    if (mVelX >= FP_FromInteger(0) && mVelX < FP_FromInteger(1))
                    {
                        mVelX = FP_FromInteger(1);
                    }

                    if (mVelX < FP_FromInteger(0) && mVelX > FP_FromInteger(-1))
                    {
                        mVelX = FP_FromInteger(-1);
                    }
                    return;
                }
                else
                {
                    mYPos -= FP_FromDouble(0.1);
                    mVelY = (-mVelY / FP_FromInteger(2));
                    mVelX = (mVelX / FP_FromInteger(2));
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

    if (sCollisions->Raycast(mPreviousXPos, mPreviousYPos, mXPos, mYPos, &BaseAliveGameObjectCollisionLine, &hitX, &hitY, GetScale() == Scale::Fg ? kFgWalls : kBgWalls) == 1)
    {
        switch (BaseAliveGameObjectCollisionLine->mLineType)
        {
            case eLineTypes::eWallLeft_1:
            case eLineTypes::eBackgroundWallLeft_5:
                if (mVelX < FP_FromInteger(0))
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
                BaseAliveGameObjectCollisionLine = nullptr;
                break;

            case eLineTypes::eWallRight_2:
            case eLineTypes::eBackgroundWallRight_6:
                if (mVelX > FP_FromInteger(0))
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
                BaseAliveGameObjectCollisionLine = nullptr;
                break;
        }
    }
}

void Bone::VUpdate()
{
    auto pObj = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    switch (mState)
    {
        case BoneStates::eSpawned_0:
            break;

        case BoneStates::eAirborne_1:
            InTheAir();
            return;

        case BoneStates::eCollided_2:
        {
            const PSX_RECT bRect = VGetBoundingRect();
            const s16 offset = GetScale() == Scale::Fg ? 5 : 0;
            const PSX_Point xy{bRect.x, static_cast<s16>(bRect.y + offset)};
            const PSX_Point wh{bRect.w, static_cast<s16>(bRect.h + offset)};
            OnCollisionWith(xy, wh, gBaseGameObjects);

            if (WallHit(FP_FromInteger(5), mVelX))
            {
                mVelX = -mVelX;
            }

            if (FP_Abs(mVelX) >= FP_FromInteger(1))
            {
                if (mVelX <= FP_FromInteger(0))
                {
                    mVelX = (FP_FromDouble(0.01) / GetSpriteScale()) + mVelX;
                }
                else
                {
                    mVelX = mVelX - (FP_FromDouble(0.01) / GetSpriteScale());
                }
                BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mXPos, &mYPos, mVelX);
            }
            else
            {
                if (abs(SnapToXGrid(GetSpriteScale(), FP_GetExponent(mXPos)) - FP_GetExponent(mXPos)) <= 1)
                {
                    mVelX = FP_FromInteger(0);
                    mCollectionRect.x = mXPos - (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2));
                    mCollectionRect.y = mYPos - ScaleToGridSize(GetSpriteScale());
                    mCollectionRect.w = mXPos + (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2));
                    mCollectionRect.h = mYPos;

                    mState = BoneStates::eOnGround_3;
                    SetInteractive(true);
                    GetAnimation().SetLoop(false);
                    mShimmerTimer = sGnFrame;
                    AddToPlatform();
                    return;
                }
                BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mXPos, &mYPos, mVelX);
            }

            if (BaseAliveGameObjectCollisionLine)
            {
                return;
            }

            GetAnimation().SetLoop(true);
            mState = BoneStates::eEdible_4;
        }
            return;

        case BoneStates::eOnGround_3:
            if (gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
            {
                mDeadTimer = sGnFrame + 300;
            }

            if (static_cast<s32>(sGnFrame) > mShimmerTimer && !pObj)
            {
                // For the shiny star twinkle effect.
                New_TintShiny_Particle(
                    mXPos + (GetSpriteScale() * FP_FromInteger(1)),
                    (GetSpriteScale() * FP_FromInteger(-7)) + mYPos,
                    FP_FromDouble(0.3),
                    Layer::eLayer_Foreground_36);

                mShimmerTimer = (Math_NextRandom() % 16) + sGnFrame + 60;
            }

            if (mDeadTimer < static_cast<s32>(sGnFrame))
            {
                SetDead(true);
            }
            return;

        case BoneStates::eEdible_4:
        {
            InTheAir();
            const PSX_RECT bRect = VGetBoundingRect();
            const s16 offset = GetScale() == Scale::Fg ? 5 : 0;
            const PSX_Point xy{bRect.x, static_cast<s16>(bRect.y + offset)};
            const PSX_Point wh{bRect.w, static_cast<s16>(bRect.h + offset)};
            OnCollisionWith(xy, wh, gBaseGameObjects);

            if (mYPos > FP_FromInteger(gMap.mPathData->field_6_bBottom))
            {
                SetDead(true);
            }
        }
            return;

        case BoneStates::eFalling_5:
            mVelY += FP_FromInteger(1);
            mXPos += mVelX;
            mYPos = mVelY + mYPos;
            if (!gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
            {
                SetDead(true);
            }
            return;
    }
}

s16 Bone::VGetCount()
{
    if (mState == BoneStates::eOnGround_3 && mBaseThrowableCount == 0)
    {
        return 1;
    }
    return mBaseThrowableCount;
}
