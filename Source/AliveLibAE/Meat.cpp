#include "stdafx.h"
#include "Meat.hpp"
#include "Function.hpp"
#include "ResourceManager.hpp"
#include "ThrowableArray.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "Shadow.hpp"
#include "PlatformBase.hpp"
#include "Sfx.hpp"
#include "ObjectIds.hpp"
#include "Events.hpp"
#include "Abe.hpp"
#include "Particle.hpp"
#include "Grid.hpp"

Meat::Meat(FP xpos, FP ypos, s16 count)
    : BaseThrowable(0)
{
    mBaseThrowableDead = 0;
    SetType(ReliveTypes::eMeat);

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kMeatResID, 0, 0))
    {
        LoadRockTypes_49AB30(mCurrentLevel, mCurrentPath);
    }

    const AnimRecord& rec = AnimRec(AnimId::Meat);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(AnimId::Meat, ppRes);

    mAnim.mFlags.Clear(AnimFlags::eBit15_bSemiTrans);

    mXPos = xpos;
    mYPos = ypos;

    field_120_xpos = xpos;
    field_124_ypos = ypos;

    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);
    field_128_timer = 0;
    mBaseGameObjectFlags.Clear(BaseGameObject::eInteractive_Bit8);

    mAnim.mFlags.Clear(AnimFlags::eBit3_Render);

    field_12C_deadtimer = sGnFrame + 600;
    field_130_pLine = nullptr;
    mBaseThrowableCount = count;
    field_11C_state = MeatStates::eCreated_0;

    mShadow = relive_new Shadow();
}

void Meat::VTimeToExplodeRandom()
{
    // TODO
}

void Meat::VScreenChanged()
{
    if (gMap.mCurrentPath != gMap.mNextPath || gMap.mCurrentLevel != gMap.mNextLevel)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void Meat::AddToPlatform()
{
    // TODO: OG bug - why doesn't meat check for trap doors ??
    BaseAddToPlatform([](ReliveTypes type)
                      { return type == ReliveTypes::eLiftPoint; });
}

void Meat::VOnTrapDoorOpen()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        BaseAliveGameObject_PlatformId = -1;
        if (field_11C_state == MeatStates::eBecomeAPickUp_3 || field_11C_state == MeatStates::eWaitForPickUp_4)
        {
            field_11C_state = MeatStates::eIdle_1;
        }
    }
}

bool Meat::VIsFalling()
{
    return field_11C_state == MeatStates::eFall_5;
}

bool Meat::VCanThrow()
{
    return field_11C_state == MeatStates::eBeingThrown_2;
}

Meat::~Meat()
{
    if (!mBaseThrowableDead)
    {
        if (gpThrowableArray)
        {
            gpThrowableArray->Remove(mBaseThrowableCount >= 1 ? mBaseThrowableCount : 1);
        }
    }
}

void Meat::VThrow(FP velX, FP velY)
{
    mAnim.mFlags.Set(AnimFlags::eBit3_Render);

    mVelX = velX;
    mVelY = velY;

    if (mBaseThrowableCount == 0)
    {
        field_11C_state = MeatStates::eBeingThrown_2;
    }
    else
    {
        field_11C_state = MeatStates::eIdle_1;
    }
}

s16 Meat::VGetCount()
{
    if (field_11C_state == MeatStates::eWaitForPickUp_4 && mBaseThrowableCount == 0)
    {
        return 1;
    }

    return mBaseThrowableCount;
}

