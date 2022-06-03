#include "stdafx.h"
#include "Rock.hpp"
#include "Function.hpp"
#include "ThrowableArray.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "Shadow.hpp"
#include "Sfx.hpp"
#include "ObjectIds.hpp"
#include "Events.hpp"
#include "Particle.hpp"
#include "Grid.hpp"
#include <assert.h>

Rock::Rock(FP xpos, FP ypos, s16 count)
    : BaseThrowable(0)
{
    SetType(AETypes::eRock_105);

    field_11A_bDead = 0;

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAberockResID, 0, 0))
    {
        LoadRockTypes_49AB30(field_C2_lvl_number, field_C0_path_number);
    }

    const AnimRecord& rec = AnimRec(AnimId::Rock);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    mBaseGameObjectFlags.Clear(BaseGameObject::eInteractive_Bit8);
    field_20_animation.mAnimFlags.Clear(AnimFlags::eBit3_Render);
    field_20_animation.mAnimFlags.Clear(AnimFlags::eBit15_bSemiTrans);

    mBaseAnimatedWithPhysicsGameObject_XPos = xpos;
    mBaseAnimatedWithPhysicsGameObject_YPos = ypos;

    field_120_xpos = xpos;
    field_124_ypos = ypos;

    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);

    field_118_count = count;
    field_11C_state = RockStates::eNone_0;

    u8** ppPal = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, AEResourceID::kAberockResID, 0, 0);
    if (ppPal)
    {
        field_20_animation.Load_Pal(ppPal, 0);
    }
    else
    {
        const FrameInfoHeader* pFrameInfo = field_20_animation.Get_FrameHeader(-1);

        const FrameHeader* pFrameHeader = reinterpret_cast<const FrameHeader*>(&(*field_20_animation.field_20_ppBlock)[pFrameInfo->field_0_frame_header_offset]);

        field_20_animation.Load_Pal(
            field_20_animation.field_20_ppBlock,
            pFrameHeader->field_0_clut_offset);
    }

    field_11E_volume = 0;

    mShadow = ae_new<Shadow>();
}

void Rock::VTimeToExplodeRandom()
{
    // Calls actual implementation of 0x411490 which is empty.
}

