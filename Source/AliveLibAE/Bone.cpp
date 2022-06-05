#include "stdafx.h"
#include "Bone.hpp"
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

Bone::Bone(FP xpos, FP ypos, s16 countId)
    : BaseThrowable(0)
{
    field_11A_bDead = 0;
    SetType(ReliveTypes::eBone);
    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kBoneResID, 0, 0))
    {
        LoadRockTypes_49AB30(field_C2_lvl_number, field_C0_path_number);
    }

    const AnimRecord& rec = AnimRec(AnimId::Bone);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    field_20_animation.mAnimFlags.Clear(AnimFlags::eBit15_bSemiTrans);

    mBaseAnimatedWithPhysicsGameObject_XPos = xpos;
    mBaseAnimatedWithPhysicsGameObject_YPos = ypos;
    field_120_xpos = xpos;
    field_124_ypos = ypos;
    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);
    mBaseGameObjectFlags.Clear(BaseGameObject::eInteractive_Bit8);
    field_130_hit_object &= ~1u;

    field_20_animation.mAnimFlags.Clear(AnimFlags::eBit3_Render);

    field_12C_time_to_live = sGnFrame + 300;
    field_118_count = countId;
    field_11C_state = BoneStates::eSpawned_0;
    field_11E_volume_modifier = 0;

    mShadow = ae_new<Shadow>();
}

void Bone::VTimeToExplodeRandom()
{
    // Empty
}

s32 Bone::CreateFromSaveState(const u8* pData)
{
    auto pState = reinterpret_cast<const Bone_SaveState*>(pData);

    auto pBone = ae_new<Bone>(pState->field_8_xpos, pState->field_C_ypos, pState->field_2A_count);

    pBone->mBaseGameObjectTlvInfo = pState->field_4_obj_id;

    pBone->mBaseAnimatedWithPhysicsGameObject_XPos = pState->field_8_xpos;
    pBone->mBaseAnimatedWithPhysicsGameObject_YPos = pState->field_C_ypos;

    pBone->field_E4_collection_rect.x = pBone->mBaseAnimatedWithPhysicsGameObject_XPos - (ScaleToGridSize(pBone->field_CC_sprite_scale) / FP_FromInteger(2));
    pBone->field_E4_collection_rect.y = pBone->mBaseAnimatedWithPhysicsGameObject_YPos - ScaleToGridSize(pBone->field_CC_sprite_scale);
    pBone->field_E4_collection_rect.w = (ScaleToGridSize(pBone->field_CC_sprite_scale) / FP_FromInteger(2)) + pBone->mBaseAnimatedWithPhysicsGameObject_XPos;
    pBone->field_E4_collection_rect.h = pBone->mBaseAnimatedWithPhysicsGameObject_YPos;

    pBone->field_C4_velx = pState->field_10_velx;
    pBone->field_C8_vely = pState->field_14_vely;

    pBone->field_C0_path_number = pState->field_1C_path_number;
    pBone->field_C2_lvl_number = pState->field_1E_lvl_number;
    pBone->field_CC_sprite_scale = pState->field_18_sprite_scale;

    pBone->field_D6_scale = pState->field_18_sprite_scale > FP_FromDouble(0.75);

    pBone->field_20_animation.mAnimFlags.Set(AnimFlags::eBit8_Loop, pState->field_20_flags.Get(Bone_SaveState::eBit3_bLoop));
    pBone->field_20_animation.mAnimFlags.Set(AnimFlags::eBit3_Render, pState->field_20_flags.Get(Bone_SaveState::eBit1_bRender));

    pBone->mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4, pState->field_20_flags.Get(Bone_SaveState::eBit2_bDrawable));
    pBone->mBaseGameObjectFlags.Set(BaseGameObject::eInteractive_Bit8, pState->field_20_flags.Get(Bone_SaveState::eBit4_bInteractive));

    pBone->mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit9_RestoredFromQuickSave);

    pBone->field_128_shine_timer = sGnFrame;

    pBone->BaseAliveGameObjectCollisionLineType = pState->field_28_line_type;
    pBone->field_118_count = pState->field_2A_count;
    pBone->field_11C_state = pState->field_2C_state;

    pBone->field_11E_volume_modifier = pState->field_2E_volume_modifier;
    pBone->field_120_xpos = pState->field_30_xpos;
    pBone->field_124_ypos = pState->field_34_ypos;

    pBone->field_12C_time_to_live = pState->field_38_time_to_live;

    pBone->field_130_hit_object = 0;
    if (pState->field_20_flags.Get(Bone_SaveState::eBit5_bHitObject))
    {
        pBone->field_130_hit_object |= 1;
    }

    return sizeof(Bone_SaveState);
}