void Meat::InTheAir()
{
    field_120_xpos = mXPos;
    field_124_ypos = mYPos;

    if (mVelY < FP_FromInteger(18))
    {
        mVelY += FP_FromInteger(1);
    }

    mXPos += mVelX;
    mYPos += mVelY;

    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(field_120_xpos, field_124_ypos, mXPos, mYPos, &field_130_pLine, &hitX, &hitY, mScale == Scale::Fg ? kFgFloorCeilingOrWalls : kBgFloorCeilingOrWalls) == 1)
    {
        switch (field_130_pLine->mLineType)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eDynamicCollision_32:
            case eLineTypes::eBackgroundDynamicCollision_36:
                if (mVelY > FP_FromInteger(0))
                {
                    mXPos = FP_FromInteger(SnapToXGrid(mSpriteScale, FP_GetExponent(hitX)));
                    mYPos = hitY;
                    field_11C_state = MeatStates::eBecomeAPickUp_3;
                    mVelY = FP_FromInteger(0);
                    mVelX = FP_FromInteger(0);
                    SFX_Play_Pitch(SoundEffect::MeatBounce_36, 0, -650);
                    EventBroadcast(kEventNoise, this);
                    EventBroadcast(kEventSuspiciousNoise, this);
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
                    SFX_Play_Pitch(SoundEffect::MeatBounce_36, 0, -650);
                    EventBroadcast(kEventNoise, this);
                    EventBroadcast(kEventSuspiciousNoise, this);
                    if (mVelY < FP_FromInteger(0))
                    {
                        mVelY = FP_FromInteger(0);
                    }
                }
                field_130_pLine = nullptr;
                break;

            case eLineTypes::eWallRight_2:
            case eLineTypes::eBackgroundWallRight_6:
                if (mVelX > FP_FromInteger(0))
                {
                    mXPos = hitX;
                    mYPos = hitY;
                    mVelX = (-mVelX / FP_FromInteger(4));
                    SFX_Play_Pitch(SoundEffect::MeatBounce_36, 0, -650);
                    EventBroadcast(kEventNoise, this);
                    EventBroadcast(kEventSuspiciousNoise, this);
                    if (mVelY < FP_FromInteger(0))
                    {
                        mVelY = FP_FromInteger(0);
                    }
                }
                field_130_pLine = nullptr;
                break;

            case eLineTypes::eCeiling_3:
            case eLineTypes::eBackgroundCeiling_7:
                if (mVelY < FP_FromInteger(0))
                {
                    mXPos = hitX;
                    mYPos = hitY + FP_FromInteger(1);
                    mVelY = FP_FromInteger(0);
                    SFX_Play_Pitch(SoundEffect::MeatBounce_36, 0, -650);
                    EventBroadcast(kEventNoise, this);
                    EventBroadcast(kEventSuspiciousNoise, this);
                }
                break;
        }
    }
}

s16 Meat::OnCollision(BaseGameObject* pHit)
{
    // TODO: Check if pHit type is correct for all throwables

    if (!pHit->mBaseGameObjectFlags.Get(BaseGameObject::eCanExplode_Bit7))
    {
        return 1;
    }

    if (pHit->Type() == ReliveTypes::eMine || pHit->Type() == ReliveTypes::eUXB || pHit->Type() == ReliveTypes::eTimedMine_or_MovingBomb)
    {
        return 1;
    }

    const PSX_RECT bRect = static_cast<BaseAliveGameObject*>(pHit)->VGetBoundingRect();

    if (field_120_xpos < FP_FromInteger(bRect.x) || field_120_xpos > FP_FromInteger(bRect.w))
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

    SFX_Play_Pitch(SoundEffect::MeatBounce_36, 0, -650);

    return 0;
}

void Meat::VUpdate()
{
    auto v2 = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
    if (sNum_CamSwappers_5C1B66 == 0)
    {
        if (EventGet(kEventDeathReset))
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }

        switch (field_11C_state)
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
                VOnCollisionWith(
                    xy,
                    wh,
                    gBaseGameObjects,
                    1,
                    (TCollisionCallBack) &Meat::OnCollision);

                // TODO: OG bug - why only checking for out of the bottom of the map?? Nades check for death object - probably should check both
                if (mYPos > FP_FromInteger(gMap.mPathData->field_6_bBottom))
                {
                    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                }
            }
            break;

            case MeatStates::eBecomeAPickUp_3:
                if (FP_Abs(mVelX) < FP_FromInteger(1))
                {
                    mAnim.mFlags.Clear(AnimFlags::eBit8_Loop);
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

                    field_130_pLine = field_130_pLine->MoveOnLine(&mXPos, &mYPos, mVelX);
                    if (!field_130_pLine)
                    {
                        mAnim.mFlags.Set(AnimFlags::eBit8_Loop);
                        field_11C_state = MeatStates::eBeingThrown_2;
                    }
                }
                else
                {
                    mVelX = FP_FromInteger(0);

                    mCollectionRect.x = mXPos - (ScaleToGridSize(mSpriteScale) / FP_FromInteger(2));
                    mCollectionRect.y = mYPos - ScaleToGridSize(mSpriteScale);
                    mCollectionRect.w = (ScaleToGridSize(mSpriteScale) / FP_FromInteger(2)) + mXPos;
                    mCollectionRect.h = mYPos;

                    mBaseGameObjectFlags.Set(BaseGameObject::eInteractive_Bit8);
                    field_11C_state = MeatStates::eWaitForPickUp_4;
                }
                break;

            case MeatStates::eWaitForPickUp_4:
                if (gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
                {
                    field_12C_deadtimer = sGnFrame + 600;
                }

                if (static_cast<s32>(sGnFrame) > field_128_timer && !v2)
                {
                    // That strange "shimmer" the meat gives off
                    New_TintShiny_Particle(
                        (mSpriteScale * FP_FromInteger(1)) + mXPos,
                        mYPos + (mSpriteScale * FP_FromInteger(-7)),
                        FP_FromDouble(0.3),
                        Layer::eLayer_Foreground_36);
                    field_128_timer = Math_NextRandom() % 16 + sGnFrame + 60;
                }
                if (field_12C_deadtimer < (s32) sGnFrame)
                {
                    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                }
                break;

            case MeatStates::eFall_5:
                mVelY += FP_FromInteger(1);
                mXPos += mVelX;
                mYPos = mVelY + mYPos;
                if (!gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
                {
                    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                }
                break;

            default:
                return;
        }
    }
}

