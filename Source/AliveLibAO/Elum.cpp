#include "stdafx_ao.h"
#include "Function.hpp"
#include "Elum.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "ResourceManager.hpp"
#include "LiftPoint.hpp"
#include "Game.hpp"
#include "Input.hpp"
#include "Abe.hpp"
#include "Collisions.hpp"
#include "Sfx.hpp"
#include "Gibs.hpp"
#include "MusicTrigger.hpp"
#include "Shadow.hpp"
#include "Midi.hpp"
#include "Events.hpp"
#include "DDCheat.hpp"

START_NS_AO;

ALIVE_VAR(1, 0x507680, Elum*, gElum_507680, nullptr);

using TElumStateFunction = decltype(&Elum::State_0_Respawn_414C60);

const TElumStateFunction sElum_motion_table_4C5148[] =
{
    &Elum::State_0_Respawn_414C60,
    &Elum::State_1_Idle_412990,
    &Elum::State_2_Unknown_412C30,
    &Elum::State_3_WalkLoop_412C90,
    &Elum::State_4_Turn_4140F0,
    &Elum::State_5_WalkToIdle_4132D0,
    &Elum::State_6_MidWalkToIdle_4133F0,
    &Elum::State_7_Unknown_413200,
    &Elum::State_8_IdleToWalk_413270,
    &Elum::State_9_ToYell_415890,
    &Elum::State_10_Yell_4158E0,
    &Elum::State_11_Unknown_4159A0,
    &Elum::State_12_RunTurn_414520,
    &Elum::State_13_RunTurnToWalk_4147C0,
    &Elum::State_14_Speak_414860,
    &Elum::State_15_Speak_4148F0,
    &Elum::State_16_Speak_414980,
    &Elum::State_17_Unknown_413620,
    &Elum::State_18_Unknown_4136A0,
    &Elum::State_19_Dead_415F90,
    &Elum::State_20_Fall_415F70,
    &Elum::State_21_Land_414A20,
    &Elum::State_22_RunOffEdge_415810,
    &Elum::State_23_WalkOffEdge_415E90,
    &Elum::State_24_JumpToFall_415ED0,
    &Elum::State_25_LickingHoney_415B50,
    &Elum::State_26_LickingToStruggling_415AC0,
    &Elum::State_27_AbeMountingEnd_415CA0,
    &Elum::State_28_AbeUnmountingEnd_415D60,
    &Elum::State_29_BeesStruggling_412A90,
    &Elum::State_30_HopBegin_414E30,
    &Elum::State_31_HopMid_414C70,
    &Elum::State_32_HopLand_415140,
    &Elum::State_33_RunJumpBegin_415400,
    &Elum::State_34_RunJumpMid_415240,
    &Elum::State_35_RunJumpLand_415580,
    &Elum::State_36_RunLoop_413720,
    &Elum::State_37_RunSlideStop_4142E0,
    &Elum::State_38_RunTurnToRun_414810,
    &Elum::State_39_IdleToRun_413B00,
    &Elum::State_40_WalkToRun_4134B0,
    &Elum::State_41_MidWalkToRun_413560,
    &Elum::State_42_RunToWalk_413B60,
    &Elum::State_43_MidRunToWalk_413E20,
    &Elum::State_44_ScratchBegin_412730,
    &Elum::State_45_ScratchLoop_4127B0,
    &Elum::State_46_ScratchEnd_412800,
    &Elum::State_47_Unknown_415A30,
    &Elum::State_48_AbeMoutingBegin_415C40,
    &Elum::State_49_AbeUnmountingBegin_415D00,
    &Elum::State_50_Knockback_415DC0
};

using TBrainStateFunc = decltype(&Elum::Brain_0_WithoutAbe_416190);

const TBrainStateFunc sElum_table_4C52E8[] =
{
    &Elum::Brain_0_WithoutAbe_416190,
    &Elum::Brain_1_HoneyAddiction_411730,
};

const int gElumFrameTables_4C5218[52] =
{
    117036,
    117036,
    117036,
    117152,
    116980,
    117112,
    117132,
    116928,
    116948,
    117068,
    117288,
    117464,
    117232,
    117092,
    117496,
    117496,
    117496,
    117112,
    117132,
    117036,
    117036,
    40404,
    40428,
    40376,
    40484,
    19988,
    20044,
    49620,
    47728,
    17568,
    127896,
    127924,
    127956,
    127980,
    128000,
    128048,
    128128,
    128168,
    128068,
    127876,
    127816,
    127796,
    127836,
    127856,
    18132,
    18188,
    18160,
    18236,
    7460,
    7460,
    21280,
    0
};

BaseGameObject* Elum::VDestructor(signed int flags)
{
    return Vdtor_411710(flags);
}

Elum* Elum::Vdtor_411710(signed int flags)
{
    dtor_410BC0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

EXPORT BaseGameObject *Elum::dtor_410BC0()
{
    SetVTable(this, 0x4BA8F8);

    for (BYTE**& ppRes : field_174_resources.res)
    {
        if (ppRes && field_10_anim.field_20_ppBlock != ppRes)
        {
            ResourceManager::FreeResource_455550(ppRes);
        }
    }

    gElum_507680 = nullptr;
    Vsub_412700();

    ResourceManager::FreeResource_455550(
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 115, 0, 0)
    );

    if (field_104_pending_resource_count)
    {
        ResourceManager::CancelPendingResourcesFor_41EA60(this);
    }
    field_104_pending_resource_count = 0;

    const int anims[] = { 230, 222, 220, 221 };
    for (int anim : anims)
    {
        ResourceManager::FreeResource_455550(
            ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, anim, 1, 0)
        );
    }

    return dtor_base_416FE0();
}

void Elum::VOn_TLV_Collision(Path_TLV* pTlv)
{
    VOn_TLV_Collision_410F10(pTlv);
}


void Elum::VOn_TLV_Collision_410F10(Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->field_4_type == TlvTypes::ContinuePoint_0)
        {
            auto pContinueTlv = static_cast<Path_ContinuePoint*>(pTlv);
            if (field_140_continue_zone_number != pContinueTlv->field_18_zone_number &&
                pContinueTlv->field_18_zone_number > field_142_zone_number &&
                pContinueTlv->field_1E_elum_restarts == 1)
            {
                field_140_continue_zone_number = pContinueTlv->field_18_zone_number;
                field_138_continue_rect.x = pContinueTlv->field_10_top_left.field_0_x;
                field_138_continue_rect.y = pContinueTlv->field_10_top_left.field_2_y;
                field_138_continue_rect.w =  pContinueTlv->field_14_bottom_right.field_0_x;
                field_138_continue_rect.h = pContinueTlv->field_14_bottom_right.field_2_y;

                field_148_continue_path = gMap_507BA8.field_2_current_path;
                field_14C_continue_camera = gMap_507BA8.field_4_current_camera;
                field_14A_continue_level =  gMap_507BA8.field_0_current_level;
                field_150_continue_sprite_scale = field_BC_sprite_scale;
                field_144 = 1;
            }
        }
        else if (pTlv->field_4_type == TlvTypes::DeathDrop_5 && field_100_health > FP_FromInteger(0))
        {
            if (sControlledCharacter_50767C != this)
            {
                field_122 = 0;
            }
            Sfx_416E10(2u, this);
            field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
            field_FC_current_motion = eElumStates::State_20_Fall_415F70;
            field_100_health = FP_FromInteger(0);
        }

        pTlv = gMap_507BA8.TLV_Get_At_446060(
            pTlv,
            field_A8_xpos,
            field_AC_ypos,
            field_A8_xpos,
            field_AC_ypos);

    }
}

__int16 Elum::VTakeDamage(BaseGameObject* pFrom)
{
    return VTakeDamage_411020(pFrom);
}

