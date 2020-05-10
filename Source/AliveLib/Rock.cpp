#include "stdafx.h"
#include "Rock.hpp"
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

Rock* Rock::ctor_49E150(FP xpos, FP ypos, __int16 count)
{
    ctor_408240(0);
    SetVTable(this, 0x546AF8);
    field_4_typeId = Types::eRock_105;

    field_11A_bDead = 0;

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAberockResID, 0, 0))
    {
        LoadRockTypes_49AB30(field_C2_lvl_number, field_C0_path_number);
    }

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kAberockResID);
    Animation_Init_424E10(488, 17, 9, ppRes, 1, 1);
    field_6_flags.Clear(BaseGameObject::eInteractive_Bit8);
    field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
    field_20_animation.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);

    field_B8_xpos = xpos;
    field_BC_ypos = ypos;

    field_120_xpos = xpos;
    field_124_ypos = ypos;

    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);

    field_118_count = count;
    field_11C_state = RockStates::eState_0;

    BYTE** ppPal = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, ResourceID::kAberockResID, 0, 0);
    if (ppPal)
    {
        field_20_animation.Load_Pal_40A530(ppPal, 0);
    }
    else
    {
        const FrameInfoHeader* pFrameInfo = field_20_animation.Get_FrameHeader_40B730(-1);
        const FrameHeader* pFrameHeader = reinterpret_cast<const FrameHeader*>(&(*field_20_animation.field_20_ppBlock)[pFrameInfo->field_0_frame_header_offset]);
        field_20_animation.Load_Pal_40A530(
            field_20_animation.field_20_ppBlock,
            pFrameHeader->field_0_clut_offset
        );

        //safety check in case IDA output was wrongly interpreted. TODO remove later
        assert((DWORD *)&(*field_20_animation.field_20_ppBlock)[*(DWORD *)&(*field_20_animation.field_20_ppBlock)[*((DWORD *) *field_20_animation.field_20_ppBlock + 124)]]
            == &(pFrameHeader->field_0_clut_offset));
    }

    field_11E_vol = 0;

    field_E0_pShadow = alive_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }
    return this;
}

BaseGameObject* Rock::VDestructor(signed int flags)
{
    return vdtor_49E370(flags);
}

void Rock::VUpdate()
{
    vUpdate_49E9F0();
}

void Rock::VScreenChanged()
{
    vScreenChanged_49F030();
}

int Rock::VGetSaveState(BYTE* pSaveBuffer)
{
    return vGetSaveState_49F9A0(reinterpret_cast<RockSaveState*>(pSaveBuffer));
}

void Rock::VThrow_49E460(FP velX, FP velY)
{
    vThrow_49E460(velX, velY);
}

BOOL Rock::VCanThrow_49E350()
{
    return vCanThrow_49E350();
}

BOOL Rock::VIsFalling_49E330()
{
    return vIsFalling_49E330();
}

void Rock::VTimeToExplodeRandom_411490()
{
    // Calls actuall impl of 0x411490 which is empty
}

