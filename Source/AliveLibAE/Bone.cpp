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
        LoadRockTypes_49AB30(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber);
    }

    const AnimRecord& rec = AnimRec(AnimId::Bone);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit15_bSemiTrans);

    mBaseAnimatedWithPhysicsGameObject_XPos = xpos;
    mBaseAnimatedWithPhysicsGameObject_YPos = ypos;
    field_120_xpos = xpos;
    field_124_ypos = ypos;
    mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
    mBaseGameObjectFlags.Clear(BaseGameObject::eInteractive_Bit8);
    field_130_hit_object &= ~1u;

    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);

    field_12C_time_to_live = sGnFrame + 300;
    field_118_count = countId;
    field_11C_state = BoneStates::eSpawned_0;
    field_11E_volume_modifier = 0;

    mShadow = relive_new Shadow();
}

void Bone::VTimeToExplodeRandom()
{
    // Empty
}

s32 Bone::CreateFromSaveState(const u8* pData)
{
    auto pState = reinterpret_cast<const Bone_SaveState*>(pData);

    auto pBone = relive_new Bone(pState->field_8_xpos, pState->field_C_ypos, pState->field_2A_count);

    pBone->mBaseGameObjectTlvInfo = pState->field_4_obj_id;

    pBone->mBaseAnimatedWithPhysicsGameObject_XPos = pState->field_8_xpos;
    pBone->mBaseAnimatedWithPhysicsGameObject_YPos = pState->field_C_ypos;

    pBone->mCollectionRect.x = pBone->mBaseAnimatedWithPhysicsGameObject_XPos - (ScaleToGridSize(pBone->mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(2));
    pBone->mCollectionRect.y = pBone->mBaseAnimatedWithPhysicsGameObject_YPos - ScaleToGridSize(pBone->mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    pBone->mCollectionRect.w = (ScaleToGridSize(pBone->mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(2)) + pBone->mBaseAnimatedWithPhysicsGameObject_XPos;
    pBone->mCollectionRect.h = pBone->mBaseAnimatedWithPhysicsGameObject_YPos;

    pBone->mBaseAnimatedWithPhysicsGameObject_VelX = pState->field_10_velx;
    pBone->mBaseAnimatedWithPhysicsGameObject_VelY = pState->field_14_vely;

    pBone->mBaseAnimatedWithPhysicsGameObject_PathNumber = pState->field_1C_path_number;
    pBone->mBaseAnimatedWithPhysicsGameObject_LvlNumber = MapWrapper::FromAE(pState->field_1E_lvl_number);
    pBone->mBaseAnimatedWithPhysicsGameObject_SpriteScale = pState->field_18_sprite_scale;

    pBone->mBaseAnimatedWithPhysicsGameObject_Scale = pState->field_18_sprite_scale > FP_FromDouble(0.75);

    pBone->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit8_Loop, pState->field_20_flags.Get(Bone_SaveState::eBit3_bLoop));
    pBone->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit3_Render, pState->field_20_flags.Get(Bone_SaveState::eBit1_bRender));

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
    mBaseAnimatedWithPhysicsGameObject_VelX = velX;
    mBaseAnimatedWithPhysicsGameObject_VelY = velY;

    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit3_Render);

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

    if (pObj->Type() == ReliveTypes::eSecurityOrb && sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_Scale != pObj->mBaseAnimatedWithPhysicsGameObject_Scale)
    {
        return 1;
    }

    const PSX_RECT bRect = pObj->VGetBoundingRect();

    if (field_120_xpos < FP_FromInteger(bRect.x) || field_120_xpos > FP_FromInteger(bRect.w))
    {
        mBaseAnimatedWithPhysicsGameObject_XPos -= mBaseAnimatedWithPhysicsGameObject_VelX;
        mBaseAnimatedWithPhysicsGameObject_VelX = (-mBaseAnimatedWithPhysicsGameObject_VelX / FP_FromInteger(2));
    }
    else
    {
        if (mBaseAnimatedWithPhysicsGameObject_VelY > FP_FromInteger(0))
        {
            const FP slowerVelY = (-mBaseAnimatedWithPhysicsGameObject_VelY / FP_FromInteger(2));
            mBaseAnimatedWithPhysicsGameObject_VelY = slowerVelY;
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

    pState->field_10_velx = mBaseAnimatedWithPhysicsGameObject_VelX;
    pState->field_14_vely = mBaseAnimatedWithPhysicsGameObject_VelY;

    pState->field_1C_path_number = mBaseAnimatedWithPhysicsGameObject_PathNumber;
    pState->field_1E_lvl_number = MapWrapper::ToAE(mBaseAnimatedWithPhysicsGameObject_LvlNumber);

    pState->field_18_sprite_scale = mBaseAnimatedWithPhysicsGameObject_SpriteScale;

    pState->field_20_flags.Set(Bone_SaveState::eBit3_bLoop, mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit8_Loop));
    pState->field_20_flags.Set(Bone_SaveState::eBit1_bRender, mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit3_Render));

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

    if (mBaseAnimatedWithPhysicsGameObject_VelY > FP_FromInteger(30))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    mBaseAnimatedWithPhysicsGameObject_VelY += FP_FromInteger(1);
    mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX;
    mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_VelY;

    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
            field_120_xpos,
            field_124_ypos,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            mBaseAnimatedWithPhysicsGameObject_Scale == 1 ? kFgFloorOrCeiling : kBgFloorOrCeiling)
        == 1)
    {
        switch (BaseAliveGameObjectCollisionLine->field_8_type)
        {
            case 0u:
            case 4u:
            case 32u:
            case 36u:
                if (mBaseAnimatedWithPhysicsGameObject_VelY <= FP_FromInteger(0))
                {
                    break;
                }

                mBaseAnimatedWithPhysicsGameObject_YPos = hitY;

                if (mBaseAnimatedWithPhysicsGameObject_VelY < FP_FromInteger(1))
                {
                    field_11C_state = BoneStates::eCollided_2;

                    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(BaseAliveGameObjectCollisionLine->field_0_rect.y);
                    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
                    if (mBaseAnimatedWithPhysicsGameObject_VelX >= FP_FromInteger(0) && mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(1))
                    {
                        mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(1);
                    }

                    if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0) && mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(-1))
                    {
                        mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(-1);
                    }
                    return;
                }
                else
                {
                    mBaseAnimatedWithPhysicsGameObject_YPos -= FP_FromDouble(0.1);
                    mBaseAnimatedWithPhysicsGameObject_VelY = (-mBaseAnimatedWithPhysicsGameObject_VelY / FP_FromInteger(2));
                    mBaseAnimatedWithPhysicsGameObject_VelX = (mBaseAnimatedWithPhysicsGameObject_VelX / FP_FromInteger(2));
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
                if (mBaseAnimatedWithPhysicsGameObject_VelY < FP_FromInteger(0))
                {
                    mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
                    mBaseAnimatedWithPhysicsGameObject_VelY = (-mBaseAnimatedWithPhysicsGameObject_VelY / FP_FromInteger(2));
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

    if (sCollisions->Raycast(field_120_xpos, field_124_ypos, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, &BaseAliveGameObjectCollisionLine, &hitX, &hitY, mBaseAnimatedWithPhysicsGameObject_Scale != 0 ? kFgWalls : kBgWalls) == 1)
    {
        switch (BaseAliveGameObjectCollisionLine->field_8_type)
        {
            case 1u:
            case 5u:
                if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0))
                {
                    mBaseAnimatedWithPhysicsGameObject_VelX = (-mBaseAnimatedWithPhysicsGameObject_VelX / FP_FromInteger(2));
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
                if (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0))
                {
                    mBaseAnimatedWithPhysicsGameObject_VelX = (-mBaseAnimatedWithPhysicsGameObject_VelX / FP_FromInteger(2));
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
            const PSX_RECT bRect = VGetBoundingRect();
            const s16 offset = mBaseAnimatedWithPhysicsGameObject_Scale != 0 ? 5 : 0;
            const PSX_Point xy{bRect.x, static_cast<s16>(bRect.y + offset)};
            const PSX_Point wh{bRect.w, static_cast<s16>(bRect.h + offset)};
            VOnCollisionWith(xy, wh, gBaseGameObjects, 1, (TCollisionCallBack) &Bone::OnCollision);

            if (WallHit(FP_FromInteger(5), mBaseAnimatedWithPhysicsGameObject_VelX))
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = -mBaseAnimatedWithPhysicsGameObject_VelX;
            }

            if (FP_Abs(mBaseAnimatedWithPhysicsGameObject_VelX) >= FP_FromInteger(1))
            {
                if (mBaseAnimatedWithPhysicsGameObject_VelX <= FP_FromInteger(0))
                {
                    mBaseAnimatedWithPhysicsGameObject_VelX = (FP_FromDouble(0.01) / mBaseAnimatedWithPhysicsGameObject_SpriteScale) + mBaseAnimatedWithPhysicsGameObject_VelX;
                }
                else
                {
                    mBaseAnimatedWithPhysicsGameObject_VelX = mBaseAnimatedWithPhysicsGameObject_VelX - (FP_FromDouble(0.01) / mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                }
                BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mBaseAnimatedWithPhysicsGameObject_XPos, &mBaseAnimatedWithPhysicsGameObject_YPos, mBaseAnimatedWithPhysicsGameObject_VelX);
            }
            else
            {
                if (abs(SnapToXGrid(mBaseAnimatedWithPhysicsGameObject_SpriteScale, FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos)) - FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos)) <= 1)
                {
                    mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
                    mCollectionRect.x = mBaseAnimatedWithPhysicsGameObject_XPos - (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(2));
                    mCollectionRect.y = mBaseAnimatedWithPhysicsGameObject_YPos - ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                    mCollectionRect.w = mBaseAnimatedWithPhysicsGameObject_XPos + (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(2));
                    mCollectionRect.h = mBaseAnimatedWithPhysicsGameObject_YPos;

                    field_11C_state = BoneStates::eOnGround_3;
                    mBaseGameObjectFlags.Set(BaseGameObject::eInteractive_Bit8);
                    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit8_Loop);
                    field_128_shine_timer = sGnFrame;
                    AddToPlatform();
                    return;
                }
                BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mBaseAnimatedWithPhysicsGameObject_XPos, &mBaseAnimatedWithPhysicsGameObject_YPos, mBaseAnimatedWithPhysicsGameObject_VelX);
            }

            if (BaseAliveGameObjectCollisionLine)
            {
                return;
            }

            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit8_Loop);
            field_11C_state = BoneStates::eEdible_4;
        }
            return;

        case BoneStates::eOnGround_3:
            if (gMap.Is_Point_In_Current_Camera_4810D0(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
            {
                field_12C_time_to_live = sGnFrame + 300;
            }

            if (static_cast<s32>(sGnFrame) > field_128_shine_timer && !pObj)
            {
                // For the shiny star twinkle effect.
                New_TintShiny_Particle(
                    mBaseAnimatedWithPhysicsGameObject_XPos + (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(1)),
                    (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(-7)) + mBaseAnimatedWithPhysicsGameObject_YPos,
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
            const PSX_RECT bRect = VGetBoundingRect();
            const s16 offset = mBaseAnimatedWithPhysicsGameObject_Scale != 0 ? 5 : 0;
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
            mBaseAnimatedWithPhysicsGameObject_VelY += FP_FromInteger(1);
            mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX;
            mBaseAnimatedWithPhysicsGameObject_YPos = mBaseAnimatedWithPhysicsGameObject_VelY + mBaseAnimatedWithPhysicsGameObject_YPos;
            if (!gMap.Is_Point_In_Current_Camera_4810D0(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
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

    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);
    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit15_bSemiTrans);
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
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
        mBaseAnimatedWithPhysicsGameObject_Scale = 0;
    }
    else if (pTlv->field_16_scale == Scale_short::eFull_0)
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromInteger(1);
        mBaseAnimatedWithPhysicsGameObject_Scale = 1;
    }

    field_11E_count = pTlv->field_18_bone_amount;
    field_120_allow_sound = 1;
    field_122_force_play_sound = 1;

    mShadow = relive_new Shadow();
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

    if (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame == 2)
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

        if (!mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            return;
        }

        const AnimRecord& animRec = AnimRec(AnimId::BoneBag_Idle);
        mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
        field_11C_bIs_hit = 0;
        return;
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mFrameChangeCounter == 0)
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mFrameChangeCounter = Math_RandomRange(2, 10);
    }

    const PSX_RECT bPlayerRect = sActiveHero->VGetBoundingRect();
    const PSX_RECT bRect = VGetBoundingRect();

    if (bRect.x <= bPlayerRect.w && bRect.w >= bPlayerRect.x && bRect.h >= bPlayerRect.y && bRect.y <= bPlayerRect.h && mBaseAnimatedWithPhysicsGameObject_SpriteScale == sActiveHero->mBaseAnimatedWithPhysicsGameObject_SpriteScale)
    {
        if (gpThrowableArray_5D1E2C)
        {
            if (gpThrowableArray_5D1E2C->field_20_count)
            {
                if (sActiveHero->mCurrentMotion == 31) // TODO constant
                {
                    const AnimRecord& animRec = AnimRec(AnimId::BoneBag_HardHit);
                    mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
                }
                else
                {
                    const AnimRecord& animRec = AnimRec(AnimId::BoneBag_SoftHit);
                    mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
                }
                field_11C_bIs_hit = 1;
                return;
            }
        }
        else
        {
            gpThrowableArray_5D1E2C = relive_new ThrowableArray();
        }

        gpThrowableArray_5D1E2C->Add(field_11E_count);

        auto pBone = relive_new Bone(mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(30), field_11E_count);

        pBone->mBaseAnimatedWithPhysicsGameObject_SpriteScale = mBaseAnimatedWithPhysicsGameObject_SpriteScale;
        pBone->mBaseAnimatedWithPhysicsGameObject_Scale = mBaseAnimatedWithPhysicsGameObject_Scale;

        pBone->VThrow(field_124_velX, field_128_velY);

        SFX_Play_Mono(SoundEffect::SackHit_25, 0);
        Environment_SFX_457A40(EnvironmentSfx::eDeathNoise_7, 0, 0x7FFF, 0);

        if (sActiveHero->mCurrentMotion == 31)
        {
            const AnimRecord& animRec = AnimRec(AnimId::BoneBag_HardHit);
            mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
        }
        else
        {
            const AnimRecord& animRec = AnimRec(AnimId::BoneBag_SoftHit);
            mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
        }

        field_11C_bIs_hit = 1;
    }
}