__int16 Elum::VTakeDamage_411020(BaseGameObject* pFrom)
{
    switch (pFrom->field_4_typeId)
    {
    case Types::eBullet_10:
    case Types::eBaseBomb_30:
    case Types::eExplosion_74:
        if (field_100_health > FP_FromInteger(0))
        {
            Sfx_416E10(7u, 0);
            SFX_Play_43AD70(78u, 75, 0);

            if (sActiveHero_507678->field_100_health > FP_FromInteger(0))
            {
                field_FC_current_motion = eElumStates::State_20_Fall_415F70;
            }
            else
            {
                field_FC_current_motion = eElumStates::State_19_Dead_415F90;
            }

            auto pGibs = ao_new<Gibs>();
            if (pGibs)
            {
                pGibs->ctor_407B20(
                    3,
                    field_A8_xpos,
                    field_AC_ypos,
                    field_B4_velx,
                    field_B8_vely,
                    field_BC_sprite_scale);
            }

            field_100_health = FP_FromInteger(0);

            if (sControlledCharacter_50767C != this)
            {
                field_122 = 0;
            }

            field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
            field_10_anim.Set_Animation_Data_402A40(
                gElumFrameTables_4C5218[field_FC_current_motion],
                GetResBlock_410D00(field_FC_current_motion));
        }
        return 1;

    case Types::eAbilityRing_69:
        return 0;

    case Types::eBeeSwarm_95:
        if (sControlledCharacter_50767C != this)
        {
            field_170_flags |= 1u;
        }
        break;

    default:
        return 1;
    }
    return 1;
}

void Elum::Vsub_412700()
{
    if (field_F8_pLiftPoint)
    {
        field_F8_pLiftPoint->VRemove(this);
        field_F8_pLiftPoint->field_C_refCount--;
        field_F8_pLiftPoint = nullptr;
    }
}

void Elum::Vsub_411260()
{
    if (!field_174_resources.res[30])
    {
        field_174_resources.res[30] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 230, 1, 0);
    }
    field_174_resources.res[22] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 222, 1, 0);
}


void Elum::Vsub_411200()
{
    if (field_126_res_idx != 20)
    {
        ResourceManager::FreeResource_455550(field_174_resources.res[20]);
        field_174_resources.res[20] = nullptr;
    }

    while (!ResourceManager::FreeResource_455550(field_174_resources.res[21]))
    {
        // Empty
    }

    field_174_resources.res[21] = nullptr;
}

void Elum::Vsub_416120()
{
    ToIdle();

    field_10_anim.Set_Animation_Data_402A40(
        gElumFrameTables_4C5218[field_FC_current_motion],
        GetResBlock_410D00(field_FC_current_motion));
}

void Elum::Vsub_411300()
{
    field_174_resources.res[20] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 220, 1, 0);
    field_174_resources.res[21] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 221, 1, 0);
}

void Elum::Vsub_4112B0()
{
    ResourceManager::FreeResource_455550(field_174_resources.res[30]);

    field_174_resources.res[30] = nullptr;

    while (!ResourceManager::FreeResource_455550(field_174_resources.res[22]))
    {
        // Empty
    }

    field_174_resources.res[22] = nullptr;
}

BaseGameObject* Elum::dtor_base_416FE0()
{
    SetVTable(this, 0x4BA970);
    return dtor_401000();
}

BYTE** Elum::GetResBlock_410D00(short currentMotion)
{
    short new_idx = 0;
    if (currentMotion < eElumStates::State_21_Land_414A20)
    {
        new_idx = 0;
    }
    else if (currentMotion < eElumStates::State_25_LickingHoney_415B50)
    {
        new_idx = 16;
    }
    else if (currentMotion < eElumStates::State_27_AbeMountingEnd_415CA0)
    {
        new_idx = 3;
    }
    else if (currentMotion < eElumStates::State_28_AbeUnmountingEnd_415D60)
    {
        new_idx = 23;
    }
    else if (currentMotion < eElumStates::State_29_BeesStruggling_412A90)
    {
        new_idx = 24;
    }
    else if (currentMotion < eElumStates::State_30_HopBegin_414E30)
    {
        new_idx = 4;
    }
    else if (currentMotion < eElumStates::State_44_ScratchBegin_412730)
    {
        new_idx = 20;
    }
    else if (currentMotion < eElumStates::State_48_AbeMoutingBegin_415C40)
    {
        new_idx = 30;
    }
    else if (currentMotion < eElumStates::State_49_AbeUnmountingBegin_415D00)
    {
        new_idx = 22;
    }
    else if (currentMotion >= eElumStates::State_50_Knockback_415DC0)
    {
        new_idx = 15;
        if (currentMotion >= 51)
        {
            new_idx = currentMotion; // OG bug ??
        }
    }
    else
    {
        new_idx = 21;
    }

    if (new_idx == field_126_res_idx)
    {
        return field_174_resources.res[field_126_res_idx];
    }

    if (field_126_res_idx)
    {
        if (field_126_res_idx != 30 && field_126_res_idx != 16 && field_126_res_idx != 20)
        {
            ResourceManager::FreeResource_455550(field_174_resources.res[field_126_res_idx]);
            field_174_resources.res[field_126_res_idx] = nullptr;
        }
    }

    if (new_idx)
    {
        if (new_idx != 30 && new_idx != 16 && new_idx != 20)
        {
            field_174_resources.res[new_idx] = ResourceManager::GetLoadedResource_4554F0(
                ResourceManager::Resource_Animation,
                new_idx + 200,
                1,
                0);
        }
    }
    
    field_126_res_idx = new_idx;

    return field_174_resources.res[new_idx];
}

void Elum::WalkLeft_412FA0()
{
    if (field_B4_velx <= FP_FromInteger(0))
    {
        if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(40), -ScaleToGridSize_41FA30(field_BC_sprite_scale)))
        {
            field_FC_current_motion = eElumStates::State_6_MidWalkToIdle_4133F0;
            return;
        }
    }
    else
    {
        if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(40),  ScaleToGridSize_41FA30(field_BC_sprite_scale)))
        {
            field_FC_current_motion = eElumStates::State_6_MidWalkToIdle_4133F0;
            return;
        }
    }

    if ((field_B4_velx > FP_FromInteger(0) && sInputObject_5009E8.isPressed(sInputKey_Left_4C6594)) ||
        (field_B4_velx < FP_FromInteger(0) && sInputObject_5009E8.isPressed(sInputKey_Right_4C6590)) ||
        !sInputObject_5009E8.isPressed(sInputKey_Right_4C6590 | sInputKey_Left_4C6594))
    {
        field_FC_current_motion = eElumStates::State_6_MidWalkToIdle_4133F0;
        field_10E = 0;
    }
    else
    {
        if (field_10E & sInputKey_Hop_4C65A0)
        {
            field_FC_current_motion = eElumStates::State_18_Unknown_4136A0;
        }
        field_10E = 0;
    }
}

void Elum::WalkRight_4130D0()
{
    if (field_B4_velx <= FP_FromInteger(0))
    {
        if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(40), -ScaleToGridSize_41FA30(field_BC_sprite_scale)))
        {
            field_FC_current_motion = eElumStates::State_5_WalkToIdle_4132D0;
            return;
        }
    }
    else
    {
        if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(40), ScaleToGridSize_41FA30(field_BC_sprite_scale)))
        {
            field_FC_current_motion = eElumStates::State_5_WalkToIdle_4132D0;
            return;
        }
    }

    if ((field_B4_velx > FP_FromInteger(0) && sInputObject_5009E8.isPressed(sInputKey_Left_4C6594)) ||
        (field_B4_velx < FP_FromInteger(0) && sInputObject_5009E8.isPressed(sInputKey_Right_4C6590)) ||
        !sInputObject_5009E8.isPressed(sInputKey_Right_4C6590 | sInputKey_Left_4C6594))
    {
        field_FC_current_motion = eElumStates::State_5_WalkToIdle_4132D0;
        field_10E = 0;
    }
    else
    {
        if (field_10E & sInputKey_Hop_4C65A0)
        {
            field_FC_current_motion = eElumStates::State_17_Unknown_413620;
        }
        field_10E = 0;
    }
}

