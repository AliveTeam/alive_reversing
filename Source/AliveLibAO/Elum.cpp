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
#include "Honey.hpp"
#include "GameSpeak.hpp"
#include "Math.hpp"
#include "BaseGameAutoPlayer.hpp"

namespace AO {

ALIVE_VAR(1, 0x507680, Elum*, gElum_507680, nullptr);

using TElumStateFunction = decltype(&Elum::Motion_0_Respawn_414C60);

const TElumStateFunction sElum_motion_table_4C5148[] = {
    &Elum::Motion_0_Respawn_414C60,
    &Elum::Motion_1_Idle_412990,
    &Elum::Motion_2_Unknown_412C30,
    &Elum::Motion_3_WalkLoop_412C90,
    &Elum::Motion_4_Turn_4140F0,
    &Elum::Motion_5_WalkToIdle_4132D0,
    &Elum::Motion_6_MidWalkToIdle_4133F0,
    &Elum::Motion_7_IdleToWalk2_413200,
    &Elum::Motion_8_IdleToWalk1_413270,
    &Elum::Motion_9_ToYell_415890,
    &Elum::Motion_10_Yell_4158E0,
    &Elum::Motion_11_Unknown_4159A0,
    &Elum::Motion_12_RunTurn_414520,
    &Elum::Motion_13_RunTurnToWalk_4147C0,
    &Elum::Motion_14_Speak_414860,
    &Elum::Motion_15_Speak_4148F0,
    &Elum::Motion_16_Speak_414980,
    &Elum::Motion_17_Unknown_413620,
    &Elum::Motion_18_Unknown_4136A0,
    &Elum::Motion_19_Dead_415F90,
    &Elum::Motion_20_Fall_415F70,
    &Elum::Motion_21_Land_414A20,
    &Elum::Motion_22_RunOffEdge_415810,
    &Elum::Motion_23_WalkOffEdge_415E90,
    &Elum::Motion_24_JumpToFall_415ED0,
    &Elum::Motion_25_LickingHoney_415B50,
    &Elum::Motion_26_LickingToStruggling_415AC0,
    &Elum::Motion_27_AbeMountingEnd_415CA0,
    &Elum::Motion_28_AbeUnmountingEnd_415D60,
    &Elum::Motion_29_BeesStruggling_412A90,
    &Elum::Motion_30_HopBegin_414E30,
    &Elum::Motion_31_HopMid_414C70,
    &Elum::Motion_32_HopLand_415140,
    &Elum::Motion_33_RunJumpBegin_415400,
    &Elum::Motion_34_RunJumpMid_415240,
    &Elum::Motion_35_RunJumpLand_415580,
    &Elum::Motion_36_RunLoop_413720,
    &Elum::Motion_37_RunSlideStop_4142E0,
    &Elum::Motion_38_RunTurnToRun_414810,
    &Elum::Motion_39_IdleToRun_413B00,
    &Elum::Motion_40_WalkToRun_4134B0,
    &Elum::Motion_41_MidWalkToRun_413560,
    &Elum::Motion_42_RunToWalk_413B60,
    &Elum::Motion_43_MidRunToWalk_413E20,
    &Elum::Motion_44_ScratchBegin_412730,
    &Elum::Motion_45_ScratchLoop_4127B0,
    &Elum::Motion_46_ScratchEnd_412800,
    &Elum::Motion_47_Unknown_415A30,
    &Elum::Motion_48_AbeMoutingBegin_415C40,
    &Elum::Motion_49_AbeUnmountingBegin_415D00,
    &Elum::Motion_50_Knockback_415DC0};

using TBrainStateFunc = decltype(&Elum::Brain_0_WithoutAbe_416190);

const TBrainStateFunc sElum_brain_table_4C52E8[] = {
    &Elum::Brain_0_WithoutAbe_416190,
    &Elum::Brain_1_HoneyAddiction_411730,
};

const AnimId gElumAnimIdTables_4C5218[52] = {
    AnimId::Elum_Idle,
    AnimId::Elum_Idle,
    AnimId::Elum_Idle,
    AnimId::Elum_WalkLoop,
    AnimId::Elum_Turn,
    AnimId::Elum_WalkToIdle,
    AnimId::Elum_MidWalkToIdle,
    AnimId::Elum_IdleToWalk2,
    AnimId::Elum_IdleToWalk1,
    AnimId::Elum_ToYell,
    AnimId::Elum_Yell,
    AnimId::Elum_Unknown1,
    AnimId::Elum_RunTurn,
    AnimId::Elum_RunTurnToWalk,
    AnimId::Elum_Speak,
    AnimId::Elum_Speak,
    AnimId::Elum_Speak,
    AnimId::Elum_WalkToIdle,
    AnimId::Elum_MidWalkToIdle,
    AnimId::Elum_Idle,
    AnimId::Elum_Idle,
    AnimId::Elum_Land,
    AnimId::Elum_RunOffEdge,
    AnimId::Elum_WalkOffEdge,
    AnimId::Elum_JumpToFall,
    AnimId::Elum_LickingHoney,
    AnimId::Elum_LickingToStruggle,
    AnimId::Elum_AbeMountingEnd,
    AnimId::Elum_AbeUnmountingEnd,
    AnimId::Elum_BeesStruggling,
    AnimId::Elum_HopBegin,
    AnimId::Elum_HopMid,
    AnimId::Elum_HopLand,
    AnimId::Elum_RunJumpBegin,
    AnimId::Elum_RunJumpMid,
    AnimId::Elum_RunJumpLand,
    AnimId::Elum_RunLoop,
    AnimId::Elum_RunSlideStop,
    AnimId::Elum_RunTurnToRun,
    AnimId::Elum_IdleToRun,
    AnimId::Elum_WalkToRun,
    AnimId::Elum_MidWalkToRun,
    AnimId::Elum_RunToWalk,
    AnimId::Elum_MidRunToWalk,
    AnimId::Elum_ScratchBegin,
    AnimId::Elum_ScratchLoop,
    AnimId::Elum_ScratchEnd,
    AnimId::Elum_ScratchUnknown,
    AnimId::Elum_MountUnmountBegin,
    AnimId::Elum_MountUnmountBegin,
    AnimId::Elum_Knockback,
    AnimId::None};

BaseGameObject* Elum::VDestructor(s32 flags)
{
    return Vdtor_411710(flags);
}

Elum* Elum::Vdtor_411710(s32 flags)
{
    dtor_410BC0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

EXPORT BaseGameObject* Elum::dtor_410BC0()
{
    SetVTable(this, 0x4BA8F8);

    for (u8**& ppRes : field_174_resources.res)
    {
        if (ppRes && field_10_anim.field_20_ppBlock != ppRes)
        {
            ResourceManager::FreeResource_455550(ppRes);
        }
    }

    gElum_507680 = nullptr;
    VOnTrapDoorOpen_412700();

    ResourceManager::FreeResource_455550(
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kAneprmntAOResID, 0, 0));

    if (field_104_pending_resource_count)
    {
        ResourceManager::WaitForPendingResources_41EA60(this);
    }
    field_104_pending_resource_count = 0;

    const AOResourceID resIDs[] = { 
        AOResourceID::kElmaloneAOResID_230, 
        AOResourceID::kElmprmntAOResID__222,
        AOResourceID::kElumRideAOResID_220,
        AOResourceID::kElumPdmntAOResID_221};

    for (s32 resID : resIDs)
    {
        ResourceManager::FreeResource_455550(
            ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, resID, 1, 0));
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
            if (field_140_continue_zone_number != pContinueTlv->field_18_zone_number && pContinueTlv->field_18_zone_number > field_142_zone_number && pContinueTlv->field_1E_elum_restarts == Choice_short::eYes_1)
            {
                field_140_continue_zone_number = pContinueTlv->field_18_zone_number;
                field_138_continue_rect.x = pContinueTlv->field_10_top_left.field_0_x;
                field_138_continue_rect.y = pContinueTlv->field_10_top_left.field_2_y;
                field_138_continue_rect.w = pContinueTlv->field_14_bottom_right.field_0_x;
                field_138_continue_rect.h = pContinueTlv->field_14_bottom_right.field_2_y;

                field_148_continue_path = gMap_507BA8.field_2_current_path;
                field_14C_continue_camera = gMap_507BA8.field_4_current_camera;
                field_14A_continue_level = gMap_507BA8.field_0_current_level;
                field_150_continue_sprite_scale = field_BC_sprite_scale;
                field_144_bRespawnOnDead = 1;
            }
        }
        else if (pTlv->field_4_type == TlvTypes::DeathDrop_5 && field_100_health > FP_FromInteger(0))
        {
            if (sControlledCharacter_50767C != this)
            {
                field_122_bDontFollowAbe = 0;
            }
            Elum_SFX_416E10(ElumSounds::eHowl_2, this);
            field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
            field_FC_current_motion = eElumMotions::Motion_20_Fall_415F70;
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

s16 Elum::VTakeDamage(BaseGameObject* pFrom)
{
    return VTakeDamage_411020(pFrom);
}

s16 Elum::VTakeDamage_411020(BaseGameObject* pFrom)
{
    switch (pFrom->field_4_typeId)
    {
        case Types::eBullet_10:
        case Types::eBaseBomb_30:
        case Types::eExplosion_74:
            if (field_100_health > FP_FromInteger(0))
            {
                Elum_SFX_416E10(ElumSounds::eExploding_7, 0);
                SFX_Play_43AD70(SoundEffect::KillEffect_78, 75, 0);

                if (sActiveHero_507678->field_100_health > FP_FromInteger(0))
                {
                    field_FC_current_motion = eElumMotions::Motion_20_Fall_415F70;
                }
                else
                {
                    field_FC_current_motion = eElumMotions::Motion_19_Dead_415F90;
                }

                auto pGibs = ao_new<Gibs>();
                if (pGibs)
                {
                    pGibs->ctor_407B20(
                        GibType::Elum_3,
                        field_A8_xpos,
                        field_AC_ypos,
                        field_B4_velx,
                        field_B8_vely,
                        field_BC_sprite_scale);
                }

                field_100_health = FP_FromInteger(0);

                if (sControlledCharacter_50767C != this)
                {
                    field_122_bDontFollowAbe = 0;
                }

                field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
                const AnimRecord& rec = AO::AnimRec(gElumAnimIdTables_4C5218[field_FC_current_motion]);
                field_10_anim.Set_Animation_Data_402A40(
                    rec.mFrameTableOffset,
                    GetResBlock_410D00(field_FC_current_motion));
            }
            return 1;

        case Types::eAbilityRing_69:
            return 0;

        case Types::eBeeSwarm_95:
            if (sControlledCharacter_50767C != this)
            {
                field_170_flags.Set(Elum::Flags_170::eStrugglingWithBees_Bit1);
            }
            break;

        default:
            return 1;
    }
    return 1;
}

void Elum::VOnTrapDoorOpen()
{
    VOnTrapDoorOpen_412700();
}

void Elum::ToKnockback()
{
    field_B4_velx = FP_FromInteger(0);
    field_FC_current_motion = eElumMotions::Motion_50_Knockback_415DC0;

    if (field_F4_pLine)
    {
        MoveOnLine_412580(0);
    }
    MapFollowMe_401D30(TRUE);
    Environment_SFX_42A220(EnvironmentSfx::eKnockback_13, 95, -200, this);
}

void Elum::VOnTrapDoorOpen_412700()
{
    if (field_F8_pLiftPoint)
    {
        field_F8_pLiftPoint->VRemove(this);
        field_F8_pLiftPoint->field_C_refCount--;
        field_F8_pLiftPoint = nullptr;
    }
}

void Elum::VLoadUnmountedResources_411260()
{
    if (!field_174_resources.res[30])
    {
        field_174_resources.res[30] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kElmaloneAOResID_230, 1, 0);
    }
    field_174_resources.res[22] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kElmprmntAOResID__222, 1, 0);
}


void Elum::VFreeMountedResources_411200()
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

    const AnimRecord& rec = AO::AnimRec(gElumAnimIdTables_4C5218[field_FC_current_motion]);
    field_10_anim.Set_Animation_Data_402A40(
        rec.mFrameTableOffset,
        GetResBlock_410D00(field_FC_current_motion));
}

void Elum::VLoadMountedResources_411300()
{
    field_174_resources.res[20] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kElumRideAOResID_220, 1, 0);
    field_174_resources.res[21] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kElumPdmntAOResID_221, 1, 0);
}

void Elum::VFreeUnmountedResources_4112B0()
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

u8** Elum::GetResBlock_410D00(s16 currentMotion)
{
    s16 new_idx = 0;
    if (currentMotion < eElumMotions::Motion_21_Land_414A20)
    {
        new_idx = 0;
    }
    else if (currentMotion < eElumMotions::Motion_25_LickingHoney_415B50)
    {
        new_idx = 16;
    }
    else if (currentMotion < eElumMotions::Motion_27_AbeMountingEnd_415CA0)
    {
        new_idx = 3;
    }
    else if (currentMotion < eElumMotions::Motion_28_AbeUnmountingEnd_415D60)
    {
        new_idx = 23;
    }
    else if (currentMotion < eElumMotions::Motion_29_BeesStruggling_412A90)
    {
        new_idx = 24;
    }
    else if (currentMotion < eElumMotions::Motion_30_HopBegin_414E30)
    {
        new_idx = 4;
    }
    else if (currentMotion < eElumMotions::Motion_44_ScratchBegin_412730)
    {
        new_idx = 20;
    }
    else if (currentMotion < eElumMotions::Motion_48_AbeMoutingBegin_415C40)
    {
        new_idx = 30;
    }
    else if (currentMotion < eElumMotions::Motion_49_AbeUnmountingBegin_415D00)
    {
        new_idx = 22;
    }
    else if (currentMotion >= eElumMotions::Motion_50_Knockback_415DC0)
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

void Elum::MidWalkToNextMotion_412FA0()
{
    if (field_B4_velx <= FP_FromInteger(0))
    {
        if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(40), -ScaleToGridSize_41FA30(field_BC_sprite_scale)))
        {
            field_FC_current_motion = eElumMotions::Motion_6_MidWalkToIdle_4133F0;
            return;
        }
    }
    else
    {
        if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(40), ScaleToGridSize_41FA30(field_BC_sprite_scale)))
        {
            field_FC_current_motion = eElumMotions::Motion_6_MidWalkToIdle_4133F0;
            return;
        }
    }

    if ((field_B4_velx > FP_FromInteger(0) && Input().IsAnyPressed(sInputKey_Left_4C6594)) || (field_B4_velx < FP_FromInteger(0) && Input().IsAnyPressed(sInputKey_Right_4C6590)) || !Input().IsAnyPressed(sInputKey_Right_4C6590 | sInputKey_Left_4C6594))
    {
        field_FC_current_motion = eElumMotions::Motion_6_MidWalkToIdle_4133F0;
    }
    else
    {
        if (field_10E_pressed & sInputKey_Hop_4C65A0)
        {
            field_FC_current_motion = eElumMotions::Motion_18_MidWalkToHop_4136A0;
        }
    }
    field_10E_pressed = 0;
}