void Rock::vScreenChanged_49F030()
{
    if (gMap_5C3030.sCurrentPathId_5C3032 != gMap_5C3030.field_C_5C303C_pathId || gMap_5C3030.sCurrentLevelId_5C3030 != gMap_5C3030.field_A_5C303A_levelId)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

BOOL Rock::vIsFalling_49E330()
{
    return field_11C_state == RockStates::eState_5;
}

BOOL Rock::vCanThrow_49E350()
{
    return field_11C_state == RockStates::eState_4;
}

Rock* Rock::vdtor_49E370(signed int flags)
{
    dtor_49E3A0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void Rock::dtor_49E3A0()
{
    SetVTable(this, 0x546AF8);

    if (!gInfiniteGrenades_5C1BDE && !field_11A_bDead)
    {
        if (gpThrowableArray_5D1E2C)
        {
            gpThrowableArray_5D1E2C->Remove_49AA00(field_118_count >= 1 ? field_118_count : 1);
        }
    }
    dtor_4080B0();
}

void Rock::vThrow_49E460(FP velX, FP velY)
{
    field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);

    field_C4_velx = velX;
    field_C8_vely = velY;

    if (field_118_count == 0)
    {
        field_11C_state = RockStates::eState_4;
    }
    else
    {
        field_11C_state = RockStates::eState_1_FallToTheVoid;
    }
}

void Rock::InTheAir_49E4B0()
{
    field_120_xpos = field_B8_xpos;
    field_124_ypos = field_BC_ypos;

    if (field_C8_vely > FP_FromInteger(30))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    field_C8_vely += FP_FromDouble(1.01);

    field_B8_xpos += field_C4_velx;
    field_BC_ypos += field_C8_vely;

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast_417A60(
        field_120_xpos,
        field_124_ypos,
        field_B8_xpos,
        field_BC_ypos,
        &field_100_pCollisionLine,
        &hitX,
        &hitY,
        field_D6_scale == 1 ? 0x09 : 0x90) == 1)
    {
        switch (field_100_pCollisionLine->field_8_type)
        {
        case 0u:
        case 4u:
        case 32u:
        case 36u:
            if (field_C8_vely <= FP_FromInteger(0))
            {
                break;
            }

            if (!gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
            {
                return;
            }

            if (field_11C_state == RockStates::eState_4 && field_C8_vely < FP_FromInteger(5))
            {
                field_11C_state = RockStates::eState_5;
                return;
            }

            if (field_11C_state == RockStates::eState_1_FallToTheVoid && field_C8_vely < FP_FromInteger(1))
            {
                field_11C_state = RockStates::eState_2;

                if (field_C4_velx >= FP_FromInteger(0) && field_C4_velx < FP_FromInteger(1))
                {
                    field_C4_velx = FP_FromInteger(1);
                }

                if (field_C4_velx > FP_FromInteger(-1) && field_C4_velx < FP_FromInteger(0))
                {
                    field_C4_velx = FP_FromInteger(-1);
                }

                field_BC_ypos = hitY;
                return;
            }
            else
            {
                field_BC_ypos = hitY;
                field_C4_velx = (field_C4_velx / FP_FromInteger(2));
                field_C8_vely = (-field_C8_vely / FP_FromInteger(2));

                short vol = 20 * (4 - field_11E_vol);
                if (vol < 40)
                {
                    vol = 40;
                }

                SFX_Play_46FA90(26u, vol);
                Event_Broadcast_422BC0(kEventNoise, this);
                Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
                field_11E_vol++;
            }
            break;

        case 3u:
        case 7u:
            if (field_C8_vely < FP_FromInteger(0))
            {
                field_BC_ypos = hitY;
                field_C8_vely = (-field_C8_vely / FP_FromInteger(2));
                short vol = 20 * (4 - field_11E_vol);
                if (vol < 40)
                {
                    vol = 40;
                }
                SFX_Play_46FA90(26, vol);
                Event_Broadcast_422BC0(kEventNoise, this);
                Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
            }
            break;
        }
    }

    if (sCollisions_DArray_5C1128->Raycast_417A60(field_120_xpos, field_124_ypos, field_B8_xpos, field_BC_ypos, &field_100_pCollisionLine, &hitX, &hitY, field_D6_scale == 1 ? 0x06 : 0x60) == 1)
    {
        switch (field_100_pCollisionLine->field_8_type)
        {
        case 1u:
        case 5u:
            if (field_C4_velx < FP_FromInteger(0))
            {
                field_C4_velx = (-field_C4_velx / FP_FromInteger(2));
                field_B8_xpos = hitX;
                field_BC_ypos = hitY;
                short vol = 20 * (4 - field_11E_vol);
                if (vol < 40)
                {
                    vol = 40;
                }
                SFX_Play_46FA90(26, vol);
                Event_Broadcast_422BC0(kEventNoise, this);
                Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
            }
            break;

        case 2u:
        case 6u:
            if (field_C4_velx > FP_FromInteger(0))
            {
                field_C4_velx = (-field_C4_velx / FP_FromInteger(2));
                field_B8_xpos = hitX;
                field_BC_ypos = hitY;
                short vol = 20 * (4 - field_11E_vol);
                if (vol < 40)
                {
                    vol = 40;
                }
                SFX_Play_46FA90(26, vol);
                Event_Broadcast_422BC0(kEventNoise, this);
                Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
            }
            break;
        }
    }
}

__int16 Rock::OnCollision_49EF10(BaseAliveGameObject* pObj)
{
    if (!pObj->field_6_flags.Get(BaseGameObject::eCanExplode_Bit7))
    {
        return 1;
    }

    PSX_RECT bRect = {};
    pObj->vGetBoundingRect_424FD0(&bRect, 1);

    if (field_120_xpos < FP_FromInteger(bRect.x) || field_120_xpos > FP_FromInteger(bRect.w))
    {
        field_B8_xpos -= field_C4_velx;
        field_C4_velx = (-field_C4_velx / FP_FromInteger(2));
    }
    else
    {
        field_BC_ypos -= field_C8_vely;
        field_C8_vely = (-field_C8_vely / FP_FromInteger(2));
    }

    pObj->VOnThrowableHit(this);

    SFX_Play_46FA90(24u, 80);
    return 0;
}

void Rock::vUpdate_49E9F0()
{
    auto pObj = sObjectIds_5C1B70.Find_449CF0(field_110_id);
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (field_114_flags.Get(Flags_114::e114_Bit9))
    {
        field_114_flags.Clear(Flags_114::e114_Bit9);
        if (field_104_collision_line_type == -1)
        {
            field_100_pCollisionLine = nullptr;
        }
        else
        {
            sCollisions_DArray_5C1128->Raycast_417A60(
                field_B8_xpos,
                field_BC_ypos - FP_FromInteger(10),
                field_B8_xpos,
                field_BC_ypos + FP_FromInteger(10),
                &field_100_pCollisionLine,
                &field_B8_xpos,
                &field_BC_ypos,
                1 << field_104_collision_line_type);
        }
        field_104_collision_line_type = 0;
    }

    switch (field_11C_state)
    {
    case RockStates::eState_0:
        break;

    case RockStates::eState_1_FallToTheVoid:
        InTheAir_49E4B0();
        return;

    case RockStates::eState_2:
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
            field_100_pCollisionLine = field_100_pCollisionLine->MoveOnLine_418260(&field_B8_xpos, &field_BC_ypos, field_C4_velx);
        }
        else
        {
            if (abs(SnapToXGrid_449930(field_CC_sprite_scale, FP_GetExponent(field_B8_xpos)) - FP_GetExponent(field_B8_xpos)) <= 1)
            {
                field_C4_velx = FP_FromInteger(0);
                field_E4_collection_rect.x = field_B8_xpos - (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(2));
                field_E4_collection_rect.w = (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(2)) + field_B8_xpos;
                field_6_flags.Set(BaseGameObject::eInteractive_Bit8);
                field_E4_collection_rect.h = field_BC_ypos;
                field_E4_collection_rect.y = field_BC_ypos - ScaleToGridSize_4498B0(field_CC_sprite_scale);
                field_11C_state = RockStates::eState_3;
                field_20_animation.field_4_flags.Clear(AnimFlags::eBit8_Loop);
                field_128_timer = sGnFrame_5C1B84;
                return;
            }
            field_100_pCollisionLine = field_100_pCollisionLine->MoveOnLine_418260(&field_B8_xpos, &field_BC_ypos, field_C4_velx);
        }

        if (field_100_pCollisionLine)
        {
            return;
        }

        field_20_animation.field_4_flags.Set(AnimFlags::eBit8_Loop);
        field_11C_state = RockStates::eState_4;
        return;

    case RockStates::eState_3:
        if (static_cast<int>(sGnFrame_5C1B84) <= field_128_timer || pObj)
        {
            return;
        }
        // The strange shimmering that rocks give off
        New_Particle_426C30(
            (field_CC_sprite_scale * FP_FromInteger(1)) + field_B8_xpos, 
            (field_CC_sprite_scale * FP_FromInteger(-7)) + field_BC_ypos, 
            FP_FromDouble(0.3),
            36);
        field_128_timer = (Math_NextRandom() % 16) + sGnFrame_5C1B84 + 60;
        return;

    case RockStates::eState_4:
    {
        InTheAir_49E4B0();
        PSX_RECT bRect = {};
        vGetBoundingRect_424FD0(&bRect, 1);
        const PSX_Point xy = { bRect.x, static_cast<short>(bRect.y + 5) };
        const PSX_Point wh = { bRect.w, static_cast<short>(bRect.h + 5) };
        vOnCollisionWith_424EE0(xy, wh,
            gBaseGameObject_list_BB47C4,
            1,
            (TCollisionCallBack)&Rock::OnCollision_49EF10);

        if (field_C8_vely > FP_FromInteger(30))
        {
            field_11C_state = RockStates::eState_5;
        }
    }
        return;

    case RockStates::eState_5:
        field_C8_vely += FP_FromDouble(1.01);
        field_B8_xpos += field_C4_velx;
        field_BC_ypos = field_C8_vely + field_BC_ypos;
        if (!gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0) && 
            !gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos + FP_FromInteger(240), 0))
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
        return;
    }
}