void Elum::SlowOnX_414210(FP amount)
{
    if (FP_GetExponent(field_B4_velx))
    {
        MoveOnLine_412580(0);

        if (field_B4_velx <= FP_FromInteger(0))
        {
            if (field_B4_velx < FP_FromInteger(0))
            {
                field_B4_velx += (field_BC_sprite_scale * amount);
                if (field_B4_velx > FP_FromInteger(0))
                {
                    field_B4_velx = FP_FromInteger(0);
                }
            }
        }
        else
        {
            field_B4_velx -= (field_BC_sprite_scale * amount);
            if (field_B4_velx < FP_FromInteger(0))
            {
                field_B4_velx = FP_FromInteger(0);
            }
        }
    }
    else
    {
        CheckLiftPointGoneAndSetCamera();

    }
}

void Elum::CheckLiftPointGoneAndSetCamera()
{
    if (field_F8_pLiftPoint)
    {
        if (field_F8_pLiftPoint->field_6_flags.Get(BaseGameObject::eDead_Bit3))
        {
            VOnTrapDoorOpen();
            field_170_flags |= 4u;
        }

        if (sControlledCharacter_50767C == this)
        {
            SetActiveCameraDelayedFromDir_401C90();
        }
    }
}

void Elum::MoveOnLine_412580(__int16 xLookAhead)
{
    CheckLiftPointGoneAndSetCamera();

    const FP oldX = field_A8_xpos;
    FP xpos_off_fp = field_A8_xpos + FP_FromInteger(xLookAhead);
    field_F4_pLine = field_F4_pLine->MoveOnLine_40CA20(
        &xpos_off_fp,
        &field_AC_ypos,
        field_B4_velx);

    if (field_F4_pLine)
    {
        field_A8_xpos += field_B4_velx;
        if (field_F8_pLiftPoint)
        {
            if (field_F4_pLine->field_8_type != 32)
            {
                VOnTrapDoorOpen();
            }
        }
        else if (field_F4_pLine->field_8_type == 32)
        {
            PSX_RECT bRect = {};
            VGetBoundingRect(&bRect, 1);
            VOnCollisionWith(
                {bRect.x, static_cast<short>(bRect.y + 5)},
                {bRect.w, static_cast<short>(bRect.h + 5)},
                ObjListPlatforms_50766C,
                1,
                (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_401C10);
        }
    }
    else
    {
        VOnTrapDoorOpen();
        field_FC_current_motion = eElumStates::State_23_WalkOffEdge_415E90;
        field_E8_LastLineYPos = field_AC_ypos;
        field_A8_xpos = field_B4_velx + oldX;
    }
}

void Elum::SetAbeAsPlayer_412520(__int16 abeMotion)
{
    // Back to Abe
    if (sControlledCharacter_50767C == this)
    {
        sControlledCharacter_50767C = sActiveHero_507678;
        sActiveHero_507678->field_FE_next_state = abeMotion;
    }
    
    // re-load some elum resources
    field_104_pending_resource_count += 2;

    ResourceManager::LoadResourceFile(
        "ELMALONE.BAN",
        BaseAliveGameObject::OnResourceLoaded_4019A0,
        this);

    ResourceManager::LoadResourceFile(
        "ELMPRMNT.BAN",
        BaseAliveGameObject::OnResourceLoaded_4019A0,
        this);
}

__int16 Elum::ToNextState_4120F0()
{
    if (sControlledCharacter_50767C == this)
    {
        return ToNextStateAbeControlled_411E40();
    }

    switch (field_FE_next_state)
    {
    case eElumStates::State_3_WalkLoop_412C90:
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(25), ScaleToGridSize_41FA30(field_BC_sprite_scale)))
            {
                return 0;
            }
            field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
        }
        else
        {
            if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(25), ScaleToGridSize_41FA30(field_BC_sprite_scale)))
            {
                return 0;
            }
            field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
        }

        if (field_124)
        {
            field_FC_current_motion = eElumStates::State_7_Unknown_413200;
            field_124 = field_124 == 0;
        }
        else
        {
            field_FC_current_motion = eElumStates::State_8_IdleToWalk_413270;
            field_124 = 1;
        }

        field_FE_next_state = -1;
        return 1;

    case eElumStates::State_29_BeesStruggling_412A90:
        Sfx_416E10(3u, 0);
        field_FC_current_motion = eElumStates::State_29_BeesStruggling_412A90;
        field_110_timer = gnFrameCount_507670 + 25;
        field_FE_next_state = -1;
        return 1;

    case eElumStates::State_4_Turn_4140F0:
    case eElumStates::State_14_Speak_414860:
    case eElumStates::State_15_Speak_4148F0:
    case eElumStates::State_16_Speak_414980:
        field_FC_current_motion = field_FE_next_state;
        field_FE_next_state = -1;
        return 1;

    case eElumStates::State_10_Yell_4158E0:
        field_FC_current_motion = eElumStates::State_9_ToYell_415890;
        field_FE_next_state = -1;
        return 1;

    case eElumStates::State_45_ScratchLoop_4127B0:
        field_FC_current_motion = eElumStates::State_44_ScratchBegin_412730;
        field_FE_next_state = -1;
        return 1;

    default:
        return 0;
    }
}

__int16 Elum::ToNextStateAbeControlled_411E40()
{
    LiftPoint* pLiftPoint = static_cast<LiftPoint*>(field_F8_pLiftPoint);
    if (pLiftPoint)
    {
        if (pLiftPoint->field_10C == 1)
        {
            if (!pLiftPoint->OnAnyFloor())
            {
                return 0;
            }
        }
    }

    if (sInputObject_5009E8.isPressed(sInputKey_Left_4C6594 | sInputKey_Right_4C6590))
    {
        FP gridSize = {};
        if (sInputObject_5009E8.isPressed(sInputKey_Right_4C6590))
        {
            gridSize = ScaleToGridSize_41FA30(field_BC_sprite_scale);

            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_FC_current_motion = eElumStates::State_4_Turn_4140F0;
                return 1;
            }
        }
        else if (sInputObject_5009E8.isPressed(sInputKey_Left_4C6594))
        {
            gridSize = -ScaleToGridSize_41FA30(field_BC_sprite_scale);

            if (!field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_FC_current_motion = eElumStates::State_4_Turn_4140F0;
                return 1;
            }
        }

        if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(40), gridSize))
        {
            return 0;
        }

        if (sInputObject_5009E8.isPressed(sInputKey_Run_4C65A8))
        {
            field_B4_velx = gridSize / FP_FromInteger(4);
            field_FC_current_motion = eElumStates::State_39_IdleToRun_413B00;
        }
        else
        {
            field_B4_velx = gridSize / FP_FromInteger(9);
            field_FC_current_motion = eElumStates::State_8_IdleToWalk_413270;
        }
        return 1;
    }
    else
    {
        if (sInputObject_5009E8.isPressed(dword_4C65B8 | dword_4C65DC)
            || !(sInputObject_5009E8.isPressed(sInputKey_Hop_4C65A0)))
        {
            return 0;
        }

        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_B4_velx = field_BC_sprite_scale * FP_FromInteger(-15);
        }
        else
        {
            field_B4_velx = field_BC_sprite_scale * FP_FromInteger(15);
        }

        field_FC_current_motion = eElumStates::State_30_HopBegin_414E30;
        return 1;
    }
}