void Elum::WalkToNextMotion_4130D0()
{
    if (field_B4_velx <= FP_FromInteger(0))
    {
        if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(40), -ScaleToGridSize_41FA30(field_BC_sprite_scale)))
        {
            field_FC_current_motion = eElumMotions::Motion_5_WalkToIdle_4132D0;
            return;
        }
    }
    else
    {
        if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(40), ScaleToGridSize_41FA30(field_BC_sprite_scale)))
        {
            field_FC_current_motion = eElumMotions::Motion_5_WalkToIdle_4132D0;
            return;
        }
    }

    if ((field_B4_velx > FP_FromInteger(0) && Input().IsAnyPressed(sInputKey_Left_4C6594)) || (field_B4_velx < FP_FromInteger(0) && Input().IsAnyPressed(sInputKey_Right_4C6590)) || !Input().IsAnyPressed(sInputKey_Right_4C6590 | sInputKey_Left_4C6594))
    {
        field_FC_current_motion = eElumMotions::Motion_5_WalkToIdle_4132D0;
        field_10E_pressed = 0;
    }
    else
    {
        if (field_10E_pressed & sInputKey_Hop_4C65A0)
        {
            field_FC_current_motion = eElumMotions::Motion_17_WalkToHop_413620;
        }
        field_10E_pressed = 0;
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
            field_170_flags.Set(Elum::Flags_170::eFalling_Bit3);
        }

        if (sControlledCharacter_50767C == this)
        {
            SetActiveCameraDelayedFromDir_401C90();
        }
    }
}

void Elum::MoveOnLine_412580(s16 xLookAhead)
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
            if (field_F4_pLine->field_8_type != eLineTypes::eUnknown_32)
            {
                VOnTrapDoorOpen();
            }
        }
        else if (field_F4_pLine->field_8_type == eLineTypes::eUnknown_32)
        {
            PSX_RECT bRect = {};
            VGetBoundingRect(&bRect, 1);
            VOnCollisionWith(
                {bRect.x, static_cast<s16>(bRect.y + 5)},
                {bRect.w, static_cast<s16>(bRect.h + 5)},
                ObjListPlatforms_50766C,
                1,
                (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_401C10);
        }
    }
    else
    {
        VOnTrapDoorOpen();
        field_FC_current_motion = eElumMotions::Motion_23_WalkOffEdge_415E90;
        field_E8_LastLineYPos = field_AC_ypos;
        field_A8_xpos = field_B4_velx + oldX;
    }
}

void Elum::SetAbeAsPlayer_412520(s16 abeMotion)
{
    // Back to Abe
    if (sControlledCharacter_50767C == this)
    {
        sControlledCharacter_50767C = sActiveHero_507678;
        sActiveHero_507678->field_FE_next_motion = abeMotion;
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

s16 Elum::ToNextMotion_4120F0()
{
    if (sControlledCharacter_50767C == this)
    {
        return ToNextMotionAbeControlled_411E40();
    }

    switch (field_FE_next_motion)
    {
        case eElumMotions::Motion_3_WalkLoop_412C90:
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

            if (field_124_bShould_IdleToWalk1)
            {
                field_FC_current_motion = eElumMotions::Motion_7_IdleToWalk1_413200;
                field_124_bShould_IdleToWalk1 = field_124_bShould_IdleToWalk1 == 0;
            }
            else
            {
                field_FC_current_motion = eElumMotions::Motion_8_IdleToWalk2_413270;
                field_124_bShould_IdleToWalk1 = 1;
            }

            field_FE_next_motion = -1;
            return 1;

        case eElumMotions::Motion_29_BeesStruggling_412A90:
            Elum_SFX_416E10(ElumSounds::eBeesStruggle_3, 0);
            field_FC_current_motion = eElumMotions::Motion_29_BeesStruggling_412A90;
            field_110_timer = gnFrameCount_507670 + 25;
            field_FE_next_motion = -1;
            return 1;

        case eElumMotions::Motion_4_Turn_4140F0:
        case eElumMotions::Motion_14_Speak_414860:
        case eElumMotions::Motion_15_Speak_4148F0:
        case eElumMotions::Motion_16_Speak_414980:
            field_FC_current_motion = field_FE_next_motion;
            field_FE_next_motion = -1;
            return 1;

        case eElumMotions::Motion_10_Yell_4158E0:
            field_FC_current_motion = eElumMotions::Motion_9_ToYell_415890;
            field_FE_next_motion = -1;
            return 1;

        case eElumMotions::Motion_45_ScratchLoop_4127B0:
            field_FC_current_motion = eElumMotions::Motion_44_ScratchBegin_412730;
            field_FE_next_motion = -1;
            return 1;

        default:
            return 0;
    }
}

s16 Elum::ToNextMotionAbeControlled_411E40()
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

    if (Input().IsAnyPressed(sInputKey_Left_4C6594 | sInputKey_Right_4C6590))
    {
        FP gridSize = {};
        if (Input().IsAnyPressed(sInputKey_Right_4C6590))
        {
            gridSize = ScaleToGridSize_41FA30(field_BC_sprite_scale);

            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_FC_current_motion = eElumMotions::Motion_4_Turn_4140F0;
                return 1;
            }
        }
        else if (Input().IsAnyPressed(sInputKey_Left_4C6594))
        {
            gridSize = -ScaleToGridSize_41FA30(field_BC_sprite_scale);

            if (!field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_FC_current_motion = eElumMotions::Motion_4_Turn_4140F0;
                return 1;
            }
        }

        if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(40), gridSize))
        {
            return 0;
        }

        if (Input().IsAnyPressed(sInputKey_Run_4C65A8))
        {
            field_B4_velx = gridSize / FP_FromInteger(4);
            field_FC_current_motion = eElumMotions::Motion_39_IdleToRun_413B00;
        }
        else
        {
            field_B4_velx = gridSize / FP_FromInteger(9);
            field_FC_current_motion = eElumMotions::Motion_8_IdleToWalk2_413270;
        }
        return 1;
    }
    else
    {
        if (Input().IsAnyPressed(sInputKey_LeftGameSpeakEnabler_4C65B8 | sInputKey_RightGameSpeakEnabler_4C65DC)
            || !(Input().IsAnyPressed(sInputKey_Hop_4C65A0)))
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

        field_FC_current_motion = eElumMotions::Motion_30_HopBegin_414E30;
        return 1;
    }
}

void Elum::HandleElumPathTrans_411460()
{
    PSX_Point camCoords = {};
    gMap_507BA8.GetCurrentCamCoords_444890(&camCoords);

    if (sActiveHero_507678->field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_A8_xpos = ScaleToGridSize_41FA30(field_BC_sprite_scale) + FP_FromInteger(camCoords.field_0_x + XGrid_Index_To_XPos_41FA60(field_BC_sprite_scale, MaxGridBlocks_41FA10(field_BC_sprite_scale)));
    }
    else
    {
        field_A8_xpos = FP_FromInteger(camCoords.field_0_x + XGrid_Index_To_XPos_41FA60(field_BC_sprite_scale, 0)) - ScaleToGridSize_41FA30(field_BC_sprite_scale);
    }

    if (sActiveHero_507678->field_F4_pLine)
    {
        field_AC_ypos = sActiveHero_507678->field_AC_ypos;
    }
    else
    {
        field_AC_ypos = FP_FromInteger(camCoords.field_2_y + FP_GetExponent(field_AC_ypos) % 480);
    }

    if (field_F4_pLine && field_F4_pLine->field_8_type == eLineTypes::eUnknown_32)
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

    field_B2_lvl_number = gMap_507BA8.field_0_current_level;
    field_B0_path_number = gMap_507BA8.field_2_current_path;

    field_8_update_delay = 20;
}

const SfxDefinition sElumSfx_4C5398[12] = {
    {0, 4, 36, 50, -127, 0, 0},
    {0, 4, 37, 50, -127, 0, 0},
    {0, 4, 38, 50, -127, 0, 0},
    {0, 5, 61, 118, -1, 1, 0},
    {0, 5, 67, 80, -630, -630, 0},
    {0, 5, 63, 110, 0, 0, 0},
    {0, 18, 46, 127, -1700, -1700, 0},
    {0, 3, 59, 90, -600, -600, 0},
    {0, 5, 64, 60, -1, 1, 0},
    {0, 5, 65, 60, -1, 1, 0},
    {0, 5, 66, 60, -1, 1, 0},
    {0, 21, 64, 60, -1, 1, 0}};

void CC Elum::Elum_SFX_416E10(ElumSounds soundId, BaseAliveGameObject* pObj)
{
    switch (soundId)
    {
        case ElumSounds::eWalkingFootstep_0:
            SFX_SfxDefinition_Play_4770F0(&sElumSfx_4C5398[Math_RandomRange_450F20(0, 2)], Math_RandomRange_450F20(54, 58), -700, -573);
            break;

        case ElumSounds::eRunningFootstep_1:
            SFX_SfxDefinition_Play_4770F0(&sElumSfx_4C5398[Math_RandomRange_450F20(0, 2)], Math_RandomRange_450F20(66, 70), -573, -446);
            break;

        case ElumSounds::eHowl_2:
        {
            CameraPos dir = CameraPos::eCamCurrent_0;
            if (pObj)
            {
                dir = gMap_507BA8.GetDirection(
                    pObj->field_B2_lvl_number,
                    pObj->field_B0_path_number,
                    pObj->field_A8_xpos,
                    pObj->field_AC_ypos);
            }

            s32 volLeft = 0;
            s32 volRight = 0;
            switch (dir)
            {
                case CameraPos::eCamCurrent_0:
                    volLeft = 118;
                    volRight = 118;
                    break;

                case CameraPos::eCamRight_4:
                    volLeft = 26;
                    volRight = 80;
                    break;

                case CameraPos::eCamLeft_3:
                    volLeft = 80;
                    volRight = 26;
                    break;

                default:
                    volLeft = 80;
                    volRight = 80;
                    break;
            }
            SFX_SfxDefinition_Play_477330(&sElumSfx_4C5398[3], (s16) volLeft, (s16) volRight, 0, 0);
            break;
        }

        case ElumSounds::eBeesStruggle_3:
            SFX_SfxDefinition_Play_4770F0(&sElumSfx_4C5398[4], 0, 0x7FFF, 0x7FFF);
            break;

        case ElumSounds::eHitGroundSoft_4:
            SFX_SfxDefinition_Play_4770F0(&sElumSfx_4C5398[7], 0, 0x7FFF, 0x7FFF);
            break;

        case ElumSounds::eRunSlide_5:
            Environment_SFX_42A220(EnvironmentSfx::eRunSlide_4, 0, 0x7FFF, 0);
            break;

        case ElumSounds::eSpottedHoney_6:
            SFX_SfxDefinition_Play_4770F0(&sElumSfx_4C5398[5], 0, 0x7FFF, 0x7FFF);
            break;

        case ElumSounds::eExploding_7:
            SFX_SfxDefinition_Play_4770F0(&sElumSfx_4C5398[6], 0, 0x7FFF, 0x7FFF);
            break;

        case ElumSounds::eLickingHoney_8:
            SFX_SfxDefinition_Play_4770F0(&sElumSfx_4C5398[Math_RandomRange_450F20(8, 10)], 0, 0x7FFF, 0x7FFF);
            break;

        default:
            return;
    }
}

void Elum::FindHoney_411600()
{
    if (field_FC_current_motion != eElumMotions::Motion_25_LickingHoney_415B50)
    {
        field_170_flags.Clear(Elum::Flags_170::eFoundHoney_Bit4);

        for (s32 i = 0; i < gBaseGameObject_list_9F2DF0->Size(); i++)
        {
            BaseGameObject* pObjIter = gBaseGameObject_list_9F2DF0->ItemAt(i);

            if (!pObjIter)
            {
                break;
            }

            if (pObjIter->field_4_typeId == Types::eHoney_47)
            {
                auto pHoney = static_cast<Honey*>(pObjIter);
                if (gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                        pHoney->field_B2_lvl_number,
                        pHoney->field_B0_path_number,
                        pHoney->field_A8_xpos,
                        pHoney->field_AC_ypos,
                        0)
                    && gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                        field_B2_lvl_number,
                        field_B0_path_number,
                        field_A8_xpos,
                        field_AC_ypos,
                        0))
                {
                    field_12C_honey_xpos = FP_GetExponent(pHoney->field_A8_xpos);
                    field_12E_honey_ypos = FP_GetExponent(pHoney->field_AC_ypos);
                    field_170_flags.Set(Elum::Flags_170::eFoundHoney_Bit4);
                    break;
                }
            }
        }
    }
}

s16 Elum::NearHoney_411DA0()
{
    if (field_170_flags.Get(Elum::Flags_170::eFoundHoney_Bit4))
    {
        auto pLiftPoint = static_cast<LiftPoint*>(field_F8_pLiftPoint);
        if (pLiftPoint && pLiftPoint->field_10C == 1 && !pLiftPoint->OnAnyFloor())
        {
            // We're on a lift that isn't on a floor
            return 0;
        }

        return abs(FP_GetExponent(field_AC_ypos) - field_12E_honey_ypos) <= 24 ? 1 : 0;
    }
    return 0;
}