// =================================================================================



const TintEntry kMeatTints_55C254[] = {
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
    {EReliveLevelIds::eNone, 127u, 127u, 127u},
};


MeatSack::MeatSack(Path_MeatSack* pTlv, s32 tlvInfo)
    : BaseAliveGameObject(0)
{
    SetType(ReliveTypes::eMeatSack);

    const AnimRecord& rec = AnimRec(AnimId::MeatSack_Idle);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(AnimId::MeatSack_Idle, ppRes);
    SetTint(&kMeatTints_55C254[0], gMap.mCurrentLevel);

    mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);
    field_118_tlvInfo = tlvInfo;

    field_11C_bDoMeatSackIdleAnim = 0;

    mXPos = FP_FromInteger(pTlv->mTopLeft.x);
    mYPos = FP_FromInteger(pTlv->mTopLeft.y);

    field_124_velX = FP_FromRaw(pTlv->field_12_xVel << 8);

    // Throw the meat up into the air as it falls from the sack
    field_128_velY = -FP_FromRaw(pTlv->field_14_yVel << 8);

    if (pTlv->field_10_meat_fall_direction == XDirection_short::eLeft_0)
    {
        field_124_velX = -field_124_velX;
    }

    if (pTlv->field_16_scale == Scale_short::eHalf_1)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mAnim.mRenderLayer = Layer::eLayer_8;
        mScale = Scale::Bg;
    }
    else if (pTlv->field_16_scale == Scale_short::eFull_0)
    {
        mSpriteScale = FP_FromInteger(1);
        mAnim.mRenderLayer = Layer::eLayer_27;
        mScale = Scale::Fg;
    }

    field_11E_amount_of_meat = pTlv->field_18_amount_of_meat;

    mShadow = relive_new Shadow();
}

s32 Meat::CreateFromSaveState(const u8* pBuffer)
{
    const auto pState = reinterpret_cast<const Meat_SaveState*>(pBuffer);

    auto pMeat = relive_new Meat(pState->field_8_xpos, pState->field_C_ypos, pState->field_2A_count);

    pMeat->mBaseGameObjectTlvInfo = pState->field_4_obj_id;

    pMeat->mXPos = pState->field_8_xpos;
    pMeat->mYPos = pState->field_C_ypos;

    pMeat->mCollectionRect.x = pMeat->mXPos - (ScaleToGridSize(pMeat->mSpriteScale) / FP_FromInteger(2));
    pMeat->mCollectionRect.y = pMeat->mYPos - ScaleToGridSize(pMeat->mSpriteScale);
    pMeat->mCollectionRect.w = (ScaleToGridSize(pMeat->mSpriteScale) / FP_FromInteger(2)) + pMeat->mXPos;
    pMeat->mCollectionRect.h = pMeat->mYPos;

    pMeat->mVelX = pState->field_10_velx;
    pMeat->mVelY = pState->field_14_vely;

    pMeat->mCurrentPath = pState->field_1C_path_number;
    pMeat->mCurrentLevel = MapWrapper::FromAESaveData(pState->field_1E_lvl_number);

    pMeat->mSpriteScale = pState->field_18_sprite_scale;

    pMeat->mAnim.mFlags.Set(AnimFlags::eBit8_Loop, pState->field_20_flags.Get(Meat_SaveState::eBit3_bLoop));
    pMeat->mAnim.mFlags.Set(AnimFlags::eBit3_Render, pState->field_20_flags.Get(Meat_SaveState::eBit1_bRender));

    pMeat->mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4, pState->field_20_flags.Get(Meat_SaveState::eBit2_bDrawable));
    pMeat->mBaseGameObjectFlags.Set(BaseGameObject::eInteractive_Bit8, pState->field_20_flags.Get(Meat_SaveState::eBit4_bInteractive));

    pMeat->mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit9_RestoredFromQuickSave);

    pMeat->field_128_timer = sGnFrame;
    pMeat->BaseAliveGameObjectCollisionLineType = pState->field_28_line_type;

    pMeat->mBaseThrowableCount = pState->field_2A_count;
    pMeat->field_11C_state = pState->field_2C_state;

    pMeat->field_120_xpos = pState->field_30_xpos;
    pMeat->field_124_ypos = pState->field_34_ypos;

    pMeat->field_12C_deadtimer = pState->field_38_savedfield12C;
    return sizeof(Meat_SaveState);
}

