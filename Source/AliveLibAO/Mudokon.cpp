#include "stdafx_ao.h"
#include "Function.hpp"
#include "Mudokon.hpp"
#include "GameSpeak.hpp"
#include "ResourceManager.hpp"
#include "Shadow.hpp"
#include "stdlib.hpp"
#include "Collisions.hpp"
#include "Game.hpp"

void Mud_ForceLink() {}

START_NS_AO

using TMudStateFunction = decltype(&Mudokon::State_0_Idle_43CA70);

#define MAKE_STRINGS(VAR) #VAR,
const char* const sMudStateNames[] =
{
    MUD_STATES_ENUM(MAKE_STRINGS)
};

const TMudStateFunction gMudMotions_4CD470[] =
{
    &Mudokon::State_0_Idle_43CA70,
    &Mudokon::State_1_WalkLoop_43CC80,
    &Mudokon::State_2_StandingTurn_43D050,

    // All the same func
    &Mudokon::State_3_Speak_43D440,
    &Mudokon::State_4_Speak_43D440,
    &Mudokon::State_5_Speak_43D440,
    &Mudokon::State_6_Speak_43D440,

    &Mudokon::State_7_WalkBegin_43CE60,
    &Mudokon::State_8_WalkToIdle_43CEF0,
    &Mudokon::State_9_MidWalkToIdle_43CFA0,
    &Mudokon::State_10_Unused_43D4D0,
    &Mudokon::State_11_Null_43D350,
    &Mudokon::State_12_LiftUse_43D360,
    &Mudokon::State_13_LiftGrabBegin_43D3F0,
    &Mudokon::State_14_LiftGrabEnd_43D420,
    &Mudokon::State_15_LeverUse_43D4B0,
    &Mudokon::State_16_StandScrubLoop_43D7C0,
    &Mudokon::State_17_StandScrubLoopToPause_43D860,
    &Mudokon::State_18_StandScrubPauseToLoop_43D880,
    &Mudokon::State_19_StandScrubPause_43D8A0,
    &Mudokon::State_20_IdleToStandScrub_43D8D0,
    &Mudokon::State_21_StandScrubToIdle_43D8F0,
    &Mudokon::State_22_CrouchScrub_43D910,
    &Mudokon::State_23_CrouchIdle_43E590,
    &Mudokon::State_24_CrouchTurn_43E5F0,
    &Mudokon::State_25_StandToCrouch_43E620,
    &Mudokon::State_26_CrouchToStand_43E640,
    &Mudokon::State_27_RunToWalk_43D980,
    &Mudokon::State_28_MidRunToWalk_43DA40,
    &Mudokon::State_29_RunLoop_43DB10,
    &Mudokon::State_30_RunToWalk_43DD50,
    &Mudokon::State_31_MidRunToWalk_43DE10,
    &Mudokon::State_32_RunSlideStop_43DEE0,
    &Mudokon::State_33_RunSlideTurn_43DF80,
    &Mudokon::State_34_RunTurnToRun_43E070,
    &Mudokon::State_35_SneakLoop_43E0F0,
    &Mudokon::State_36_WalkToSneak_43E240,
    &Mudokon::State_37_SneakToWalk_43E2E0,
    &Mudokon::State_38_MidWalkToSneak_43E380,
    &Mudokon::State_39_MidSneakToWalk_43E430,
    &Mudokon::State_40_SneakBegin_43E4E0,
    &Mudokon::State_41_SneakToIdle_43E530,
    &Mudokon::State_42_MidSneakToIdle_43E560,
    &Mudokon::State_43_JumpBegin_43E870,
    &Mudokon::State_44_JumpMid_43E960,
    &Mudokon::State_45_ToRunToPortal_43EB00,
    &Mudokon::State_46_FallLandDie_43E660,
    &Mudokon::State_47_Knockback_43E730,
    &Mudokon::State_48_KnockbackGetUp_43E7D0,
    &Mudokon::State_49_FallOfEdge_43E800,
    &Mudokon::State_50_LandSoft_43E820,
    &Mudokon::State_51_Fall_43D0D0,
    &Mudokon::State_52_Chant_43D520,
    &Mudokon::State_53_ChantEnd_43D640,
    &Mudokon::State_54_ToDuck_43EB70,
    &Mudokon::State_55_Duck_43EB90,
    &Mudokon::State_56_DuckToCrouch_43EBC0,
    &Mudokon::State_57_Struggle_43EBE0,
    &Mudokon::State_58_StruggleToCrouchChant_43EC00,
    &Mudokon::State_59_CrouchChant_43EC20,
    &Mudokon::State_60_CrouchChantToSruggle_43ED50,
    &Mudokon::State_61_DuckKnockback_43E6E0,
    &Mudokon::State_62_Choke_43ED70,
};