signed int Rock::vGetSaveState_49F9A0(RockSaveState* pState)
{
    pState->field_0_type = Types::eRock_105;
    pState->field_4_obj_id = field_C_objectId;

    pState->field_8_xpos = field_B8_xpos;
    pState->field_C_ypos = field_BC_ypos;

    pState->field_10_velx = field_C4_velx;
    pState->field_14_vely = field_C8_vely;

    pState->field_1C_path_number = field_C0_path_number;
    pState->field_1E_lvl_number = field_C2_lvl_number;

    pState->field_18_sprite_scale = field_CC_sprite_scale;

    pState->field_20_flags.Set(RockSaveState::eBit1_bRender, field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render));
    pState->field_20_flags.Set(RockSaveState::eBit2_bDrawable, field_6_flags.Get(BaseGameObject::eDrawable_Bit4));

    pState->field_20_flags.Set(RockSaveState::eBit3_bLoop, field_20_animation.field_4_flags.Get(AnimFlags::eBit8_Loop));
    pState->field_20_flags.Set(RockSaveState::eBit4_bInteractive, field_6_flags.Get(BaseGameObject::eInteractive_Bit8));

    if (field_100_pCollisionLine)
    {
        pState->field_28_line_type = field_100_pCollisionLine->field_8_type;
    }
    else
    {
        pState->field_28_line_type = -1;
    }
    pState->field_24_id = field_110_id;
    pState->field_2A_count = field_118_count;
    pState->field_2C_state = field_11C_state;
    pState->field_2E_state = field_11E_vol;
    pState->field_30_xpos = field_120_xpos;
    pState->field_34_ypos = field_124_ypos;
    return sizeof(RockSaveState);
}