Bone::~Bone()
{
    if (!gInfiniteGrenades_5C1BDE && !field_11A_bDead)
    {
        if (gpThrowableArray_5D1E2C)
        {
            gpThrowableArray_5D1E2C->Remove(field_118_count >= 1 ? field_118_count : 1);
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
    field_C4_velx = velX;
    field_C8_vely = velY;

    field_20_animation.mAnimFlags.Set(AnimFlags::eBit3_Render);

    if (field_118_count == 0)
    {
        field_11C_state = BoneStates::eEdible_4;
    }
    else
    {
        field_11C_state = BoneStates::eAirborne_1;
    }
}

void Bone::VOnTrapDoorOpen()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObjectId));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        BaseAliveGameObjectId = -1;
        if (field_11C_state == BoneStates::eCollided_2 || field_11C_state == BoneStates::eOnGround_3)
        {
            field_11C_state = BoneStates::eAirborne_1;
        }
    }
}

Bool32 Bone::VIsFalling()
{
    return field_11C_state == BoneStates::eFalling_5;
}

Bool32 Bone::VCanThrow()
{
    return field_11C_state != BoneStates::eSpawned_0 && field_11C_state != BoneStates::eAirborne_1;
}

s16 Bone::OnCollision(BaseAnimatedWithPhysicsGameObject* pObj)
{
    if (!pObj->mBaseGameObjectFlags.Get(BaseGameObject::eCanExplode_Bit7))
    {
        return 1;
    }

    if (pObj->Type() != ReliveTypes::eMine && pObj->Type() != ReliveTypes::eUXB && (field_130_hit_object & 1))
    {
        return 1;
    }

    if (pObj->Type() == ReliveTypes::eSecurityOrb && sControlledCharacter_5C1B8C->field_D6_scale != pObj->field_D6_scale)
    {
        return 1;
    }

    PSX_RECT bRect = {};
    pObj->VGetBoundingRect(&bRect, 1);

    if (field_120_xpos < FP_FromInteger(bRect.x) || field_120_xpos > FP_FromInteger(bRect.w))
    {
        mBaseAnimatedWithPhysicsGameObject_XPos -= field_C4_velx;
        field_C4_velx = (-field_C4_velx / FP_FromInteger(2));
    }
    else
    {
        if (field_C8_vely > FP_FromInteger(0))
        {
            const FP slowerVelY = (-field_C8_vely / FP_FromInteger(2));
            field_C8_vely = slowerVelY;
            mBaseAnimatedWithPhysicsGameObject_YPos += slowerVelY;
        }
    }

    pObj->VOnThrowableHit(this);

    field_130_hit_object |= 1u;
    SFX_Play_Mono(SoundEffect::RockBounceOnMine_24, 80);

    if (pObj->Type() == ReliveTypes::eMine || pObj->Type() == ReliveTypes::eUXB)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    return 0;
}

