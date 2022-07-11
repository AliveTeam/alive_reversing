#include "stdafx.h"
#include "Bone.hpp"
#include "Function.hpp"
#include "ResourceManager.hpp"
#include "ThrowableArray.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Shadow.hpp"
#include "PlatformBase.hpp"
#include "Sfx.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "../relive_lib/Events.hpp"
#include "Abe.hpp"
#include "Particle.hpp"
#include "Grid.hpp"

Bone::Bone(FP xpos, FP ypos, s16 countId)
    : BaseThrowable(0)
{
    mBaseThrowableDead = 0;
    SetType(ReliveTypes::eBone);
    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kBoneResID, 0, 0))
    {
        LoadRockTypes_49AB30(mCurrentLevel, mCurrentPath);
    }

    const AnimRecord& rec = AnimRec(AnimId::Bone);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(AnimId::Bone, ppRes);

    mAnim.mFlags.Clear(AnimFlags::eBit15_bSemiTrans);

    mXPos = xpos;
    mYPos = ypos;
    mInitialXPos = xpos;
    mInitialYPos = ypos;
    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);
    mBaseGameObjectFlags.Clear(BaseGameObject::eInteractive_Bit8);
    mHitObject &= ~1u;

    mAnim.mFlags.Clear(AnimFlags::eBit3_Render);

    mTimeToLiveTimer = sGnFrame + 300;
    mBaseThrowableCount = countId;
    mState = BoneStates::eSpawned_0;
    mVolumeModifier = 0;

    mShadow = relive_new Shadow();
}

void Bone::VTimeToExplodeRandom()
{
    // Empty
}

