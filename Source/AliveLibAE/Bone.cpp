#include "stdafx.h"
#include "Bone.hpp"
#include "Function.hpp"
#include "ThrowableArray.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Shadow.hpp"
#include "PlatformBase.hpp"
#include "Sfx.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "../relive_lib/Events.hpp"
#include "Abe.hpp"
#include "../relive_lib/Particle.hpp"
#include "Grid.hpp"
#include "Map.hpp"
#include "Path.hpp"

const static AnimId sBoneBagAnimIds[] =
{
    AnimId::BoneBag_Idle,
    AnimId::BoneBag_SoftHit,
    AnimId::BoneBag_HardHit
};

void BoneBag::LoadAnimations()
{
    for (auto& animId : sBoneBagAnimIds)
    {
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(animId));
    }
}

Bone::Bone(FP xpos, FP ypos, s16 countId)
    : BaseThrowable(0)
{
    mBaseThrowableDead = 0;
    SetType(ReliveTypes::eBone);

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Bone));
    Animation_Init(GetAnimRes(AnimId::Bone));

    GetAnimation().mFlags.Clear(AnimFlags::eSemiTrans);

    mXPos = xpos;
    mYPos = ypos;
    mInitialXPos = xpos;
    mInitialYPos = ypos;
    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);
    mBaseGameObjectFlags.Clear(BaseGameObject::eInteractive_Bit8);
    mHitObject = false;

    GetAnimation().mFlags.Clear(AnimFlags::eRender);

    mTimeToLiveTimer = sGnFrame + 300;
    mBaseThrowableCount = countId;
    mState = BoneStates::eSpawned_0;
    mVolumeModifier = 0;

    CreateShadow();
}

void Bone::VTimeToExplodeRandom()
{
    // Empty
}

s32 Bone::CreateFromSaveState(const u8* pData)
{
    auto pState = reinterpret_cast<const BoneSaveState*>(pData);

    auto pBone = relive_new Bone(pState->mXPos, pState->mYPos, pState->mBaseThrowableCount);

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

    pBone->GetAnimation().mFlags.Set(AnimFlags::eLoop, pState->mLoop);
    pBone->GetAnimation().mFlags.Set(AnimFlags::eRender, pState->mRender);

    pBone->mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4, pState->mDrawable);
    pBone->mBaseGameObjectFlags.Set(BaseGameObject::eInteractive_Bit8, pState->mInteractive);

    pBone->mBaseAliveGameObjectFlags.Set(AliveObjectFlags::eRestoredFromQuickSave);

    pBone->mShineTimer = sGnFrame;

    pBone->BaseAliveGameObjectCollisionLineType = pState->mCollisionLineType;
    pBone->mBaseThrowableCount = pState->mBaseThrowableCount;
    pBone->mState = pState->mState;

    pBone->mVolumeModifier = pState->mVolumeModifier;
    pBone->mInitialXPos = pState->mInitialXPos;
    pBone->mInitialYPos = pState->mInitialYPos;

    pBone->mTimeToLiveTimer = pState->mTimeToLiveTimer;

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

    GetAnimation().mFlags.Set(AnimFlags::eRender);

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

s16 Bone::OnCollision(BaseAnimatedWithPhysicsGameObject* pObj)
{
    if (!pObj->mBaseGameObjectFlags.Get(BaseGameObject::eCanExplode_Bit7))
    {
        return 1;
    }

    if (pObj->Type() != ReliveTypes::eMine && pObj->Type() != ReliveTypes::eUXB && (mHitObject & 1))
    {
        return 1;
    }

    if (pObj->Type() == ReliveTypes::eSecurityOrb && sControlledCharacter->GetScale() != pObj->GetScale())
    {
        return 1;
    }

    const PSX_RECT bRect = pObj->VGetBoundingRect();

    if (mInitialXPos < FP_FromInteger(bRect.x) || mInitialXPos > FP_FromInteger(bRect.w))
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

    mHitObject |= 1u;
    SfxPlayMono(relive::SoundEffects::RockBounceOnMine, 80);

    if (pObj->Type() == ReliveTypes::eMine || pObj->Type() == ReliveTypes::eUXB)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    return 0;
}