void Bone::VScreenChanged()
{
    if (gMap.mCurrentPath != gMap.mPath || gMap.mCurrentLevel != gMap.mLevel)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

Bool32 Bone::VCanBeEaten()
{
    return field_11C_state == BoneStates::eEdible_4;
}

s32 Bone::VGetSaveState(u8* pSaveBuffer)
{
    auto pState = reinterpret_cast<Bone_SaveState*>(pSaveBuffer);

    pState->field_0_type = AETypes::eBone_11;
    pState->field_4_obj_id = mBaseGameObjectTlvInfo;

    pState->field_8_xpos = mBaseAnimatedWithPhysicsGameObject_XPos;
    pState->field_C_ypos = mBaseAnimatedWithPhysicsGameObject_YPos;

    pState->field_10_velx = field_C4_velx;
    pState->field_14_vely = field_C8_vely;

    pState->field_1C_path_number = field_C0_path_number;
    pState->field_1E_lvl_number = field_C2_lvl_number;

    pState->field_18_sprite_scale = field_CC_sprite_scale;

    pState->field_20_flags.Set(Bone_SaveState::eBit3_bLoop, field_20_animation.mAnimFlags.Get(AnimFlags::eBit8_Loop));
    pState->field_20_flags.Set(Bone_SaveState::eBit1_bRender, field_20_animation.mAnimFlags.Get(AnimFlags::eBit3_Render));

    pState->field_20_flags.Set(Bone_SaveState::eBit2_bDrawable, mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4));
    pState->field_20_flags.Set(Bone_SaveState::eBit4_bInteractive, mBaseGameObjectFlags.Get(BaseGameObject::eInteractive_Bit8));

    pState->field_20_flags.Set(Bone_SaveState::eBit5_bHitObject, field_130_hit_object & 1);

    if (BaseAliveGameObjectCollisionLine)
    {
        pState->field_28_line_type = BaseAliveGameObjectCollisionLine->field_8_type;
    }
    else
    {
        pState->field_28_line_type = -1;
    }

    pState->field_24_base_id = BaseAliveGameObjectId;
    pState->field_2A_count = field_118_count;
    pState->field_2C_state = field_11C_state;

    pState->field_2E_volume_modifier = field_11E_volume_modifier;
    pState->field_30_xpos = field_120_xpos;

    pState->field_34_ypos = field_124_ypos;
    pState->field_38_time_to_live = field_12C_time_to_live;

    return sizeof(Bone_SaveState);
}

void Bone::InTheAir()
{
    field_120_xpos = mBaseAnimatedWithPhysicsGameObject_XPos;
    field_124_ypos = mBaseAnimatedWithPhysicsGameObject_YPos;

    if (field_C8_vely > FP_FromInteger(30))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    field_C8_vely += FP_FromInteger(1);
    mBaseAnimatedWithPhysicsGameObject_XPos += field_C4_velx;
    mBaseAnimatedWithPhysicsGameObject_YPos += field_C8_vely;

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast(
            field_120_xpos,
            field_124_ypos,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            field_D6_scale == 1 ? 0x09 : 0x90)
        == 1)
    {
        switch (BaseAliveGameObjectCollisionLine->field_8_type)
        {
            case 0u:
            case 4u:
            case 32u:
            case 36u:
                if (field_C8_vely <= FP_FromInteger(0))
                {
                    break;
                }

                mBaseAnimatedWithPhysicsGameObject_YPos = hitY;

                if (field_C8_vely < FP_FromInteger(1))
                {
                    field_11C_state = BoneStates::eCollided_2;

                    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(BaseAliveGameObjectCollisionLine->field_0_rect.y);
                    field_C8_vely = FP_FromInteger(0);
                    if (field_C4_velx >= FP_FromInteger(0) && field_C4_velx < FP_FromInteger(1))
                    {
                        field_C4_velx = FP_FromInteger(1);
                    }

                    if (field_C4_velx < FP_FromInteger(0) && field_C4_velx > FP_FromInteger(-1))
                    {
                        field_C4_velx = FP_FromInteger(-1);
                    }
                    return;
                }
                else
                {
                    mBaseAnimatedWithPhysicsGameObject_YPos -= FP_FromDouble(0.1);
                    field_C8_vely = (-field_C8_vely / FP_FromInteger(2));
                    field_C4_velx = (field_C4_velx / FP_FromInteger(2));
                    s16 vol = 20 * (4 - field_11E_volume_modifier);
                    if (vol < 40)
                    {
                        vol = 40;
                    }
                    SFX_Play_Mono(SoundEffect::RockBounce_26, vol);
                    Event_Broadcast(kEventNoise, this);
                    Event_Broadcast(kEventSuspiciousNoise, this);
                    field_11E_volume_modifier++;
                }
                break;

            case 3u:
            case 7u:
                if (field_C8_vely < FP_FromInteger(0))
                {
                    mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
                    field_C8_vely = (-field_C8_vely / FP_FromInteger(2));
                    s16 vol = 20 * (4 - field_11E_volume_modifier);
                    if (vol < 40)
                    {
                        vol = 40;
                    }
                    SFX_Play_Mono(SoundEffect::RockBounce_26, vol);
                    Event_Broadcast(kEventNoise, this);
                    Event_Broadcast(kEventSuspiciousNoise, this);
                }
                break;
        }
    }

    if (sCollisions_DArray_5C1128->Raycast(field_120_xpos, field_124_ypos, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, &BaseAliveGameObjectCollisionLine, &hitX, &hitY, field_D6_scale == 1 ? 0x06 : 0x60) == 1)
    {
        switch (BaseAliveGameObjectCollisionLine->field_8_type)
        {
            case 1u:
            case 5u:
                if (field_C4_velx < FP_FromInteger(0))
                {
                    field_C4_velx = (-field_C4_velx / FP_FromInteger(2));
                    mBaseAnimatedWithPhysicsGameObject_XPos = hitX;
                    mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
                    s16 vol = 20 * (4 - field_11E_volume_modifier);
                    if (vol < 40)
                    {
                        vol = 40;
                    }
                    SFX_Play_Mono(SoundEffect::RockBounce_26, vol);
                    Event_Broadcast(kEventNoise, this);
                    Event_Broadcast(kEventSuspiciousNoise, this);
                }
                BaseAliveGameObjectCollisionLine = nullptr;
                break;

            case 2u:
            case 6u:
                if (field_C4_velx > FP_FromInteger(0))
                {
                    field_C4_velx = (-field_C4_velx / FP_FromInteger(2));
                    mBaseAnimatedWithPhysicsGameObject_XPos = hitX;
                    mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
                    s16 vol = 20 * (4 - field_11E_volume_modifier);
                    if (vol < 40)
                    {
                        vol = 40;
                    }
                    SFX_Play_Mono(SoundEffect::RockBounce_26, vol);
                    Event_Broadcast(kEventNoise, this);
                    Event_Broadcast(kEventSuspiciousNoise, this);
                }
                BaseAliveGameObjectCollisionLine = nullptr;
                break;
        }
    }
}