void Elum::HandleElumPathTrans_411460()
{
    PSX_Point camCoords = {};
    gMap_507BA8.GetCurrentCamCoords_444890(&camCoords);

    if (sActiveHero_507678->field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_A8_xpos = ScaleToGridSize_41FA30(field_BC_sprite_scale) +
            FP_FromInteger(camCoords.field_0_x + GridXMidPos_41FA60(field_BC_sprite_scale, ConvertScale_41FA10(field_BC_sprite_scale)));
    }
    else
    {
        field_A8_xpos = ScaleToGridSize_41FA30(field_BC_sprite_scale) -
            FP_FromInteger(camCoords.field_0_x +  GridXMidPos_41FA60(field_BC_sprite_scale, 0));
    }

    if (sActiveHero_507678->field_F4_pLine)
    {
        field_AC_ypos = sActiveHero_507678->field_AC_ypos;
    }
    else
    {
        field_AC_ypos = FP_FromInteger(camCoords.field_2_y + FP_GetExponent(field_AC_ypos) % 480);
    }

    if (field_F4_pLine)
    {
        if (field_F4_pLine->field_8_type == 32)
        {
            field_F8_pLiftPoint = nullptr;
        }

        PathLine* pLine = nullptr;
        FP hitX = {};
        FP hitY = {};
        if (sCollisions_DArray_504C6C->RayCast_40C410(
            field_A8_xpos,
            field_AC_ypos - FP_FromInteger(40),
            field_A8_xpos,
            field_AC_ypos + FP_FromInteger(40),
            &pLine,
            &hitX,
            &hitY,
            field_BC_sprite_scale != FP_FromDouble(0.5) ? 7 : 0x70))
        {
            field_AC_ypos = hitY;
            field_F4_pLine = pLine;
        }
        else
        {
            field_F4_pLine = nullptr;
        }
    }

    field_B2_lvl_number = gMap_507BA8.field_0_current_level;
    field_B0_path_number = gMap_507BA8.field_2_current_path;

    field_8_update_delay = 20;
}

void CC Elum::Sfx_416E10(unsigned __int8 /*direction*/, BaseAliveGameObject* /*pObj*/)
{
    NOT_IMPLEMENTED();
}

void Elum::FindHoney_411600()
{
    NOT_IMPLEMENTED();
}

__int16 Elum::Brain_0_WithoutAbe_416190()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Elum::Brain_1_HoneyAddiction_411730()
{
    NOT_IMPLEMENTED();
    return 0;
}

void Elum::State_0_Respawn_414C60()
{
    State_21_Land_414A20();
}

void Elum::State_1_Idle_412990()
{
    CheckLiftPointGoneAndSetCamera();

    if (sActiveHero_507678->field_FC_current_motion != eAbeStates::State_115_ElumSpeak_4299F0 && !ToNextState_4120F0())
    {
        if (sActiveHero_507678->field_FC_current_motion == eAbeStates::State_150_Chant_42FD50 &&
            sControlledCharacter_50767C == sActiveHero_507678)
        {
            field_FC_current_motion = eElumStates::State_9_ToYell_415890;
        }
        else if (sActiveHero_507678->field_FC_current_motion == eAbeStates::State_137_ElumUnmountBegin_42E2B0)
        {
            ResourceManager::FreeResource_455550(field_174_resources.res[20]);
            field_174_resources.res[20] = nullptr;
            field_FC_current_motion = eElumStates::State_49_AbeUnmountingBegin_415D00;
        }
        else if (gnFrameCount_507670 - field_110_timer > 200 && sControlledCharacter_50767C != this)
        {
            if (field_174_resources.res[30])
            {
                field_FC_current_motion = eElumStates::State_44_ScratchBegin_412730;
            }
            else
            {
                field_174_resources.res[30] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 230, 1, 0);
            }
        }
    }
}

void Elum::State_2_Unknown_412C30()
{
    if (field_104_pending_resource_count == 0)
    {
        Vsub_411200();
        Vsub_411260();

        ToIdle();
    }
}

void Elum::State_3_WalkLoop_412C90()
{
    field_10E |= sInputObject_5009E8.field_0_pads[sCurrentControllerIndex_5076B8].field_0_pressed;
    
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);
    
    MoveOnLine_412580(0);

    if (field_FC_current_motion == eElumStates::State_3_WalkLoop_412C90)
    {
        if (field_10_anim.field_92_current_frame == 2)
        {
            if (field_FE_next_state == eElumStates::State_25_LickingHoney_415B50)
            {
                field_FC_current_motion = eElumStates::State_6_MidWalkToIdle_4133F0;
            }
            else
            {
                FP offX_directed = {};
                if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    offX_directed = -ScaleToGridSize_41FA30(field_BC_sprite_scale);
                }
                else
                {
                    offX_directed = ScaleToGridSize_41FA30(field_BC_sprite_scale);
                }

                if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(25), offX_directed))
                {
                    field_FC_current_motion = eElumStates::State_6_MidWalkToIdle_4133F0;
                }
                else if (sControlledCharacter_50767C == this && !(field_170_flags & 8))
                {
                    WalkLeft_412FA0();
                }
                else if (field_104_pending_resource_count > 0)
                {
                    field_FC_current_motion = eElumStates::State_6_MidWalkToIdle_4133F0;
                }
                else
                {
                    if (field_FE_next_state == eElumStates::State_1_Idle_412990)
                    {
                        field_FC_current_motion = eElumStates::State_6_MidWalkToIdle_4133F0;
                        field_FE_next_state = -1;
                    }
                    else if (field_FE_next_state == eElumStates::State_4_Turn_4140F0)
                    {
                        field_FC_current_motion = eElumStates::State_6_MidWalkToIdle_4133F0;
                    }
                }
            }
        }
        else if (field_10_anim.field_92_current_frame == 5)
        {
            if (!field_10C)
            {
                field_10C = 1;
                MapFollowMe_401D30(1);
            }

            if (sControlledCharacter_50767C != this)
            {
                Sfx_416E10(0, 0);
                return;
            }

            if (sInputObject_5009E8.isPressed(sInputKey_Run_4C65A8))
            {
                field_FC_current_motion = eElumStates::State_41_MidWalkToRun_413560;
            }

            field_10E = 0;
            Sfx_416E10(0, 0);
        }
        else if (field_10_anim.field_92_current_frame == 11)
        {
            if (field_FE_next_state == eElumStates::State_25_LickingHoney_415B50)
            {
                field_FC_current_motion = eElumStates::State_6_MidWalkToIdle_4133F0;
            }
            else
            {
                FP offX_directed = {};
                if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    offX_directed = -ScaleToGridSize_41FA30(field_BC_sprite_scale);
                }
                else
                {
                    offX_directed = ScaleToGridSize_41FA30(field_BC_sprite_scale);
                }

                if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(25), offX_directed))
                {
                    field_FC_current_motion = eElumStates::State_5_WalkToIdle_4132D0;
                }
                else if (sControlledCharacter_50767C == this && !(field_170_flags & 8))
                {
                    WalkRight_4130D0();
                }
                else if (field_104_pending_resource_count)
                {
                    field_FC_current_motion = eElumStates::State_5_WalkToIdle_4132D0;
                }
                else
                {
                    if (field_FE_next_state == eElumStates::State_1_Idle_412990)
                    {
                        field_FC_current_motion = eElumStates::State_5_WalkToIdle_4132D0;
                        field_FE_next_state = -1;
                    }
                    else if (field_FE_next_state == eElumStates::State_4_Turn_4140F0
                        || field_FE_next_state == eElumStates::State_44_ScratchBegin_412730
                        || field_FE_next_state == eElumStates::State_29_BeesStruggling_412A90)
                    {
                        field_FC_current_motion = eElumStates::State_5_WalkToIdle_4132D0;
                    }
                }
            }
        }
        else if (field_10_anim.field_92_current_frame == 14)
        {
            if (!field_10C)
            {
                field_10C = 1;
                MapFollowMe_401D30(1);
            }

            if (sControlledCharacter_50767C != this)
            {
                Sfx_416E10(0, 0);
                return;
            }

            if (sInputObject_5009E8.isPressed(sInputKey_Run_4C65A8))
            {
                field_FC_current_motion = eElumStates::State_40_WalkToRun_4134B0;
            }

            field_10E = 0;
            Sfx_416E10(0, 0);
        }
        else
        {
            field_10C = 0;
        }
    }
}