static const TintEntry sMudTints_4CD320[] =
{
    { 5, 25u, 25u, 25u },
    { 6, 25u, 25u, 25u },
    { -1, 87u, 103u, 67u }
};

Mudokon* Mudokon::ctor_43EED0(Path_TLV* pTlv, int tlvInfo)
{
    ctor_401090();
    SetVTable(this, 0x4BBB28);
    field_4_typeId = Types::eMudokon_52;

    field_128 = -1;
    field_13E = -1;
    field_FE_next_state = -1;
    field_13C = 0;
    field_1C0_timer = 0;
    field_1B8 = 0;
    field_1BA_sub_state = 0;
    field_EC_oldY = 3;
    field_1BC = 0;
    field_124 = 0;
    field_148_res_array = {};

    field_148_res_array.res[0] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 55, 1, 0);
    Animation_Init_417FD0(55968, 135, 80, field_148_res_array.res[0], 1);

    field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);

    field_10A_flags.Set(Flags_10A::e10A_Bit4);
    field_10A_flags.Set(Flags_10A::e10A_Bit6);

    field_144_flags.Clear(Flags_144::e144_Bit6_bDeaf);
    field_144_flags.Clear(Flags_144::e144_Bit7);
    field_144_flags.Clear(Flags_144::e144_Bit8);
    field_144_flags.Clear(Flags_144::e144_Bit9);
    field_144_flags.Clear(Flags_144::e144_Bit10);

    field_1B6 = 0;
    field_198 = 0;

    SetTint_418750(sMudTints_4CD320, field_B2_lvl_number);

    int scale = 0;
    switch (pTlv->field_4_type)
    {
    case TlvTypes::LiftMud_32:
    {
        auto liftMudTlv = static_cast<Path_Lift_Mud*>(pTlv);

        field_148_res_array.res[1] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 53, 1, 0);


        field_18C = liftMudTlv->field_18_how_far_to_walk << 16;
        field_110 = liftMudTlv->field_1A_lift_id;

        field_144_flags.Set(Flags_144::e144_Bit5, liftMudTlv->field_1C_direction); // TODO: Check
        field_144_flags.Clear(Flags_144::e144_Bit4_bSnapToGrid);
        field_144_flags.Clear(Flags_144::e144_Bit11_bPersist);

        field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX, liftMudTlv->field_1C_direction == 0); // TODO: Check direction

        field_186 = liftMudTlv->field_1E_silent;
        field_184 = 1;
        field_1B8 = 0;
        field_188 = 5;

        field_1A4 = Code_Convert_476000(liftMudTlv->field_22_code1, liftMudTlv->field_24_code2);
        field_1A8 = Code_Length_475FD0(field_1A4);

        scale = liftMudTlv->field_20_scale;
    }
    break;

    case TlvTypes::RingMud_50:
    {
        auto ringMudTlv = static_cast<Path_Ring_Mud*>(pTlv);

        field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX, ringMudTlv->field_18_facing == 0); // TODO: Check
        field_184 = 0;
        field_186 = ringMudTlv->field_1E_silent;
        field_198 = ringMudTlv->field_1A_abe_must_be_same_direction == 0;
        field_1B8 = 2;

        if (ringMudTlv->field_24_action == 0)
        {
            // Pull switch
            field_148_res_array.res[2] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 11, 1, 0);
            field_188 = 6;
        }
        else
        {
            // Give ring
            field_148_res_array.res[9] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 48, 1, 0);
            field_188 = 7;
        }

        field_1AA = ringMudTlv->field_26_ring_timeout;
        field_1A4 = Code_Convert_476000(ringMudTlv->field_20_code1, ringMudTlv->field_22_code2);
        field_1A8 = Code_Length_475FD0(field_1A4);

        field_144_flags.Set(Flags_144::e144_Bit10, ringMudTlv->field_28_instant_powerup & 1);
        field_144_flags.Clear(Flags_144::e144_Bit4_bSnapToGrid);
        field_144_flags.Clear(Flags_144::e144_Bit11_bPersist);


        scale = ringMudTlv->field_1C_scale;
    }
    break;

    case TlvTypes::Mud_82:
    {
        auto mudTlv = static_cast<Path_Mud*>(pTlv);

        if (mudTlv->field_1A_job == 0)
        {
            // stand scrub
            field_1B8 = 8;
            field_148_res_array.res[3] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 511, 1, 0);
        }
        else if (mudTlv->field_1A_job == 1)
        {
            // sit scrub
            field_1B8 = 9;
        }
        else if (mudTlv->field_1A_job == 2)
        {
            // chant
            field_1B8 = 14;
            field_148_res_array.res[12] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 516, 1, 0);
        }

        field_148_res_array.res[10] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 514, 1, 0);
        field_148_res_array.res[13] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 517, 1, 0);
        field_148_res_array.res[4] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 510, 1, 0);
        field_148_res_array.res[5] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 10, 1, 0);
        field_148_res_array.res[6] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 27, 1, 0);
        field_148_res_array.res[7] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 26, 1, 0);
        field_148_res_array.res[8] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 43, 1, 0);
        field_148_res_array.res[14] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 118, 1, 0);


        field_124 = mudTlv->field_1E_voice_adjust;
        field_4_typeId = Types::eMudokon_75;
        field_1B2_switch_id = mudTlv->field_20_rescue_id;

        field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX, mudTlv->field_1C_direction == 0); // TODO: Check
  
        // TODO: Check these as well
        field_144_flags.Set(Flags_144::e144_Bit11_bPersist, mudTlv->field_26_persist & 1);
        field_144_flags.Set(Flags_144::e144_Bit6_bDeaf, mudTlv->field_22_deaf & 1);
        field_144_flags.Set(Flags_144::e144_Bit4_bSnapToGrid);

        field_1B4 = 0;

        scale = mudTlv->field_18_scale;

    }
    break;
    }

    field_FC_current_motion = eMudStates::State_0_Idle_43CA70;

    if (scale == 1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_10_anim.field_C_layer = 13;
        field_C6_scale = 0;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_10_anim.field_C_layer = 32;
        field_C6_scale = 1;
    }

    FP hitX = {};
    FP hitY = {};
    const __int16 bHit = sCollisions_DArray_504C6C->RayCast_40C410(
        FP_FromInteger(pTlv->field_10_top_left.field_0_x),
        FP_FromInteger(pTlv->field_10_top_left.field_2_y),
        FP_FromInteger(pTlv->field_14_bottom_right.field_0_x),
        FP_FromInteger(pTlv->field_14_bottom_right.field_2_y),
        &field_F4_pLine,
        &hitX,
        &hitY,
        field_BC_sprite_scale != FP_FromDouble(0.5) ? 7 : 0x70);

    field_A8_xpos = FP_FromInteger((pTlv->field_10_top_left.field_0_x + pTlv->field_14_bottom_right.field_0_x) / 2);

    if (bHit)
    {
        field_AC_ypos = hitY;
        if (field_F4_pLine->field_8_type == 32 || field_F4_pLine->field_8_type == 36)
        {
            PSX_RECT bRect = {};
            VGetBoundingRect(&bRect, 1);
            VOnCollisionWith(
                PSX_Point{ bRect.x, bRect.y },
                PSX_Point{ bRect.w + 5, bRect.h }, // TODO: Check + 5
                ObjListPlatforms_50766C,
                1,
                (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_401C10);
        }
    }

    field_E8_LastLineYPos = field_AC_ypos;

    if (field_1B8 == 9 || field_1B8 == 8)
    {
        field_144_flags.Set(Flags_144::e144_Bit7);
        MapFollowMe_401D30(field_144_flags.Get(Flags_144::e144_Bit4_bSnapToGrid));
    }
    else
    {
        field_144_flags.Clear(Flags_144::e144_Bit7);
    }

    field_144_flags.Set(Flags_144::e144_Bit2);

    field_19A = 99;
    field_19C = 99;
    field_1BA_sub_state = 0;
    field_1A0 = 0;
    field_10C = tlvInfo;
    field_1C4_bDoPathTrans = FALSE;

    if (field_188 == 6)
    {
        field_A8_xpos += FP_FromInteger(8);
    }

    field_1AC_pBirdPortal = nullptr;
    field_194_pLiftPoint = nullptr;

    field_D0_pShadow = ao_new<Shadow>();
    if (field_D0_pShadow)
    {
        field_D0_pShadow->ctor_461FB0();
    }

    VUpdate();

    return this;
}