void Bone::VUpdate()
{
    auto pObj = sObjectIds.Find_Impl(BaseAliveGameObjectId);
    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    switch (field_11C_state)
    {
        case BoneStates::eSpawned_0:
            break;

        case BoneStates::eAirborne_1:
            InTheAir();
            return;

        case BoneStates::eCollided_2:
        {
            PSX_RECT bRect = {};
            VGetBoundingRect(&bRect, 1);
            const s16 offset = field_D6_scale != 0 ? 5 : 0;
            const PSX_Point xy{bRect.x, static_cast<s16>(bRect.y + offset)};
            const PSX_Point wh{bRect.w, static_cast<s16>(bRect.h + offset)};
            VOnCollisionWith(xy, wh, gBaseGameObjects, 1, (TCollisionCallBack) &Bone::OnCollision);

            if (WallHit(FP_FromInteger(5), field_C4_velx))
            {
                field_C4_velx = -field_C4_velx;
            }

            if (FP_Abs(field_C4_velx) >= FP_FromInteger(1))
            {
                if (field_C4_velx <= FP_FromInteger(0))
                {
                    field_C4_velx = (FP_FromDouble(0.01) / field_CC_sprite_scale) + field_C4_velx;
                }
                else
                {
                    field_C4_velx = field_C4_velx - (FP_FromDouble(0.01) / field_CC_sprite_scale);
                }
                BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mBaseAnimatedWithPhysicsGameObject_XPos, &mBaseAnimatedWithPhysicsGameObject_YPos, field_C4_velx);
            }
            else
            {
                if (abs(SnapToXGrid(field_CC_sprite_scale, FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos)) - FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos)) <= 1)
                {
                    field_C4_velx = FP_FromInteger(0);
                    field_E4_collection_rect.x = mBaseAnimatedWithPhysicsGameObject_XPos - (ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(2));
                    field_E4_collection_rect.y = mBaseAnimatedWithPhysicsGameObject_YPos - ScaleToGridSize(field_CC_sprite_scale);
                    field_E4_collection_rect.w = mBaseAnimatedWithPhysicsGameObject_XPos + (ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(2));
                    field_E4_collection_rect.h = mBaseAnimatedWithPhysicsGameObject_YPos;

                    field_11C_state = BoneStates::eOnGround_3;
                    mBaseGameObjectFlags.Set(BaseGameObject::eInteractive_Bit8);
                    field_20_animation.mAnimFlags.Clear(AnimFlags::eBit8_Loop);
                    field_128_shine_timer = sGnFrame;
                    AddToPlatform();
                    return;
                }
                BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mBaseAnimatedWithPhysicsGameObject_XPos, &mBaseAnimatedWithPhysicsGameObject_YPos, field_C4_velx);
            }

            if (BaseAliveGameObjectCollisionLine)
            {
                return;
            }

            field_20_animation.mAnimFlags.Set(AnimFlags::eBit8_Loop);
            field_11C_state = BoneStates::eEdible_4;
        }
            return;

        case BoneStates::eOnGround_3:
            if (gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
            {
                field_12C_time_to_live = sGnFrame + 300;
            }

            if (static_cast<s32>(sGnFrame) > field_128_shine_timer && !pObj)
            {
                // For the shiny star twinkle effect.
                New_TintShiny_Particle(
                    mBaseAnimatedWithPhysicsGameObject_XPos + (field_CC_sprite_scale * FP_FromInteger(1)),
                    (field_CC_sprite_scale * FP_FromInteger(-7)) + mBaseAnimatedWithPhysicsGameObject_YPos,
                    FP_FromDouble(0.3),
                    Layer::eLayer_Foreground_36);

                field_128_shine_timer = (Math_NextRandom() % 16) + sGnFrame + 60;
            }

            if (field_12C_time_to_live < static_cast<s32>(sGnFrame))
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
            return;

        case BoneStates::eEdible_4:
        {
            InTheAir();
            PSX_RECT bRect = {};
            VGetBoundingRect(&bRect, 1);
            const s16 offset = field_D6_scale != 0 ? 5 : 0;
            const PSX_Point xy{bRect.x, static_cast<s16>(bRect.y + offset)};
            const PSX_Point wh{bRect.w, static_cast<s16>(bRect.h + offset)};
            VOnCollisionWith(xy, wh, gBaseGameObjects, 1, (TCollisionCallBack) &Bone::OnCollision);

            if (mBaseAnimatedWithPhysicsGameObject_YPos > FP_FromInteger(gMap.field_D4_ptr->field_6_bBottom))
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
        }
            return;

        case BoneStates::eFalling_5:
            field_C8_vely += FP_FromInteger(1);
            mBaseAnimatedWithPhysicsGameObject_XPos += field_C4_velx;
            mBaseAnimatedWithPhysicsGameObject_YPos = field_C8_vely + mBaseAnimatedWithPhysicsGameObject_YPos;
            if (!gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
            return;
    }
}

