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
    SetType(AETypes::eBone_11);
    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kBoneResID, 0, 0))
    {
        LoadRockTypes_49AB30(mLvlNumber, mPathNumber);
    }

    const AnimRecord& rec = AnimRec(AnimId::Bone);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    mAnim.mAnimFlags.Clear(AnimFlags::eBit15_bSemiTrans);

    mXPos = xpos;
    mYPos = ypos;
    field_120_xpos = xpos;
    field_124_ypos = ypos;
    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);
    mGameObjectFlags.Clear(BaseGameObject::eInteractive_Bit8);
    field_130_hit_object &= ~1u;

    mAnim.mAnimFlags.Clear(AnimFlags::eBit3_Render);

    field_12C_time_to_live = sGnFrame_5C1B84 + 300;
    mCount = countId;
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

    pBone->field_C_objectId = pState->field_4_obj_id;

    pBone->mXPos = pState->field_8_xpos;
    pBone->mYPos = pState->field_C_ypos;

    pBone->mCollectionRect.x = pBone->mXPos - (ScaleToGridSize(pBone->mSpriteScale) / FP_FromInteger(2));
    pBone->mCollectionRect.y = pBone->mYPos - ScaleToGridSize(pBone->mSpriteScale);
    pBone->mCollectionRect.w = (ScaleToGridSize(pBone->mSpriteScale) / FP_FromInteger(2)) + pBone->mXPos;
    pBone->mCollectionRect.h = pBone->mYPos;

    pBone->mVelX = pState->field_10_velx;
    pBone->mVelY = pState->field_14_vely;

    pBone->mPathNumber = pState->field_1C_path_number;
    pBone->mLvlNumber = pState->field_1E_lvl_number;
    pBone->mSpriteScale = pState->field_18_sprite_scale;

    pBone->mScale = pState->field_18_sprite_scale > FP_FromDouble(0.75);

    pBone->mAnim.mAnimFlags.Set(AnimFlags::eBit8_Loop, pState->field_20_flags.Get(Bone_SaveState::eBit3_bLoop));
    pBone->mAnim.mAnimFlags.Set(AnimFlags::eBit3_Render, pState->field_20_flags.Get(Bone_SaveState::eBit1_bRender));

    pBone->mGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4, pState->field_20_flags.Get(Bone_SaveState::eBit2_bDrawable));
    pBone->mGameObjectFlags.Set(BaseGameObject::eInteractive_Bit8, pState->field_20_flags.Get(Bone_SaveState::eBit4_bInteractive));

    pBone->mAliveGameObjectFlags.Set(Flags_114::e114_Bit9_RestoredFromQuickSave);

    pBone->field_128_shine_timer = sGnFrame_5C1B84;

    pBone->mCollisionLineType = pState->field_28_line_type;
    pBone->mCount = pState->field_2A_count;
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
            gpThrowableArray_5D1E2C->Remove(mCount >= 1 ? mCount : 1);
        }
    }
}

void Bone::AddToPlatform()
{
    BaseAddToPlatform([](AETypes type)
                      { return type == AETypes::eLiftPoint_78 || type == AETypes::eTrapDoor_142; });
}

void Bone::VThrow(FP velX, FP velY)
{
    mVelX = velX;
    mVelY = velY;

    mAnim.mAnimFlags.Set(AnimFlags::eBit3_Render);

    if (mCount == 0)
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
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(mObjectId));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        mObjectId = -1;
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
    if (!pObj->mGameObjectFlags.Get(BaseGameObject::eCanExplode_Bit7))
    {
        return 1;
    }

    if (pObj->Type() != AETypes::eMine_88 && pObj->Type() != AETypes::eUXB_143 && (field_130_hit_object & 1))
    {
        return 1;
    }

    if (pObj->Type() == AETypes::eSecurityOrb_83 && sControlledCharacter->mScale != pObj->mScale)
    {
        return 1;
    }

    PSX_RECT bRect = {};
    pObj->VGetBoundingRect(&bRect, 1);

    if (field_120_xpos < FP_FromInteger(bRect.x) || field_120_xpos > FP_FromInteger(bRect.w))
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

    field_130_hit_object |= 1u;
    SFX_Play_Mono(SoundEffect::RockBounceOnMine_24, 80);

    if (pObj->Type() == AETypes::eMine_88 || pObj->Type() == AETypes::eUXB_143)
    {
        mGameObjectFlags.Set(BaseGameObject::eDead);
    }

    return 0;
}