s16 Elum::Brain_0_WithoutAbe_416190()
{
    const s32 last_event_index = pEventSystem_4FF954->field_18_last_event_index;

    GameSpeakEvents last_event_v1;
    GameSpeakEvents last_event_v2;

    if (field_158_last_event_idx == last_event_index)
    {
        const bool bLastEventMinus1 = pEventSystem_4FF954->field_10_last_event == GameSpeakEvents::eNone_m1;
        last_event_v2 = bLastEventMinus1 ? GameSpeakEvents::eNone_m1 : GameSpeakEvents::eSameAsLast_m2;
        last_event_v1 = bLastEventMinus1 ? GameSpeakEvents::eNone_m1 : GameSpeakEvents::eSameAsLast_m2;
    }
    else
    {
        field_158_last_event_idx = last_event_index;
        last_event_v1 = pEventSystem_4FF954->field_10_last_event;
        last_event_v2 = pEventSystem_4FF954->field_10_last_event;
    }

    if (field_170_flags.Get(Elum::Flags_170::eStrugglingWithBees_Bit1))
    {
        field_FE_next_motion = eElumMotions::Motion_1_Idle_412990;
        field_128_brain_idx = 1;
        return 0;
    }

    const FP kGridSize = ScaleToGridSize_41FA30(field_BC_sprite_scale);

    // TODO: A lot of duplicated logic below and a lot of simplifaction
    // can be done also

    switch (field_12A_brain_sub_state)
    {
        case 0:
            return 2;

        case 1:
        {
            const FP xd = sActiveHero_507678->field_A8_xpos - field_A8_xpos;
            if (FP_Abs(xd) < (kGridSize * FP_FromInteger(2)))
            {
                field_FE_next_motion = eElumMotions::Motion_1_Idle_412990;
                return 2;
            }

            if (xd > FP_FromInteger(0))
            {
                if (field_B4_velx < FP_FromInteger(0))
                {
                    field_FE_next_motion = eElumMotions::Motion_1_Idle_412990;
                    return 2;
                }
            }
            else if (xd < FP_FromInteger(0))
            {
                if (field_B4_velx > FP_FromInteger(0))
                {
                    field_FE_next_motion = eElumMotions::Motion_1_Idle_412990;
                    return 2;
                }
            }

            if (field_B4_velx < FP_FromInteger(0))
            {
                if (Check_IsOnEndOfLine_4021A0(1, 1))
                {
                    field_FE_next_motion = eElumMotions::Motion_1_Idle_412990;
                    return 2;
                }
            }
            else if (field_B4_velx > FP_FromInteger(0))
            {
                if (Check_IsOnEndOfLine_4021A0(0, 1))
                {
                    field_FE_next_motion = eElumMotions::Motion_1_Idle_412990;
                    return 2;
                }
            }
            return field_12A_brain_sub_state;
        }

        case 2:
        {
            if (NearHoney_411DA0())
            {
                field_128_brain_idx = 1;
                field_122_bDontFollowAbe = 1;
                return 0;
            }

            const FP xd = sActiveHero_507678->field_A8_xpos - field_A8_xpos;
            if (xd > (kGridSize / FP_FromInteger(2)) && field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_FE_next_motion = eElumMotions::Motion_4_Turn_4140F0;
                return 4;
            }
            else if (xd < -(kGridSize / FP_FromInteger(2)) && !field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_FE_next_motion = eElumMotions::Motion_4_Turn_4140F0;
                return 4;
            }

            if (field_F8_pLiftPoint)
            {
                if (field_F8_pLiftPoint->field_10C == 1)
                {
                    auto pLift = static_cast<LiftPoint*>(field_F8_pLiftPoint);
                    if (!pLift->OnAnyFloor()) // TODO: Check logic
                    {
                        if (field_A8_xpos == sActiveHero_507678->field_A8_xpos)
                        {
                            return 2;
                        }

                        if (VIsFacingMe(sActiveHero_507678))
                        {
                            field_FE_next_motion = eElumMotions::Motion_3_WalkLoop_412C90;
                            return 3;
                        }

                        field_FE_next_motion = eElumMotions::Motion_4_Turn_4140F0;
                        return 4;
                    }
                }
            }

            if (sActiveHero_507678->field_FC_current_motion == eAbeMotions::Motion_139_ElumMountBegin_42E090)
            {
                field_FC_current_motion = eElumMotions::Motion_48_AbeMoutingBegin_415C40;
                return 16;
            }

            if (last_event_v1 == GameSpeakEvents::eFollowMe_10)
            {
                if (gMap_507BA8.field_0_current_level == field_B2_lvl_number
                    && gMap_507BA8.field_2_current_path == field_B0_path_number)
                {
                    if (Is_In_Current_Camera_417CC0() == CameraPos::eCamCurrent_0)
                    {
                        if (FP_Abs(xd) > (kGridSize / FP_FromInteger(2)))
                        {
                            field_114_respond_timer = gnFrameCount_507670 + 14;
                            return 12;
                        }
                    }
                }
            }

            if (last_event_v1 == GameSpeakEvents::eHello_9
                && gMap_507BA8.field_0_current_level == field_B2_lvl_number
                && gMap_507BA8.field_2_current_path == field_B0_path_number)
            {
                if (Is_In_Current_Camera_417CC0() == CameraPos::eCamCurrent_0)
                {
                    field_114_respond_timer = gnFrameCount_507670 + 14;
                    return 9;
                }
            }

            if (last_event_v1 == GameSpeakEvents::eFart_3
                && gMap_507BA8.field_0_current_level == field_B2_lvl_number
                && gMap_507BA8.field_2_current_path == field_B0_path_number)
            {
                if (Is_In_Current_Camera_417CC0() == CameraPos::eCamCurrent_0)
                {
                    field_114_respond_timer = gnFrameCount_507670 + 28;
                    return 11;
                }
            }

            if (last_event_v1 == GameSpeakEvents::eWait_12
                && gMap_507BA8.field_0_current_level == field_B2_lvl_number
                && gMap_507BA8.field_2_current_path == field_B0_path_number
                && Is_In_Current_Camera_417CC0() == CameraPos::eCamCurrent_0)
            {
                field_122_bDontFollowAbe = 1;
                field_114_respond_timer = gnFrameCount_507670 + 14;
                return 5;
            }

            if (FP_Abs(xd) > (kGridSize * FP_FromInteger(3)) && VOnSameYLevel(sActiveHero_507678))
            {
                if (xd < FP_FromInteger(0))
                {
                    if (!Check_IsOnEndOfLine_4021A0(1, 1))
                    {
                        if (!WallHit_401930(field_BC_sprite_scale * FP_FromInteger(25), -kGridSize))
                        {
                            field_FE_next_motion = eElumMotions::Motion_3_WalkLoop_412C90;
                            return 1;
                        }
                    }
                }
                else if (xd > FP_FromInteger(0))
                {
                    if (!Check_IsOnEndOfLine_4021A0(0, 1))
                    {
                        if (!WallHit_401930(field_BC_sprite_scale * FP_FromInteger(25), kGridSize))
                        {
                            field_FE_next_motion = eElumMotions::Motion_3_WalkLoop_412C90;
                            return 1;
                        }
                    }
                }
            }
            return field_12A_brain_sub_state;
        }

        case 3:
            if (field_FC_current_motion == eElumMotions::Motion_3_WalkLoop_412C90)
            {
                field_FE_next_motion = eElumMotions::Motion_1_Idle_412990;
            }

            if (field_FC_current_motion != eElumMotions::Motion_1_Idle_412990)
            {
                return field_12A_brain_sub_state;
            }

            if (field_A8_xpos == sActiveHero_507678->field_A8_xpos)
            {
                return 2;
            }

            field_FE_next_motion = eElumMotions::Motion_3_WalkLoop_412C90;
            return field_12A_brain_sub_state;

        case 4:
            if (field_FC_current_motion != eElumMotions::Motion_4_Turn_4140F0 || !field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_12A_brain_sub_state;
            }
            return 2;

        case 5:
            if (static_cast<s32>(gnFrameCount_507670) < field_114_respond_timer)
            {
                return field_12A_brain_sub_state;
            }
            field_FE_next_motion = eElumMotions::Motion_16_Speak_414980;
            return 6;

        case 6:
        {
            if (NearHoney_411DA0())
            {
                field_128_brain_idx = 1;
                field_122_bDontFollowAbe = 1;
                return 0;
            }

            const FP xd_1 = sActiveHero_507678->field_A8_xpos - field_A8_xpos;
            if (xd_1 > (kGridSize / FP_FromInteger(2)) && field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_FE_next_motion = eElumMotions::Motion_4_Turn_4140F0;
                return 7;
            }
            else if (xd_1 < -(kGridSize / FP_FromInteger(2)) && !field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_FE_next_motion = eElumMotions::Motion_4_Turn_4140F0;
                return 7;
            }

            if (sActiveHero_507678->field_FC_current_motion == eAbeMotions::Motion_139_ElumMountBegin_42E090)
            {
                field_FC_current_motion = eElumMotions::Motion_48_AbeMoutingBegin_415C40;
                return 16;
            }

            if (last_event_v2 == GameSpeakEvents::eFollowMe_10 && gMap_507BA8.field_0_current_level == field_B2_lvl_number && gMap_507BA8.field_2_current_path == field_B0_path_number)
            {
                if (Is_In_Current_Camera_417CC0() == CameraPos::eCamCurrent_0)
                {
                    field_122_bDontFollowAbe = 0;
                    if (FP_Abs(xd_1) < (kGridSize / FP_FromInteger(2)))
                    {
                        return 2;
                    }

                    field_114_respond_timer = gnFrameCount_507670 + 14;
                    return 12;
                }
            }

            if (last_event_v2 == GameSpeakEvents::eWait_12
                && gMap_507BA8.field_0_current_level == field_B2_lvl_number
                && gMap_507BA8.field_2_current_path == field_B0_path_number)
            {
                if (Is_In_Current_Camera_417CC0() == CameraPos::eCamCurrent_0)
                {
                    field_122_bDontFollowAbe = 1;
                    field_114_respond_timer = gnFrameCount_507670 + 14;
                    return 5;
                }
            }

            if (last_event_v2 == GameSpeakEvents::eHello_9
                && gMap_507BA8.field_0_current_level == field_B2_lvl_number
                && gMap_507BA8.field_2_current_path == field_B0_path_number)
            {
                if (Is_In_Current_Camera_417CC0() == CameraPos::eCamCurrent_0)
                {
                    field_114_respond_timer = gnFrameCount_507670 + 14;
                    return 8;
                }
            }

            if (last_event_v2 == GameSpeakEvents::eFart_3
                && gMap_507BA8.field_0_current_level == field_B2_lvl_number
                && gMap_507BA8.field_2_current_path == field_B0_path_number
                && Is_In_Current_Camera_417CC0() == CameraPos::eCamCurrent_0)
            {
                field_114_respond_timer = gnFrameCount_507670 + 28;
                return 10;
            }

            if (Event_Get_417250(kEventDeathReset_4) == 0)
            {
                return field_12A_brain_sub_state;
            }
            field_FE_next_motion = eElumMotions::Motion_1_Idle_412990;
            return 2;
        }

        case 7:
            if (field_FC_current_motion != eElumMotions::Motion_1_Idle_412990)
            {
                return field_12A_brain_sub_state;
            }
            field_122_bDontFollowAbe = 1;
            return 6;

        case 8:
            if (static_cast<s32>(gnFrameCount_507670) < field_114_respond_timer)
            {
                return field_12A_brain_sub_state;
            }
            field_FE_next_motion = eElumMotions::Motion_14_Speak_414860;
            return 6;

        case 9:
            if (static_cast<s32>(gnFrameCount_507670) < field_114_respond_timer)
            {
                return field_12A_brain_sub_state;
            }
            field_FE_next_motion = eElumMotions::Motion_14_Speak_414860;
            return 2;

        case 10:
            if (static_cast<s32>(gnFrameCount_507670) < field_114_respond_timer)
            {
                return field_12A_brain_sub_state;
            }
            field_FE_next_motion = eElumMotions::Motion_15_Speak_4148F0;
            return 6;

        case 11:
            if (static_cast<s32>(gnFrameCount_507670) < field_114_respond_timer)
            {
                return field_12A_brain_sub_state;
            }
            field_FE_next_motion = eElumMotions::Motion_15_Speak_4148F0;
            return 2;

        case 12:
            if (static_cast<s32>(gnFrameCount_507670) < field_114_respond_timer)
            {
                return field_12A_brain_sub_state;
            }
            field_FE_next_motion = eElumMotions::Motion_14_Speak_414860;
            return 13;

        case 13:
        {
            if (field_FC_current_motion == eElumMotions::Motion_1_Idle_412990)
            {
                field_FE_next_motion = eElumMotions::Motion_14_Speak_414860;
            }

            if (field_FC_current_motion != eElumMotions::Motion_14_Speak_414860 || !field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_12A_brain_sub_state;
            }

            const FP xd = sActiveHero_507678->field_A8_xpos - field_A8_xpos;
            if (xd >= FP_FromInteger(0))
            {
                if (xd >= (kGridSize / FP_FromInteger(2)))
                {
                    field_FE_next_motion = eElumMotions::Motion_3_WalkLoop_412C90;
                    return 14;
                }
            }
            else
            {
                if (-xd >= (kGridSize / FP_FromInteger(2)))
                {
                    field_FE_next_motion = eElumMotions::Motion_3_WalkLoop_412C90;
                    return 14;
                }
            }
            field_FE_next_motion = eElumMotions::Motion_1_Idle_412990;
            return 2;
        }

        case 14:
        {
            if (last_event_v2 == GameSpeakEvents::eWait_12)
            {
                field_122_bDontFollowAbe = 1;
                field_FE_next_motion = eElumMotions::Motion_1_Idle_412990;
                field_114_respond_timer = gnFrameCount_507670 + 14;
                return 5;
            }

            const FP xd = sActiveHero_507678->field_A8_xpos - field_A8_xpos;
            if (FP_Abs(xd) < (kGridSize / FP_FromInteger(2)))
            {
                field_FE_next_motion = eElumMotions::Motion_1_Idle_412990;
                return 2;
            }

            if (xd > FP_FromInteger(0))
            {
                if (field_B4_velx < FP_FromInteger(0))
                {
                    field_FE_next_motion = eElumMotions::Motion_4_Turn_4140F0;
                    return 15;
                }
            }
            else if (xd < FP_FromInteger(0))
            {
                if (field_B4_velx > FP_FromInteger(0))
                {
                    field_FE_next_motion = eElumMotions::Motion_4_Turn_4140F0;
                    return 15;
                }
            }

            if (field_B4_velx > FP_FromInteger(0))
            {
                if (Check_IsOnEndOfLine_4021A0(0, 1))
                {
                    field_FE_next_motion = eElumMotions::Motion_1_Idle_412990;
                    return 2;
                }
            }

            if (field_B4_velx < FP_FromInteger(0))
            {
                if (Check_IsOnEndOfLine_4021A0(1, 1))
                {
                    field_FE_next_motion = eElumMotions::Motion_1_Idle_412990;
                    return 2;
                }
            }
            return field_12A_brain_sub_state;
        }

        case 15:
        {
            if (field_FC_current_motion != eElumMotions::Motion_4_Turn_4140F0 || !field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_12A_brain_sub_state;
            }

            if (FP_Abs(sActiveHero_507678->field_A8_xpos - field_A8_xpos) < (kGridSize / FP_FromInteger(2)))
            {
                field_FE_next_motion = eElumMotions::Motion_1_Idle_412990;
                return 2;
            }

            field_FE_next_motion = eElumMotions::Motion_3_WalkLoop_412C90;
            return 14;
        }

        case 16:
            if (NearHoney_411DA0())
            {
                field_128_brain_idx = 1;
                field_122_bDontFollowAbe = 1;
                return 0;
            }

            if (sControlledCharacter_50767C == this
                || field_FC_current_motion == eElumMotions::Motion_48_AbeMoutingBegin_415C40
                || field_FC_current_motion == eElumMotions::Motion_27_AbeMountingEnd_415CA0)
            {
                return field_12A_brain_sub_state;
            }
            field_122_bDontFollowAbe = 0;
            return 2;

        default:
            return field_12A_brain_sub_state;
    }
}