s16 Bone::VGetCount()
{
    if (field_11C_state == BoneStates::eOnGround_3 && field_118_count == 0)
    {
        return 1;
    }
    return field_118_count;
}

const TintEntry kBoneTints_550EC0[18] = {
    {LevelIds::eMines_1, 127u, 127u, 127u},
    {LevelIds::eNecrum_2, 137u, 137u, 137u},
    {LevelIds::eMudomoVault_3, 127u, 127u, 127u},
    {LevelIds::eMudancheeVault_4, 127u, 127u, 127u},
    {LevelIds::eFeeCoDepot_5, 127u, 127u, 127u},
    {LevelIds::eBarracks_6, 127u, 127u, 127u},
    {LevelIds::eMudancheeVault_Ender_7, 127u, 127u, 127u},
    {LevelIds::eBonewerkz_8, 127u, 127u, 127u},
    {LevelIds::eBrewery_9, 127u, 127u, 127u},
    {LevelIds::eBrewery_Ender_10, 127u, 127u, 127u},
    {LevelIds::eMudomoVault_Ender_11, 127u, 127u, 127u},
    {LevelIds::eFeeCoDepot_Ender_12, 127u, 127u, 127u},
    {LevelIds::eBarracks_Ender_13, 127u, 127u, 127u},
    {LevelIds::eBonewerkz_Ender_14, 127u, 127u, 127u},
    {LevelIds::eNone, 127u, 127u, 127u}};