//TODO Identical to AO - merge
void Rock::VScreenChanged()
{
    if (gMap.mCurrentPath != gMap.mPath
        || gMap.mCurrentLevel != gMap.mLevel)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

Bool32 Rock::VIsFalling()
{
    return field_11C_state == RockStates::eFallingOutOfWorld_5;
}

Bool32 Rock::VCanThrow()
{
    return field_11C_state == RockStates::eBouncing_4;
}

Rock::~Rock()
{
    if (!gInfiniteGrenades_5C1BDE && !field_11A_bDead)
    {
        if (gpThrowableArray_5D1E2C)
        {
            gpThrowableArray_5D1E2C->Remove(field_118_count >= 1 ? field_118_count : 1);
        }
    }
}

//TODO Identical to AO - merge
void Rock::VThrow(FP velX, FP velY)
{
    field_C4_velx = velX;
    field_C8_vely = velY;

    field_20_animation.mAnimFlags.Set(AnimFlags::eBit3_Render);

    if (field_118_count == 0)
    {
        field_11C_state = RockStates::eBouncing_4;
    }
    else
    {
        field_11C_state = RockStates::eFallingOutOfRockSack_1;
    }
}

void Rock::InTheAir()
{
    field_120_xpos = mBaseAnimatedWithPhysicsGameObject_XPos;
    field_124_ypos = mBaseAnimatedWithPhysicsGameObject_YPos;

    if (field_C8_vely > FP_FromInteger(30))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    field_C8_vely += FP_FromDouble(1.01);

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
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eUnknown_32:
            case eLineTypes::eUnknown_36:
                if (field_C8_vely <= FP_FromInteger(0))
                {
                    break;
                }

                if (!gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
                {
                    return;
                }

                if (field_11C_state == RockStates::eBouncing_4 && field_C8_vely < FP_FromInteger(5))
                {
                    field_11C_state = RockStates::eFallingOutOfWorld_5;
                    return;
                }

                if (field_11C_state == RockStates::eFallingOutOfRockSack_1 && field_C8_vely < FP_FromInteger(1))
                {
                    field_11C_state = RockStates::eRolling_2;

                    if (field_C4_velx >= FP_FromInteger(0) && field_C4_velx < FP_FromInteger(1))
                    {
                        field_C4_velx = FP_FromInteger(1);
                    }

                    if (field_C4_velx > FP_FromInteger(-1) && field_C4_velx < FP_FromInteger(0))
                    {
                        field_C4_velx = FP_FromInteger(-1);
                    }

                    mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
                    return;
                }
                else
                {
                    mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
                    field_C4_velx = (field_C4_velx / FP_FromInteger(2));
                    field_C8_vely = (-field_C8_vely / FP_FromInteger(2));

                    s16 vol = 20 * (4 - field_11E_volume);
                    if (vol < 40)
                    {
                        vol = 40;
                    }

                    SFX_Play_Mono(SoundEffect::RockBounce_26, vol);
                    Event_Broadcast(kEventNoise, this);
                    Event_Broadcast(kEventSuspiciousNoise, this);
                    field_11E_volume++;
                }
                break;

            case eLineTypes::eCeiling_3:
            case eLineTypes::eBackgroundCeiling_7:
                if (field_C8_vely < FP_FromInteger(0))
                {
                    mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
                    field_C8_vely = (-field_C8_vely / FP_FromInteger(2));
                    s16 vol = 20 * (4 - field_11E_volume);
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
            case eLineTypes::eWallLeft_1:
            case eLineTypes::eBackgroundWallLeft_5:
                if (field_C4_velx < FP_FromInteger(0))
                {
                    BounceHorizontally( hitX, hitY );
                }
                break;

            case eLineTypes::eWallRight_2:
            case eLineTypes::eBackgroundWallRight_6:
                if (field_C4_velx > FP_FromInteger(0))
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
    field_C4_velx = (-field_C4_velx / FP_FromInteger(2));
    mBaseAnimatedWithPhysicsGameObject_XPos = hitX;
    mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
    s16 vol = 20 * (4 - field_11E_volume);
    if (vol < 40)
    {
        vol = 40;
    }
    SFX_Play_Mono(SoundEffect::RockBounce_26, vol);
    Event_Broadcast(kEventNoise, this);
    Event_Broadcast(kEventSuspiciousNoise, this);
}

//TODO Identical to AO - merge
s16 Rock::OnCollision(BaseAliveGameObject* pObj)
{
    if (!pObj->mBaseGameObjectFlags.Get(BaseGameObject::eCanExplode_Bit7))
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
        mBaseAnimatedWithPhysicsGameObject_YPos -= field_C8_vely;
        field_C8_vely = (-field_C8_vely / FP_FromInteger(2));
    }

    pObj->VOnThrowableHit(this);

    SFX_Play_Mono(SoundEffect::RockBounceOnMine_24, 80);
    return 0;
}

void Rock::VUpdate()
{
    auto pObj = sObjectIds.Find_Impl(BaseAliveGameObjectId);
    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit9_RestoredFromQuickSave))
    {
        mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit9_RestoredFromQuickSave);
        if (BaseAliveGameObjectCollisionLineType == -1)
        {
            BaseAliveGameObjectCollisionLine = nullptr;
        }
        else
        {
            sCollisions_DArray_5C1128->Raycast(
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(10),
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(10),
                &BaseAliveGameObjectCollisionLine,
                &mBaseAnimatedWithPhysicsGameObject_XPos,
                &mBaseAnimatedWithPhysicsGameObject_YPos,
                1 << BaseAliveGameObjectCollisionLineType);
        }
        BaseAliveGameObjectCollisionLineType = 0;
    }

    switch (field_11C_state)
    {
        case RockStates::eNone_0:
            break;

        case RockStates::eFallingOutOfRockSack_1:
            InTheAir();
            return;

        case RockStates::eRolling_2:
            if (FP_Abs(field_C4_velx) >= FP_FromInteger(1))
            {
                if (field_C4_velx <= FP_FromInteger(0))
                {
                    field_C4_velx += FP_FromDouble(0.01);
                }
                else
                {
                    field_C4_velx -= FP_FromDouble(0.01);
                }
                BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mBaseAnimatedWithPhysicsGameObject_XPos, &mBaseAnimatedWithPhysicsGameObject_YPos, field_C4_velx);
            }
            else
            {
                if (abs(SnapToXGrid(field_CC_sprite_scale, FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos)) - FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos)) <= 1)
                {
                    field_C4_velx = FP_FromInteger(0);
                    field_E4_collection_rect.x = mBaseAnimatedWithPhysicsGameObject_XPos - (ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(2));
                    field_E4_collection_rect.w = (ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(2)) + mBaseAnimatedWithPhysicsGameObject_XPos;
                    mBaseGameObjectFlags.Set(BaseGameObject::eInteractive_Bit8);
                    field_E4_collection_rect.h = mBaseAnimatedWithPhysicsGameObject_YPos;
                    field_E4_collection_rect.y = mBaseAnimatedWithPhysicsGameObject_YPos - ScaleToGridSize(field_CC_sprite_scale);
                    field_11C_state = RockStates::eOnGround_3;
                    field_20_animation.mAnimFlags.Clear(AnimFlags::eBit8_Loop);
                    field_128_shimmer_timer = sGnFrame_5C1B84;
                    return;
                }
                BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mBaseAnimatedWithPhysicsGameObject_XPos, &mBaseAnimatedWithPhysicsGameObject_YPos, field_C4_velx);
            }

            if (BaseAliveGameObjectCollisionLine)
            {
                return;
            }

            field_20_animation.mAnimFlags.Set(AnimFlags::eBit8_Loop);
            field_11C_state = RockStates::eBouncing_4;
            return;

        case RockStates::eOnGround_3:
            if (static_cast<s32>(sGnFrame_5C1B84) <= field_128_shimmer_timer || pObj)
            {
                return;
            }
            // The strange shimmering that rocks give off.
            New_TintShiny_Particle(
                (field_CC_sprite_scale * FP_FromInteger(1)) + mBaseAnimatedWithPhysicsGameObject_XPos,
                (field_CC_sprite_scale * FP_FromInteger(-7)) + mBaseAnimatedWithPhysicsGameObject_YPos,
                FP_FromDouble(0.3),
                Layer::eLayer_Foreground_36);
            field_128_shimmer_timer = (Math_NextRandom() % 16) + sGnFrame_5C1B84 + 60;
            return;

        case RockStates::eBouncing_4:
        {
            InTheAir();
            PSX_RECT bRect = {};
            VGetBoundingRect(&bRect, 1);
            const PSX_Point xy = {bRect.x, static_cast<s16>(bRect.y + 5)};
            const PSX_Point wh = {bRect.w, static_cast<s16>(bRect.h + 5)};
            VOnCollisionWith(xy, wh,
                                    gBaseGameObjects,
                                    1,
                                    (TCollisionCallBack) &Rock::OnCollision);

            if (field_C8_vely > FP_FromInteger(30))
            {
                field_11C_state = RockStates::eFallingOutOfWorld_5;
            }
        }
            return;

        case RockStates::eFallingOutOfWorld_5:
            field_C8_vely += FP_FromDouble(1.01);
            mBaseAnimatedWithPhysicsGameObject_XPos += field_C4_velx;
            mBaseAnimatedWithPhysicsGameObject_YPos = field_C8_vely + mBaseAnimatedWithPhysicsGameObject_YPos;
            if (!gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && !gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(240), 0))
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
            return;
    }
}