s16 Elum::Brain_1_HoneyAddiction_411730()
{
    const FP kGridSize = ScaleToGridSize_41FA30(field_BC_sprite_scale);

    switch (field_12A_brain_sub_state)
    {
        case 0:
        {
            if (field_170_flags.Get(Elum::Flags_170::eStrugglingWithBees_Bit1))
            {
                field_170_flags.Clear(Elum::Flags_170::eFoundHoney_Bit4);
                field_FE_next_motion = eElumMotions::Motion_29_BeesStruggling_412A90;
                return 5;
            }

            const FP honey_xd = field_A8_xpos - FP_FromInteger(field_12C_honey_xpos);
            if (honey_xd >= FP_FromInteger(0))
            {
                if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    if (honey_xd >= (kGridSize * FP_FromInteger(2)))
                    {
                        field_FE_next_motion = eElumMotions::Motion_3_WalkLoop_412C90;
                        Elum_SFX_416E10(ElumSounds::eSpottedHoney_6, 0);
                        return 2;
                    }
                    else
                    {
                        Elum_SFX_416E10(ElumSounds::eSpottedHoney_6, 0);
                        field_FE_next_motion = eElumMotions::Motion_25_LickingHoney_415B50;
                        if (sControlledCharacter_50767C == this)
                        {
                            SetAbeAsPlayer_412520(eAbeMotions::Motion_128_KnockForward_429330);
                        }
                        return 3;
                    }
                }
                field_FE_next_motion = eElumMotions::Motion_4_Turn_4140F0;
                return 1;
            }
            else
            {
                if (!field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    if (honey_xd <= -(kGridSize * FP_FromInteger(2)))
                    {
                        field_FE_next_motion = eElumMotions::Motion_3_WalkLoop_412C90;
                        Elum_SFX_416E10(ElumSounds::eSpottedHoney_6, 0);
                        return 2;
                    }
                    else
                    {
                        Elum_SFX_416E10(ElumSounds::eSpottedHoney_6, 0);
                        field_FE_next_motion = eElumMotions::Motion_25_LickingHoney_415B50;
                        if (sControlledCharacter_50767C == this)
                        {
                            SetAbeAsPlayer_412520(eAbeMotions::Motion_128_KnockForward_429330);
                        }
                        return 3;
                    }
                }
                field_FE_next_motion = eElumMotions::Motion_4_Turn_4140F0;
                return 1;
            }
            break;
        }

        case 1:
        {
            if (field_FC_current_motion != eElumMotions::Motion_4_Turn_4140F0 || !field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_12A_brain_sub_state;
            }

            const FP honey_xd_1 = field_A8_xpos - FP_FromInteger(field_12C_honey_xpos);

            if (honey_xd_1 >= FP_FromInteger(0))
            {
                if (honey_xd_1 < (kGridSize * FP_FromInteger(2)))
                {
                    Elum_SFX_416E10(ElumSounds::eSpottedHoney_6, 0);
                    field_FE_next_motion = eElumMotions::Motion_25_LickingHoney_415B50;
                    if (sControlledCharacter_50767C == this)
                    {
                        SetAbeAsPlayer_412520(eAbeMotions::Motion_128_KnockForward_429330);
                    }
                    return 3;
                }
            }
            else
            {
                if (honey_xd_1 > -(kGridSize * FP_FromInteger(2)))
                {
                    Elum_SFX_416E10(ElumSounds::eSpottedHoney_6, 0);
                    field_FE_next_motion = eElumMotions::Motion_25_LickingHoney_415B50;
                    if (sControlledCharacter_50767C == this)
                    {
                        SetAbeAsPlayer_412520(eAbeMotions::Motion_128_KnockForward_429330);
                    }
                    return 3;
                }
            }
            field_FE_next_motion = eElumMotions::Motion_3_WalkLoop_412C90;
            Elum_SFX_416E10(ElumSounds::eSpottedHoney_6, 0);
            return 2;
        }

        case 2:
            if (!NearHoney_411DA0())
            {
                field_FE_next_motion = eElumMotions::Motion_1_Idle_412990;
                field_128_brain_idx = 0;
                return 0;
            }

            if ((field_B4_velx <= FP_FromInteger(0) && field_A8_xpos - FP_FromInteger(field_12C_honey_xpos) >= (kGridSize * FP_FromInteger(2))) ||
                (field_B4_velx >= FP_FromInteger(0) && FP_FromInteger(field_12C_honey_xpos) - field_A8_xpos >= (kGridSize * FP_FromInteger(2))))
            {
                return field_12A_brain_sub_state;
            }

            if (sControlledCharacter_50767C == this)
            {
                SetAbeAsPlayer_412520(eAbeMotions::Motion_128_KnockForward_429330);
            }

            field_FE_next_motion = eElumMotions::Motion_25_LickingHoney_415B50;
            return 3;

        case 3:
            if (!field_F4_pLine)
            {
                field_FE_next_motion = eElumMotions::Motion_1_Idle_412990;
                field_128_brain_idx = 0;
                return 0;
            }

            if (!field_170_flags.Get(Elum::Flags_170::eStrugglingWithBees_Bit1))
            {
                return field_12A_brain_sub_state;
            }

            field_170_flags.Clear(Elum::Flags_170::eFoundHoney_Bit4);
            field_170_flags.Set(Elum::Flags_170::eStungByBees_Bit2);

            field_146_honey_ypos = gMap_507BA8.field_4_current_camera;

            field_122_bDontFollowAbe = 0;

            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_FE_next_motion = eElumMotions::Motion_4_Turn_4140F0;
                return 4;
            }
            else
            {
                field_FE_next_motion = eElumMotions::Motion_29_BeesStruggling_412A90;

                field_170_flags.Clear(Elum::Flags_170::eStrugglingWithBees_Bit1);
                return 5;
            }
            break;

        case 4:
            if (field_FC_current_motion == eElumMotions::Motion_4_Turn_4140F0 && field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_170_flags.Clear(Elum::Flags_170::eStrugglingWithBees_Bit1);
                field_FE_next_motion = eElumMotions::Motion_29_BeesStruggling_412A90;
                return 5;
            }
            return field_12A_brain_sub_state;

        case 5:
            if (field_FC_current_motion != eElumMotions::Motion_29_BeesStruggling_412A90 || field_110_timer > static_cast<s32>(gnFrameCount_507670))
            {
                return field_12A_brain_sub_state;
            }
            field_FE_next_motion = eElumMotions::Motion_44_ScratchBegin_412730;
            return 6;

        case 6:
        {
            if (field_FC_current_motion != eElumMotions::Motion_46_ScratchEnd_412800 || !field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_12A_brain_sub_state;
            }

            if (!field_170_flags.Get(Elum::Flags_170::eStrugglingWithBees_Bit1))
            {
                field_FE_next_motion = eElumMotions::Motion_1_Idle_412990;
                field_128_brain_idx = 0;
                return 0;
            }

            if (!field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                if (Check_IsOnEndOfLine_4021A0(0, 2))
                {
                    field_170_flags.Clear(Elum::Flags_170::eStrugglingWithBees_Bit1);
                    field_FE_next_motion = eElumMotions::Motion_29_BeesStruggling_412A90;
                    return 5;
                }
            }
            else
            {
                if (Check_IsOnEndOfLine_4021A0(1, 2))
                {
                    field_170_flags.Clear(Elum::Flags_170::eStrugglingWithBees_Bit1);
                    field_FE_next_motion = eElumMotions::Motion_29_BeesStruggling_412A90;
                    return 5;
                }
            }

            FP gridSizeDirected = {};
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                gridSizeDirected = -kGridSize;
            }
            else
            {
                gridSizeDirected = kGridSize;
            }

            if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(25), gridSizeDirected))
            {
                field_170_flags.Clear(Elum::Flags_170::eStrugglingWithBees_Bit1);
                field_FE_next_motion = eElumMotions::Motion_29_BeesStruggling_412A90;
                return 5;
            }
            else
            {
                field_170_flags.Clear(Elum::Flags_170::eStrugglingWithBees_Bit1);
                field_FE_next_motion = eElumMotions::Motion_3_WalkLoop_412C90;
                return 7;
            }
            break;
        }

        case 7:
            if (field_B4_velx > FP_FromInteger(0))
            {
                if (Check_IsOnEndOfLine_4021A0(0, 1))
                {
                    field_FE_next_motion = eElumMotions::Motion_29_BeesStruggling_412A90;
                    field_110_timer = gnFrameCount_507670 + 40;
                    return 5;
                }
            }

            if (field_B4_velx < FP_FromInteger(0))
            {
                if (Check_IsOnEndOfLine_4021A0(1, 1))
                {
                    field_FE_next_motion = eElumMotions::Motion_29_BeesStruggling_412A90;
                    field_110_timer = gnFrameCount_507670 + 40;
                    return 5;
                }
            }

            if (field_FC_current_motion == eElumMotions::Motion_3_WalkLoop_412C90 && field_10_anim.field_92_current_frame == 11)
            {
                field_FE_next_motion = eElumMotions::Motion_44_ScratchBegin_412730;
                return 9;
            }

            if (field_FC_current_motion != eElumMotions::Motion_1_Idle_412990)
            {
                return field_12A_brain_sub_state;
            }

            field_FE_next_motion = eElumMotions::Motion_29_BeesStruggling_412A90;
            field_110_timer = gnFrameCount_507670 + 40;
            return 5;

        case 8:
            if (field_110_timer > static_cast<s32>(gnFrameCount_507670))
            {
                return field_12A_brain_sub_state;
            }

            if (!field_170_flags.Get(Elum::Flags_170::eStrugglingWithBees_Bit1))
            {
                field_FE_next_motion = eElumMotions::Motion_1_Idle_412990;
                field_128_brain_idx = 0;
                return 0;
            }

            if (!field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                if (Check_IsOnEndOfLine_4021A0(0, 2))
                {
                    field_170_flags.Clear(Elum::Flags_170::eStrugglingWithBees_Bit1);
                    field_FE_next_motion = eElumMotions::Motion_44_ScratchBegin_412730;
                    return 9;
                }
            }
            else
            {
                if (Check_IsOnEndOfLine_4021A0(1, 2))
                {
                    field_170_flags.Clear(Elum::Flags_170::eStrugglingWithBees_Bit1);
                    field_FE_next_motion = eElumMotions::Motion_44_ScratchBegin_412730;
                    return 9;
                }
            }
            field_170_flags.Clear(Elum::Flags_170::eStrugglingWithBees_Bit1);
            field_FE_next_motion = eElumMotions::Motion_3_WalkLoop_412C90;
            return 10;

        case 9:
            if (field_FC_current_motion != eElumMotions::Motion_46_ScratchEnd_412800 || !field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_12A_brain_sub_state;
            }
            field_FE_next_motion = eElumMotions::Motion_29_BeesStruggling_412A90;
            return 8;

        case 10:
            if (field_B4_velx > FP_FromInteger(0))
            {
                if (Check_IsOnEndOfLine_4021A0(0, 1))
                {
                    field_FE_next_motion = eElumMotions::Motion_29_BeesStruggling_412A90;
                    field_110_timer = gnFrameCount_507670 + 40;
                    return 8;
                }
            }
            else if (field_B4_velx < FP_FromInteger(0))
            {
                if (Check_IsOnEndOfLine_4021A0(1, 1))
                {
                    field_FE_next_motion = eElumMotions::Motion_29_BeesStruggling_412A90;
                    field_110_timer = gnFrameCount_507670 + 40;
                    return 8;
                }
            }

            if (field_FC_current_motion != eElumMotions::Motion_3_WalkLoop_412C90 || field_10_anim.field_92_current_frame != 11)
            {
                if (field_FC_current_motion == eElumMotions::Motion_1_Idle_412990)
                {
                    field_FE_next_motion = eElumMotions::Motion_29_BeesStruggling_412A90;
                    field_110_timer = gnFrameCount_507670 + 40;
                    return 8;
                }
                else
                {
                    return field_12A_brain_sub_state;
                }
            }

            field_FE_next_motion = eElumMotions::Motion_29_BeesStruggling_412A90;
            return 5;

        default:
            return field_12A_brain_sub_state;
    }
}

void Elum::Motion_0_Respawn_414C60()
{
    Motion_21_Land_414A20();
}