BoneBag::BoneBag(Path_BoneBag* pTlv, s32 tlvInfo)
    : BaseAliveGameObject(0)
{
    SetType(ReliveTypes::eBoneBag);

    const AnimRecord& rec = AnimRec(AnimId::BoneBag_Idle);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    // TODO: Super super OWI hack, WTF?? Figure out exactly what this is patching in the animation
    *((u16*) *ppRes + 4374) = 0;

    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);
    field_20_animation.mAnimFlags.Clear(AnimFlags::eBit15_bSemiTrans);
    SetTint(&kBoneTints_550EC0[0], gMap.mCurrentLevel);

    field_11C_bIs_hit = 0;
    field_118_tlvInfo = tlvInfo;

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->field_C_bottom_right.field_2_y);

    mApplyShadows &= ~1u;

    field_124_velX = FP_FromRaw(pTlv->field_12_x_vel << 8);
    field_128_velY = FP_FromRaw(-256 * pTlv->field_14_y_vel); // TODO: << 8 negated ??

    if (pTlv->field_10_bone_fall_direction == XDirection_short::eLeft_0)
    {
        field_124_velX = -field_124_velX;
    }

    if (pTlv->field_16_scale == Scale_short::eHalf_1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
    }
    else if (pTlv->field_16_scale == Scale_short::eFull_0)
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_D6_scale = 1;
    }

    field_11E_count = pTlv->field_18_bone_amount;
    field_120_allow_sound = 1;
    field_122_force_play_sound = 1;

    mShadow = ae_new<Shadow>();
}

void BoneBag::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

BoneBag::~BoneBag()
{
    Path::TLV_Reset(field_118_tlvInfo, -1, 0, 0);
}

void BoneBag::VUpdate()
{
    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (field_20_animation.field_92_current_frame == 2)
    {
        if (field_120_allow_sound)
        {
            if (Math_NextRandom() < 40 || field_122_force_play_sound)
            {
                field_120_allow_sound = 0;
                field_122_force_play_sound = 0;
                SFX_Play_Pitch(SoundEffect::SackWobble_29, 24, Math_RandomRange(-2400, -2200));
            }
        }
    }
    else
    {
        field_120_allow_sound = 0;
    }

    if (field_11C_bIs_hit)
    {
        if (field_11C_bIs_hit != 1)
        {
            return;
        }

        if (!field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            return;
        }

        const AnimRecord& animRec = AnimRec(AnimId::BoneBag_Idle);
        field_20_animation.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
        field_11C_bIs_hit = 0;
        return;
    }

    if (field_20_animation.mFrameChangeCounter == 0)
    {
        field_20_animation.mFrameChangeCounter = Math_RandomRange(2, 10);
    }

    PSX_RECT bPlayerRect = {};
    sActiveHero->VGetBoundingRect(&bPlayerRect, 1);

    PSX_RECT bRect = {};
    VGetBoundingRect(&bRect, 1);

    if (bRect.x <= bPlayerRect.w && bRect.w >= bPlayerRect.x && bRect.h >= bPlayerRect.y && bRect.y <= bPlayerRect.h && field_CC_sprite_scale == sActiveHero->field_CC_sprite_scale)
    {
        if (gpThrowableArray_5D1E2C)
        {
            if (gpThrowableArray_5D1E2C->field_20_count)
            {
                if (sActiveHero->mCurrentMotion == 31)
                {
                    const AnimRecord& animRec = AnimRec(AnimId::BoneBag_HardHit);
                    field_20_animation.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
                }
                else
                {
                    const AnimRecord& animRec = AnimRec(AnimId::BoneBag_SoftHit);
                    field_20_animation.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
                }
                field_11C_bIs_hit = 1;
                return;
            }
        }
        else
        {
            gpThrowableArray_5D1E2C = ae_new<ThrowableArray>();
        }

        gpThrowableArray_5D1E2C->Add(field_11E_count);

        auto pBone = ae_new<Bone>(mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(30), field_11E_count);

        pBone->field_CC_sprite_scale = field_CC_sprite_scale;
        pBone->field_D6_scale = field_D6_scale;

        pBone->VThrow(field_124_velX, field_128_velY);

        SFX_Play_Mono(SoundEffect::SackHit_25, 0);
        Environment_SFX_457A40(EnvironmentSfx::eDeathNoise_7, 0, 0x7FFF, 0);

        if (sActiveHero->mCurrentMotion == 31)
        {
            const AnimRecord& animRec = AnimRec(AnimId::BoneBag_HardHit);
            field_20_animation.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
        }
        else
        {
            const AnimRecord& animRec = AnimRec(AnimId::BoneBag_SoftHit);
            field_20_animation.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
        }

        field_11C_bIs_hit = 1;
    }
}
