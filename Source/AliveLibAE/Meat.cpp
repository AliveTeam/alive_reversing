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
    field_11A_bDead = 0;
    SetType(ReliveTypes::eMeat);

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kMeatResID, 0, 0))
    {
        LoadRockTypes_49AB30(field_C2_lvl_number, field_C0_path_number);
    }

    const AnimRecord& rec = AnimRec(AnimId::Meat);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    field_20_animation.mAnimFlags.Clear(AnimFlags::eBit15_bSemiTrans);

    mBaseAnimatedWithPhysicsGameObject_XPos = xpos;
    mBaseAnimatedWithPhysicsGameObject_YPos = ypos;

    field_120_xpos = xpos;
    field_124_ypos = ypos;

    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);
    field_128_timer = 0;
    mBaseGameObjectFlags.Clear(BaseGameObject::eInteractive_Bit8);

    field_20_animation.mAnimFlags.Clear(AnimFlags::eBit3_Render);

    field_12C_deadtimer = sGnFrame + 600;
    field_130_pLine = nullptr;
    field_118_count = count;
    field_11C_state = MeatStates::eCreated_0;

    mShadow = ae_new<Shadow>();
}

void Meat::VTimeToExplodeRandom()
{
    // TODO
}

void Meat::VScreenChanged()
{
    if (gMap.mCurrentPath != gMap.mPath || gMap.mCurrentLevel != gMap.mLevel)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void Meat::AddToPlatform()
{
    // TODO: OG bug - why doesn't meat check for trap doors ??
    BaseAddToPlatform([](AETypes type)
                      { return type == AETypes::eLiftPoint_78; });
}

void Meat::VOnTrapDoorOpen()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObjectId));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        BaseAliveGameObjectId = -1;
        if (field_11C_state == MeatStates::eBecomeAPickUp_3 || field_11C_state == MeatStates::eWaitForPickUp_4)
        {
            field_11C_state = MeatStates::eIdle_1;
        }
    }
}

Bool32 Meat::VIsFalling()
{
    return field_11C_state == MeatStates::eFall_5;
}

Bool32 Meat::VCanThrow()
{
    return field_11C_state == MeatStates::eBeingThrown_2;
}

Meat::~Meat()
{
    if (!field_11A_bDead)
    {
        if (gpThrowableArray_5D1E2C)
        {
            gpThrowableArray_5D1E2C->Remove(field_118_count >= 1 ? field_118_count : 1);
        }
    }
}

void Meat::VThrow(FP velX, FP velY)
{
    field_20_animation.mAnimFlags.Set(AnimFlags::eBit3_Render);

    field_C4_velx = velX;
    field_C8_vely = velY;

    if (field_118_count == 0)
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
    if (field_11C_state == MeatStates::eWaitForPickUp_4 && field_118_count == 0)
    {
        return 1;
    }

    return field_118_count;
}