void Bone::VScreenChanged()
{
    if (gMap.mCurrentPath != gMap.mPath || gMap.mCurrentLevel != gMap.mLevel)
    {
        mGameObjectFlags.Set(BaseGameObject::eDead);
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
    pState->field_4_obj_id = field_C_objectId;

    pState->field_8_xpos = mXPos;
    pState->field_C_ypos = mYPos;

    pState->field_10_velx = mVelX;
    pState->field_14_vely = mVelY;

    pState->field_1C_path_number = mPathNumber;
    pState->field_1E_lvl_number = mLvlNumber;

    pState->field_18_sprite_scale = mSpriteScale;

    pState->field_20_flags.Set(Bone_SaveState::eBit3_bLoop, mAnim.mAnimFlags.Get(AnimFlags::eBit8_Loop));
    pState->field_20_flags.Set(Bone_SaveState::eBit1_bRender, mAnim.mAnimFlags.Get(AnimFlags::eBit3_Render));

    pState->field_20_flags.Set(Bone_SaveState::eBit2_bDrawable, mGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4));
    pState->field_20_flags.Set(Bone_SaveState::eBit4_bInteractive, mGameObjectFlags.Get(BaseGameObject::eInteractive_Bit8));

    pState->field_20_flags.Set(Bone_SaveState::eBit5_bHitObject, field_130_hit_object & 1);

    if (mCollisionLine)
    {
        pState->field_28_line_type = mCollisionLine->field_8_type;
    }
    else
    {
        pState->field_28_line_type = -1;
    }

    pState->field_24_base_id = mObjectId;
    pState->field_2A_count = mCount;
    pState->field_2C_state = field_11C_state;

    pState->field_2E_volume_modifier = field_11E_volume_modifier;
    pState->field_30_xpos = field_120_xpos;

    pState->field_34_ypos = field_124_ypos;
    pState->field_38_time_to_live = field_12C_time_to_live;

    return sizeof(Bone_SaveState);
}