int CC Rock::CreateFromSaveState_49F720(const BYTE* pData)
{
    auto pState = reinterpret_cast<const RockSaveState*>(pData);

    auto pRock = alive_new<Rock>();
    pRock->ctor_49E150(pState->field_8_xpos, pState->field_C_ypos, pState->field_2A_count);

    pRock->field_C_objectId = pState->field_4_obj_id;

    pRock->field_B8_xpos = pState->field_8_xpos;
    pRock->field_BC_ypos = pState->field_C_ypos;

    pRock->field_E4_collection_rect.x = pRock->field_B8_xpos - (ScaleToGridSize_4498B0(pRock->field_CC_sprite_scale) / FP_FromInteger(2));
    pRock->field_E4_collection_rect.w = pRock->field_B8_xpos + (ScaleToGridSize_4498B0(pRock->field_CC_sprite_scale) / FP_FromInteger(2));
    pRock->field_E4_collection_rect.h = pRock->field_BC_ypos;
    pRock->field_E4_collection_rect.y = pRock->field_BC_ypos - ScaleToGridSize_4498B0(pRock->field_CC_sprite_scale);

    pRock->field_C4_velx = pState->field_10_velx;
    pRock->field_C8_vely = pState->field_14_vely;

    pRock->field_C0_path_number = pState->field_1C_path_number;
    pRock->field_C2_lvl_number = pState->field_1E_lvl_number;

    pRock->field_CC_sprite_scale = pState->field_18_sprite_scale;
    pRock->field_D6_scale = pState->field_18_sprite_scale > FP_FromDouble(0.75);

    pRock->field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render, pState->field_20_flags.Get(RockSaveState::eBit1_bRender));
    pRock->field_20_animation.field_4_flags.Set(AnimFlags::eBit8_Loop, pState->field_20_flags.Get(RockSaveState::eBit3_bLoop));

    pRock->field_6_flags.Set(BaseGameObject::eDrawable_Bit4, pState->field_20_flags.Get(RockSaveState::eBit2_bDrawable));
    pRock->field_6_flags.Set(BaseGameObject::eInteractive_Bit8, pState->field_20_flags.Get(RockSaveState::eBit4_bInteractive));

    pRock->field_114_flags.Set(Flags_114::e114_Bit9);

    pRock->field_128_timer = sGnFrame_5C1B84;

    pRock->field_104_collision_line_type = pState->field_28_line_type;

    pRock->field_118_count = pState->field_2A_count;
    pRock->field_11C_state = pState->field_2C_state;

    pRock->field_11E_vol = pState->field_2E_state;

    pRock->field_120_xpos = pState->field_30_xpos;
    pRock->field_124_ypos = pState->field_34_ypos;

    return sizeof(RockSaveState);
}