MeatSack::~MeatSack()
{
    Path::TLV_Reset(field_118_tlvInfo, -1, 0, 0);
}

void MeatSack::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

void MeatSack::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (mAnim.mCurrentFrame == 2)
    {
        if (field_120_bPlayWobbleSound)
        {
            if (Math_NextRandom() < 40u || field_122_always_0)
            {
                field_120_bPlayWobbleSound = 0;
                field_122_always_0 = 0;
                SFX_Play_Pitch(SoundEffect::SackWobble_29, 24, Math_RandomRange(-2400, -2200));
            }
        }
    }
    else
    {
        field_120_bPlayWobbleSound = 1;
    }

    if (field_11C_bDoMeatSackIdleAnim)
    {
        if (field_11C_bDoMeatSackIdleAnim == 1 && mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            mAnim.Set_Animation_Data(AnimId::MeatSack_Idle, nullptr);
            field_11C_bDoMeatSackIdleAnim = 0;
        }
    }
    else
    {
        const PSX_RECT abeRect = sActiveHero->VGetBoundingRect();
        const PSX_RECT ourRect = VGetBoundingRect();

        if (RectsOverlap(ourRect, abeRect) && mSpriteScale == sActiveHero->mSpriteScale)
        {
            if (gpThrowableArray)
            {
                if (gpThrowableArray->field_20_count)
                {
                    mAnim.Set_Animation_Data(AnimId::MeatSack_Hit, nullptr);
                    field_11C_bDoMeatSackIdleAnim = 1;
                    return;
                }
            }
            else
            {
                gpThrowableArray = relive_new ThrowableArray();
            }

            gpThrowableArray->Add(field_11E_amount_of_meat);

            auto pMeat = relive_new Meat(mXPos, mYPos - FP_FromInteger(30), field_11E_amount_of_meat);
             pMeat->VThrow(field_124_velX, field_128_velY);
            pMeat->mSpriteScale = mSpriteScale;

            SfxPlayMono(SoundEffect::SackHit_25, 0);
            Environment_SFX_457A40(EnvironmentSfx::eDeathNoise_7, 0, 0x7FFF, 0);

            mAnim.Set_Animation_Data(AnimId::MeatSack_Hit, nullptr);
            field_11C_bDoMeatSackIdleAnim = 1;
        }
    }
}

s32 Meat::VGetSaveState(u8* pSaveBuffer)
{
    auto pState = reinterpret_cast<Meat_SaveState*>(pSaveBuffer);

    pState->field_0_type = AETypes::eMeat_84;
    pState->field_4_obj_id = mBaseGameObjectTlvInfo;

    pState->field_8_xpos = mXPos;
    pState->field_C_ypos = mYPos;

    pState->field_10_velx = mVelX;
    pState->field_14_vely = mVelY;

    pState->field_1C_path_number = mCurrentPath;
    pState->field_1E_lvl_number = MapWrapper::ToAE(mCurrentLevel);

    pState->field_18_sprite_scale = mSpriteScale;

    pState->field_20_flags.Set(Meat_SaveState::eBit3_bLoop, mAnim.mFlags.Get(AnimFlags::eBit8_Loop));
    pState->field_20_flags.Set(Meat_SaveState::eBit1_bRender, mAnim.mFlags.Get(AnimFlags::eBit3_Render));

    pState->field_20_flags.Set(Meat_SaveState::eBit2_bDrawable, mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4));
    pState->field_20_flags.Set(Meat_SaveState::eBit4_bInteractive, mBaseGameObjectFlags.Get(BaseGameObject::eInteractive_Bit8));

    if (field_130_pLine)
    {
        pState->field_28_line_type = field_130_pLine->mLineType;
    }
    else
    {
        pState->field_28_line_type = -1;
    }

    pState->field_24_base_id = BaseAliveGameObject_PlatformId;
    pState->field_2A_count = mBaseThrowableCount;
    pState->field_2C_state = field_11C_state;

    pState->field_30_xpos = field_120_xpos;
    pState->field_34_ypos = field_124_ypos;

    pState->field_38_savedfield12C = field_12C_deadtimer;

    return sizeof(Meat_SaveState);
}

bool Meat::VCanEatMe()
{
    return field_11C_state != MeatStates::eCreated_0;
}