s32 Rock::VGetSaveState(u8* pSaveBuffer)
{
    auto pState = reinterpret_cast<RockSaveState*>(pSaveBuffer);

    pState->field_0_type = AETypes::eRock_105;
    pState->field_4_obj_id = mBaseGameObjectTlvInfo;

    pState->field_8_xpos = mBaseAnimatedWithPhysicsGameObject_XPos;
    pState->field_C_ypos = mBaseAnimatedWithPhysicsGameObject_YPos;

    pState->field_10_velx = field_C4_velx;
    pState->field_14_vely = field_C8_vely;

    pState->field_1C_path_number = field_C0_path_number;
    pState->field_1E_lvl_number = field_C2_lvl_number;

    pState->field_18_sprite_scale = field_CC_sprite_scale;

    pState->field_20_flags.Set(RockSaveState::eBit1_bRender, field_20_animation.mAnimFlags.Get(AnimFlags::eBit3_Render));
    pState->field_20_flags.Set(RockSaveState::eBit2_bDrawable, mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4));

    pState->field_20_flags.Set(RockSaveState::eBit3_bLoop, field_20_animation.mAnimFlags.Get(AnimFlags::eBit8_Loop));
    pState->field_20_flags.Set(RockSaveState::eBit4_bInteractive, mBaseGameObjectFlags.Get(BaseGameObject::eInteractive_Bit8));

    if (BaseAliveGameObjectCollisionLine)
    {
        pState->field_28_line_type = BaseAliveGameObjectCollisionLine->field_8_type;
    }
    else
    {
        pState->field_28_line_type = -1;
    }
    pState->field_24_id = BaseAliveGameObjectId;
    pState->field_2A_count = field_118_count;
    pState->field_2C_state = field_11C_state;
    pState->field_2E_volume = field_11E_volume;
    pState->field_30_xpos = field_120_xpos;
    pState->field_34_ypos = field_124_ypos;
    return sizeof(RockSaveState);
}