void Meat::InTheAir()
{
    field_120_xpos = mBaseAnimatedWithPhysicsGameObject_XPos;
    field_124_ypos = mBaseAnimatedWithPhysicsGameObject_YPos;

    if (field_C8_vely < FP_FromInteger(18))
    {
        field_C8_vely += FP_FromInteger(1);
    }

    mBaseAnimatedWithPhysicsGameObject_XPos += field_C4_velx;
    mBaseAnimatedWithPhysicsGameObject_YPos += field_C8_vely;

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast(field_120_xpos, field_124_ypos, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, &field_130_pLine, &hitX, &hitY, field_D6_scale == 0 ? 0xF0 : 0xF) == 1)
    {
        switch (field_130_pLine->field_8_type)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eUnknown_32:
            case eLineTypes::eUnknown_36:
                if (field_C8_vely > FP_FromInteger(0))
                {
                    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(SnapToXGrid(field_CC_sprite_scale, FP_GetExponent(hitX)));
                    mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
                    field_11C_state = MeatStates::eBecomeAPickUp_3;
                    field_C8_vely = FP_FromInteger(0);
                    field_C4_velx = FP_FromInteger(0);
                    SFX_Play_Pitch(SoundEffect::MeatBounce_36, 0, -650);
                    Event_Broadcast(kEventNoise, this);
                    Event_Broadcast(kEventSuspiciousNoise, this);
                    AddToPlatform();
                }
                break;

            case eLineTypes::eWallLeft_1:
            case eLineTypes::eBackgroundWallLeft_5:
                if (field_C4_velx < FP_FromInteger(0))
                {
                    mBaseAnimatedWithPhysicsGameObject_XPos = hitX;
                    mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
                    field_C4_velx = (-field_C4_velx / FP_FromInteger(4));
                    SFX_Play_Pitch(SoundEffect::MeatBounce_36, 0, -650);
                    Event_Broadcast(kEventNoise, this);
                    Event_Broadcast(kEventSuspiciousNoise, this);
                    if (field_C8_vely < FP_FromInteger(0))
                    {
                        field_C8_vely = FP_FromInteger(0);
                    }
                }
                field_130_pLine = nullptr;
                break;

            case eLineTypes::eWallRight_2:
            case eLineTypes::eBackgroundWallRight_6:
                if (field_C4_velx > FP_FromInteger(0))
                {
                    mBaseAnimatedWithPhysicsGameObject_XPos = hitX;
                    mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
                    field_C4_velx = (-field_C4_velx / FP_FromInteger(4));
                    SFX_Play_Pitch(SoundEffect::MeatBounce_36, 0, -650);
                    Event_Broadcast(kEventNoise, this);
                    Event_Broadcast(kEventSuspiciousNoise, this);
                    if (field_C8_vely < FP_FromInteger(0))
                    {
                        field_C8_vely = FP_FromInteger(0);
                    }
                }
                field_130_pLine = nullptr;
                break;

            case eLineTypes::eCeiling_3:
            case eLineTypes::eBackgroundCeiling_7:
                if (field_C8_vely < FP_FromInteger(0))
                {
                    mBaseAnimatedWithPhysicsGameObject_XPos = hitX;
                    mBaseAnimatedWithPhysicsGameObject_YPos = hitY + FP_FromInteger(1);
                    field_C8_vely = FP_FromInteger(0);
                    SFX_Play_Pitch(SoundEffect::MeatBounce_36, 0, -650);
                    Event_Broadcast(kEventNoise, this);
                    Event_Broadcast(kEventSuspiciousNoise, this);
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

    PSX_RECT bRect = {};
    static_cast<BaseAliveGameObject*>(pHit)->VGetBoundingRect(&bRect, 1);

    if (field_120_xpos < FP_FromInteger(bRect.x) || field_120_xpos > FP_FromInteger(bRect.w))
    {
        mBaseAnimatedWithPhysicsGameObject_XPos -= field_C4_velx;
        field_C4_velx = (-field_C4_velx / FP_FromInteger(2));
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_YPos -= field_C8_vely;
        field_C8_vely = (-field_C8_vely / FP_FromInteger(2));
    }

    static_cast<BaseAliveGameObject*>(pHit)->VOnThrowableHit(this);

    SFX_Play_Pitch(SoundEffect::MeatBounce_36, 0, -650);

    return 0;
}

void Meat::VUpdate()
{
    auto v2 = sObjectIds.Find_Impl(BaseAliveGameObjectId);
    if (sNum_CamSwappers_5C1B66 == 0)
    {
        if (Event_Get(kEventDeathReset))
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
                PSX_RECT bRect = {};
                VGetBoundingRect(&bRect, 1);
                const PSX_Point xy = {bRect.x, static_cast<s16>(bRect.y + 5)};
                const PSX_Point wh = {bRect.w, static_cast<s16>(bRect.h + 5)};
                VOnCollisionWith(
                    xy,
                    wh,
                    gBaseGameObjects,
                    1,
                    (TCollisionCallBack) &Meat::OnCollision);

                // TODO: OG bug - why only checking for out of the bottom of the map?? Nades check for death object - probably should check both
                if (mBaseAnimatedWithPhysicsGameObject_YPos > FP_FromInteger(gMap.field_D4_ptr->field_6_bBottom))
                {
                    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                }
            }
            break;

            case MeatStates::eBecomeAPickUp_3:
                if (FP_Abs(field_C4_velx) < FP_FromInteger(1))
                {
                    field_20_animation.mAnimFlags.Clear(AnimFlags::eBit8_Loop);
                }

                if (FP_Abs(field_C4_velx) >= FP_FromDouble(0.5))
                {
                    if (field_C4_velx <= FP_FromInteger(0))
                    {
                        field_C4_velx += FP_FromDouble(0.01);
                    }
                    else
                    {
                        field_C4_velx -= FP_FromDouble(0.01);
                    }

                    field_130_pLine = field_130_pLine->MoveOnLine(&mBaseAnimatedWithPhysicsGameObject_XPos, &mBaseAnimatedWithPhysicsGameObject_YPos, field_C4_velx);
                    if (!field_130_pLine)
                    {
                        field_20_animation.mAnimFlags.Set(AnimFlags::eBit8_Loop);
                        field_11C_state = MeatStates::eBeingThrown_2;
                    }
                }
                else
                {
                    field_C4_velx = FP_FromInteger(0);

                    field_E4_collection_rect.x = mBaseAnimatedWithPhysicsGameObject_XPos - (ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(2));
                    field_E4_collection_rect.y = mBaseAnimatedWithPhysicsGameObject_YPos - ScaleToGridSize(field_CC_sprite_scale);
                    field_E4_collection_rect.w = (ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(2)) + mBaseAnimatedWithPhysicsGameObject_XPos;
                    field_E4_collection_rect.h = mBaseAnimatedWithPhysicsGameObject_YPos;

                    mBaseGameObjectFlags.Set(BaseGameObject::eInteractive_Bit8);
                    field_11C_state = MeatStates::eWaitForPickUp_4;
                }
                break;

            case MeatStates::eWaitForPickUp_4:
                if (gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
                {
                    field_12C_deadtimer = sGnFrame + 600;
                }

                if (static_cast<s32>(sGnFrame) > field_128_timer && !v2)
                {
                    // That strange "shimmer" the meat gives off
                    New_TintShiny_Particle(
                        (field_CC_sprite_scale * FP_FromInteger(1)) + mBaseAnimatedWithPhysicsGameObject_XPos,
                        mBaseAnimatedWithPhysicsGameObject_YPos + (field_CC_sprite_scale * FP_FromInteger(-7)),
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
                field_C8_vely += FP_FromInteger(1);
                mBaseAnimatedWithPhysicsGameObject_XPos += field_C4_velx;
                mBaseAnimatedWithPhysicsGameObject_YPos = field_C8_vely + mBaseAnimatedWithPhysicsGameObject_YPos;
                if (!gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
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
    {LevelIds_s8::eMines_1, 127u, 127u, 127u},
    {LevelIds_s8::eNecrum_2, 137u, 137u, 137u},
    {LevelIds_s8::eMudomoVault_3, 127u, 127u, 127u},
    {LevelIds_s8::eMudancheeVault_4, 127u, 127u, 127u},
    {LevelIds_s8::eFeeCoDepot_5, 127u, 127u, 127u},
    {LevelIds_s8::eBarracks_6, 127u, 127u, 127u},
    {LevelIds_s8::eMudancheeVault_Ender_7, 127u, 127u, 127u},
    {LevelIds_s8::eBonewerkz_8, 127u, 127u, 127u},
    {LevelIds_s8::eBrewery_9, 127u, 127u, 127u},
    {LevelIds_s8::eBrewery_Ender_10, 127u, 127u, 127u},
    {LevelIds_s8::eMudomoVault_Ender_11, 127u, 127u, 127u},
    {LevelIds_s8::eFeeCoDepot_Ender_12, 127u, 127u, 127u},
    {LevelIds_s8::eBarracks_Ender_13, 127u, 127u, 127u},
    {LevelIds_s8::eBonewerkz_Ender_14, 127u, 127u, 127u},
    {LevelIds_s8::eNone, 127u, 127u, 127u},
};


MeatSack::MeatSack(Path_MeatSack* pTlv, s32 tlvInfo)
    : BaseAliveGameObject(0)
{
    SetType(ReliveTypes::eMeatSack);

    const AnimRecord& rec = AnimRec(AnimId::MeatSack_Idle);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);
    SetTint(&kMeatTints_55C254[0], gMap.mCurrentLevel);

    mApplyShadows &= ~1u;
    field_118_tlvInfo = tlvInfo;

    field_11C_bDoMeatSackIdleAnim = 0;

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    field_124_velX = FP_FromRaw(pTlv->field_12_xVel << 8);

    // Throw the meat up into the air as it falls from the sack
    field_128_velY = -FP_FromRaw(pTlv->field_14_yVel << 8);

    if (pTlv->field_10_meat_fall_direction == XDirection_short::eLeft_0)
    {
        field_124_velX = -field_124_velX;
    }

    if (pTlv->field_16_scale == Scale_short::eHalf_1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_20_animation.mRenderLayer = Layer::eLayer_8;
        field_D6_scale = 0;
    }
    else if (pTlv->field_16_scale == Scale_short::eFull_0)
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_20_animation.mRenderLayer = Layer::eLayer_27;
        field_D6_scale = 1;
    }

    field_11E_amount_of_meat = pTlv->field_18_amount_of_meat;

    mShadow = ae_new<Shadow>();
}

s32 Meat::CreateFromSaveState(const u8* pBuffer)
{
    const auto pState = reinterpret_cast<const Meat_SaveState*>(pBuffer);

    auto pMeat = ae_new<Meat>(pState->field_8_xpos, pState->field_C_ypos, pState->field_2A_count);

    pMeat->mBaseGameObjectTlvInfo = pState->field_4_obj_id;

    pMeat->mBaseAnimatedWithPhysicsGameObject_XPos = pState->field_8_xpos;
    pMeat->mBaseAnimatedWithPhysicsGameObject_YPos = pState->field_C_ypos;

    pMeat->field_E4_collection_rect.x = pMeat->mBaseAnimatedWithPhysicsGameObject_XPos - (ScaleToGridSize(pMeat->field_CC_sprite_scale) / FP_FromInteger(2));
    pMeat->field_E4_collection_rect.y = pMeat->mBaseAnimatedWithPhysicsGameObject_YPos - ScaleToGridSize(pMeat->field_CC_sprite_scale);
    pMeat->field_E4_collection_rect.w = (ScaleToGridSize(pMeat->field_CC_sprite_scale) / FP_FromInteger(2)) + pMeat->mBaseAnimatedWithPhysicsGameObject_XPos;
    pMeat->field_E4_collection_rect.h = pMeat->mBaseAnimatedWithPhysicsGameObject_YPos;

    pMeat->field_C4_velx = pState->field_10_velx;
    pMeat->field_C8_vely = pState->field_14_vely;

    pMeat->field_C0_path_number = pState->field_1C_path_number;
    pMeat->field_C2_lvl_number = pState->field_1E_lvl_number;

    pMeat->field_CC_sprite_scale = pState->field_18_sprite_scale;

    pMeat->field_20_animation.mAnimFlags.Set(AnimFlags::eBit8_Loop, pState->field_20_flags.Get(Meat_SaveState::eBit3_bLoop));
    pMeat->field_20_animation.mAnimFlags.Set(AnimFlags::eBit3_Render, pState->field_20_flags.Get(Meat_SaveState::eBit1_bRender));

    pMeat->mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4, pState->field_20_flags.Get(Meat_SaveState::eBit2_bDrawable));
    pMeat->mBaseGameObjectFlags.Set(BaseGameObject::eInteractive_Bit8, pState->field_20_flags.Get(Meat_SaveState::eBit4_bInteractive));

    pMeat->mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit9_RestoredFromQuickSave);

    pMeat->field_128_timer = sGnFrame;
    pMeat->BaseAliveGameObjectCollisionLineType = pState->field_28_line_type;

    pMeat->field_118_count = pState->field_2A_count;
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
    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (field_20_animation.field_92_current_frame == 2)
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
        if (field_11C_bDoMeatSackIdleAnim == 1 && field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            const AnimRecord& rec = AnimRec(AnimId::MeatSack_Idle);
            field_20_animation.Set_Animation_Data(rec.mFrameTableOffset, 0);
            field_11C_bDoMeatSackIdleAnim = 0;
        }
    }
    else
    {
        PSX_RECT abeRect = {};
        sActiveHero->VGetBoundingRect(&abeRect, 1);

        PSX_RECT ourRect = {};
        VGetBoundingRect(&ourRect, 1);

        if (RectsOverlap(ourRect, abeRect) && field_CC_sprite_scale == sActiveHero->field_CC_sprite_scale)
        {
            const AnimRecord& MeatSackHitRec = AnimRec(AnimId::MeatSack_Hit);
            if (gpThrowableArray_5D1E2C)
            {
                if (gpThrowableArray_5D1E2C->field_20_count)
                {
                    field_20_animation.Set_Animation_Data(MeatSackHitRec.mFrameTableOffset, 0);
                    field_11C_bDoMeatSackIdleAnim = 1;
                    return;
                }
            }
            else
            {
                gpThrowableArray_5D1E2C = ae_new<ThrowableArray>();
            }

            gpThrowableArray_5D1E2C->Add(field_11E_amount_of_meat);

            auto pMeat = ae_new<Meat>(mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(30), field_11E_amount_of_meat);
             pMeat->VThrow(field_124_velX, field_128_velY);
            pMeat->field_CC_sprite_scale = field_CC_sprite_scale;

            SFX_Play_Mono(SoundEffect::SackHit_25, 0);
            Environment_SFX_457A40(EnvironmentSfx::eDeathNoise_7, 0, 0x7FFF, 0);

            field_20_animation.Set_Animation_Data(MeatSackHitRec.mFrameTableOffset, 0);
            field_11C_bDoMeatSackIdleAnim = 1;
        }
    }
}