void Elum::State_4_Turn_4140F0()
{
    NOT_IMPLEMENTED();
}

void Elum::State_5_WalkToIdle_4132D0()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);
    
    MoveOnLine_412580(0);

    if (field_10_anim.field_92_current_frame == 0)
    {
        Sfx_416E10(0, 0);
    }
    else if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        MapFollowMe_401D30(1);

        if (field_FE_next_state == eElumStates::State_29_BeesStruggling_412A90)
        {
            Sfx_416E10(3u, 0);
            field_FC_current_motion = eElumStates::State_29_BeesStruggling_412A90;
            field_110_timer = gnFrameCount_507670 + 25;
            field_FE_next_state = -1;
        }
        else if (field_FE_next_state == eElumStates::State_44_ScratchBegin_412730)
        {
            field_FC_current_motion = eElumStates::State_44_ScratchBegin_412730;
            field_FE_next_state = -1;
        }
        else
        {
            ToIdle();

            if (field_104_pending_resource_count > 0)
            {
                field_FC_current_motion = eElumStates::State_2_Unknown_412C30;
            }
            else
            {
                if (field_FE_next_state == eElumStates::State_4_Turn_4140F0 ||
                    field_FE_next_state == eElumStates::State_25_LickingHoney_415B50)
                {
                    field_FC_current_motion = field_FE_next_state;
                    field_FE_next_state = -1;
                }
            }
        }
    }
}

void Elum::State_6_MidWalkToIdle_4133F0()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);
    
    MoveOnLine_412580(0);

    if (field_10_anim.field_92_current_frame == 0)
    {
        Sfx_416E10(0, 0);
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        MapFollowMe_401D30(1);

        ToIdle();

        if (field_104_pending_resource_count)
        {
            field_FC_current_motion = eElumStates::State_2_Unknown_412C30;
        }
        else
        {
            if (field_FE_next_state == eElumStates::State_4_Turn_4140F0 || field_FE_next_state == eElumStates::State_25_LickingHoney_415B50)
            {
                field_FC_current_motion = field_FE_next_state;
                field_FE_next_state = -1;
            }
        }
    }
}

void Elum::State_7_Unknown_413200()
{
    field_10E |= sInputObject_5009E8.field_0_pads[sCurrentControllerIndex_5076B8].field_0_pressed;

    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_E4 = 3;
        field_E6_last_anim_frame = eElumStates::State_9_ToYell_415890;
        field_120 = 1;
    }

    MoveOnLine_412580(0);
}

void Elum::State_8_IdleToWalk_413270()
{
    field_10E |= sInputObject_5009E8.field_0_pads[sCurrentControllerIndex_5076B8].field_0_pressed;

    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eElumStates::State_3_WalkLoop_412C90;
    }

    MoveOnLine_412580(0);
}

void Elum::State_9_ToYell_415890()
{
    CheckLiftPointGoneAndSetCamera();

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eElumStates::State_10_Yell_4158E0;
    }
}

void Elum::State_10_Yell_4158E0()
{
    Event_Broadcast_417220(kEventNoise_0, this);

    CheckLiftPointGoneAndSetCamera();

    if (field_10_anim.field_92_current_frame || !(field_170_flags & 0x20))
    {
        field_170_flags |= 0x20u;
    }
    else
    {
        Sfx_416E10(2u, this);
        field_170_flags &= ~0x20u;
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (!ToNextState_4120F0())
        {
            ToIdle();
        }
    }
}

void Elum::State_11_Unknown_4159A0()
{
    State_47_Unknown_415A30();
}

void Elum::State_12_RunTurn_414520()
{
    NOT_IMPLEMENTED();
}

void Elum::State_13_RunTurnToWalk_4147C0()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);

    MoveOnLine_412580(0);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eElumStates::State_3_WalkLoop_412C90;
        field_10_anim.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
    }
}

void Elum::State_14_Speak_414860()
{
    NOT_IMPLEMENTED();
}

void Elum::State_15_Speak_4148F0()
{
    NOT_IMPLEMENTED();
}

void Elum::State_16_Speak_414980()
{
    NOT_IMPLEMENTED();
}

void Elum::State_17_Unknown_413620()
{
    MoveOnLine_412580(0);

    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        MapFollowMe_401D30(1);

        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_B4_velx = (field_BC_sprite_scale * FP_FromInteger(-15));
        }
        else
        {
            field_B4_velx = (field_BC_sprite_scale * FP_FromInteger(15));
        }

        field_FC_current_motion = eElumStates::State_30_HopBegin_414E30;
    }
}

void Elum::State_18_Unknown_4136A0()
{
    State_17_Unknown_413620();
}

void Elum::State_19_Dead_415F90()
{
    if (sActiveHero_507678->field_100_health > FP_FromInteger(0))
    {
        if (!sActiveHero_507678->field_2A8_flags.Get(Flags_2A8::e2A8_Bit6_bShrivel) &&
            sActiveHero_507678->field_2A8_flags.Get(Flags_2A8::e2A8_Bit8) &&
            field_104_pending_resource_count == 0)
        {
            field_A8_xpos = FP_FromInteger(field_138_continue_rect.x);
            field_AC_ypos = FP_FromInteger(field_138_continue_rect.y);

            field_B8_vely = FP_FromInteger(0);
            field_B4_velx = FP_FromInteger(0);

            field_B2_lvl_number = field_14A_continue_level;
            field_B0_path_number = field_148_continue_path;
            field_BC_sprite_scale = field_150_continue_sprite_scale;

            field_128_brain_idx = 0;
            field_12A_brain_state = 6;
            field_122 = 1;

            if (!gMap_507BA8.Is_Point_In_Current_Camera_4449C0(field_14A_continue_level, field_148_continue_path, field_A8_xpos, field_AC_ypos, 0))
            {
                Sfx_416E10(2u, this);
            }

            field_170_flags &= ~9u;
            field_110_timer = gnFrameCount_507670;

            field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX, sActiveHero_507678->field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX));

            if (field_170_flags & 2)
            {
                if (field_14C_continue_camera < field_146)
                {
                    field_170_flags &= ~2;
                }
            }

            field_10_anim.field_4_flags.Set(AnimFlags::eBit3_Render);

            field_E8_LastLineYPos = field_AC_ypos;
            field_FC_current_motion = 0;
            field_FE_next_state = -1;
            field_F4_pLine = nullptr;
            field_100_health = FP_FromInteger(1);

            if ( field_174_resources.res[20])
            {
                ResourceManager::FreeResource_455550( field_174_resources.res[20]);
                field_174_resources.res[20] = nullptr;
            }

            if (!field_174_resources.res[30])
            {
                Vsub_411260();
            }
        }
    }
}

void Elum::State_20_Fall_415F70()
{
    if (sActiveHero_507678->field_100_health <= FP_FromInteger(0))
    {
        field_FC_current_motion = eElumStates::State_19_Dead_415F90;
    }
}

void Elum::State_21_Land_414A20()
{
    NOT_IMPLEMENTED();
}

void Elum::State_22_RunOffEdge_415810()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        MapFollowMe_401D30(1);
        if (!ToNextState_4120F0())
        {
            ToIdle();
        }
    }
}

void Elum::State_23_WalkOffEdge_415E90()
{
    State_24_JumpToFall_415ED0();
}

void Elum::State_24_JumpToFall_415ED0()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eElumStates::State_21_Land_414A20;
    }

    State_21_Land_414A20();
}

void Elum::State_25_LickingHoney_415B50()
{
    Event_Broadcast_417220(0, this);

    CheckLiftPointGoneAndSetCamera();

    if (field_10_anim.field_92_current_frame == 6)
    {
        if (gMap_507BA8.GetDirection(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos) != CameraPos::eCamCurrent_0)
        {
            if (field_170_flags & 0x20)
            {
                Elum::Sfx_416E10(8u, 0);
                field_170_flags &= ~0x20;
            }
        }
    }
    else
    {
        field_170_flags |= 0x20u;
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        if (field_FE_next_state == eElumStates::State_4_Turn_4140F0 ||
            field_FE_next_state == eElumStates::State_29_BeesStruggling_412A90)
        {
            field_FC_current_motion = eElumStates::State_26_LickingToStruggling_415AC0;
            if (!field_174_resources.res[30])
            {
                Vsub_411260();
            }
        }
    }
}