s32 Rock::CreateFromSaveState(const u8* pData)
{
    auto pState = reinterpret_cast<const RockSaveState*>(pData);

    auto pRock = ae_new<Rock>(pState->field_8_xpos, pState->field_C_ypos, pState->field_2A_count);

    pRock->mBaseGameObjectTlvInfo = pState->field_4_obj_id;

    pRock->mBaseAnimatedWithPhysicsGameObject_XPos = pState->field_8_xpos;
    pRock->mBaseAnimatedWithPhysicsGameObject_YPos = pState->field_C_ypos;

    pRock->field_E4_collection_rect.x = pRock->mBaseAnimatedWithPhysicsGameObject_XPos - (ScaleToGridSize(pRock->field_CC_sprite_scale) / FP_FromInteger(2));
    pRock->field_E4_collection_rect.w = pRock->mBaseAnimatedWithPhysicsGameObject_XPos + (ScaleToGridSize(pRock->field_CC_sprite_scale) / FP_FromInteger(2));
    pRock->field_E4_collection_rect.h = pRock->mBaseAnimatedWithPhysicsGameObject_YPos;
    pRock->field_E4_collection_rect.y = pRock->mBaseAnimatedWithPhysicsGameObject_YPos - ScaleToGridSize(pRock->field_CC_sprite_scale);

    pRock->field_C4_velx = pState->field_10_velx;
    pRock->field_C8_vely = pState->field_14_vely;

    pRock->field_C0_path_number = pState->field_1C_path_number;
    pRock->field_C2_lvl_number = pState->field_1E_lvl_number;

    pRock->field_CC_sprite_scale = pState->field_18_sprite_scale;
    pRock->field_D6_scale = pState->field_18_sprite_scale > FP_FromDouble(0.75);

    pRock->field_20_animation.mAnimFlags.Set(AnimFlags::eBit3_Render, pState->field_20_flags.Get(RockSaveState::eBit1_bRender));
    pRock->field_20_animation.mAnimFlags.Set(AnimFlags::eBit8_Loop, pState->field_20_flags.Get(RockSaveState::eBit3_bLoop));

    pRock->mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4, pState->field_20_flags.Get(RockSaveState::eBit2_bDrawable));
    pRock->mBaseGameObjectFlags.Set(BaseGameObject::eInteractive_Bit8, pState->field_20_flags.Get(RockSaveState::eBit4_bInteractive));

    pRock->mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit9_RestoredFromQuickSave);

    pRock->field_128_shimmer_timer = sGnFrame_5C1B84;

    pRock->BaseAliveGameObjectCollisionLineType = pState->field_28_line_type;

    pRock->field_118_count = pState->field_2A_count;
    pRock->field_11C_state = pState->field_2C_state;

    pRock->field_11E_volume = pState->field_2E_volume;

    pRock->field_120_xpos = pState->field_30_xpos;
    pRock->field_124_ypos = pState->field_34_ypos;

    return sizeof(RockSaveState);
}