void Bone::InTheAir()
{
    field_120_xpos = mXPos;
    field_124_ypos = mYPos;

    if (mVelY > FP_FromInteger(30))
    {
        mGameObjectFlags.Set(BaseGameObject::eDead);
    }

    mVelY += FP_FromInteger(1);
    mXPos += mVelX;
    mYPos += mVelY;

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast(
            field_120_xpos,
            field_124_ypos,
            mXPos,
            mYPos,
            &mCollisionLine,
            &hitX,
            &hitY,
            mScale == 1 ? 0x09 : 0x90)
        == 1)
    {
        switch (mCollisionLine->field_8_type)
        {
            case 0u:
            case 4u:
            case 32u:
            case 36u:
                if (mVelY <= FP_FromInteger(0))
                {
                    break;
                }

                mYPos = hitY;

                if (mVelY < FP_FromInteger(1))
                {
                    field_11C_state = BoneStates::eCollided_2;

                    mYPos = FP_FromInteger(mCollisionLine->field_0_rect.y);
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
                if (mVelY < FP_FromInteger(0))
                {
                    mYPos = hitY;
                    mVelY = (-mVelY / FP_FromInteger(2));
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

    if (sCollisions_DArray_5C1128->Raycast(field_120_xpos, field_124_ypos, mXPos, mYPos, &mCollisionLine, &hitX, &hitY, mScale == 1 ? 0x06 : 0x60) == 1)
    {
        switch (mCollisionLine->field_8_type)
        {
            case 1u:
            case 5u:
                if (mVelX < FP_FromInteger(0))
                {
                    mVelX = (-mVelX / FP_FromInteger(2));
                    mXPos = hitX;
                    mYPos = hitY;
                    s16 vol = 20 * (4 - field_11E_volume_modifier);
                    if (vol < 40)
                    {
                        vol = 40;
                    }
                    SFX_Play_Mono(SoundEffect::RockBounce_26, vol);
                    Event_Broadcast(kEventNoise, this);
                    Event_Broadcast(kEventSuspiciousNoise, this);
                }
                mCollisionLine = nullptr;
                break;

            case 2u:
            case 6u:
                if (mVelX > FP_FromInteger(0))
                {
                    mVelX = (-mVelX / FP_FromInteger(2));
                    mXPos = hitX;
                    mYPos = hitY;
                    s16 vol = 20 * (4 - field_11E_volume_modifier);
                    if (vol < 40)
                    {
                        vol = 40;
                    }
                    SFX_Play_Mono(SoundEffect::RockBounce_26, vol);
                    Event_Broadcast(kEventNoise, this);
                    Event_Broadcast(kEventSuspiciousNoise, this);
                }
                mCollisionLine = nullptr;
                break;
        }
    }
}

void Bone::VUpdate()
{
    auto pObj = sObjectIds.Find_Impl(mObjectId);
    if (Event_Get(kEventDeathReset))
    {
        mGameObjectFlags.Set(BaseGameObject::eDead);
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
            const s16 offset = mScale != 0 ? 5 : 0;
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
                mCollisionLine = mCollisionLine->MoveOnLine(&mXPos, &mYPos, mVelX);
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

                    field_11C_state = BoneStates::eOnGround_3;
                    mGameObjectFlags.Set(BaseGameObject::eInteractive_Bit8);
                    mAnim.mAnimFlags.Clear(AnimFlags::eBit8_Loop);
                    field_128_shine_timer = sGnFrame_5C1B84;
                    AddToPlatform();
                    return;
                }
                mCollisionLine = mCollisionLine->MoveOnLine(&mXPos, &mYPos, mVelX);
            }

            if (mCollisionLine)
            {
                return;
            }

            mAnim.mAnimFlags.Set(AnimFlags::eBit8_Loop);
            field_11C_state = BoneStates::eEdible_4;
        }
            return;

        case BoneStates::eOnGround_3:
            if (gMap.Is_Point_In_Current_Camera_4810D0(mLvlNumber, mPathNumber, mXPos, mYPos, 0))
            {
                field_12C_time_to_live = sGnFrame_5C1B84 + 300;
            }

            if (static_cast<s32>(sGnFrame_5C1B84) > field_128_shine_timer && !pObj)
            {
                // For the shiny star twinkle effect.
                New_TintShiny_Particle(
                    mXPos + (mSpriteScale * FP_FromInteger(1)),
                    (mSpriteScale * FP_FromInteger(-7)) + mYPos,
                    FP_FromDouble(0.3),
                    Layer::eLayer_Foreground_36);

                field_128_shine_timer = (Math_NextRandom() % 16) + sGnFrame_5C1B84 + 60;
            }

            if (field_12C_time_to_live < static_cast<s32>(sGnFrame_5C1B84))
            {
                mGameObjectFlags.Set(BaseGameObject::eDead);
            }
            return;

        case BoneStates::eEdible_4:
        {
            InTheAir();
            PSX_RECT bRect = {};
            VGetBoundingRect(&bRect, 1);
            const s16 offset = mScale != 0 ? 5 : 0;
            const PSX_Point xy{bRect.x, static_cast<s16>(bRect.y + offset)};
            const PSX_Point wh{bRect.w, static_cast<s16>(bRect.h + offset)};
            VOnCollisionWith(xy, wh, gBaseGameObjects, 1, (TCollisionCallBack) &Bone::OnCollision);

            if (mYPos > FP_FromInteger(gMap.field_D4_ptr->field_6_bBottom))
            {
                mGameObjectFlags.Set(BaseGameObject::eDead);
            }
        }
            return;

        case BoneStates::eFalling_5:
            mVelY += FP_FromInteger(1);
            mXPos += mVelX;
            mYPos = mVelY + mYPos;
            if (!gMap.Is_Point_In_Current_Camera_4810D0(mLvlNumber, mPathNumber, mXPos, mYPos, 0))
            {
                mGameObjectFlags.Set(BaseGameObject::eDead);
            }
            return;
    }
}

s16 Bone::VGetCount()
{
    if (field_11C_state == BoneStates::eOnGround_3 && mCount == 0)
    {
        return 1;
    }
    return mCount;
}

const TintEntry kBoneTints_550EC0[18] = {
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
    {LevelIds_s8::eNone, 127u, 127u, 127u}};