void Mudokon::VUpdate()
{
    VUpdate_43F560();
}

void Mudokon::VUpdate_43F560()
{
  NOT_IMPLEMENTED();
}

void Mudokon::State_0_Idle_43CA70()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_1_WalkLoop_43CC80()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_2_StandingTurn_43D050()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_3_Speak_43D440()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_4_Speak_43D440()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_5_Speak_43D440()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_6_Speak_43D440()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_7_WalkBegin_43CE60()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_8_WalkToIdle_43CEF0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_9_MidWalkToIdle_43CFA0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_10_Unused_43D4D0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_11_Null_43D350()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_12_LiftUse_43D360()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_13_LiftGrabBegin_43D3F0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_14_LiftGrabEnd_43D420()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_15_LeverUse_43D4B0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_16_StandScrubLoop_43D7C0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_17_StandScrubLoopToPause_43D860()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_18_StandScrubPauseToLoop_43D880()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_19_StandScrubPause_43D8A0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_20_IdleToStandScrub_43D8D0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_21_StandScrubToIdle_43D8F0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_22_CrouchScrub_43D910()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_23_CrouchIdle_43E590()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_24_CrouchTurn_43E5F0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_25_StandToCrouch_43E620()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_26_CrouchToStand_43E640()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_27_RunToWalk_43D980()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_28_MidRunToWalk_43DA40()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_29_RunLoop_43DB10()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_30_RunToWalk_43DD50()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_31_MidRunToWalk_43DE10()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_32_RunSlideStop_43DEE0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_33_RunSlideTurn_43DF80()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_34_RunTurnToRun_43E070()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_35_SneakLoop_43E0F0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_36_WalkToSneak_43E240()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_37_SneakToWalk_43E2E0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_38_MidWalkToSneak_43E380()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_39_MidSneakToWalk_43E430()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_40_SneakBegin_43E4E0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_41_SneakToIdle_43E530()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_42_MidSneakToIdle_43E560()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_43_JumpBegin_43E870()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_44_JumpMid_43E960()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_45_ToRunToPortal_43EB00()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_46_FallLandDie_43E660()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_47_Knockback_43E730()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_48_KnockbackGetUp_43E7D0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_49_FallOfEdge_43E800()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_50_LandSoft_43E820()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_51_Fall_43D0D0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_52_Chant_43D520()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_53_ChantEnd_43D640()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_54_ToDuck_43EB70()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_55_Duck_43EB90()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_56_DuckToCrouch_43EBC0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_57_Struggle_43EBE0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_58_StruggleToCrouchChant_43EC00()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_59_CrouchChant_43EC20()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_60_CrouchChantToSruggle_43ED50()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_61_DuckKnockback_43E6E0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::State_62_Choke_43ED70()
{
    NOT_IMPLEMENTED();
}

END_NS_AO