void Elum::Motion_1_Idle_412990()
{
    CheckLiftPointGoneAndSetCamera();

    if (sActiveHero_507678->field_FC_current_motion != eAbeMotions::Motion_115_ElumSpeak_4299F0 && !ToNextMotion_4120F0())
    {
        if (sActiveHero_507678->field_FC_current_motion == eAbeMotions::Motion_150_Chant_42FD50 && sControlledCharacter_50767C == sActiveHero_507678)
        {
            field_FC_current_motion = eElumMotions::Motion_9_ToYell_415890;
        }
        else if (sActiveHero_507678->field_FC_current_motion == eAbeMotions::Motion_137_ElumUnmountBegin_42E2B0)
        {
            ResourceManager::FreeResource_455550(field_174_resources.res[20]);
            field_174_resources.res[20] = nullptr;
            field_FC_current_motion = eElumMotions::Motion_49_AbeUnmountingBegin_415D00;
        }
        else if (gnFrameCount_507670 - field_110_timer > 200 && sControlledCharacter_50767C != this)
        {
            if (field_174_resources.res[30])
            {
                field_FC_current_motion = eElumMotions::Motion_44_ScratchBegin_412730;
            }
            else
            {
                field_174_resources.res[30] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kElmaloneAOResID_230, 1, 0);
            }
        }
    }
}

void Elum::Motion_2_Unknown_412C30()
{
    if (field_104_pending_resource_count == 0)
    {
        VFreeMountedResources_411200();
        VLoadUnmountedResources_411260();

        ToIdle();
    }
}

void Elum::Motion_3_WalkLoop_412C90()
{
    field_10E_pressed |= Input().Pressed();

    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);

    MoveOnLine_412580(0);

    if (field_FC_current_motion == eElumMotions::Motion_3_WalkLoop_412C90)
    {
        if (field_10_anim.field_92_current_frame == 2)
        {
            if (field_FE_next_motion == eElumMotions::Motion_25_LickingHoney_415B50)
            {
                field_FC_current_motion = eElumMotions::Motion_6_MidWalkToIdle_4133F0;
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
                    field_FC_current_motion = eElumMotions::Motion_6_MidWalkToIdle_4133F0;
                }
                else if (sControlledCharacter_50767C == this && !field_170_flags.Get(Elum::Flags_170::eFoundHoney_Bit4))
                {
                    MidWalkToNextMotion_412FA0();
                }
                else if (field_104_pending_resource_count > 0)
                {
                    field_FC_current_motion = eElumMotions::Motion_6_MidWalkToIdle_4133F0;
                }
                else
                {
                    if (field_FE_next_motion == eElumMotions::Motion_1_Idle_412990)
                    {
                        field_FC_current_motion = eElumMotions::Motion_6_MidWalkToIdle_4133F0;
                        field_FE_next_motion = -1;
                    }
                    else if (field_FE_next_motion == eElumMotions::Motion_4_Turn_4140F0)
                    {
                        field_FC_current_motion = eElumMotions::Motion_6_MidWalkToIdle_4133F0;
                    }
                }
            }
        }
        else if (field_10_anim.field_92_current_frame == 5)
        {
            if (!field_10C_bFootStep2)
            {
                field_10C_bFootStep2 = 1;
                MapFollowMe_401D30(1);
            }

            if (sControlledCharacter_50767C != this)
            {
                Elum_SFX_416E10(ElumSounds::eWalkingFootstep_0, 0);
                return;
            }

            if (Input().IsAnyPressed(sInputKey_Run_4C65A8))
            {
                field_FC_current_motion = eElumMotions::Motion_41_MidWalkToRun_413560;
            }

            field_10E_pressed = 0;
            Elum_SFX_416E10(ElumSounds::eWalkingFootstep_0, 0);
        }
        else if (field_10_anim.field_92_current_frame == 11)
        {
            if (field_FE_next_motion == eElumMotions::Motion_25_LickingHoney_415B50)
            {
                field_FC_current_motion = eElumMotions::Motion_6_MidWalkToIdle_4133F0;
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
                    field_FC_current_motion = eElumMotions::Motion_5_WalkToIdle_4132D0;
                }
                else if (sControlledCharacter_50767C == this && !field_170_flags.Get(Elum::Flags_170::eFoundHoney_Bit4))
                {
                    WalkToNextMotion_4130D0();
                }
                else if (field_104_pending_resource_count)
                {
                    field_FC_current_motion = eElumMotions::Motion_5_WalkToIdle_4132D0;
                }
                else
                {
                    if (field_FE_next_motion == eElumMotions::Motion_1_Idle_412990)
                    {
                        field_FC_current_motion = eElumMotions::Motion_5_WalkToIdle_4132D0;
                        field_FE_next_motion = -1;
                    }
                    else if (field_FE_next_motion == eElumMotions::Motion_4_Turn_4140F0
                             || field_FE_next_motion == eElumMotions::Motion_44_ScratchBegin_412730
                             || field_FE_next_motion == eElumMotions::Motion_29_BeesStruggling_412A90)
                    {
                        field_FC_current_motion = eElumMotions::Motion_5_WalkToIdle_4132D0;
                    }
                }
            }
        }
        else if (field_10_anim.field_92_current_frame == 14)
        {
            if (!field_10C_bFootStep2)
            {
                field_10C_bFootStep2 = 1;
                MapFollowMe_401D30(1);
            }

            if (sControlledCharacter_50767C != this)
            {
                Elum_SFX_416E10(ElumSounds::eWalkingFootstep_0, 0);
                return;
            }

            if (Input().IsAnyPressed(sInputKey_Run_4C65A8))
            {
                field_FC_current_motion = eElumMotions::Motion_40_WalkToRun_4134B0;
            }

            field_10E_pressed = 0;
            Elum_SFX_416E10(ElumSounds::eWalkingFootstep_0, 0);
        }
        else
        {
            field_10C_bFootStep2 = 0;
        }
    }
}

void Elum::Motion_4_Turn_4140F0()
{
    CheckLiftPointGoneAndSetCamera();

    if (!field_10_anim.field_92_current_frame)
    {
        Environment_SFX_42A220(EnvironmentSfx::eRollingNoise_8, 0, 0x7FFF, this);
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        MapFollowMe_401D30(TRUE);
        field_10_anim.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);

        if (field_FE_next_motion == eElumMotions::Motion_29_BeesStruggling_412A90)
        {
            Elum_SFX_416E10(ElumSounds::eBeesStruggle_3, 0);
            field_FC_current_motion = eElumMotions::Motion_29_BeesStruggling_412A90;
            field_FE_next_motion = -1;
            field_110_timer = gnFrameCount_507670 + 25;
        }
        else if (ToNextMotion_4120F0())
        {
            field_10_anim.Set_Animation_Data_402A40(field_10_anim.field_18_frame_table_offset, 0);
            if (sControlledCharacter_50767C == this)
            {
                sActiveHero_507678->SyncToElum_42D850(field_FC_current_motion);
            }
        }
        else
        {
            ToIdle();
        }
    }
}

void Elum::Motion_5_WalkToIdle_4132D0()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);

    MoveOnLine_412580(0);

    if (field_10_anim.field_92_current_frame == 0)
    {
        Elum_SFX_416E10(ElumSounds::eWalkingFootstep_0, 0);
    }
    else if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        MapFollowMe_401D30(1);

        if (field_FE_next_motion == eElumMotions::Motion_29_BeesStruggling_412A90)
        {
            Elum_SFX_416E10(ElumSounds::eBeesStruggle_3, 0);
            field_FC_current_motion = eElumMotions::Motion_29_BeesStruggling_412A90;
            field_110_timer = gnFrameCount_507670 + 25;
            field_FE_next_motion = -1;
        }
        else if (field_FE_next_motion == eElumMotions::Motion_44_ScratchBegin_412730)
        {
            field_FC_current_motion = eElumMotions::Motion_44_ScratchBegin_412730;
            field_FE_next_motion = -1;
        }
        else
        {
            ToIdle();

            if (field_104_pending_resource_count > 0)
            {
                field_FC_current_motion = eElumMotions::Motion_2_Unknown_412C30;
            }
            else
            {
                if (field_FE_next_motion == eElumMotions::Motion_4_Turn_4140F0 || field_FE_next_motion == eElumMotions::Motion_25_LickingHoney_415B50)
                {
                    field_FC_current_motion = field_FE_next_motion;
                    field_FE_next_motion = -1;
                }
            }
        }
    }
}

void Elum::Motion_6_MidWalkToIdle_4133F0()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);

    MoveOnLine_412580(0);

    if (field_10_anim.field_92_current_frame == 0)
    {
        Elum_SFX_416E10(ElumSounds::eWalkingFootstep_0, 0);
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        MapFollowMe_401D30(1);

        ToIdle();

        if (field_104_pending_resource_count)
        {
            field_FC_current_motion = eElumMotions::Motion_2_Unknown_412C30;
        }
        else
        {
            if (field_FE_next_motion == eElumMotions::Motion_4_Turn_4140F0 || field_FE_next_motion == eElumMotions::Motion_25_LickingHoney_415B50)
            {
                field_FC_current_motion = field_FE_next_motion;
                field_FE_next_motion = -1;
            }
        }
    }
}

void Elum::Motion_7_IdleToWalk2_413200()
{
    field_10E_pressed |= Input().Pressed();

    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_E4_previous_motion = 3;
        field_E6_last_anim_frame = eElumMotions::Motion_9_ToYell_415890;
        field_120_bUnknown = 1;
    }

    MoveOnLine_412580(0);
}

void Elum::Motion_8_IdleToWalk1_413270()
{
    field_10E_pressed |= Input().Pressed();

    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eElumMotions::Motion_3_WalkLoop_412C90;
    }

    MoveOnLine_412580(0);
}

void Elum::Motion_9_ToYell_415890()
{
    CheckLiftPointGoneAndSetCamera();

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eElumMotions::Motion_10_Yell_4158E0;
    }
}

void Elum::Motion_10_Yell_4158E0()
{
    Event_Broadcast_417220(kEventNoise_0, this);

    CheckLiftPointGoneAndSetCamera();

    if (field_10_anim.field_92_current_frame || !field_170_flags.Get(Elum::Flags_170::eCanSpeak_Bit6))
    {
        field_170_flags.Set(Elum::Flags_170::eCanSpeak_Bit6);
    }
    else
    {
        Elum_SFX_416E10(ElumSounds::eHowl_2, this);
        field_170_flags.Clear(Elum::Flags_170::eCanSpeak_Bit6);
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (!ToNextMotion_4120F0())
        {
            ToIdle();
        }
    }
}

void Elum::Motion_11_Unknown_4159A0()
{
    Motion_47_Unknown_415A30();
}

void Elum::Motion_12_RunTurn_414520()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);

    if (!field_10_anim.field_92_current_frame)
    {
        Elum_SFX_416E10(ElumSounds::eRunSlide_5, 0);
    }

    const FP offY = (sControlledCharacter_50767C == this) ? field_BC_sprite_scale * FP_FromInteger(40) : field_BC_sprite_scale * FP_FromInteger(25);
    if (WallHit_401930(offY, field_B4_velx))
    {
        field_FC_current_motion = eElumMotions::Motion_50_Knockback_415DC0;
        if (field_F4_pLine)
        {
            field_B4_velx = -field_B4_velx;
            MoveOnLine_412580(0);
        }
        else
        {
            field_A8_xpos -= field_B4_velx;
        }
        field_B4_velx = FP_FromInteger(0);
        MapFollowMe_401D30(TRUE);
        Environment_SFX_42A220(EnvironmentSfx::eKnockback_13, 95, -200, this);
        return;
    }

    SlowOnX_414210(FP_FromDouble(2.125));

    if (field_FC_current_motion == eElumMotions::Motion_12_RunTurn_414520
        && field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        MapFollowMe_401D30(TRUE);

        const FP gridSize = (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX)) ? ScaleToGridSize_41FA30(field_BC_sprite_scale) : -ScaleToGridSize_41FA30(field_BC_sprite_scale);
        if (Input().IsAnyPressed(sInputKey_Run_4C65A8))
        {
            field_B4_velx = gridSize / FP_FromInteger(4);
            field_FC_current_motion = eElumMotions::Motion_38_RunTurnToRun_414810;
            return;
        }
        field_B4_velx = gridSize / FP_FromInteger(9);
        field_FC_current_motion = eElumMotions::Motion_13_RunTurnToWalk_4147C0;
    }
}

void Elum::Motion_13_RunTurnToWalk_4147C0()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);

    MoveOnLine_412580(0);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eElumMotions::Motion_3_WalkLoop_412C90;
        field_10_anim.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
    }
}

void Elum::Motion_14_Speak_414860()
{
    if (field_10_anim.field_92_current_frame == 3 && field_170_flags.Get(Elum::Flags_170::eCanSpeak_Bit6))
    {
        Environment_SFX_42A220(EnvironmentSfx::eElumOkay_21, 75, 0, this);
        field_170_flags.Clear(Elum::Flags_170::eCanSpeak_Bit6);
    }
    else
    {
        field_170_flags.Set(Elum::Flags_170::eCanSpeak_Bit6);
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (!ToNextMotion_4120F0())
        {
            ToIdle();
        }
    }
}

void Elum::Motion_15_Speak_4148F0()
{
    if (field_10_anim.field_92_current_frame == 3 && field_170_flags.Get(Elum::Flags_170::eCanSpeak_Bit6))
    {
        Environment_SFX_42A220(EnvironmentSfx::eElumHowl_20, 75, 0, this);
        field_170_flags.Clear(Elum::Flags_170::eCanSpeak_Bit6);
    }
    else
    {
        field_170_flags.Set(Elum::Flags_170::eCanSpeak_Bit6);
    }
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (!ToNextMotion_4120F0())
        {
            ToIdle();
        }
    }
}

void Elum::Motion_16_Speak_414980()
{
    if (field_10_anim.field_92_current_frame == 3 && field_170_flags.Get(Elum::Flags_170::eCanSpeak_Bit6))
    {
        Environment_SFX_42A220(EnvironmentSfx::eElumHowl_20, 75, 0, this);
        field_170_flags.Clear(Elum::Flags_170::eCanSpeak_Bit6);
    }
    else
    {
        field_170_flags.Set(Elum::Flags_170::eCanSpeak_Bit6);
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FE_next_motion = eElumMotions::Motion_10_Yell_4158E0;
        if (!ToNextMotion_4120F0())
        {
            ToIdle();
        }
    }
}

void Elum::Motion_17_Unknown_413620()
{
    MoveOnLine_412580(0);

    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);

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

        field_FC_current_motion = eElumMotions::Motion_30_HopBegin_414E30;
    }
}