BoneBag::BoneBag(Path_BoneBag* pTlv, s32 tlvInfo)
    : BaseAliveGameObject(0)
{
    SetType(AETypes::eBoneBag_12);

    const AnimRecord& rec = AnimRec(AnimId::BoneBag_Idle);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    // TODO: Super super OWI hack, WTF?? Figure out exactly what this is patching in the animation
    *((u16*) *ppRes + 4374) = 0;

    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);
    mAnim.mAnimFlags.Clear(AnimFlags::eBit15_bSemiTrans);
    SetTint(&kBoneTints_550EC0[0], gMap.mCurrentLevel);

    field_11C_bIs_hit = 0;
    field_118_tlvInfo = tlvInfo;

    mXPos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    mYPos = FP_FromInteger(pTlv->field_C_bottom_right.field_2_y);

    mApplyShadows &= ~1u;

    field_124_velX = FP_FromRaw(pTlv->field_12_x_vel << 8);
    field_128_velY = FP_FromRaw(-256 * pTlv->field_14_y_vel); // TODO: << 8 negated ??

    if (pTlv->field_10_bone_fall_direction == XDirection_short::eLeft_0)
    {
        field_124_velX = -field_124_velX;
    }

    if (pTlv->field_16_scale == Scale_short::eHalf_1)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mScale = 0;
    }
    else if (pTlv->field_16_scale == Scale_short::eFull_0)
    {
        mSpriteScale = FP_FromInteger(1);
        mScale = 1;
    }

    field_11E_count = pTlv->field_18_bone_amount;
    field_120_allow_sound = 1;
    field_122_force_play_sound = 1;

    mShadow = ae_new<Shadow>();
}

void BoneBag::VScreenChanged()
{
    mGameObjectFlags.Set(BaseGameObject::eDead);
}

BoneBag::~BoneBag()
{
    Path::TLV_Reset(field_118_tlvInfo, -1, 0, 0);
}

void BoneBag::VUpdate()
{
    if (Event_Get(kEventDeathReset))
    {
        mGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (mAnim.field_92_current_frame == 2)
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

        if (!mAnim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            return;
        }

        const AnimRecord& animRec = AnimRec(AnimId::BoneBag_Idle);
        mAnim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
        field_11C_bIs_hit = 0;
        return;
    }

    if (mAnim.mFrameChangeCounter == 0)
    {
        mAnim.mFrameChangeCounter = Math_RandomRange(2, 10);
    }

    PSX_RECT bPlayerRect = {};
    sActiveHero->VGetBoundingRect(&bPlayerRect, 1);

    PSX_RECT bRect = {};
    VGetBoundingRect(&bRect, 1);

    if (bRect.x <= bPlayerRect.w && bRect.w >= bPlayerRect.x && bRect.h >= bPlayerRect.y && bRect.y <= bPlayerRect.h && mSpriteScale == sActiveHero->mSpriteScale)
    {
        if (gpThrowableArray_5D1E2C)
        {
            if (gpThrowableArray_5D1E2C->field_20_count)
            {
                if (sActiveHero->mCurrentMotion == 31)
                {
                    const AnimRecord& animRec = AnimRec(AnimId::BoneBag_HardHit);
                    mAnim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
                }
                else
                {
                    const AnimRecord& animRec = AnimRec(AnimId::BoneBag_SoftHit);
                    mAnim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
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

        auto pBone = ae_new<Bone>(mXPos, mYPos - FP_FromInteger(30), field_11E_count);

        pBone->mSpriteScale = mSpriteScale;
        pBone->mScale = mScale;

        pBone->VThrow(field_124_velX, field_128_velY);

        SFX_Play_Mono(SoundEffect::SackHit_25, 0);
        Environment_SFX_457A40(EnvironmentSfx::eDeathNoise_7, 0, 0x7FFF, 0);

        if (sActiveHero->mCurrentMotion == 31)
        {
            const AnimRecord& animRec = AnimRec(AnimId::BoneBag_HardHit);
            mAnim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
        }
        else
        {
            const AnimRecord& animRec = AnimRec(AnimId::BoneBag_SoftHit);
            mAnim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
        }

        field_11C_bIs_hit = 1;
    }
}