void Bone::VScreenChanged()
{
    if (gMap.mCurrentPath != gMap.mNextPath || gMap.mCurrentLevel != gMap.mNextLevel)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
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

    pState->mLoop = GetAnimation().mFlags.Get(AnimFlags::eLoop);
    pState->mRender = GetAnimation().mFlags.Get(AnimFlags::eRender);

    pState->mDrawable = mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4);
    pState->mInteractive = mBaseGameObjectFlags.Get(BaseGameObject::eInteractive_Bit8);

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
    pState->mBaseThrowableCount = mBaseThrowableCount;
    pState->mState = mState;

    pState->mVolumeModifier = mVolumeModifier;
    pState->mInitialXPos = mInitialXPos;

    pState->mInitialYPos = mInitialYPos;
    pState->mTimeToLiveTimer = mTimeToLiveTimer;

    return sizeof(BoneSaveState);
}

void Bone::InTheAir()
{
    mInitialXPos = mXPos;
    mInitialYPos = mYPos;

    if (mVelY > FP_FromInteger(30))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    mVelY += FP_FromInteger(1);
    mXPos += mVelX;
    mYPos += mVelY;

    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
            mInitialXPos,
            mInitialYPos,
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
                    s16 vol = 20 * (4 - mVolumeModifier);
                    if (vol < 40)
                    {
                        vol = 40;
                    }
                    SfxPlayMono(relive::SoundEffects::RockBounce, vol);
                    EventBroadcast(kEventNoise, this);
                    EventBroadcast(kEventSuspiciousNoise, this);
                    mVolumeModifier++;
                }
                break;

            case eLineTypes::eCeiling_3:
            case eLineTypes::eBackgroundCeiling_7:
                if (mVelY < FP_FromInteger(0))
                {
                    mYPos = hitY;
                    mVelY = (-mVelY / FP_FromInteger(2));
                    s16 vol = 20 * (4 - mVolumeModifier);
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

    if (sCollisions->Raycast(mInitialXPos, mInitialYPos, mXPos, mYPos, &BaseAliveGameObjectCollisionLine, &hitX, &hitY, GetScale() == Scale::Fg ? kFgWalls : kBgWalls) == 1)
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
                    s16 vol = 20 * (4 - mVolumeModifier);
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
                    s16 vol = 20 * (4 - mVolumeModifier);
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
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
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
                    mBaseGameObjectFlags.Set(BaseGameObject::eInteractive_Bit8);
                    GetAnimation().mFlags.Clear(AnimFlags::eLoop);
                    mShineTimer = sGnFrame;
                    AddToPlatform();
                    return;
                }
                BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mXPos, &mYPos, mVelX);
            }

            if (BaseAliveGameObjectCollisionLine)
            {
                return;
            }

            GetAnimation().mFlags.Set(AnimFlags::eLoop);
            mState = BoneStates::eEdible_4;
        }
            return;

        case BoneStates::eOnGround_3:
            if (gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
            {
                mTimeToLiveTimer = sGnFrame + 300;
            }

            if (static_cast<s32>(sGnFrame) > mShineTimer && !pObj)
            {
                // For the shiny star twinkle effect.
                New_TintShiny_Particle(
                    mXPos + (GetSpriteScale() * FP_FromInteger(1)),
                    (GetSpriteScale() * FP_FromInteger(-7)) + mYPos,
                    FP_FromDouble(0.3),
                    Layer::eLayer_Foreground_36);

                mShineTimer = (Math_NextRandom() % 16) + sGnFrame + 60;
            }

            if (mTimeToLiveTimer < static_cast<s32>(sGnFrame))
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
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
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
        }
            return;

        case BoneStates::eFalling_5:
            mVelY += FP_FromInteger(1);
            mXPos += mVelX;
            mYPos = mVelY + mYPos;
            if (!gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
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

const TintEntry kBoneTints_550EC0[16] = {
    {EReliveLevelIds::eMenu, 127u, 127u, 127u},
    {EReliveLevelIds::eMines, 127u, 127u, 127u},
    {EReliveLevelIds::eNecrum, 137u, 137u, 137u},
    {EReliveLevelIds::eMudomoVault, 127u, 127u, 127u},
    {EReliveLevelIds::eMudancheeVault, 127u, 127u, 127u},
    {EReliveLevelIds::eFeeCoDepot, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks, 127u, 127u, 127u},
    {EReliveLevelIds::eMudancheeVault_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz, 127u, 127u, 127u},
    {EReliveLevelIds::eBrewery, 127u, 127u, 127u},
    {EReliveLevelIds::eBrewery_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eMudomoVault_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eFeeCoDepot_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eCredits, 127u, 127u, 127u}};


BoneBag::BoneBag(relive::Path_BoneBag* pTlv, const Guid& tlvId)
    : BaseAliveGameObject(0)
{
    SetType(ReliveTypes::eBoneBag);

    LoadAnimations();
    Animation_Init(GetAnimRes(AnimId::BoneBag_Idle));
    GetAnimation().mFlags.Clear(AnimFlags::eSemiTrans);
    SetTint(&kBoneTints_550EC0[0], gMap.mCurrentLevel);

    mIsBagHit = false;
    mTlvInfo = tlvId;

    mXPos = FP_FromInteger((pTlv->mTopLeftX + pTlv->mBottomRightX) / 2);
    mYPos = FP_FromInteger(pTlv->mBottomRightY);

    mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);

    mVelX = FP_FromRaw(pTlv->mVelX << 8);
    mVelY = FP_FromRaw(-256 * pTlv->mVelY); // TODO: << 8 negated ??

    if (pTlv->mBoneFallDirection == relive::reliveXDirection::eLeft)
    {
        mVelX = -mVelX;
    }

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        SetScale(Scale::Bg);
    }
    else if (pTlv->mScale == relive::reliveScale::eFull)
    {
        SetSpriteScale(FP_FromInteger(1));
        SetScale(Scale::Fg);
    }

    mBoneAmount = pTlv->mBoneAmount;
    mAllowSound = true;
    mForcePlayWobbleSound = true;

    CreateShadow();
}