void Elum::Motion_18_Unknown_4136A0()
{
    Motion_17_Unknown_413620();
}

void Elum::Motion_19_Dead_415F90()
{
    if (sActiveHero_507678->field_100_health > FP_FromInteger(0))
    {
        if (!sActiveHero_507678->field_2A8_flags.Get(Flags_2A8::e2A8_Bit6_bShrivel) && sActiveHero_507678->field_2A8_flags.Get(Flags_2A8::e2A8_Bit8_bLandSoft) && field_104_pending_resource_count == 0)
        {
            field_A8_xpos = FP_FromInteger(field_138_continue_rect.x);
            field_AC_ypos = FP_FromInteger(field_138_continue_rect.y);

            field_B8_vely = FP_FromInteger(0);
            field_B4_velx = FP_FromInteger(0);

            field_B2_lvl_number = field_14A_continue_level;
            field_B0_path_number = field_148_continue_path;
            field_BC_sprite_scale = field_150_continue_sprite_scale;

            field_128_brain_idx = 0;
            field_12A_brain_sub_state = 6;
            field_122_bDontFollowAbe = 1;

            if (!gMap_507BA8.Is_Point_In_Current_Camera_4449C0(field_14A_continue_level, field_148_continue_path, field_A8_xpos, field_AC_ypos, 0))
            {
                Elum_SFX_416E10(ElumSounds::eHowl_2, this);
            }

            field_170_flags.Clear(Elum::Flags_170::eStrugglingWithBees_Bit1);
            field_170_flags.Clear(Elum::Flags_170::eFoundHoney_Bit4);
            field_110_timer = gnFrameCount_507670;

            field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX, sActiveHero_507678->field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX));

            if (field_170_flags.Get(Elum::Flags_170::eStungByBees_Bit2))
            {
                if (field_14C_continue_camera < field_146_honey_ypos)
                {
                    field_170_flags.Clear(Elum::Flags_170::eStungByBees_Bit2);
                }
            }

            field_10_anim.field_4_flags.Set(AnimFlags::eBit3_Render);

            field_E8_LastLineYPos = field_AC_ypos;
            field_FC_current_motion = 0;
            field_FE_next_motion = -1;
            field_F4_pLine = nullptr;
            field_100_health = FP_FromInteger(1);

            if (field_174_resources.res[20])
            {
                ResourceManager::FreeResource_455550(field_174_resources.res[20]);
                field_174_resources.res[20] = nullptr;
            }

            if (!field_174_resources.res[30])
            {
                VLoadUnmountedResources_411260();
            }
        }
    }
}

void Elum::Motion_20_Fall_415F70()
{
    if (sActiveHero_507678->field_100_health <= FP_FromInteger(0))
    {
        field_FC_current_motion = eElumMotions::Motion_19_Dead_415F90;
    }
}

void Elum::Motion_21_Land_414A20()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);

    if (field_B4_velx > FP_FromInteger(0))
    {
        field_B4_velx -= (field_BC_sprite_scale * field_118_jump_velx);
        if (field_B4_velx < FP_FromInteger(0))
        {
            field_B4_velx = FP_FromInteger(0);
        }
    }
    else if (field_B4_velx < FP_FromInteger(0))
    {
        field_B4_velx += (field_BC_sprite_scale * field_118_jump_velx);
        if (field_B4_velx > FP_FromInteger(0))
        {
            field_B4_velx = FP_FromInteger(0);
        }
    }

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    const s16 bHit = InAirCollision_4019C0(&pLine, &hitX, &hitY, FP_FromDouble(1.8));

    if (sControlledCharacter_50767C == this)
    {
        SetActiveCameraDelayedFromDir_401C90();
    }

    if (bHit)
    {
        switch (pLine->field_8_type)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eUnknown_32:
            case eLineTypes::eUnknown_36:
            {
                Elum_SFX_416E10(ElumSounds::eHitGroundSoft_4, 0);
                field_F4_pLine = pLine;
                if (field_AC_ypos - field_E8_LastLineYPos >= FP_FromInteger(20))
                {
                    field_FC_current_motion = eElumMotions::Motion_22_RunOffEdge_415810;
                }
                else
                {
                    field_FC_current_motion = eElumMotions::Motion_1_Idle_412990;
                }

                field_A8_xpos = hitX;
                field_AC_ypos = hitY;
                MapFollowMe_401D30(TRUE);

                PSX_Point xy = {};
                xy.field_0_x = FP_GetExponent(field_A8_xpos - FP_FromInteger(10));
                xy.field_2_y = FP_GetExponent(field_AC_ypos - FP_FromInteger(10));

                PSX_Point wh = {};
                wh.field_0_x = FP_GetExponent(field_A8_xpos + FP_FromInteger(10));
                wh.field_2_y = FP_GetExponent(field_AC_ypos + FP_FromInteger(10));

                VOnCollisionWith(
                    xy,
                    wh,
                    ObjListPlatforms_50766C,
                    1,
                    (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_401C10);
                break;
            }

            case eLineTypes::eWallLeft_1:
            case eLineTypes::eWallRight_2:
            case eLineTypes::eBackgroundWallLeft_5:
            case eLineTypes::eBackgroundWallRight_6:
                if (bHit)
                {
                    field_B4_velx = (-field_B4_velx / FP_FromInteger(2));
                }
                break;

            default:
                break;
        }
    }
}

void Elum::Motion_22_RunOffEdge_415810()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        MapFollowMe_401D30(1);
        if (!ToNextMotion_4120F0())
        {
            ToIdle();
        }
    }
}

void Elum::Motion_23_WalkOffEdge_415E90()
{
    Motion_24_JumpToFall_415ED0();
}

void Elum::Motion_24_JumpToFall_415ED0()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eElumMotions::Motion_21_Land_414A20;
    }

    Motion_21_Land_414A20();
}

void Elum::Motion_25_LickingHoney_415B50()
{
    Event_Broadcast_417220(kEventNoise_0, this);

    CheckLiftPointGoneAndSetCamera();

    if (field_10_anim.field_92_current_frame == 6)
    {
        if (gMap_507BA8.GetDirection(
                field_B2_lvl_number,
                field_B0_path_number,
                field_A8_xpos,
                field_AC_ypos)
            == CameraPos::eCamCurrent_0)
        {
            if (field_170_flags.Get(Elum::Flags_170::eCanSpeak_Bit6))
            {
                Elum_SFX_416E10(ElumSounds::eLickingHoney_8, 0);
                field_170_flags.Clear(Elum::Flags_170::eCanSpeak_Bit6);
            }
        }
    }
    else
    {
        field_170_flags.Set(Elum::Flags_170::eCanSpeak_Bit6);
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        if (field_FE_next_motion == eElumMotions::Motion_4_Turn_4140F0 || field_FE_next_motion == eElumMotions::Motion_29_BeesStruggling_412A90)
        {
            field_FC_current_motion = eElumMotions::Motion_26_LickingToStruggling_415AC0;
            if (!field_174_resources.res[30])
            {
                VLoadUnmountedResources_411260();
            }
        }
    }
}

void Elum::Motion_26_LickingToStruggling_415AC0()
{
    Motion_47_Unknown_415A30();
}

void Elum::Motion_27_AbeMountingEnd_415CA0()
{
    if (sActiveHero_507678->field_FC_current_motion != eAbeMotions::Motion_136_ElumMountEnd_42E110 && field_104_pending_resource_count == 0)
    {
        VLoadMountedResources_411300();
        ToIdle();
    }
}

void Elum::Motion_28_AbeUnmountingEnd_415D60()
{
    if (sActiveHero_507678->field_FC_current_motion != eAbeMotions::Motion_138_ElumUnmountEnd_42E390
        && !field_104_pending_resource_count)
    {
        VLoadUnmountedResources_411260();
        ToIdle();
    }
}

void Elum::Motion_29_BeesStruggling_412A90()
{
    Event_Broadcast_417220(kEventNoise_0, this);

    if (field_FE_next_motion == eElumMotions::Motion_44_ScratchBegin_412730)
    {
        field_FC_current_motion = eElumMotions::Motion_44_ScratchBegin_412730;
        if (!field_174_resources.res[30])
        {
            VLoadUnmountedResources_411260();
        }
        field_FE_next_motion = -1;
        return;
    }

    if (field_FE_next_motion != eElumMotions::Motion_3_WalkLoop_412C90)
    {
        if (field_FE_next_motion == eElumMotions::Motion_1_Idle_412990)
        {
            ToIdle();
            field_FE_next_motion = -1;
        }
        return;
    }

    if (WallHit_401930(
            field_BC_sprite_scale * FP_FromInteger(25),
            ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromInteger(field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX) != 0 ? -1 : 1)))
    {
        ToIdle();
        field_FE_next_motion = -1;
        return;
    }

    field_FC_current_motion = eElumMotions::Motion_8_IdleToWalk2_413270;
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_FE_next_motion = -1;
        field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
    }
    else
    {
        field_FE_next_motion = -1;
        field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
    }
}

void Elum::Motion_30_HopBegin_414E30()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);

    CheckLiftPointGoneAndSetCamera();

    const FP xpos = field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX) ? field_BC_sprite_scale * FP_FromInteger(-56) : field_BC_sprite_scale * FP_FromInteger(56);
    const FP velX = field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX) ? field_BC_sprite_scale * FP_FromDouble(-9.85) : field_BC_sprite_scale * FP_FromDouble(9.85);
    const FP offX = field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX) ? FP_FromInteger(-56) : FP_FromInteger(56);

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(40), offX))
    {
        ToKnockback();
        return;
    }

    if (!(field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame)))
    {
        return;
    }

    field_A8_xpos += xpos;
    field_B4_velx = velX;

    field_B8_vely = field_BC_sprite_scale * FP_FromDouble(-2.7);
    field_E8_LastLineYPos = field_AC_ypos;

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(40), field_B4_velx))
    {
        ToKnockback();
        return;
    }

    field_A8_xpos += field_B4_velx;
    field_AC_ypos += field_B8_vely;

    VOnTrapDoorOpen();
    field_FC_current_motion = eElumMotions::Motion_31_HopMid_414C70;
    field_F4_pLine = nullptr;
}

void Elum::RunJumpMidAndHopMid(MidType midType)
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);

    if (midType == MidType::eRunJumpMid)
    {
        Event_Broadcast_417220(kEvent_11, this);
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(40), field_B4_velx))
    {
        RunSlideStopKnockback();
    }
    else
    {
        FP hitX = {};
        FP hitY = {};

        const FP velY = midType == MidType::eRunJumpMid ? FP_FromDouble(0.8) : FP_FromDouble(0.9);
        const FP jump_velx = midType == MidType::eRunJumpMid ? FP_FromDouble(1.1) : FP_FromDouble(2.15);

        const auto InAirCollision = InAirCollision_4019C0(&field_F4_pLine, &hitX, &hitY, velY);

        if (sControlledCharacter_50767C == this)
        {
            SetActiveCameraDelayedFromDir_401C90();
        }

        if (InAirCollision)
        {
            switch (field_F4_pLine->field_8_type)
            {
                case eLineTypes::eFloor_0:
                case eLineTypes::eBackgroundFloor_4:
                case eLineTypes::eUnknown_32:
                case eLineTypes::eUnknown_36:
                    Elum_SFX_416E10(ElumSounds::eHitGroundSoft_4, 0);

                    switch (midType)
                    {
                        case MidType::eRunJumpMid:
                            field_A8_xpos = hitX;
                            field_AC_ypos = hitY;
                            field_FC_current_motion = eElumMotions::Motion_35_RunJumpLand_415580;
                            MapFollowMe_401D30(TRUE);
                            break;

                        case MidType::eHopMid:
                            field_B8_vely = FP_FromInteger(0);
                            field_B4_velx = FP_FromInteger(0);
                            field_FC_current_motion = eElumMotions::Motion_32_HopLand_415140;
                            field_A8_xpos = hitX;
                            field_AC_ypos = hitY;
                            break;

                        default:
                            break;
                    }

                    return;
                default:
                    break;
            }
        }

        if (field_AC_ypos - field_E8_LastLineYPos > FP_FromInteger(2))
        {
            field_118_jump_velx = jump_velx;
            field_FC_current_motion = eElumMotions::Motion_24_JumpToFall_415ED0;
        }
    }
}

void Elum::Motion_31_HopMid_414C70()
{
    RunJumpMidAndHopMid(MidType::eHopMid);
}

void Elum::Motion_32_HopLand_415140()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);

    CheckLiftPointGoneAndSetCamera();

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        PSX_RECT bRect = {};
        VGetBoundingRect(&bRect, 1);

        VOnCollisionWith(
            {bRect.x, static_cast<s16>(bRect.y + 5)},
            {bRect.w, static_cast<s16>(bRect.h + 5)},
            ObjListPlatforms_50766C,
            1,
            (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_401C10);

        MapFollowMe_401D30(TRUE);

        if (!ToNextMotion_4120F0())
        {
            ToIdle();
        }
    }
}

void Elum::Motion_33_RunJumpBegin_415400()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);

    CheckLiftPointGoneAndSetCamera();

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(40), field_B4_velx))
    {
        ToKnockback();
    }
    else if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        FP velX = {};
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            velX = field_BC_sprite_scale * FP_FromDouble(-11.43);
        }
        else
        {
            velX = field_BC_sprite_scale * FP_FromDouble(11.43);
        }

        field_B4_velx = velX;

        field_E8_LastLineYPos = field_AC_ypos;
        field_B8_vely = field_BC_sprite_scale * FP_FromInteger(-4);

        field_A8_xpos += field_B4_velx;
        field_AC_ypos += field_BC_sprite_scale * FP_FromInteger(-4);

        VOnTrapDoorOpen();
        field_FC_current_motion = eElumMotions::Motion_34_RunJumpMid_415240;
        field_F4_pLine = nullptr;
    }
}

void Elum::Motion_34_RunJumpMid_415240()
{
    RunJumpMidAndHopMid(MidType::eRunJumpMid);
}