void Elum::State_26_LickingToStruggling_415AC0()
{
    State_47_Unknown_415A30();
}

void Elum::State_27_AbeMountingEnd_415CA0()
{
    if (sActiveHero_507678->field_FC_current_motion != eAbeStates::State_136_ElumMountEnd_42E110  && field_104_pending_resource_count == 0)
    {
        Vsub_411300();
        ToIdle();
    }
}

void Elum::State_28_AbeUnmountingEnd_415D60()
{
    if (sActiveHero_507678->field_FC_current_motion != State_138_ElumUnmountEnd_42E390
        && !field_104_pending_resource_count)
    {
        Vsub_411260();
        ToIdle();
    }
}

void Elum::State_29_BeesStruggling_412A90()
{
    Event_Broadcast_417220(0, this);

    if (field_FE_next_state == eElumStates::State_44_ScratchBegin_412730)
    {
        field_FC_current_motion = eElumStates::State_44_ScratchBegin_412730;
        if (!field_174_resources.res[30])
        {
            Vsub_411260();
        }
        field_FE_next_state = -1;
        return;
    }

    if (field_FE_next_state != eElumStates::State_3_WalkLoop_412C90)
    {
        if (field_FE_next_state == eElumStates::State_1_Idle_412990)
        {
            ToIdle();
            field_FE_next_state = -1;
        }
        return;
    }

    if (WallHit_401930(
        field_BC_sprite_scale * FP_FromInteger(25),
        ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromInteger(field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX) != 0 ? -1 : 1)))
    {
        ToIdle();
        field_FE_next_state = -1;
        return;
    }

    field_FC_current_motion = eElumStates::State_8_IdleToWalk_413270;
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_FE_next_state = -1;
        field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
    }
    else
    {
        field_FE_next_state = -1;
        field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
    }
}

void Elum::State_30_HopBegin_414E30()
{
    NOT_IMPLEMENTED();
}

void Elum::State_31_HopMid_414C70()
{
    NOT_IMPLEMENTED();
}

void Elum::State_32_HopLand_415140()
{
    NOT_IMPLEMENTED();
}

void Elum::State_33_RunJumpBegin_415400()
{
    NOT_IMPLEMENTED();
}

void Elum::State_34_RunJumpMid_415240()
{
    NOT_IMPLEMENTED();
}

void Elum::State_35_RunJumpLand_415580()
{
    NOT_IMPLEMENTED();
}

void Elum::State_36_RunLoop_413720()
{
    NOT_IMPLEMENTED();
}

void Elum::State_37_RunSlideStop_4142E0()
{
    NOT_IMPLEMENTED();
}

void Elum::State_38_RunTurnToRun_414810()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);
    
    MoveOnLine_412580(0);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eElumStates::State_36_RunLoop_413720;
        field_10_anim.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
    }
}

void Elum::State_39_IdleToRun_413B00()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);
    Event_Broadcast_417220(kEvent_11, this);

    field_10E |= sInputObject_5009E8.field_0_pads[sCurrentControllerIndex_5076B8].field_0_pressed;

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eElumStates::State_36_RunLoop_413720;
    }

    MoveOnLine_412580(0);
}

void Elum::State_40_WalkToRun_4134B0()
{
    field_10E |= sInputObject_5009E8.field_0_pads[sCurrentControllerIndex_5076B8].field_0_pressed;

    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);
    Event_Broadcast_417220(kEvent_11, this);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
    }
    else
    {
        field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eElumStates::State_36_RunLoop_413720;
    }

    MoveOnLine_412580(0);
}

void Elum::State_41_MidWalkToRun_413560()
{
    field_10E |= sInputObject_5009E8.field_0_pads[sCurrentControllerIndex_5076B8].field_0_pressed;

    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);
    Event_Broadcast_417220(kEvent_11, this);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
    }
    else
    {
        field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_E4 = eElumStates::State_36_RunLoop_413720;
        field_E6_last_anim_frame = eElumStates::State_4_Turn_4140F0;
        field_120 = 1;
    }

    MoveOnLine_412580(0);
}

void Elum::State_42_RunToWalk_413B60()
{
    NOT_IMPLEMENTED();
}

void Elum::State_43_MidRunToWalk_413E20()
{
    NOT_IMPLEMENTED();
}

void Elum::State_44_ScratchBegin_412730()
{
    CheckLiftPointGoneAndSetCamera();

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eElumStates::State_45_ScratchLoop_4127B0;
        if (gMap_507BA8.GetDirection(field_B2_lvl_number, field_B0_path_number, field_A8_xpos, field_AC_ypos) == CameraPos::eCamCurrent_0)
        {
            SND_SEQ_PlaySeq_4775A0(15u, 1, 1);
        }
    }
}

void Elum::State_45_ScratchLoop_4127B0()
{
    CheckLiftPointGoneAndSetCamera();

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eElumStates::State_46_ScratchEnd_412800;
    }
}

void Elum::State_46_ScratchEnd_412800()
{
    CheckLiftPointGoneAndSetCamera();

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_FE_next_state == eElumStates::State_3_WalkLoop_412C90)
        {
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                if (!WallHit_401930(field_BC_sprite_scale * FP_FromInteger(40), -ScaleToGridSize_41FA30(field_BC_sprite_scale)))
                {
                    field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
                    field_FC_current_motion = eElumStates::State_8_IdleToWalk_413270;
                    field_FE_next_state = -1;
                    return;
                }
            }
            else
            {
                if (!WallHit_401930(field_BC_sprite_scale * FP_FromInteger(40), ScaleToGridSize_41FA30(field_BC_sprite_scale)))
                {
                    field_B4_velx =  (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
                    field_FC_current_motion = eElumStates::State_8_IdleToWalk_413270;
                    field_FE_next_state = -1;
                    return;
                }
            }
            field_FE_next_state = -1;
            return;
        }

        if (field_FE_next_state == eElumStates::State_29_BeesStruggling_412A90)
        {
            Sfx_416E10(3u, 0);
            field_FC_current_motion = eElumStates::State_29_BeesStruggling_412A90;
            field_110_timer = gnFrameCount_507670 + 25;
            field_FE_next_state = -1;
            return;
        }

        ToIdle();

    }
}

void Elum::ToIdle()
{
    field_118 = 0;
    field_110_timer = gnFrameCount_507670;
    field_B4_velx = FP_FromInteger(0);
    field_B8_vely = FP_FromInteger(0);
    field_FC_current_motion = eElumStates::State_1_Idle_412990;
    field_10E = 0;
    MapFollowMe_401D30(1);
}

void Elum::State_47_Unknown_415A30()
{
    Event_Broadcast_417220(kEventNoise_0, this);

    CheckLiftPointGoneAndSetCamera();

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (!ToNextState_4120F0())
        {
            ToIdle();
        }
    }
}

void Elum::State_48_AbeMoutingBegin_415C40()
{
    if (sActiveHero_507678->field_FC_current_motion == eAbeStates::State_136_ElumMountEnd_42E110)
    {
        Vsub_4112B0();
        
        field_104_pending_resource_count += 2;

        ResourceManager::LoadResourceFile(
            "ELMRIDE.BAN",
            BaseAliveGameObject::OnResourceLoaded_4019A0,
            this);

        ResourceManager::LoadResourceFile(
            "ELMPDMNT.BAN",
            BaseAliveGameObject::OnResourceLoaded_4019A0,
            this);

        field_FC_current_motion = eElumStates::State_27_AbeMountingEnd_415CA0;
    }
}