void BoneBag::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

BoneBag::~BoneBag()
{
    Path::TLV_Reset(mTlvInfo, -1, 0, 0);
}

void BoneBag::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (GetAnimation().GetCurrentFrame() == 2)
    {
        if (mAllowSound)
        {
            if (Math_NextRandom() < 40 || mForcePlayWobbleSound)
            {
                mAllowSound = false;
                mForcePlayWobbleSound = false;
                SFX_Play_Pitch(relive::SoundEffects::SackWobble, 24, Math_RandomRange(-2400, -2200));
            }
        }
    }
    else
    {
        mAllowSound = false;
    }

    if (mIsBagHit)
    {
        if (mIsBagHit != 1)
        {
            return;
        }

        if (!GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
        {
            return;
        }

        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::BoneBag_Idle));
        mIsBagHit = false;
        return;
    }

    if (GetAnimation().GetFrameChangeCounter() == 0)
    {
        GetAnimation().SetFrameChangeCounter(Math_RandomRange(2, 10));
    }

    const PSX_RECT bPlayerRect = sActiveHero->VGetBoundingRect();
    const PSX_RECT bRect = VGetBoundingRect();

    if (bRect.x <= bPlayerRect.w && bRect.w >= bPlayerRect.x && bRect.h >= bPlayerRect.y && bRect.y <= bPlayerRect.h && GetSpriteScale() == sActiveHero->GetSpriteScale())
    {
        if (gpThrowableArray)
        {
            if (gpThrowableArray->mCount)
            {
                if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_31_RunJumpMid_452C10)
                {
                    GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::BoneBag_HardHit));
                }
                else
                {
                    GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::BoneBag_SoftHit));
                }
                mIsBagHit = true;
                return;
            }
        }
        else
        {
            gpThrowableArray = relive_new ThrowableArray();
        }

        gpThrowableArray->Add(mBoneAmount);

        auto pBone = relive_new Bone(mXPos, mYPos - FP_FromInteger(30), mBoneAmount);

        pBone->SetSpriteScale(GetSpriteScale());
        pBone->SetScale(GetScale());

        pBone->VThrow(mVelX, mVelY);

        SfxPlayMono(relive::SoundEffects::SackHit, 0);
        Environment_SFX_457A40(EnvironmentSfx::eDeathNoise_7, 0, 0x7FFF, 0);

        if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_31_RunJumpMid_452C10)
        {
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::BoneBag_HardHit));
        }
        else
        {
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::BoneBag_SoftHit));
        }

        mIsBagHit = true;
    }
}