void Elum::Motion_35_RunJumpLand_415580()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);

    CheckLiftPointGoneAndSetCamera();

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        PSX_RECT bRect = {};
        VGetBoundingRect(&bRect, 1);

        VOnCollisionWith(
            {bRect.x, static_cast<s16>(bRect.y + 5)},
            {bRect.w, static_cast<s16>(bRect.h + 5)},
            ObjListPlatforms_50766C,
            1,
            (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_401C10);

        if (sControlledCharacter_50767C == this)
        {
            FP offX = {};
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                offX = -(field_BC_sprite_scale * FP_FromDouble(18.75));
            }
            else
            {
                offX = field_BC_sprite_scale * FP_FromDouble(18.75);
            }

            if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(40), offX))
            {
                ToKnockback();
            }
            else
            {
                field_E4_previous_motion = eElumMotions::Motion_36_RunLoop_413720;
                field_E6_last_anim_frame = 4;
                field_120_bUnknown = 1;
                if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    field_A8_xpos -= field_BC_sprite_scale * FP_FromDouble(18.75);
                    field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
                }
                else
                {
                    field_A8_xpos += field_BC_sprite_scale * FP_FromDouble(18.75);
                    field_B4_velx = ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4);
                }
            }
        }
        else
        {
            field_B4_velx = (field_B4_velx >= FP_FromInteger(0)) ? FP_FromDouble(13.3) : FP_FromDouble(-13.3);
            field_FC_current_motion = eElumMotions::Motion_12_RunTurn_414520;
        }
    }
}

void Elum::Motion_36_RunLoop_413720()
{
    field_10E_pressed |= Input().Pressed();

    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);
    Event_Broadcast_417220(kEvent_11, this);

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(40), field_B4_velx))
    {
        ToKnockback();
        return;
    }

    s16 gridSizeDirected = 0;
    if (field_B4_velx <= FP_FromInteger(0))
    {
        gridSizeDirected = FP_GetExponent(ScaleToGridSize_41FA30(field_BC_sprite_scale));
    }
    else
    {
        gridSizeDirected = -FP_GetExponent(ScaleToGridSize_41FA30(field_BC_sprite_scale));
    }

    MoveOnLine_412580(gridSizeDirected);

    if (field_FC_current_motion == eElumMotions::Motion_36_RunLoop_413720)
    {
        if (field_10_anim.field_92_current_frame != 0 && field_10_anim.field_92_current_frame != 4)
        {
            field_10C_bFootStep2 = 0;
            return;
        }

        Elum_SFX_416E10(ElumSounds::eRunningFootstep_1, 0);

        if (!field_10C_bFootStep2)
        {
            field_10C_bFootStep2 = 1;
            MapFollowMe_401D30(TRUE);
        }

        if (sControlledCharacter_50767C == this)
        {
            if (sInputKey_Hop_4C65A0 & field_10E_pressed)
            {
                field_FC_current_motion = eElumMotions::Motion_33_RunJumpBegin_415400;
                field_10E_pressed = 0;
            }
            else if (Input().IsAnyPressed(sInputKey_Right_4C6590) && field_B4_velx < FP_FromInteger(0))
            {
                field_FC_current_motion = eElumMotions::Motion_12_RunTurn_414520;
                field_B4_velx = field_BC_sprite_scale * FP_FromDouble(-13.3);
            }
            else if (Input().IsAnyPressed(sInputKey_Left_4C6594) && field_B4_velx > FP_FromInteger(0))
            {
                field_FC_current_motion = eElumMotions::Motion_12_RunTurn_414520;
                field_B4_velx = field_BC_sprite_scale * FP_FromDouble(13.3);
            }
            else if (!Input().IsAnyPressed(sInputKey_Right_4C6590 | sInputKey_Left_4C6594))
            {
                field_FC_current_motion = eElumMotions::Motion_37_RunSlideStop_4142E0;

                if (field_B4_velx >= FP_FromInteger(0))
                {
                    field_B4_velx = (field_BC_sprite_scale * FP_FromDouble(13.3));
                }
                else
                {
                    field_B4_velx = (field_BC_sprite_scale * FP_FromDouble(-13.3));
                }
            }
            else if (!Input().IsAnyPressed(sInputKey_Run_4C65A8))
            {
                if (field_10_anim.field_92_current_frame == 0)
                {
                    field_FC_current_motion = eElumMotions::Motion_42_RunToWalk_413B60;
                }
                else
                {
                    field_FC_current_motion = eElumMotions::Motion_43_MidRunToWalk_413E20;
                }
            }
            return;
        }
        else
        {
            field_FC_current_motion = eElumMotions::Motion_12_RunTurn_414520;

            if (field_B4_velx >= FP_FromInteger(0))
            {
                field_B4_velx = (field_BC_sprite_scale * FP_FromDouble(13.3));
            }
            else
            {
                field_B4_velx = (field_BC_sprite_scale * FP_FromDouble(-13.3));
            }
        }

        field_10E_pressed = 0;
    }
}

void Elum::RunSlideStopKnockback()
{
    field_FC_current_motion = eElumMotions::Motion_50_Knockback_415DC0;
    if (field_F4_pLine)
    {
        field_B4_velx = -field_B4_velx;
        MoveOnLine_412580(FALSE);
    }
    else
    {
        field_A8_xpos -= field_B4_velx;
    }
    field_B4_velx = FP_FromInteger(0);
    MapFollowMe_401D30(TRUE);
    Environment_SFX_42A220(EnvironmentSfx::eKnockback_13, 95, -200, this);
}

void Elum::Motion_37_RunSlideStop_4142E0()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);

    if (field_10_anim.field_92_current_frame == 0)
    {
        Elum_SFX_416E10(ElumSounds::eRunSlide_5, 0);
    }

    const FP offY = (sControlledCharacter_50767C == this) ? field_BC_sprite_scale * FP_FromInteger(40) : field_BC_sprite_scale * FP_FromInteger(25);

    if (WallHit_401930(offY, field_B4_velx))
    {
        RunSlideStopKnockback();
        return;
    }

    SlowOnX_414210(FP_FromDouble(2.125));

    if (sControlledCharacter_50767C == this && field_10_anim.field_92_current_frame < 7)
    {
        if ((field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX) && Input().IsAnyPressed(sInputKey_Right_4C6590)) || (!field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX) && Input().IsAnyPressed(sInputKey_Left_4C6594)))
        {
            field_E4_previous_motion = eElumMotions::Motion_12_RunTurn_414520;
            field_E6_last_anim_frame = field_10_anim.field_92_current_frame;
            field_120_bUnknown = 1;
        }
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        MapFollowMe_401D30(TRUE);
        if (!ToNextMotion_4120F0())
        {
            field_118_jump_velx = FP_FromInteger(0);
            field_B4_velx = FP_FromInteger(0);
            field_B8_vely = FP_FromInteger(0);
            field_FC_current_motion = eAbeMotions::Motion_1_WalkLoop_423F90;
            field_110_timer = gnFrameCount_507670;
            field_10E_pressed = 0;
            MapFollowMe_401D30(TRUE);
        }
    }
}

void Elum::Motion_38_RunTurnToRun_414810()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);

    MoveOnLine_412580(0);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eElumMotions::Motion_36_RunLoop_413720;
        field_10_anim.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
    }
}

void Elum::Motion_39_IdleToRun_413B00()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);
    Event_Broadcast_417220(kEvent_11, this);

    field_10E_pressed |= Input().Pressed();

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eElumMotions::Motion_36_RunLoop_413720;
    }

    MoveOnLine_412580(0);
}

void Elum::Motion_40_WalkToRun_4134B0()
{
    field_10E_pressed |= Input().Pressed();

    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);
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
        field_FC_current_motion = eElumMotions::Motion_36_RunLoop_413720;
    }

    MoveOnLine_412580(0);
}

void Elum::Motion_41_MidWalkToRun_413560()
{
    field_10E_pressed |= Input().Pressed();

    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);
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
        field_E4_previous_motion = eElumMotions::Motion_36_RunLoop_413720;
        field_E6_last_anim_frame = eElumMotions::Motion_4_Turn_4140F0;
        field_120_bUnknown = 1;
    }

    MoveOnLine_412580(0);
}

void Elum::Motion_42_RunToWalk_413B60()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);

    field_10E_pressed |= Input().Pressed();

    FP velX{};
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        velX = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
    }
    else
    {
        velX = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
    }

    field_B4_velx = velX;

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eElumMotions::Motion_3_WalkLoop_412C90;
        VCheckCollisionLineStillValid(10);

        PSX_Point xy{FP_GetExponent(field_A8_xpos - FP_FromInteger(10)), FP_GetExponent(field_AC_ypos - FP_FromInteger(10))};
        PSX_Point wh{FP_GetExponent(field_A8_xpos + FP_FromInteger(10)), FP_GetExponent(field_AC_ypos + FP_FromInteger(10))};
        VOnCollisionWith(
            xy,
            wh,
            ObjListPlatforms_50766C,
            1,
            (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_401C10);

        FP offX = {};
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            offX = FP_FromInteger(-2) * ScaleToGridSize_41FA30(field_BC_sprite_scale);
        }
        else
        {
            offX = FP_FromInteger(2) * ScaleToGridSize_41FA30(field_BC_sprite_scale);
        }

        if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(40), offX))
        {
            ToKnockback();
            return;
        }
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(40), field_B4_velx))
    {
        ToKnockback();
        return;
    }

    MoveOnLine_412580(0);
}

void Elum::Motion_43_MidRunToWalk_413E20()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);

    field_10E_pressed |= Input().Pressed();

    FP velX = {};
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        velX = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
    }
    else
    {
        velX = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
    }

    field_B4_velx = velX;

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_E4_previous_motion = 3;
        field_E6_last_anim_frame = 9;
        field_120_bUnknown = 1;
        VCheckCollisionLineStillValid(10);

        PSX_Point xy{FP_GetExponent(field_A8_xpos - FP_FromInteger(10)), FP_GetExponent(field_AC_ypos - FP_FromInteger(10))};
        PSX_Point wh{FP_GetExponent(field_A8_xpos + FP_FromInteger(10)), FP_GetExponent(field_AC_ypos + FP_FromInteger(10))};

        VOnCollisionWith(
            xy,
            wh,
            ObjListPlatforms_50766C,
            1,
            (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_401C10);

        FP offX = {};
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            offX = FP_FromInteger(-2) * ScaleToGridSize_41FA30(field_BC_sprite_scale);
        }
        else
        {
            offX = FP_FromInteger(2) * ScaleToGridSize_41FA30(field_BC_sprite_scale);
        }

        if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(40), offX))
        {
            ToKnockback();
            return;
        }
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(40), field_B4_velx))
    {
        ToKnockback();
        return;
    }
    MoveOnLine_412580(0);
}

void Elum::Motion_44_ScratchBegin_412730()
{
    CheckLiftPointGoneAndSetCamera();

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eElumMotions::Motion_45_ScratchLoop_4127B0;
        if (gMap_507BA8.GetDirection(field_B2_lvl_number, field_B0_path_number, field_A8_xpos, field_AC_ypos) == CameraPos::eCamCurrent_0)
        {
            SND_SEQ_PlaySeq_4775A0(SeqId::Unknown_15, 1, 1);
        }
    }
}

void Elum::Motion_45_ScratchLoop_4127B0()
{
    CheckLiftPointGoneAndSetCamera();

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eElumMotions::Motion_46_ScratchEnd_412800;
    }
}

void Elum::Motion_46_ScratchEnd_412800()
{
    CheckLiftPointGoneAndSetCamera();

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_FE_next_motion == eElumMotions::Motion_3_WalkLoop_412C90)
        {
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                if (!WallHit_401930(field_BC_sprite_scale * FP_FromInteger(40), -ScaleToGridSize_41FA30(field_BC_sprite_scale)))
                {
                    field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
                    field_FC_current_motion = eElumMotions::Motion_8_IdleToWalk2_413270;
                    field_FE_next_motion = -1;
                    return;
                }
            }
            else
            {
                if (!WallHit_401930(field_BC_sprite_scale * FP_FromInteger(40), ScaleToGridSize_41FA30(field_BC_sprite_scale)))
                {
                    field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
                    field_FC_current_motion = eElumMotions::Motion_8_IdleToWalk2_413270;
                    field_FE_next_motion = -1;
                    return;
                }
            }
            field_FE_next_motion = -1;
            return;
        }

        if (field_FE_next_motion == eElumMotions::Motion_29_BeesStruggling_412A90)
        {
            Elum_SFX_416E10(ElumSounds::eBeesStruggle_3, 0);
            field_FC_current_motion = eElumMotions::Motion_29_BeesStruggling_412A90;
            field_110_timer = gnFrameCount_507670 + 25;
            field_FE_next_motion = -1;
            return;
        }

        ToIdle();
    }
}

void Elum::ToIdle()
{
    field_118_jump_velx = FP_FromInteger(0);
    field_110_timer = gnFrameCount_507670;
    field_B4_velx = FP_FromInteger(0);
    field_B8_vely = FP_FromInteger(0);
    field_FC_current_motion = eElumMotions::Motion_1_Idle_412990;
    field_10E_pressed = 0;
    MapFollowMe_401D30(TRUE);
}

void Elum::Motion_47_Unknown_415A30()
{
    Event_Broadcast_417220(kEventNoise_0, this);

    CheckLiftPointGoneAndSetCamera();

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (!ToNextMotion_4120F0())
        {
            ToIdle();
        }
    }
}

void Elum::Motion_48_AbeMoutingBegin_415C40()
{
    if (sActiveHero_507678->field_FC_current_motion == eAbeMotions::Motion_136_ElumMountEnd_42E110)
    {
        VFreeUnmountedResources_4112B0();

        field_104_pending_resource_count += 2;

        ResourceManager::LoadResourceFile(
            "ELMRIDE.BAN",
            BaseAliveGameObject::OnResourceLoaded_4019A0,
            this);

        ResourceManager::LoadResourceFile(
            "ELMPDMNT.BAN",
            BaseAliveGameObject::OnResourceLoaded_4019A0,
            this);

        field_FC_current_motion = eElumMotions::Motion_27_AbeMountingEnd_415CA0;
    }
}

void Elum::Motion_49_AbeUnmountingBegin_415D00()
{
    if (sActiveHero_507678->field_FC_current_motion != eAbeMotions::Motion_137_ElumUnmountBegin_42E2B0)
    {
        VFreeMountedResources_411200();

        field_104_pending_resource_count += 2;

        ResourceManager::LoadResourceFile(
            "ELMALONE.BAN",
            BaseAliveGameObject::OnResourceLoaded_4019A0,
            this);

        ResourceManager::LoadResourceFile(
            "ELMPRMNT.BAN",
            BaseAliveGameObject::OnResourceLoaded_4019A0,
            this);

        field_FC_current_motion = eElumMotions::Motion_28_AbeUnmountingEnd_415D60;
    }
}