void Elum::State_49_AbeUnmountingBegin_415D00()
{
    if (sActiveHero_507678->field_FC_current_motion != eAbeStates::State_137_ElumUnmountBegin_42E2B0)
    {
        Vsub_411200();
        
        field_104_pending_resource_count += 2;

        ResourceManager::LoadResourceFile(
            "ELMALONE.BAN",
            BaseAliveGameObject::OnResourceLoaded_4019A0,
            this);

        ResourceManager::LoadResourceFile(
            "ELMPRMNT.BAN",
            BaseAliveGameObject::OnResourceLoaded_4019A0,
            this);

        field_FC_current_motion = eElumStates::State_28_AbeUnmountingEnd_415D60;
    }
}

void Elum::State_50_Knockback_415DC0()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);

    if (!field_F4_pLine)
    {
        State_21_Land_414A20();
        if (field_FC_current_motion == eElumStates::State_1_Idle_412990)
        {
            field_FC_current_motion = eElumStates::State_50_Knockback_415DC0;
            field_110_timer = gnFrameCount_507670 + 10;
        }
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_110_timer <= static_cast<int>(gnFrameCount_507670))
        {
            if (field_F4_pLine)
            {
                if (sControlledCharacter_50767C != this
                    && !field_174_resources.res[30]
                    && !field_104_pending_resource_count)
                {
                    Vsub_411260();
                }

                ToIdle();
            }
        }
    }
}

void Elum::VUpdate()
{
    VUpdate_4102A0();
}

void Elum::VRender(int** pOrderingTable)
{
    VRender_410E40(pOrderingTable);
}

void Elum::VScreenChanged()
{
    vScreenChange_411340();
}

void Elum::VUpdate_4102A0()
{
    if (sDDCheat_FlyingEnabled_50771C && sControlledCharacter_50767C == this)
    {
        VOnTrapDoorOpen();

        field_FC_current_motion = eElumStates::State_21_Land_414A20;
        field_F4_pLine = nullptr;

        static int sElum_velx_table_4FF968[] =
        {
            4,
            4,
            0,
            -4,
            -4,
            -4,
            0,
            4
        };

        static int sElum_vely_table_4FF988[] =
        {
             0,
             -4,
             -4,
             -4,
             0,
             4,
             4,
             4
        };

        if (sInputObject_5009E8.isPressed(0xF000u))
        {
            const int tableIdx = sInputObject_5009E8.field_0_pads[sCurrentControllerIndex_5076B8].field_2 >> 5;
            field_B4_velx = FP_FromRaw(sElum_velx_table_4FF968[tableIdx]);
            field_B8_vely = FP_FromRaw(sElum_vely_table_4FF988[tableIdx]);

            if (sInputObject_5009E8.isPressed(sInputKey_Run_4C65A8))
            {
                field_B4_velx += FP_FromRaw(sElum_velx_table_4FF968[tableIdx] * 2);
                field_B8_vely += FP_FromRaw(sElum_vely_table_4FF988[tableIdx]);
            }

            field_A8_xpos += field_B4_velx;
            field_AC_ypos += field_B8_vely;

            PSX_Point map_size = {};
            gMap_507BA8.Get_map_size_444870(&map_size);

            if (field_A8_xpos < FP_FromInteger(0))
            {
                field_A8_xpos = FP_FromInteger(0);
            }

            if (field_A8_xpos >= FP_FromInteger(map_size.field_0_x))
            {
                field_A8_xpos = FP_FromInteger(map_size.field_0_x) - FP_FromInteger(1);
            }

            if (field_AC_ypos < FP_FromInteger(0))
            {
                field_AC_ypos = FP_FromInteger(0);
            }

            if (field_AC_ypos >= FP_FromInteger(map_size.field_2_y))
            {
                field_AC_ypos = FP_FromInteger(map_size.field_2_y) - FP_FromInteger(1);
            }
        }
        else
        {
            field_B4_velx = FP_FromInteger(0);
            field_B8_vely = FP_FromInteger(0);
        }
        
        SetActiveCameraDelayedFromDir_401C90();

        sActiveHero_507678->field_A8_xpos = field_A8_xpos;
        sActiveHero_507678->field_AC_ypos = field_AC_ypos;
        return;
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit3_Render) || field_104_pending_resource_count == 0)
    {
        if (field_174_resources.res[0])
        {
            if (!field_174_resources.res[30])
            {
                field_174_resources.res[30] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 230, 1, 0);
            }
        }
        else
        {
            field_174_resources.res[0] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 200, 1, 0);
        }

        if (!(field_170_flags & 0xA) && field_128_brain_idx != 1)
        {
            FindHoney_411600();
        }

        if ((field_170_flags >> 4) & 1)
        {
            HandleElumPathTrans_411460();
            field_170_flags &= ~0x50u;
        }

        if (field_FC_current_motion == eElumStates::State_19_Dead_415F90 || field_B0_path_number == gMap_507BA8.field_2_current_path)
        {
            PathLine* pLine = nullptr;
            if ((field_170_flags >> 6) & 1)
            {
                FP hitX = {};
                FP hitY = {};
                if (sCollisions_DArray_504C6C->RayCast_40C410(
                    field_A8_xpos,
                    field_AC_ypos - FP_FromInteger(40),
                    field_A8_xpos,
                    field_AC_ypos + FP_FromInteger(40),
                    &pLine,
                    &hitX,
                    &hitY,
                    field_BC_sprite_scale != FP_FromDouble(0.5) ? 7 : 0x70))
                {
                    field_AC_ypos = hitY;
                    field_F4_pLine = pLine;
                }
                else
                {
                    field_F4_pLine = nullptr;
                    field_FC_current_motion = eElumStates::State_21_Land_414A20;
                }
                field_170_flags &= ~0x40;
            }

            const auto oldMotion = field_FC_current_motion;
            const FP old_x = field_A8_xpos;
            const FP old_y = field_AC_ypos;

            if (oldMotion != eElumStates::State_19_Dead_415F90)
            {
                field_12A_brain_state = (this->*sElum_table_4C52E8[field_128_brain_idx])();
            }

            if ((field_170_flags >> 2) & 1)
            {
                VCheckCollisionLineStillValid(10);

                PSX_RECT bRect = {};
                VGetBoundingRect(&bRect, 1);

                VOnCollisionWith(
                    {bRect.x, static_cast<short>(bRect.y + 5)},
                    {bRect.w, static_cast<short>(bRect.h + 5)},
                    ObjListPlatforms_50766C,
                    1,
                    (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_401C10);

                if (field_F8_pLiftPoint)
                {
                    field_170_flags &= ~4u;
                }
            }
            
            (this->*sElum_motion_table_4C5148[field_FC_current_motion])();
            
            if (old_x != field_A8_xpos || old_y != field_AC_ypos)
            {
                field_F0_pTlv = gMap_507BA8.TLV_Get_At_446060(
                    nullptr,
                    field_A8_xpos,
                    field_AC_ypos,
                    field_A8_xpos,
                    field_AC_ypos);
                VOn_TLV_Collision(field_F0_pTlv);
            }

            if (oldMotion == field_FC_current_motion)
            {
                if (field_120)
                {
                    field_FC_current_motion = field_E4;
                    BYTE** ppRes = GetResBlock_410D00(field_E4);
                    if (!ppRes)
                    {
                        return;
                    }

                    field_10_anim.Set_Animation_Data_402A40(gElumFrameTables_4C5218[field_FC_current_motion], ppRes);
                    field_10_anim.SetFrame_402AC0(field_E6_last_anim_frame);
                    field_120 = 0;
                    if (sControlledCharacter_50767C == this)
                    {
                        sActiveHero_507678->SyncToElum_42D850(field_FC_current_motion);
                    }
                }
            }
            else
            {
                BYTE** ppRes = GetResBlock_410D00(field_FC_current_motion);
                if (!ppRes)
                {
                    return;
                }

                field_10_anim.Set_Animation_Data_402A40(gElumFrameTables_4C5218[field_FC_current_motion], ppRes);
                if (sControlledCharacter_50767C == this)
                {
                    sActiveHero_507678->SyncToElum_42D850(field_FC_current_motion);
                }
            }

            if (Event_Get_417250(kEventDeathReset_4))
            {
                if (!field_154)
                {
                    if (sActiveHero_507678->field_146_zone_number != field_140_continue_zone_number)
                    {
                        field_128_brain_idx = 0;
                        field_12A_brain_state = 6;
                        field_122 = 1;

                        if (sControlledCharacter_50767C == this)
                        {
                            sActiveHero_507678->field_A8_xpos = field_A8_xpos;
                            sActiveHero_507678->field_AC_ypos = field_AC_ypos;
                            field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX, sActiveHero_507678->field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX));
                        }
                        return;
                    }
                }

                field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
                field_FC_current_motion = eElumStates::State_19_Dead_415F90;
                VOnTrapDoorOpen();
            }

            if (sControlledCharacter_50767C == this)
            {
                sActiveHero_507678->field_A8_xpos = field_A8_xpos;
                sActiveHero_507678->field_AC_ypos = field_AC_ypos;
                field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX, sActiveHero_507678->field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX));
            }
            return;
        }
    }
}