RockSack* RockSack::ctor_49F100(Path_RockSack* pTlv, int tlvInfo)
{
    ctor_408240(0);
    SetVTable(this, 0x546B88);

    field_4_typeId = Types::eRockSack_106;
    
    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kP2c2bagResID);

    //Set RockSack idle anim speed
    auto pAnimationHeader = reinterpret_cast<AnimationHeader*>(*ppRes + 29748);
    pAnimationHeader->field_0_fps = 0;

    Animation_Init_424E10(29748, 71, 60u, ppRes, 1, 1);

    field_20_animation.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);

    field_DC_bApplyShadows &= ~1u;

    field_118_tlvInfo = tlvInfo;
    field_11C = 0;
    field_B8_xpos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    field_BC_ypos = FP_FromInteger(pTlv->field_C_bottom_right.field_2_y);
    field_124_x_vel = FP_FromRaw(pTlv->field_12_x_vel << 8);
    field_128_y_vel = FP_FromRaw(-256 * pTlv->field_14_y_vel);

    if (!pTlv->field_10_side)
    {
        field_124_x_vel = -field_124_x_vel;
    }

    if (pTlv->field_16_scale == 1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
    }
    else if (pTlv->field_16_scale == 0)
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_D6_scale = 1;
    }

    field_11E_num_rocks = pTlv->field_18_num_rocks;
    field_120 = 1;
    field_122 = 1;

    field_E0_pShadow = alive_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }

    field_DC_bApplyShadows |= 2u;
    return this;
}

BaseGameObject* RockSack::VDestructor(signed int flags)
{
    return vdtor_49F2E0(flags);
}

void RockSack::VScreenChanged()
{
    vScreenChanged_49F700();
}

void RockSack::VUpdate()
{
    vUpdate_49F3A0();
}

RockSack* RockSack::vdtor_49F2E0(signed int flags)
{
    dtor_49F310();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void RockSack::dtor_49F310()
{
    SetVTable(this, 0x546B88);
    Path::TLV_Reset_4DB8E0(field_118_tlvInfo, -1, 0, 0);
    dtor_4080B0();
}

void RockSack::vScreenChanged_49F700()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

void RockSack::vUpdate_49F3A0()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (field_20_animation.field_92_current_frame == 2)
    {
        if (field_120)
        {
            if (Math_NextRandom() < 40u || field_122)
            {
                field_120 = 0;
                field_122 = 0;
                SFX_Play_46FBA0(29u, 24, Math_RandomRange_496AB0(-2400, -2200));
            }
        }
    }
    else
    {
        field_120 = 1;
    }

    if (field_11C)
    {
        if (field_11C == 1 && field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_20_animation.Set_Animation_Data_409C80(29748, nullptr);
            field_11C = 0;
        }
    }
    else
    {
        if (field_20_animation.field_E_frame_change_counter == 0)
        {
            field_20_animation.field_E_frame_change_counter = Math_RandomRange_496AB0(2, 10);
        }

        PSX_RECT bPlayerRect = {};
        sActiveHero_5C1B68->vGetBoundingRect_424FD0(&bPlayerRect, 1);

        PSX_RECT bRect = {};
        vGetBoundingRect_424FD0(&bRect, 1);

        if (bRect.x <= bPlayerRect.w &&
            bRect.w >= bPlayerRect.x &&
            bRect.h >= bPlayerRect.y &&
            bRect.y <= bPlayerRect.h &&
            field_CC_sprite_scale == sActiveHero_5C1B68->field_CC_sprite_scale)
        {
            if (gpThrowableArray_5D1E2C)
            {
                if (gpThrowableArray_5D1E2C->field_20_count)
                {
                    if (sActiveHero_5C1B68->field_106_current_motion == 31)
                    {
                        field_20_animation.Set_Animation_Data_409C80(29700, 0);
                    }
                    else
                    {
                        field_20_animation.Set_Animation_Data_409C80(29772, 0);
                    }
                    field_11C = 1;
                    return;
                }
            }
            else
            {
                gpThrowableArray_5D1E2C = alive_new<ThrowableArray>();
                gpThrowableArray_5D1E2C->ctor_49A630();
            }

            gpThrowableArray_5D1E2C->Add_49A7A0(field_11E_num_rocks);

            auto pRock = alive_new<Rock>();
            if (pRock)
            {
                pRock->ctor_49E150(field_B8_xpos, field_BC_ypos - FP_FromInteger(30), field_11E_num_rocks);
            }

            pRock->VThrow_49E460(field_124_x_vel, field_128_y_vel);

            SFX_Play_46FA90(25u, 0);
            Abe_SFX_2_457A40(7, 0, 0x7FFF, 0);

            if (sActiveHero_5C1B68->field_106_current_motion == 31)
            {
                field_20_animation.Set_Animation_Data_409C80(29700, nullptr);
            }
            else
            {
                field_20_animation.Set_Animation_Data_409C80(29772, nullptr);
            }
            field_11C = 1;
        }
    }
}