s32 Bone::CreateFromSaveState(const u8* pData)
{
    auto pState = reinterpret_cast<const Bone_SaveState*>(pData);

    auto pBone = relive_new Bone(pState->mXPos, pState->mYPos, pState->mBaseThrowableCount);

    pBone->mBaseGameObjectTlvInfo = pState->field_4_obj_id;

    pBone->mXPos = pState->mXPos;
    pBone->mYPos = pState->mYPos;

    pBone->mCollectionRect.x = pBone->mXPos - (ScaleToGridSize(pBone->mSpriteScale) / FP_FromInteger(2));
    pBone->mCollectionRect.y = pBone->mYPos - ScaleToGridSize(pBone->mSpriteScale);
    pBone->mCollectionRect.w = (ScaleToGridSize(pBone->mSpriteScale) / FP_FromInteger(2)) + pBone->mXPos;
    pBone->mCollectionRect.h = pBone->mYPos;

    pBone->mVelX = pState->mVelX;
    pBone->mVelY = pState->mVelY;

    pBone->mCurrentPath = pState->mCurrentPath;
    pBone->mCurrentLevel = MapWrapper::FromAESaveData(pState->mCurrentLevel);
    pBone->mSpriteScale = pState->mSpriteScale;

    pBone->mScale = pState->mSpriteScale > FP_FromDouble(0.75) ? Scale::Fg : Scale::Bg;

    pBone->mAnim.mFlags.Set(AnimFlags::eBit8_Loop, pState->field_20_flags.Get(Bone_SaveState::eBit3_bLoop));
    pBone->mAnim.mFlags.Set(AnimFlags::eBit3_Render, pState->field_20_flags.Get(Bone_SaveState::eBit1_bRender));

    pBone->mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4, pState->field_20_flags.Get(Bone_SaveState::eBit2_bDrawable));
    pBone->mBaseGameObjectFlags.Set(BaseGameObject::eInteractive_Bit8, pState->field_20_flags.Get(Bone_SaveState::eBit4_bInteractive));

    pBone->mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit9_RestoredFromQuickSave);

    pBone->mShineTimer = sGnFrame;

    pBone->BaseAliveGameObjectCollisionLineType = pState->mCollisionLineType;
    pBone->mBaseThrowableCount = pState->mBaseThrowableCount;
    pBone->mState = pState->mState;

    pBone->mVolumeModifier = pState->mVolumeModifier;
    pBone->mInitialXPos = pState->mInitialXPos;
    pBone->mInitialYPos = pState->mInitialYPos;

    pBone->mTimeToLiveTimer = pState->mTimeToLiveTimer;

    pBone->mHitObject = 0;
    if (pState->field_20_flags.Get(Bone_SaveState::eBit5_bHitObject))
    {
        pBone->mHitObject |= 1;
    }

    return sizeof(Bone_SaveState);
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

    mAnim.mFlags.Set(AnimFlags::eBit3_Render);

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
        BaseAliveGameObject_PlatformId = -1;
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

    if (pObj->Type() == ReliveTypes::eSecurityOrb && sControlledCharacter->mScale != pObj->mScale)
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
    SfxPlayMono(SoundEffect::RockBounceOnMine_24, 80);

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
    auto pState = reinterpret_cast<Bone_SaveState*>(pSaveBuffer);

    pState->mAEType = AETypes::eBone_11;
    pState->field_4_obj_id = mBaseGameObjectTlvInfo;

    pState->mXPos = mXPos;
    pState->mYPos = mYPos;

    pState->mVelX = mVelX;
    pState->mVelY = mVelY;

    pState->mCurrentPath = mCurrentPath;
    pState->mCurrentLevel = MapWrapper::ToAE(mCurrentLevel);

    pState->mSpriteScale = mSpriteScale;

    pState->field_20_flags.Set(Bone_SaveState::eBit3_bLoop, mAnim.mFlags.Get(AnimFlags::eBit8_Loop));
    pState->field_20_flags.Set(Bone_SaveState::eBit1_bRender, mAnim.mFlags.Get(AnimFlags::eBit3_Render));

    pState->field_20_flags.Set(Bone_SaveState::eBit2_bDrawable, mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4));
    pState->field_20_flags.Set(Bone_SaveState::eBit4_bInteractive, mBaseGameObjectFlags.Get(BaseGameObject::eInteractive_Bit8));

    pState->field_20_flags.Set(Bone_SaveState::eBit5_bHitObject, mHitObject & 1);

    if (BaseAliveGameObjectCollisionLine)
    {
        pState->mCollisionLineType = BaseAliveGameObjectCollisionLine->mLineType;
    }
    else
    {
        pState->mCollisionLineType = -1;
    }

    pState->field_24_base_id = BaseAliveGameObject_PlatformId;
    pState->mBaseThrowableCount = mBaseThrowableCount;
    pState->mState = mState;

    pState->mVolumeModifier = mVolumeModifier;
    pState->mInitialXPos = mInitialXPos;

    pState->mInitialYPos = mInitialYPos;
    pState->mTimeToLiveTimer = mTimeToLiveTimer;

    return sizeof(Bone_SaveState);
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
            mScale == Scale::Fg ? kFgFloorOrCeiling : kBgFloorOrCeiling)
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
                    SfxPlayMono(SoundEffect::RockBounce_26, vol);
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
                    SfxPlayMono(SoundEffect::RockBounce_26, vol);
                    EventBroadcast(kEventNoise, this);
                    EventBroadcast(kEventSuspiciousNoise, this);
                }
                break;
        }
    }

    if (sCollisions->Raycast(mInitialXPos, mInitialYPos, mXPos, mYPos, &BaseAliveGameObjectCollisionLine, &hitX, &hitY, mScale == Scale::Fg ? kFgWalls : kBgWalls) == 1)
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
                    SfxPlayMono(SoundEffect::RockBounce_26, vol);
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
                    SfxPlayMono(SoundEffect::RockBounce_26, vol);
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
            const s16 offset = mScale == Scale::Fg ? 5 : 0;
            const PSX_Point xy{bRect.x, static_cast<s16>(bRect.y + offset)};
            const PSX_Point wh{bRect.w, static_cast<s16>(bRect.h + offset)};
            VOnCollisionWith(xy, wh, gBaseGameObjects, 1, (TCollisionCallBack) &Bone::OnCollision);

            if (WallHit(FP_FromInteger(5), mVelX))
            {
                mVelX = -mVelX;
            }

            if (FP_Abs(mVelX) >= FP_FromInteger(1))
            {
                if (mVelX <= FP_FromInteger(0))
                {
                    mVelX = (FP_FromDouble(0.01) / mSpriteScale) + mVelX;
                }
                else
                {
                    mVelX = mVelX - (FP_FromDouble(0.01) / mSpriteScale);
                }
                BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mXPos, &mYPos, mVelX);
            }
            else
            {
                if (abs(SnapToXGrid(mSpriteScale, FP_GetExponent(mXPos)) - FP_GetExponent(mXPos)) <= 1)
                {
                    mVelX = FP_FromInteger(0);
                    mCollectionRect.x = mXPos - (ScaleToGridSize(mSpriteScale) / FP_FromInteger(2));
                    mCollectionRect.y = mYPos - ScaleToGridSize(mSpriteScale);
                    mCollectionRect.w = mXPos + (ScaleToGridSize(mSpriteScale) / FP_FromInteger(2));
                    mCollectionRect.h = mYPos;

                    mState = BoneStates::eOnGround_3;
                    mBaseGameObjectFlags.Set(BaseGameObject::eInteractive_Bit8);
                    mAnim.mFlags.Clear(AnimFlags::eBit8_Loop);
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

            mAnim.mFlags.Set(AnimFlags::eBit8_Loop);
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
                    mXPos + (mSpriteScale * FP_FromInteger(1)),
                    (mSpriteScale * FP_FromInteger(-7)) + mYPos,
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
            const s16 offset = mScale == Scale::Fg ? 5 : 0;
            const PSX_Point xy{bRect.x, static_cast<s16>(bRect.y + offset)};
            const PSX_Point wh{bRect.w, static_cast<s16>(bRect.h + offset)};
            VOnCollisionWith(xy, wh, gBaseGameObjects, 1, (TCollisionCallBack) &Bone::OnCollision);

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

const TintEntry kBoneTints_550EC0[18] = {
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
    {EReliveLevelIds::eNone, 127u, 127u, 127u}};


BoneBag::BoneBag(Path_BoneBag* pTlv, s32 tlvInfo)
    : BaseAliveGameObject(0)
{
    SetType(ReliveTypes::eBoneBag);

    const AnimRecord& rec = AnimRec(AnimId::BoneBag_Idle);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    // TODO: Super super OWI hack, WTF?? Figure out exactly what this is patching in the animation
    *((u16*) *ppRes + 4374) = 0;

    Animation_Init(AnimId::BoneBag_Idle, ppRes);
    mAnim.mFlags.Clear(AnimFlags::eBit15_bSemiTrans);
    SetTint(&kBoneTints_550EC0[0], gMap.mCurrentLevel);

    mIsBagHit = false;
    mTlvInfo = tlvInfo;

    mXPos = FP_FromInteger((pTlv->mTopLeft.x + pTlv->mBottomRight.x) / 2);
    mYPos = FP_FromInteger(pTlv->mBottomRight.y);

    mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);

    mBoneVelX = FP_FromRaw(pTlv->mBoneVelX << 8);
    mBoneVelY = FP_FromRaw(-256 * pTlv->mBoneVelY); // TODO: << 8 negated ??

    if (pTlv->mBoneFallDirection == XDirection_short::eLeft_0)
    {
        mBoneVelX = -mBoneVelX;
    }

    if (pTlv->mScale == Scale_short::eHalf_1)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mScale = Scale::Bg;
    }
    else if (pTlv->mScale == Scale_short::eFull_0)
    {
        mSpriteScale = FP_FromInteger(1);
        mScale = Scale::Fg;
    }

    mBoneCount = pTlv->mBoneCount;
    mAllowSound = true;
    mForcePlayWobbleSound = true;

    mShadow = relive_new Shadow();
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

    if (mAnim.mCurrentFrame == 2)
    {
        if (mAllowSound)
        {
            if (Math_NextRandom() < 40 || mForcePlayWobbleSound)
            {
                mAllowSound = false;
                mForcePlayWobbleSound = false;
                SFX_Play_Pitch(SoundEffect::SackWobble_29, 24, Math_RandomRange(-2400, -2200));
            }
        }
    }
    else
    {
        mAllowSound = false;
    }

    if (mIsBagHit)
    {
        // dead code??
        if (mIsBagHit != 1)
        {
            return;
        }

        if (!mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            return;
        }

        mAnim.Set_Animation_Data(AnimId::BoneBag_Idle, nullptr);
        mIsBagHit = false;
        return;
    }

    if (mAnim.mFrameChangeCounter == 0)
    {
        mAnim.mFrameChangeCounter = Math_RandomRange(2, 10);
    }

    const PSX_RECT bPlayerRect = sActiveHero->VGetBoundingRect();
    const PSX_RECT bRect = VGetBoundingRect();

    if (bRect.x <= bPlayerRect.w && bRect.w >= bPlayerRect.x && bRect.h >= bPlayerRect.y && bRect.y <= bPlayerRect.h && mSpriteScale == sActiveHero->mSpriteScale)
    {
        if (gpThrowableArray)
        {
            if (gpThrowableArray->field_20_count)
            {
                if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_31_RunJumpMid_452C10)
                {
                    mAnim.Set_Animation_Data(AnimId::BoneBag_HardHit, nullptr);
                }
                else
                {
                    mAnim.Set_Animation_Data(AnimId::BoneBag_SoftHit, nullptr);
                }
                mIsBagHit = true;
                return;
            }
        }
        else
        {
            gpThrowableArray = relive_new ThrowableArray();
        }

        gpThrowableArray->Add(mBoneCount);

        auto pBone = relive_new Bone(mXPos, mYPos - FP_FromInteger(30), mBoneCount);

        pBone->mSpriteScale = mSpriteScale;
        pBone->mScale = mScale;

        pBone->VThrow(mBoneVelX, mBoneVelY);

        SfxPlayMono(SoundEffect::SackHit_25, 0);
        Environment_SFX_457A40(EnvironmentSfx::eDeathNoise_7, 0, 0x7FFF, 0);

        if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_31_RunJumpMid_452C10)
        {
            mAnim.Set_Animation_Data(AnimId::BoneBag_HardHit, nullptr);
        }
        else
        {
            mAnim.Set_Animation_Data(AnimId::BoneBag_SoftHit, nullptr);
        }

        mIsBagHit = true;
    }
}