void Elum::Motion_50_Knockback_415DC0()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);

    if (!field_F4_pLine)
    {
        Motion_21_Land_414A20();
        if (field_FC_current_motion == eElumMotions::Motion_1_Idle_412990)
        {
            field_FC_current_motion = eElumMotions::Motion_50_Knockback_415DC0;
            field_110_timer = gnFrameCount_507670 + 10;
        }
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_110_timer <= static_cast<s32>(gnFrameCount_507670))
        {
            if (field_F4_pLine)
            {
                if (sControlledCharacter_50767C != this
                    && !field_174_resources.res[30]
                    && !field_104_pending_resource_count)
                {
                    VLoadUnmountedResources_411260();
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

void Elum::VRender(PrimHeader** ppOt)
{
    VRender_410E40(ppOt);
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

        field_FC_current_motion = eElumMotions::Motion_21_Land_414A20;
        field_F4_pLine = nullptr;

        static s32 sElum_velx_table_4FF968[] = {
            4,
            4,
            0,
            -4,
            -4,
            -4,
            0,
            4};

        static s32 sElum_vely_table_4FF988[] = {
            0,
            -4,
            -4,
            -4,
            0,
            4,
            4,
            4};

        if (Input().IsAnyPressed(0xF000u))
        {
            const s32 tableIdx = Input().Dir();
            field_B4_velx = FP_FromRaw(sElum_velx_table_4FF968[tableIdx]);
            field_B8_vely = FP_FromRaw(sElum_vely_table_4FF988[tableIdx]);

            if (Input().IsAnyPressed(sInputKey_Run_4C65A8))
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
                field_174_resources.res[30] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kElmaloneAOResID_230, 1, 0);
            }
        }
        else
        {
            field_174_resources.res[0] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kElmbasicAOResID_200, 1, 0);
        }

        if (!(field_170_flags.Get(Elum::Flags_170::eFoundHoney_Bit4) || field_170_flags.Get(Elum::Flags_170::eStungByBees_Bit2)) && field_128_brain_idx != 1)
        {
            FindHoney_411600();
        }

        if (field_170_flags.Get(Elum::Flags_170::eChangedPathNotMounted_Bit5))
        {
            HandleElumPathTrans_411460();
            field_170_flags.Clear(Elum::Flags_170::eChangedPathMounted_Bit7);
            field_170_flags.Clear(Elum::Flags_170::eChangedPathNotMounted_Bit5);
        }

        if (field_FC_current_motion == eElumMotions::Motion_19_Dead_415F90 || field_B0_path_number == gMap_507BA8.field_2_current_path)
        {
            PathLine* pLine = nullptr;
            if (field_170_flags.Get(Elum::Flags_170::eChangedPathMounted_Bit7))
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
                    field_FC_current_motion = eElumMotions::Motion_21_Land_414A20;
                }
                field_170_flags.Clear(Elum::Flags_170::eChangedPathMounted_Bit7);
            }

            const auto oldMotion = field_FC_current_motion;
            const FP old_x = field_A8_xpos;
            const FP old_y = field_AC_ypos;

            if (oldMotion != eElumMotions::Motion_19_Dead_415F90)
            {
                field_12A_brain_sub_state = (this->*sElum_brain_table_4C52E8[field_128_brain_idx])();
            }

            if (field_170_flags.Get(Elum::Flags_170::eFalling_Bit3))
            {
                VCheckCollisionLineStillValid(10);

                PSX_RECT bRect = {};
                VGetBoundingRect(&bRect, 1);

                VOnCollisionWith(
                    {bRect.x, static_cast<s16>(bRect.y + 5)},
                    {bRect.w, static_cast<s16>(bRect.h + 5)},
                    ObjListPlatforms_50766C,
                    1,
                    (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_401C10);

                if (field_F8_pLiftPoint)
                {
                    field_170_flags.Clear(Elum::Flags_170::eFalling_Bit3);
                }
            }

            (this->*sElum_motion_table_4C5148[field_FC_current_motion])();

            if ((oldMotion != field_FC_current_motion && oldMotion == 2) || oldMotion == 11 || oldMotion == 47)
            {
                LOG_INFO("old motion: " << oldMotion << " | new motion: " << field_FC_current_motion);
            }

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
                if (field_120_bUnknown)
                {
                    field_FC_current_motion = field_E4_previous_motion;
                    u8** ppRes = GetResBlock_410D00(field_E4_previous_motion);
                    if (!ppRes)
                    {   
                        if (GetGameAutoPlayer().IsPlaying() || GetGameAutoPlayer().IsRecording())
                        {
                            ResourceManager::LoadResourceFile_455270("ELMRIDE.BAN", nullptr);
                            ResourceManager::LoadResourceFile_455270("ELMPDMNT.BAN", nullptr);
                            VLoadMountedResources_411300();
                            ppRes = GetResBlock_410D00(field_E4_previous_motion);
                            if (!ppRes)
                            {
                                LOG_WARNING("failed to hack load mounted elum resources");
                                return;
                            }
                            LOG_INFO("successfully hack loaded mounted elum resources");
                        }
                        else
                        {
                            return;
                        }
                    }

                    const AnimRecord& rec = AO::AnimRec(gElumAnimIdTables_4C5218[field_FC_current_motion]);
                    field_10_anim.Set_Animation_Data_402A40(rec.mFrameTableOffset, ppRes);
                    field_10_anim.SetFrame_402AC0(field_E6_last_anim_frame);
                    field_120_bUnknown = 0;

                    if (sControlledCharacter_50767C == this)
                    {
                        sActiveHero_507678->SyncToElum_42D850(field_FC_current_motion);
                    }
                }
            }
            else
            {
                u8** ppRes = GetResBlock_410D00(field_FC_current_motion);
                if (!ppRes)
                {
                    if (GetGameAutoPlayer().IsPlaying() || GetGameAutoPlayer().IsRecording())
                    {
                        ResourceManager::LoadResourceFile_455270("ELMRIDE.BAN", nullptr);
                        ResourceManager::LoadResourceFile_455270("ELMPDMNT.BAN", nullptr);
                        VLoadMountedResources_411300();
                        ppRes = GetResBlock_410D00(field_FC_current_motion);
                        if (!ppRes)
                        {
                            LOG_WARNING("failed to hack load mounted elum resources");
                            return;
                        }
                        LOG_INFO("successfully hack loaded mounted elum resources");
                    }
                    else
                    {
                        return;
                    }
                }

                const AnimRecord& rec = AO::AnimRec(gElumAnimIdTables_4C5218[field_FC_current_motion]);
                field_10_anim.Set_Animation_Data_402A40(rec.mFrameTableOffset, ppRes);

                if (sControlledCharacter_50767C == this)
                {
                    sActiveHero_507678->SyncToElum_42D850(field_FC_current_motion);
                }
            }

            if (Event_Get_417250(kEventDeathReset_4))
            {
                if (!field_154_bAbeForcedDownFromElum)
                {
                    if (sActiveHero_507678->field_146_zone_number != field_140_continue_zone_number)
                    {
                        field_128_brain_idx = 0;
                        field_12A_brain_sub_state = 6;
                        field_122_bDontFollowAbe = 1;

                        if (sControlledCharacter_50767C == this)
                        {
                            sActiveHero_507678->field_A8_xpos = field_A8_xpos;
                            sActiveHero_507678->field_AC_ypos = field_AC_ypos;
                            sActiveHero_507678->field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX, field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX));
                        }
                        return;
                    }
                }

                field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
                field_FC_current_motion = eElumMotions::Motion_19_Dead_415F90;
                VOnTrapDoorOpen();
            }

            if (sControlledCharacter_50767C == this)
            {
                sActiveHero_507678->field_A8_xpos = field_A8_xpos;
                sActiveHero_507678->field_AC_ypos = field_AC_ypos;
                sActiveHero_507678->field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX, field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX));
            }
            return;
        }
    }
}

void Elum::VRender_410E40(PrimHeader** ppOt)
{
    if (field_B2_lvl_number == gMap_507BA8.field_0_current_level
        && field_B0_path_number == gMap_507BA8.field_2_current_path
        && !field_8_update_delay)
    {
        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
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
                    if (field_122_bDontFollowAbe != 1 && sControlledCharacter_50767C != this && pElumPathTrans->field_18_level == gMap_507BA8.field_A_level && pElumPathTrans->field_1A_path == gMap_507BA8.field_C_path)
                    {
                        field_170_flags.Set(Elum::Flags_170::eChangedPathNotMounted_Bit5);
                    }
                }
            }

            if (field_F8_pLiftPoint)
            {
                VOnTrapDoorOpen();
                field_170_flags.Set(Elum::Flags_170::eFalling_Bit3);
            }

            if (!field_170_flags.Get(Elum::Flags_170::eChangedPathNotMounted_Bit5))
            {
                field_F4_pLine = nullptr;
                field_170_flags.Set(Elum::Flags_170::eChangedPathMounted_Bit7);
            }
        }
    }
}

ALIVE_VAR(1, 0x4C52F0, s16, word_4C52F0, 0x88);
ALIVE_VAR(1, 0x4C52F2, s16, word_4C52F2, 0x38);
ALIVE_VAR(1, 0x4C52F4, u32, dword_4C52F4, 0x1C92C);
ALIVE_VAR(1, 0x4C52F8, u32, dword_4C52F8, 0xC8);
ALIVE_VAR(1, 0x4C52FA, s16, word_4C52FA, 0);

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

Elum* Elum::ctor_410870(s32, anythingForTheTimeBeing, anythingForTheTimeBeing, s32, TlvItemInfoUnion tlvInfo)
{
    ctor_401090();
    SetVTable(this, 0x4BA8F8);
    field_4_typeId = Types::eElum_26;

    field_158_last_event_idx = -1;
    field_16C_never_read = 0;
    field_16E_never_read = -1;
    field_1F0_tlvInfo = tlvInfo.all;
    field_174_resources = {};

    field_174_resources.res[16] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kElmfallAOResID_216, 1, 0);
    if (!field_174_resources.res[16])
    {
        ResourceManager::LoadResourceFile_455270("ELMFALL.BAN", nullptr);
        field_174_resources.res[16] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kElmfallAOResID_216, 1, 0);
    }

    field_104_pending_resource_count = 0;

    field_174_resources.res[22] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kElmprmntAOResID__222, 1, 0);
    if (!field_174_resources.res[22])
    {
        field_104_pending_resource_count++;
        ResourceManager::LoadResourceFile(
            "ELMPRMNT.BAN",
            BaseAliveGameObject::OnResourceLoaded_4019A0,
            this);
        field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
    }

    field_174_resources.res[0] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kElmbasicAOResID_200, 1, 0);
    if (!field_174_resources.res[0])
    {
        field_104_pending_resource_count++;
        ResourceManager::LoadResourceFile(
            "ELMBASIC.BAN",
            BaseAliveGameObject::OnResourceLoaded_4019A0,
            this);
        field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
    }

    field_174_resources.res[30] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kElmaloneAOResID_230, 1, 0);
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

    const AnimRecord& fallRec = AO::AnimRec(AnimId::Elum_Land);
    Animation_Init_417FD0(
        fallRec.mFrameTableOffset,
        fallRec.mMaxW,
        fallRec.mMaxH,
        field_174_resources.res[16],
        1);

    field_BC_sprite_scale = sActiveHero_507678->field_BC_sprite_scale;

    if (field_BC_sprite_scale == FP_FromInteger(1))
    {
        field_C6_scale = 1;
        field_10_anim.field_C_layer = Layer::eLayer_ZapLinesElum_28;
    }
    else
    {
        field_C6_scale = 0;
        field_10_anim.field_C_layer = Layer::eLayer_ZapLinesElum_Half_9;
    }

    field_10A_flags.Set(Flags_10A::e10A_Bit4_SetOffExplosives);

    field_170_flags.Clear(Elum::Flags_170::eStungByBees_Bit2);
    field_170_flags.Clear(Elum::Flags_170::eFoundHoney_Bit4);
    field_170_flags.Clear(Elum::Flags_170::eChangedPathNotMounted_Bit5);
    field_170_flags.Clear(Elum::Flags_170::eChangedPathMounted_Bit7);

    field_F8_pLiftPoint = nullptr;
    gElum_507680 = this;

    field_FC_current_motion = eElumMotions::Motion_21_Land_414A20;
    field_120_bUnknown = 0;

    field_12E_honey_ypos = 0;
    field_12C_honey_xpos = 0;

    field_144_bRespawnOnDead = 0;
    field_110_timer = gnFrameCount_507670;

    field_A8_xpos = sActiveHero_507678->field_A8_xpos - (ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromInteger(2));
    field_AC_ypos = sActiveHero_507678->field_AC_ypos - FP_FromInteger(5);

    field_122_bDontFollowAbe = 0;
    field_124_bShould_IdleToWalk1 = 1;
    field_10E_pressed = 0;
    field_EC = 2;
    field_E6_last_anim_frame = 0;
    field_130_unused = 0;

    field_140_continue_zone_number = 0;
    field_142_zone_number = sActiveHero_507678->field_146_zone_number;

    field_154_bAbeForcedDownFromElum = 0;
    field_128_brain_idx = 0;
    field_12A_brain_sub_state = 0;

    field_10A_flags.Set(Flags_10A::e10A_Bit6);


    field_170_flags.Clear(Elum::Flags_170::eStrugglingWithBees_Bit1);
    field_170_flags.Clear(Elum::Flags_170::eFalling_Bit3);

    field_170_flags.Clear(Elum::Flags_170::eCanSpeak_Bit6);

    auto pMusic = ao_new<MusicTrigger>();
    if (pMusic)
    {
        pMusic->ctor_443A60(MusicTriggerMusicType::eSecretAreaShort_6, TriggeredBy::eTouching_1, 0, 30);
    }

    field_D0_pShadow = ao_new<Shadow>();
    if (field_D0_pShadow)
    {
        field_D0_pShadow->ctor_461FB0();
    }

    return this;
}

} // namespace AO