void Elum::VRender_410E40(int** ot)
{
    if (field_B2_lvl_number == gMap_507BA8.field_0_current_level
        && field_B0_path_number == gMap_507BA8.field_2_current_path
        && !field_8_update_delay)
    {
        VRender_417DA0(ot);
    }
}

void Elum::vScreenChange_411340()
{
    if (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
    else
    {
        if (gMap_507BA8.field_2_current_path != gMap_507BA8.field_C_path)
        {
            if (gMap_507BA8.field_0_current_level == LevelIds::eLines_2)
            {
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
            }
            else if (gMap_507BA8.field_0_current_level == LevelIds::eDesert_8 && gMap_507BA8.field_C_path == 9)
            {
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
            }
            else if (field_B0_path_number == gMap_507BA8.field_2_current_path)
            {
                auto pElumPathTrans = static_cast<Path_ElumPathTrans*>(gMap_507BA8.TLV_Get_At_446260(
                    FP_GetExponent(field_A8_xpos),
                    FP_GetExponent(field_AC_ypos),
                    FP_GetExponent(field_A8_xpos),
                    FP_GetExponent(field_AC_ypos),
                    TlvTypes::ElumPathTrans_99));

                if (pElumPathTrans)
                {
                    if (field_122 != 1 &&
                        sControlledCharacter_50767C != this &&
                        pElumPathTrans->field_18_level == gMap_507BA8.field_A_level &&
                        pElumPathTrans->field_1A_path == gMap_507BA8.field_C_path)
                    {
                        field_170_flags |= 0x10u;
                    }
                }
            }

            if (field_F8_pLiftPoint)
            {
                VOnTrapDoorOpen();
                field_170_flags |= 4u;
            }

            if (!(field_170_flags & 0x10))
            {
                field_F4_pLine = nullptr;
                field_170_flags |= 0x40;
            }
        }
    }
}

ALIVE_VAR(1, 0x4C52F0, short, word_4C52F0, 0x88);
ALIVE_VAR(1, 0x4C52F2, short, word_4C52F2, 0x38);
ALIVE_VAR(1, 0x4C52F4, DWORD, dword_4C52F4, 0x1C92C);
ALIVE_VAR(1, 0x4C52F8, DWORD, dword_4C52F8, 0xC8);
ALIVE_VAR(1, 0x4C52FA, short, word_4C52FA, 0);

void Elum::Spawn_410E90(TlvItemInfoUnion tlvInfo)
{
    anythingForTheTimeBeing struct1;
    anythingForTheTimeBeing struct2;

    auto pElum = ao_new<Elum>();
    struct1.field_0 = word_4C52F2;
    struct1.field_2 = tlvInfo.parts.tlvOffset; //todo check it (if it's ever used)

    struct2.field_0 = word_4C52F0;
    struct2.field_2 = word_4C52FA;

    pElum->ctor_410870(dword_4C52F4, struct2, struct1, dword_4C52F8, tlvInfo);
}

Elum* Elum::ctor_410870(int, anythingForTheTimeBeing, anythingForTheTimeBeing, int, TlvItemInfoUnion tlvInfo)
{
    ctor_401090();
    SetVTable(this, 0x4BA8F8);
    field_4_typeId = Types::eElum_26;

    field_158 = -1;
    field_16C = 0;
    field_16E = -1;
    field_1F0_tlvInfo = tlvInfo.all;
    field_174_resources = {};

    field_174_resources.res[16] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 216, 1, 0);
    if (!field_174_resources.res[16])
    {
        ResourceManager::LoadResourceFile_455270("ELMFALL.BAN", 0, 0);
        field_174_resources.res[16] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 216, 1, 0);
    }

    field_104_pending_resource_count = 0;

    field_174_resources.res[22] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 222, 1, 0);
    if (!field_174_resources.res[22] )
    {
        field_104_pending_resource_count++;
        ResourceManager::LoadResourceFile(
            "ELMPRMNT.BAN",
            BaseAliveGameObject::OnResourceLoaded_4019A0,
            this);
        field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
    }

    field_174_resources.res[0] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 200, 1, 0);
    if (!field_174_resources.res[0])
    {
        field_104_pending_resource_count++;
        ResourceManager::LoadResourceFile(
            "ELMBASIC.BAN",
            BaseAliveGameObject::OnResourceLoaded_4019A0,
            this);
        field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
    }

    field_174_resources.res[30] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 230, 1, 0);
    if (!field_174_resources.res[30])
    {
        field_104_pending_resource_count++;
        ResourceManager::LoadResourceFile(
            "ELMALONE.BAN",
            BaseAliveGameObject::OnResourceLoaded_4019A0,
            this);
        field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
    }

    field_126_res_idx = 16;

    Animation_Init_417FD0(
        40404,
        169,
        169,
        field_174_resources.res[16],
        1);

    field_BC_sprite_scale = sActiveHero_507678->field_BC_sprite_scale;

    if (field_BC_sprite_scale == FP_FromInteger(1))
    {
        field_C6_scale = 1;
        field_10_anim.field_C_layer = 28;
    }
    else
    {
        field_C6_scale = 0;
        field_10_anim.field_C_layer = 9;
    }

    field_10A_flags.Set(Flags_10A::e10A_Bit4_SetOffExplosives);
    field_170_flags &= ~0x5Au;

    field_F8_pLiftPoint = nullptr;
    gElum_507680 = this;

    field_FC_current_motion = eElumStates::State_21_Land_414A20;
    field_120 = 0;

    field_12E_honey_ypos = 0;
    field_12C_honey_xpos = 0;

    field_144 = 0;
    field_110_timer = gnFrameCount_507670;

    field_A8_xpos = sActiveHero_507678->field_A8_xpos - (ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromInteger(2));
    field_AC_ypos = sActiveHero_507678->field_AC_ypos - FP_FromInteger(5);

    field_122 = 0;
    field_124 = 1;
    field_10E = 0;
    field_EC = 2;
    field_E6_last_anim_frame = 0;
    field_130 = 0;

    field_140_continue_zone_number = 0;
    field_142_zone_number = sActiveHero_507678->field_146_zone_number;

    field_154 = 0;
    field_128_brain_idx = 0;
    field_12A_brain_state = 0;

    field_10A_flags.Set(Flags_10A::e10A_Bit6);
  
    field_170_flags &= ~5;
    field_170_flags |= 0x20;

    auto pMusic = ao_new<MusicTrigger>();
    if (pMusic)
    {
        pMusic->ctor_443A60(6, 1, 0, 30);
    }

    field_D0_pShadow = ao_new<Shadow>();
    if (field_D0_pShadow)
    {
        field_D0_pShadow->ctor_461FB0();
    }

    return this;
}

END_NS_AO;