s32 Meat::VGetSaveState(u8* pSaveBuffer)
{
    auto pState = reinterpret_cast<Meat_SaveState*>(pSaveBuffer);

    pState->field_0_type = AETypes::eMeat_84;
    pState->field_4_obj_id = mBaseGameObjectTlvInfo;

    pState->field_8_xpos = mBaseAnimatedWithPhysicsGameObject_XPos;
    pState->field_C_ypos = mBaseAnimatedWithPhysicsGameObject_YPos;

    pState->field_10_velx = field_C4_velx;
    pState->field_14_vely = field_C8_vely;

    pState->field_1C_path_number = field_C0_path_number;
    pState->field_1E_lvl_number = field_C2_lvl_number;

    pState->field_18_sprite_scale = field_CC_sprite_scale;

    pState->field_20_flags.Set(Meat_SaveState::eBit3_bLoop, field_20_animation.mAnimFlags.Get(AnimFlags::eBit8_Loop));
    pState->field_20_flags.Set(Meat_SaveState::eBit1_bRender, field_20_animation.mAnimFlags.Get(AnimFlags::eBit3_Render));

    pState->field_20_flags.Set(Meat_SaveState::eBit2_bDrawable, mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4));
    pState->field_20_flags.Set(Meat_SaveState::eBit4_bInteractive, mBaseGameObjectFlags.Get(BaseGameObject::eInteractive_Bit8));

    if (field_130_pLine)
    {
        pState->field_28_line_type = field_130_pLine->field_8_type;
    }
    else
    {
        pState->field_28_line_type = -1;
    }

    pState->field_24_base_id = BaseAliveGameObjectId;
    pState->field_2A_count = field_118_count;
    pState->field_2C_state = field_11C_state;

    pState->field_30_xpos = field_120_xpos;
    pState->field_34_ypos = field_124_ypos;

    pState->field_38_savedfield12C = field_12C_deadtimer;

    return sizeof(Meat_SaveState);
}

Bool32 Meat::VCanEatMe()
{
    return field_11C_state != MeatStates::eCreated_0;
}
