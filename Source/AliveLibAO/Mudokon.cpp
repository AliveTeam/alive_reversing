#include "stdafx_ao.h"
#include "Function.hpp"
#include "Mudokon.hpp"
#include "GameSpeak.hpp"
#include "ResourceManager.hpp"
#include "Shadow.hpp"
#include "stdlib.hpp"
#include "Collisions.hpp"
#include "Game.hpp"
#include "Abe.hpp"
#include "Midi.hpp"
#include "BirdPortal.hpp"
#include "LiftPoint.hpp"
#include "DDCheat.hpp"
#include "Events.hpp"
#include "Gibs.hpp"
#include "Blood.hpp"
#include "Math.hpp"
#include "Bullet.hpp"
#include "Sfx.hpp"
#include "MusicTrigger.hpp"
#include "SwitchStates.hpp"
#include "Particle.hpp"
#include "CameraSwapper.hpp"
#include "Lever.hpp"
#include "ScreenShake.hpp"
#include "PsxDisplay.hpp"
#include "PossessionFlicker.hpp"
#include "AbilityRing.hpp"
#include "CheatController.hpp"
#include "Sys_common.hpp"
#include "magic_enum/include/magic_enum.hpp"

void Mud_ForceLink()
{ }

namespace AO {

using TMudStateFunction = decltype(&Mudokon::Motion_0_Idle_43CA70);

#define MAKE_STRINGS(VAR) #VAR,
const char_type* const sMudStateNames[] = {
    MUD_STATES_ENUM(MAKE_STRINGS)};

const TMudStateFunction gMudMotions_4CD470[] = {
    &Mudokon::Motion_0_Idle_43CA70,
    &Mudokon::Motion_1_WalkLoop_43CC80,
    &Mudokon::Motion_2_StandingTurn_43D050,

    // All the same func
    &Mudokon::Motion_3_Speak_43D440,
    &Mudokon::Motion_4_Speak_43D440,
    &Mudokon::Motion_5_Speak_43D440,
    &Mudokon::Motion_6_Speak_43D440,

    &Mudokon::Motion_7_WalkBegin_43CE60,
    &Mudokon::Motion_8_WalkToIdle_43CEF0,
    &Mudokon::Motion_9_MidWalkToIdle_43CFA0,
    &Mudokon::Motion_10_Unused_43D4D0,
    &Mudokon::Motion_11_Null_43D350,
    &Mudokon::Motion_12_LiftUse_43D360,
    &Mudokon::Motion_13_LiftGrabBegin_43D3F0,
    &Mudokon::Motion_14_LiftGrabEnd_43D420,
    &Mudokon::Motion_15_LeverUse_43D4B0,
    &Mudokon::Motion_16_StandScrubLoop_43D7C0,
    &Mudokon::Motion_17_StandScrubLoopToPause_43D860,
    &Mudokon::Motion_18_StandScrubPauseToLoop_43D880,
    &Mudokon::Motion_19_StandScrubPause_43D8A0,
    &Mudokon::Motion_20_IdleToStandScrub_43D8D0,
    &Mudokon::Motion_21_StandScrubToIdle_43D8F0,
    &Mudokon::Motion_22_CrouchScrub_43D910,
    &Mudokon::Motion_23_CrouchIdle_43E590,
    &Mudokon::Motion_24_CrouchTurn_43E5F0,
    &Mudokon::Motion_25_StandToCrouch_43E620,
    &Mudokon::Motion_26_CrouchToStand_43E640,
    &Mudokon::Motion_27_WalkToRun_43D980,
    &Mudokon::Motion_28_MidWalkToRun_43DA40,
    &Mudokon::Motion_29_RunLoop_43DB10,
    &Mudokon::Motion_30_RunToWalk_43DD50,
    &Mudokon::Motion_31_MidRunToWalk_43DE10,
    &Mudokon::Motion_32_RunSlideStop_43DEE0,
    &Mudokon::Motion_33_RunSlideTurn_43DF80,
    &Mudokon::Motion_34_RunTurnToRun_43E070,
    &Mudokon::Motion_35_SneakLoop_43E0F0,
    &Mudokon::Motion_36_MidWalkToSneak_43E240,
    &Mudokon::Motion_37_SneakToWalk_43E2E0,
    &Mudokon::Motion_38_WalkToSneak_43E380,
    &Mudokon::Motion_39_MidSneakToWalk_43E430,
    &Mudokon::Motion_40_SneakBegin_43E4E0,
    &Mudokon::Motion_41_SneakToIdle_43E530,
    &Mudokon::Motion_42_MidSneakToIdle_43E560,
    &Mudokon::Motion_43_RunJumpBegin_43E870,
    &Mudokon::Motion_44_RunJumpMid_43E960,
    &Mudokon::Motion_45_StandToRun_43EB00,
    &Mudokon::Motion_46_FallLandDie_43E660,
    &Mudokon::Motion_47_Knockback_43E730,
    &Mudokon::Motion_48_KnockbackGetUp_43E7D0,
    &Mudokon::Motion_49_WalkOffEdge_43E800,
    &Mudokon::Motion_50_LandSoft_43E820,
    &Mudokon::Motion_51_Fall_43D0D0,
    &Mudokon::Motion_52_Chant_43D520,
    &Mudokon::Motion_53_ChantEnd_43D640,
    &Mudokon::Motion_54_ToDuck_43EB70,
    &Mudokon::Motion_55_Duck_43EB90,
    &Mudokon::Motion_56_DuckToCrouch_43EBC0,
    &Mudokon::Motion_57_Struggle_43EBE0,
    &Mudokon::Motion_58_StruggleToCrouchChant_43EC00,
    &Mudokon::Motion_59_CrouchChant_43EC20,
    &Mudokon::Motion_60_CrouchChantToSruggle_43ED50,
    &Mudokon::Motion_61_DuckKnockback_43E6E0,
    &Mudokon::Motion_62_PoisonGasDeath_43ED70,
};

using TMudBrain = decltype(&Mudokon::Brain_ComingIn_0_441DE0);

const TMudBrain gMudBrains_4CD430[] = {
    &Mudokon::Brain_ComingIn_0_441DE0,
    &Mudokon::Brain_ComingOut_1_441E90,
    &Mudokon::Brain_SingSequenceIdle_2_441CA0,
    &Mudokon::Brain_SingSequenceSing_3_441510,
    &Mudokon::Brain_SingSequencePassword_4_441260,
    &Mudokon::Brain_LiftUse_5_43C180,
    &Mudokon::Brain_LeverUse_6_43C250,
    &Mudokon::Brain_GiveRings_7_43C2F0,
    &Mudokon::Brain_StandScrub_8_441F40,
    &Mudokon::Brain_CrouchScrub_9_4422A0,
    &Mudokon::Brain_ListeningToAbe_10_440300,
    &Mudokon::Brain_ShrivelDeath_11_43C5F0,
    &Mudokon::Brain_Escape_12_440FD0,
    &Mudokon::Brain_FallAndSmackDeath_13_43C700,
    &Mudokon::Brain_Chant_14_442710,
    &Mudokon::Brain_Choke_15_43C5D0,
};


static const TintEntry sMudTints_4CD320[] = {
    {LevelIds_s8::eStockYards_5, 25u, 25u, 25u},
    {LevelIds_s8::eStockYardsReturn_6, 25u, 25u, 25u},
    {LevelIds_s8::eNone, 87u, 103u, 67u}};

ALIVE_VAR(1, 0x507B90, s16, sAlertedMudCount_507B90, 0);
ALIVE_VAR(1, 0x507B94, s16, sMudRunningToPortalCount_507B94, 0);

Mudokon* Mudokon::ctor_43EED0(Path_TLV* pTlv, s32 tlvInfo)
{
    ctor_401090();
    SetVTable(this, 0x4BBB28);
    field_4_typeId = Types::eMudokon_52;

    field_128 = -1;
    field_13E = -1;
    field_FE_next_motion = -1;
    field_13C = 0;
    field_1C0_timer = 0;
    field_1B8_brain_idx = 0;
    field_1BA_brain_sub_state = 0;
    field_EC = 3;
    field_1BC = 0;
    field_124_voice_pitch = 0;
    field_148_res_array = {};

    const AnimRecord& mudRec = AO::AnimRec(AnimId::Mudokon_Idle);
    field_148_res_array.res[0] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, mudRec.mResourceId, 1, 0);
    Animation_Init_417FD0(mudRec.mFrameTableOffset, mudRec.mMaxW, mudRec.mMaxH, field_148_res_array.res[0], 1);

    field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);

    field_10A_flags.Set(Flags_10A::e10A_Bit4_SetOffExplosives);
    field_10A_flags.Set(Flags_10A::e10A_Bit6);

    field_144_flags.Clear(Flags_144::e144_Bit6_bPersist);
    field_144_flags.Clear(Flags_144::e144_Bit7);
    field_144_flags.Clear(Flags_144::e144_Bit8);
    field_144_flags.Clear(Flags_144::e144_Bit9);
    field_144_flags.Clear(Flags_144::e144_Bit10_give_ring_without_password);

    field_1B6 = 0;
    field_198_abe_must_face_mud = 0;

    SetTint_418750(sMudTints_4CD320, field_B2_lvl_number);

    Scale_short scale = Scale_short::eFull_0;
    switch (pTlv->field_4_type.mType)
    {
        case TlvTypes::None_m1:
            LOG_ERROR("Mudokon ctor pTlv->field_4_type.mType was None_m1. This shouldn't happen.");
            break;
        case TlvTypes::LiftMudokon_32:
        {
            auto liftMudTlv = static_cast<Path_LiftMudokon*>(pTlv);

            field_148_res_array.res[1] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kAbeliftAOResID, 1, 0);


            field_18C_how_far_to_walk = FP_FromInteger(liftMudTlv->field_18_how_far_to_walk);
            field_110_lift_switch_id = liftMudTlv->field_1A_lift_switch_id;

            field_144_flags.Set(Flags_144::e144_Bit5_unused, liftMudTlv->field_1C_direction == Path_LiftMudokon::Direction::eLeft_1);
            field_144_flags.Clear(Flags_144::e144_Bit4_bSnapToGrid);
            field_144_flags.Clear(Flags_144::e144_Bit11_bDeaf);

            field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX, liftMudTlv->field_1C_direction == Path_LiftMudokon::Direction::eLeft_1);

            field_186_give_password = liftMudTlv->field_1E_give_password;
            field_184 = 1;
            field_1B8_brain_idx = 0;
            field_188 = 5;

            field_1A4_code_converted = Code_Convert_476000(liftMudTlv->field_22_code1, liftMudTlv->field_24_code2);
            field_1A8_code_length = Code_Length_475FD0(field_1A4_code_converted);

            scale = liftMudTlv->field_20_scale;
        }
        break;

        case TlvTypes::RingMudokon_50:
        {
            auto ringMudTlv = static_cast<Path_RingMudokon*>(pTlv);

            field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX, ringMudTlv->field_18_direction == XDirection_short::eLeft_0); // TODO: Check
            field_184 = 0;
            field_186_give_password = ringMudTlv->field_1E_give_password;
            field_198_abe_must_face_mud = ringMudTlv->field_1A_abe_must_face_mud == Path_RingMudokon::MustFaceMud::eYes_0;
            field_1B8_brain_idx = 2;

            if (ringMudTlv->field_24_action == SwitchOp::eSetTrue_0)
            {
                // Pull switch
                field_148_res_array.res[2] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kAbepullAOResID, 1, 0);
                field_188 = 6;
            }
            else
            {
                // Give ring
                field_148_res_array.res[9] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kAbeommAOResID, 1, 0);
                field_188 = 7;
            }

            field_1AA_ring_timeout = ringMudTlv->field_26_ring_timeout;
            field_1A4_code_converted = Code_Convert_476000(ringMudTlv->field_20_code1, ringMudTlv->field_22_code2);
            field_1A8_code_length = Code_Length_475FD0(field_1A4_code_converted);

            field_144_flags.Set(Flags_144::e144_Bit10_give_ring_without_password, ringMudTlv->field_28_give_ring_without_password == Choice_short::eYes_1);
            field_144_flags.Clear(Flags_144::e144_Bit4_bSnapToGrid);
            field_144_flags.Clear(Flags_144::e144_Bit11_bDeaf);


            scale = ringMudTlv->field_1C_scale;
        }
        break;

        case TlvTypes::Mudokon_82:
        {
            auto mudTlv = static_cast<Path_Mudokon*>(pTlv);

            if (mudTlv->field_1A_job == Path_Mudokon::MudJobs::eStandScrub_0)
            {
                field_1B8_brain_idx = 8;
                field_148_res_array.res[3] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kMudchslAOResID, 1, 0);
            }
            else if (mudTlv->field_1A_job == Path_Mudokon::MudJobs::eSitScrub_1)
            {
                field_1B8_brain_idx = 9;
            }
            else if (mudTlv->field_1A_job == Path_Mudokon::MudJobs::eSitChant_2)
            {
                field_1B8_brain_idx = 14;
                field_148_res_array.res[12] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kMudltusAOResID, 1, 0);
            }

            field_148_res_array.res[10] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kMudoduckAOResID, 1, 0);
            field_148_res_array.res[13] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kMudbtlnkAOResID, 1, 0);
            field_148_res_array.res[4] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kMudscrubAOResID, 1, 0);
            field_148_res_array.res[5] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kAbebasicAOResID, 1, 0);
            field_148_res_array.res[6] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kAbeknfdAOResID, 1, 0);
            field_148_res_array.res[7] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kAbeknbkAOResID, 1, 0);
            field_148_res_array.res[8] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kAbeedgeAOResID, 1, 0);
            field_148_res_array.res[14] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kAbegasAOResID, 1, 0);


            field_124_voice_pitch = mudTlv->field_1E_voice_pitch;
            field_4_typeId = Types::eMudokon_75;
            field_1B2_rescue_switch_id = mudTlv->field_20_rescue_switch_id;

            field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX, mudTlv->field_1C_direction == XDirection_short::eLeft_0);

            // TODO: Check these as well
            field_144_flags.Set(Flags_144::e144_Bit11_bDeaf, mudTlv->field_22_deaf == Choice_short::eYes_1);
            field_144_flags.Set(Flags_144::e144_Bit6_bPersist, mudTlv->field_26_persist & 1);
            field_144_flags.Set(Flags_144::e144_Bit4_bSnapToGrid);

            field_1B4_idle_time = 0;

            scale = mudTlv->field_18_scale;
        }
        break;
        default:
            LOG_WARNING("Mudokon ctor pTlv->field_4_type.mType was " << magic_enum::enum_name(pTlv->field_4_type.mType) << ". This is unhandled.");
            break;
    }

    field_FC_current_motion = eMudMotions::Motion_0_Idle_43CA70;

    if (scale == Scale_short::eHalf_1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_10_anim.field_C_layer = Layer::eLayer_AbeMenu_Half_13;
        field_C6_scale = 0;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_10_anim.field_C_layer = Layer::eLayer_AbeMenu_32;
        field_C6_scale = 1;
    }

    FP hitX = {};
    FP hitY = {};
    const s16 bHit = sCollisions_DArray_504C6C->RayCast_40C410(
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
        if (field_F4_pLine->field_8_type == eLineTypes::eUnknown_32 || field_F4_pLine->field_8_type == eLineTypes::eUnknown_36)
        {
            PSX_RECT bRect = {};
            VGetBoundingRect(&bRect, 1);
            VOnCollisionWith(
                PSX_Point{bRect.x, static_cast<s16>(bRect.y + 5)},
                PSX_Point{bRect.w, static_cast<s16>(bRect.h + 5)},
                ObjListPlatforms_50766C,
                1,
                (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_401C10);
        }
    }

    field_E8_LastLineYPos = field_AC_ypos;

    if (field_1B8_brain_idx == 9 || field_1B8_brain_idx == 8)
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
    field_1BA_brain_sub_state = 0;
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

BaseGameObject* Mudokon::dtor_43F6A0()
{
    SetVTable(this, 0x4BBB28);

    if (field_100_health <= FP_FromInteger(0))
    {
        sKilledMudokons_5076BC++;
    }

    RemoveAlerted();

    KillBirdPortal();
    KillLiftPoint_194();

    if (!field_144_flags.Get(Flags_144::e144_Bit2) || field_100_health <= FP_FromInteger(0) || field_10A_flags.Get(Flags_10A::e10A_Bit5_Electrocuted))
    {
        gMap_507BA8.TLV_Reset_446870(field_10C, -1, 0, 1);
    }
    else
    {
        gMap_507BA8.TLV_Reset_446870(field_10C, -1, 0, 0);
    }

    for (auto& res : field_148_res_array.res)
    {
        if (res && res != field_10_anim.field_20_ppBlock)
        {
            ResourceManager::FreeResource_455550(res);
        }
    }

    if (field_1B8_brain_idx == 7)
    {
        if (field_1AA_ring_timeout)
        {
            sActiveHero_507678->field_168_ring_pulse_timer = field_1AA_ring_timeout + gnFrameCount_507670;
        }
        else
        {
            sActiveHero_507678->field_168_ring_pulse_timer = gnFrameCount_507670 + 200000;
        }
        sActiveHero_507678->field_16C_bHaveShrykull = FALSE;
    }

    if (field_FC_current_motion == eMudMotions::Motion_52_Chant_43D520 || field_FC_current_motion == eMudMotions::Motion_59_CrouchChant_43EC20)
    {
        SND_Seq_Stop_477A60(SeqId::eMudokonChant_12);
    }

    return dtor_401000();
}

void Mudokon::KillLiftPoint_194()
{
    if (field_194_pLiftPoint)
    {
        field_194_pLiftPoint->field_C_refCount--;
        field_194_pLiftPoint = nullptr;
    }
}

BaseGameObject* Mudokon::VDestructor(s32 flags)
{
    return Vdtor_440230(flags);
}

Mudokon* Mudokon::Vdtor_440230(s32 flags)
{
    dtor_43F6A0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void Mudokon::VUpdate()
{
    VUpdate_43F560();
}

void Mudokon::VUpdate_43F560()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        return;
    }

    if (field_1C4_bDoPathTrans)
    {
        DoPathTrans_43FE00();
        field_1C4_bDoPathTrans = FALSE;
    }

    const auto old_motion = field_FC_current_motion;
    const auto old_brain = field_1B8_brain_idx;
    const auto old_brain_sub_state = field_1BA_brain_sub_state;
    field_1BA_brain_sub_state = (this->*gMudBrains_4CD430[field_1B8_brain_idx])();

    if (old_brain != field_1B8_brain_idx)
    {
        // Main brain state changed
        //LOG_INFO("Brain changed from " << old_brain << " to " << field_1B8_brain_idx);
    }
    else if (old_brain_sub_state != field_1BA_brain_sub_state)
    {
        //LOG_INFO("Brain sub state changed from " << old_brain_sub_state << " to " << field_1BA_brain_sub_state);
    }

    const FP oldX = field_A8_xpos;
    const FP oldY = field_AC_ypos;
    (this->*gMudMotions_4CD470[field_FC_current_motion])();

    if (oldX != field_A8_xpos || oldY != field_AC_ypos)
    {
        field_F0_pTlv = gMap_507BA8.TLV_Get_At_446060(
            0,
            field_A8_xpos,
            field_AC_ypos,
            field_A8_xpos,
            field_AC_ypos);
        VOn_TLV_Collision(field_F0_pTlv);
    }

    if (old_motion != field_FC_current_motion || field_108_bMotionChanged)
    {
        field_108_bMotionChanged = FALSE;
        VUpdateResBlock_43EDB0();

        if (old_motion == eMudMotions::Motion_10_Unused_43D4D0)
        {
            field_10_anim.SetFrame_402AC0(field_E6_last_anim_frame);
        }
    }
    else if (field_1BC)
    {
        field_FC_current_motion = field_E4_previous_motion;
        VUpdateResBlock_43EDB0();
        field_10_anim.SetFrame_402AC0(field_E6_last_anim_frame);
        field_1BC = 0;
    }

    if (field_10A_flags.Get(Flags_10A::e10A_Bit5_Electrocuted))
    {
        Event_Broadcast_417220(kEventMudokonDead_15, sActiveHero_507678);
    }
}

const AnimId sMudFrameTables_4CD330[64] = {
    AnimId::Mudokon_Idle,
    AnimId::Mudokon_Walk,
    AnimId::Mudokon_StandingTurn,
    AnimId::Mudokon_Speak1,
    AnimId::Mudokon_Speak2,
    AnimId::Mudokon_Speak3,
    AnimId::Mudokon_SpeakFart,
    AnimId::Mudokon_WalkBegin,
    AnimId::Mudokon_WalkToIdle,
    AnimId::Mudokon_MidWalkToIdle,
    AnimId::Mudokon_Idle,
    AnimId::Mudokon_AO_Null,
    AnimId::Mudokon_AO_LiftUse,
    AnimId::Mudokon_LiftGrabBegin,
    AnimId::Mudokon_LiftGrabEnd,
    AnimId::Mudokon_LeverUse,
    AnimId::Mudokon_StandScrubLoop,
    AnimId::Mudokon_StandScrubLoopToPause,
    AnimId::Mudokon_StandScrubPauseToLoop,
    AnimId::Mudokon_StandScrubPause,
    AnimId::Mudokon_IdleToStandScrub,
    AnimId::Mudokon_StandScrubToIdle,
    AnimId::Mudokon_CrouchScrub,
    AnimId::Mudokon_CrouchIdle,
    AnimId::Mudokon_CrouchTurn,
    AnimId::Mudokon_StandToCrouch,
    AnimId::Mudokon_CrouchToStand,
    AnimId::Mudokon_WalkToRun,
    AnimId::Mudokon_MidWalkToRun,
    AnimId::Mudokon_Run,
    AnimId::Mudokon_RunToWalk,
    AnimId::Mudokon_MidRunToWalk,
    AnimId::Mudokon_RunSlideStop,
    AnimId::Mudokon_RunSlideTurn,
    AnimId::Mudokon_RunTurnToRun,
    AnimId::Mudokon_Sneak,
    AnimId::Mudokon_WalkToSneak,
    AnimId::Mudokon_SneakToWalk,
    AnimId::Mudokon_MidWalkToSneak,
    AnimId::Mudokon_MidSneakToWalk,
    AnimId::Mudokon_SneakBegin,
    AnimId::Mudokon_SneakToIdle,
    AnimId::Mudokon_MidSneakToIdle,
    AnimId::Mudokon_RunJumpBegin,
    AnimId::Mudokon_RunJumpMid,
    AnimId::Mudokon_StandToRun,
    AnimId::Mudokon_KnockForward,
    AnimId::Mudokon_Knockback,
    AnimId::Mudokon_KnockbackGetUp,
    AnimId::Mudokon_WalkOffEdge,
    AnimId::Mudokon_AO_LandSoft_Long,
    AnimId::Mudokon_FallingFromGrab, // check/rename
    AnimId::Mudokon_Chant,
    AnimId::Mudokon_ChantEnd,
    AnimId::Mudokon_ToDuck,
    AnimId::Mudokon_Duck,
    AnimId::Mudokon_DuckToCrouch,
    AnimId::Mudokon_Struggle,
    AnimId::Mudokon_StruggleToCrouchChant,
    AnimId::Mudokon_CrouchChant,
    AnimId::Mudokon_CrouchChantToStruggle,
    AnimId::Mudokon_DuckKnockback,
    AnimId::Mudokon_PoisonGasDeath,
    AnimId::None};

void Mudokon::VUpdateResBlock_43EDB0()
{
    const AnimRecord& rec = AO::AnimRec(sMudFrameTables_4CD330[field_FC_current_motion]);
    field_10_anim.Set_Animation_Data_402A40(
        rec.mFrameTableOffset,
        GetResBlockForMotion_43EDE0(field_FC_current_motion));
}

void Mudokon::VScreenChanged()
{
    VScreenChanged_43FFC0();
}

void Mudokon::VScreenChanged_43FFC0()
{
    // Map/overlay changed or mud shouldn't persist
    if (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level || !field_144_flags.Get(Flags_144::e144_Bit6_bPersist))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        KillBirdPortal();
        KillLiftPoint_194();
        return;
    }

    // Path changed?
    if (gMap_507BA8.field_2_current_path != gMap_507BA8.field_C_path)
    {
        // See if we need to go to the next path
        auto pTlv = gMap_507BA8.TLV_Get_At_446060(nullptr, field_A8_xpos, field_AC_ypos, field_A8_xpos, field_AC_ypos);
        while (pTlv)
        {
            if (pTlv->field_4_type == TlvTypes::MudokonPathTrans_89)
            {
                // Gonna go to the next path
                field_1C4_bDoPathTrans = TRUE;
                return;
            }
            pTlv = gMap_507BA8.TLV_Get_At_446060(pTlv, field_A8_xpos, field_AC_ypos, field_A8_xpos, field_AC_ypos);
        }

        // Wasn't a path trans and path changed, die
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        KillBirdPortal();
        KillLiftPoint_194();
    }
}

s16 Mudokon::VTakeDamage(BaseGameObject* pFrom)
{
    return VTakeDamage_43F830(pFrom);
}

s16 Mudokon::VTakeDamage_43F830(BaseGameObject* pFrom)
{
    switch (pFrom->field_4_typeId)
    {
        case Types::eGasClock_16:
            if (field_100_health > FP_FromInteger(0))
            {
                field_1B8_brain_idx = 15;
                field_1BA_brain_sub_state = 0;
            }
            return 1;

        case Types::eElectricWall_25:
            Mudokon_SFX_42A4D0(MudSounds::eDeathDropScream_17, 0, 0, this);
            return 1;

        case Types::eBaseBomb_30:
        case Types::eMeatSaw_56:
        case Types::eExplosion_74:
            if (field_100_health > FP_FromInteger(0))
            {
                field_100_health = FP_FromInteger(0);

                auto pGibs = ao_new<Gibs>();
                if (pGibs)
                {
                    pGibs->ctor_407B20(
                        GibType::Mud_4,
                        field_A8_xpos,
                        field_AC_ypos,
                        FP_FromInteger(0),
                        FP_FromInteger(0),
                        field_BC_sprite_scale);
                }

                pGibs = ao_new<Gibs>();
                if (pGibs)
                {
                    pGibs->ctor_407B20(
                        GibType::Mud_4,
                        field_A8_xpos,
                        field_AC_ypos,
                        FP_FromInteger(0),
                        FP_FromInteger(0),
                        field_BC_sprite_scale);
                }

                field_6_flags.Set(BaseGameObject::eDead_Bit3);
                Event_Broadcast_417220(kEventMudokonDead_15, sActiveHero_507678);
            }
            return 1;

        case Types::eAbilityRing_69:
            return 0;

        case Types::eBullet_10:
            field_106_shot = TRUE;
            if (field_100_health > FP_FromInteger(0))
            {
                auto pBullet = static_cast<Bullet*>(pFrom);
                FP bloodXOff = {};
                if (pBullet->field_20_x_distance <= FP_FromInteger(0))
                {
                    bloodXOff = FP_FromInteger(-24);
                }
                else
                {
                    bloodXOff = FP_FromInteger(24);
                }

                auto pBlood = ao_new<Blood>();
                if (pBlood)
                {
                    pBlood->ctor_4072B0(
                        field_A8_xpos,
                        pBullet->field_1C_ypos,
                        bloodXOff,
                        FP_FromInteger(0),
                        field_BC_sprite_scale,
                        50);
                }

                Event_Broadcast_417220(kEventMudokonDead_15, sActiveHero_507678);
                Event_Broadcast_417220(kEventMudokonDead_15, sActiveHero_507678);
                return DoSmashDamage();
            }
            return 1;

        case Types::eRockSpawner_32:
            Event_Broadcast_417220(kEventMudokonDead_15, sActiveHero_507678);
            return DoSmashDamage();

        case Types::eScrab_77:
            return DoSmashDamage();

        case Types::eShrykull_85:
        case Types::eElectrocute_103:
            if (field_100_health > FP_FromInteger(0))
            {
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
                field_100_health = FP_FromInteger(0);
            }
            return 1;

        case Types::eSlog_89:
            if (field_100_health > FP_FromInteger(0))
            {
                field_144_flags.Clear(Flags_144::e144_Bit6_bPersist);
                field_100_health = FP_FromInteger(0);
                field_1B8_brain_idx = 11;
                field_1C0_timer = gnFrameCount_507670 + 90;
                field_FC_current_motion = eMudMotions::Motion_46_FallLandDie_43E660;
                VUpdateResBlock_43EDB0();
            }
            return 1;

        case Types::eBeeSwarm_95:
            if (field_100_health > FP_FromInteger(0))
            {
                field_100_health -= FP_FromDouble(0.15);
                if (field_100_health > FP_FromInteger(0))
                {
                    Mudokon_SFX_42A4D0(Math_RandomRange_450F20(0, 127) >= 64 ? MudSounds::eBeesStruggle_18 : MudSounds::eKnockbackOuch_10, 0, 2 * Math_NextRandom() - 256, 0);
                    field_1B6 = 1;
                }
                else
                {
                    Mudokon_SFX_42A4D0(MudSounds::eKnockbackOuch_10, 0, 1000, this);
                    Environment_SFX_42A220(EnvironmentSfx::eDeathNoise_7, 0, 0x7FFF, this);
                    field_144_flags.Clear(Flags_144::e144_Bit6_bPersist);
                    field_100_health = FP_FromInteger(0);
                    field_1B8_brain_idx = 11;
                    field_1C0_timer = gnFrameCount_507670 + 90;
                    field_FC_current_motion = eMudMotions::Motion_46_FallLandDie_43E660;
                    field_FE_next_motion = -1;
                    VUpdateResBlock_43EDB0();
                }
            }
            return 1;

        default:
            if (field_100_health > FP_FromInteger(0))
            {
                Mudokon_SFX_42A4D0(Math_RandomRange_450F20(0, 127) >= 64 ? MudSounds::eBeesStruggle_18 : MudSounds::eKnockbackOuch_10, 127, 2 * Math_NextRandom() - 256, 0);
                field_100_health -= FP_FromDouble(0.06);
                field_FC_current_motion = eMudMotions::Motion_46_FallLandDie_43E660;
                field_FE_next_motion = -1;
                field_1C0_timer = gnFrameCount_507670 + 30;
                VUpdateResBlock_43EDB0();
                field_1B8_brain_idx = 9;
                field_1BA_brain_sub_state = 6;

                RemoveAlerted();

                if (field_100_health <= FP_FromInteger(0))
                {
                    field_144_flags.Clear(Flags_144::e144_Bit6_bPersist);
                    field_1B8_brain_idx = 11;
                    field_1C0_timer = gnFrameCount_507670 + 90;
                }
            }
            return 1;
    }
}

s16 Mudokon::DoSmashDamage()
{
    if (field_100_health > FP_FromInteger(0))
    {
        field_144_flags.Clear(Flags_144::e144_Bit6_bPersist);
        field_1C0_timer = gnFrameCount_507670 + 90;
        field_100_health = FP_FromInteger(0);
        field_1B8_brain_idx = 11;
        field_FC_current_motion = eMudMotions::Motion_46_FallLandDie_43E660;
        field_FE_next_motion = -1;
        VUpdateResBlock_43EDB0();
        Mudokon_SFX_42A4D0(MudSounds::eKnockbackOuch_10, 0, Math_RandomRange_450F20(-127, 127), this);
        return 1;
    }
    return 0;
}

void Mudokon::KillBirdPortal()
{
    if (field_1AC_pBirdPortal)
    {
        sMudRunningToPortalCount_507B94--;
        if (sMudRunningToPortalCount_507B94 == 0)
        {
            field_1AC_pBirdPortal->VKillPortalClipper();
            field_1AC_pBirdPortal->VGiveShrukull(1);
        }

        field_1AC_pBirdPortal->field_C_refCount--;
        field_1AC_pBirdPortal = nullptr;
    }
}

u8** Mudokon::GetResBlockForMotion_43EDE0(s16 motion)
{
    if (motion < eMudMotions::Motion_11_Null_43D350)
    {
        return field_148_res_array.res[0];
    }
    if (motion < eMudMotions::Motion_15_LeverUse_43D4B0)
    {
        return field_148_res_array.res[1];
    }
    if (motion < eMudMotions::Motion_16_StandScrubLoop_43D7C0)
    {
        return field_148_res_array.res[2];
    }
    if (motion < eMudMotions::Motion_22_CrouchScrub_43D910)
    {
        return field_148_res_array.res[3];
    }
    if (motion < eMudMotions::Motion_23_CrouchIdle_43E590)
    {
        return field_148_res_array.res[4];
    }
    if (motion < eMudMotions::Motion_46_FallLandDie_43E660)
    {
        return field_148_res_array.res[5];
    }
    if (motion < eMudMotions::Motion_47_Knockback_43E730)
    {
        return field_148_res_array.res[6];
    }
    if (motion < eMudMotions::Motion_49_WalkOffEdge_43E800)
    {
        return field_148_res_array.res[7];
    }
    if (motion < eMudMotions::Motion_52_Chant_43D520)
    {
        return field_148_res_array.res[8];
    }
    if (motion < eMudMotions::Motion_54_ToDuck_43EB70)
    {
        return field_148_res_array.res[9];
    }
    if (motion < eMudMotions::Motion_57_Struggle_43EBE0)
    {
        return field_148_res_array.res[10];
    }
    if (motion < eMudMotions::Motion_58_StruggleToCrouchChant_43EC00)
    {
        return field_148_res_array.res[11];
    }
    if (motion < eMudMotions::Motion_61_DuckKnockback_43E6E0)
    {
        return field_148_res_array.res[12];
    }
    if (motion < eMudMotions::Motion_62_PoisonGasDeath_43ED70)
    {
        return field_148_res_array.res[13];
    }

    if (motion >= 63)
    {
        return nullptr;
    }

    return field_148_res_array.res[14];
}

void Mudokon::DoPathTrans_43FE00()
{
    PSX_Point camCoords = {};
    gMap_507BA8.GetCurrentCamCoords_444890(&camCoords);

    if (sActiveHero_507678->field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_A8_xpos = FP_FromInteger((camCoords.field_0_x + XGrid_Index_To_XPos_41FA60(field_BC_sprite_scale, MaxGridBlocks_41FA10(field_BC_sprite_scale)))) + ScaleToGridSize_41FA30(field_BC_sprite_scale);
    }
    else
    {
        field_A8_xpos = (FP_FromInteger(camCoords.field_0_x + XGrid_Index_To_XPos_41FA60(field_BC_sprite_scale, 0))) - ScaleToGridSize_41FA30(field_BC_sprite_scale);
    }

    if (sActiveHero_507678->field_F4_pLine)
    {
        field_AC_ypos = sActiveHero_507678->field_AC_ypos;
    }
    else
    {
        field_AC_ypos = FP_FromInteger(FP_GetExponent(field_AC_ypos) % 480 + camCoords.field_2_y);
    }

    if (field_F4_pLine)
    {
        if (field_F4_pLine->field_8_type == eLineTypes::eUnknown_32)
        {
            field_F8_pLiftPoint->VRemove(this);
            field_F8_pLiftPoint->field_C_refCount--;
            field_F8_pLiftPoint = nullptr;
        }

        PathLine* pLine = nullptr;
        FP hitY = {};
        FP hitX = {};
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
    field_8_update_delay = 20;
    field_B2_lvl_number = gMap_507BA8.field_0_current_level;
    field_B0_path_number = gMap_507BA8.field_2_current_path;
}

void Mudokon::ToStand_43CA40()
{
    field_10_anim.field_4_flags.Clear(AnimFlags::eBit6_FlipY);
    field_10_anim.field_4_flags.Clear(AnimFlags::eBit7_SwapXY);
    field_11C = FP_FromInteger(0);
    field_120_unused = 0;
    field_B4_velx = FP_FromInteger(0);
    field_B8_vely = FP_FromInteger(0);
    field_FC_current_motion = eMudMotions::Motion_0_Idle_43CA70;
}

void Mudokon::CheckFloorGone_43C9B0()
{
    if (field_F8_pLiftPoint)
    {
        if (field_F8_pLiftPoint->field_6_flags.Get(BaseGameObject::eDead_Bit3))
        {
            VOnTrapDoorOpen();
            field_144_flags.Set(Flags_144::e144_Bit8);
        }
    }
    else
    {
        if (field_144_flags.Get(Flags_144::e144_Bit8))
        {
            Motion_51_Fall_43D0D0();
        }
    }
}

s16 CC Mudokon::IsAbeSneaking_43D660(Mudokon* /*pMud*/)
{
    return sActiveHero_507678->field_FC_current_motion == eAbeMotions::Motion_42_SneakLoop_424BB0
        || sActiveHero_507678->field_FC_current_motion == eAbeMotions::Motion_47_SneakBegin_424ED0
        || sActiveHero_507678->field_FC_current_motion == eAbeMotions::Motion_43_WalkToSneak_424790
        || sActiveHero_507678->field_FC_current_motion == eAbeMotions::Motion_45_MidWalkToSneak_424890
        || sActiveHero_507678->field_FC_current_motion == eAbeMotions::Motion_44_SneakToWalk_4249A0
        || sActiveHero_507678->field_FC_current_motion == eAbeMotions::Motion_46_MidSneakToWalk_424AA0;
}

void Mudokon::ToKnockback_43D6E0()
{
    Environment_SFX_42A220(EnvironmentSfx::eKnockback_13, 0, 0x7FFF, this);

    if (field_B4_velx > FP_FromInteger(0))
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_A8_xpos += field_B4_velx + (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(2));
        }
        else
        {
            field_A8_xpos += field_B4_velx - (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(2));
        }
    }
    MapFollowMe_401D30(field_144_flags.Get(Flags_144::e144_Bit4_bSnapToGrid));

    field_B4_velx = FP_FromInteger(0);

    if (field_B8_vely < FP_FromInteger(0))
    {
        field_B8_vely = FP_FromInteger(0);
    }

    Environment_SFX_42A220(EnvironmentSfx::eKnockback_13, 0, 0x7FFF, this);
    field_FC_current_motion = eMudMotions::Motion_47_Knockback_43E730;
}

void Mudokon::ReduceXVelocityBy_43C920(FP amount)
{
    if (field_B4_velx > FP_FromInteger(0))
    {
        field_B4_velx -= (field_BC_sprite_scale * amount);
        if (field_B4_velx < FP_FromInteger(0))
        {
            field_B4_velx = FP_FromInteger(0);
        }
    }
    else if (field_B4_velx < FP_FromInteger(0))
    {
        field_B4_velx += (field_BC_sprite_scale * amount);
        if (field_B4_velx > FP_FromInteger(0))
        {
            field_B4_velx = FP_FromInteger(0);
        }
    }

    if (FP_GetDouble(field_B4_velx))
    {
        MoveOnLine_43C7E0();
    }
}

void Mudokon::MoveOnLine_43C7E0()
{
    CheckFloorGone_43C9B0();

    const FP old_xpos = field_A8_xpos;
    if (field_F4_pLine)
    {
        field_F4_pLine = field_F4_pLine->MoveOnLine_40CA20(
            &field_A8_xpos,
            &field_AC_ypos,
            field_B4_velx);
    }

    if (field_F4_pLine)
    {
        if (field_F8_pLiftPoint)
        {
            if (field_F4_pLine->field_8_type != eLineTypes::eUnknown_32 &&
                field_F4_pLine->field_8_type != eLineTypes::eUnknown_36)
            {
                field_F8_pLiftPoint->VRemove(this);
                field_F8_pLiftPoint->field_C_refCount--;
                field_F8_pLiftPoint = nullptr;
            }
        }
        else
        {
            if (field_F4_pLine->field_8_type == eLineTypes::eUnknown_32 ||
                field_F4_pLine->field_8_type == eLineTypes::eUnknown_36)
            {
                PSX_RECT bRect = {};
                VGetBoundingRect(&bRect, 1);
                VOnCollisionWith(
                    PSX_Point{bRect.x, static_cast<s16>(bRect.y + 5)},
                    PSX_Point{bRect.w, static_cast<s16>(bRect.h + 5)},
                    ObjListPlatforms_50766C,
                    1,
                    (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_401C10);
            }
        }
    }
    else
    {
        field_11C = FP_FromInteger(0); // TODO: is 0 wrong? check AE
        field_E8_LastLineYPos = field_AC_ypos;
        field_FC_current_motion = eMudMotions::Motion_49_WalkOffEdge_43E800;
        field_A8_xpos = old_xpos + field_B4_velx;
    }
}

s16 Mudokon::FindBirdPortal_440250()
{
    if (field_1AC_pBirdPortal)
    {
        return 0;
    }

    field_1AC_pBirdPortal = static_cast<BirdPortal*>(Event_Get_417250(kEvent_18));
    if (!field_1AC_pBirdPortal)
    {
        return 0;
    }

    if (FP_Abs(field_1AC_pBirdPortal->field_18_xpos - field_A8_xpos) < FP_FromInteger(gPsxDisplay_504C78.field_0_width) && FP_Abs(field_1AC_pBirdPortal->field_28_ypos - field_AC_ypos) < FP_FromInteger(10))
    {
        if (field_1AC_pBirdPortal->field_10_portal_type == PortalType::eWorker_1 || field_1AC_pBirdPortal->field_10_portal_type == PortalType::eShrykull_2)
        {
            sActiveHero_507678->ChangeChantState_430510(1);
            field_1AC_pBirdPortal->field_C_refCount++;
            sMudRunningToPortalCount_507B94++;
            return 1;
        }
    }
    field_1AC_pBirdPortal = nullptr;
    return 0;
}

s16 Mudokon::FacingTarget_43D6A0(BirdPortal* pTarget)
{
    if (pTarget->field_18_xpos == field_A8_xpos)
    {
        return TRUE;
    }
    else if (pTarget->field_18_xpos > field_A8_xpos && !field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        return TRUE;
    }
    else if (pTarget->field_18_xpos < field_A8_xpos && field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        return TRUE;
    }
    return FALSE;
}

GameSpeakEvents Mudokon::LastGameSpeak_4400B0()
{
    if (!gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos,
            1)
        || field_144_flags.Get(Flags_144::e144_Bit11_bDeaf))
    {
        return GameSpeakEvents::eNone_m1;
    }

    if (field_128 == pEventSystem_4FF954->field_18_last_event_index)
    {
        if (pEventSystem_4FF954->field_10_last_event == GameSpeakEvents::eNone_m1)
        {
            return GameSpeakEvents::eNone_m1;
        }
        else
        {
            return GameSpeakEvents::eSameAsLast_m2;
        }
    }

    field_128 = pEventSystem_4FF954->field_18_last_event_index;

    return pEventSystem_4FF954->field_10_last_event;
}

void Mudokon::AddAlerted()
{
    if (!field_144_flags.Get(Flags_144::e144_Bit9))
    {
        sAlertedMudCount_507B90++;
        field_144_flags.Set(Flags_144::e144_Bit9);
    }
}


void Mudokon::RemoveAlerted()
{
    if (field_144_flags.Get(Flags_144::e144_Bit9))
    {
        sAlertedMudCount_507B90--;
        field_144_flags.Clear(Flags_144::e144_Bit9);
    }
}

s16 Mudokon::IAmNearestToAbe_440120()
{
    for (s32 i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
    {
        BaseAliveGameObject* pObjIter = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter != this)
        {
            if (pObjIter->field_4_typeId == Types::eMudokon_52 || pObjIter->field_4_typeId == Types::eMudokon_75) // mud or password mud?
            {
                if (Math_Distance_451270(
                        FP_GetExponent(sActiveHero_507678->field_A8_xpos),
                        FP_GetExponent(sActiveHero_507678->field_AC_ypos),
                        FP_GetExponent(field_A8_xpos),
                        FP_GetExponent(field_AC_ypos))
                    > Math_Distance_451270(
                        FP_GetExponent(sActiveHero_507678->field_A8_xpos),
                        FP_GetExponent(sActiveHero_507678->field_AC_ypos),
                        FP_GetExponent(pObjIter->field_A8_xpos),
                        FP_GetExponent(pObjIter->field_AC_ypos)))
                {
                    // Our distance is to abe is larger than the mud we just found so we can't be nearest
                    return 0;
                }
            }
        }
    }
    return 1;
}

void Mudokon::VOnTrapDoorOpen()
{
    VOnTrapDoorOpen_43C9F0();
}

void Mudokon::VOnTrapDoorOpen_43C9F0()
{
    if (field_F8_pLiftPoint)
    {
        if (field_106_shot)
        {
            field_144_flags.Set(Flags_144::e144_Bit8);
        }
        else
        {
            VSetMotion(eMudMotions::Motion_49_WalkOffEdge_43E800);
        }

        field_F8_pLiftPoint->VRemove(this);
        field_F8_pLiftPoint->field_C_refCount--;
        field_F8_pLiftPoint = nullptr;
    }
}

void Mudokon::VOn_TLV_Collision(Path_TLV* pTlv)
{
    VOnTlvCollision_43FD90(pTlv);
}

void Mudokon::VOnTlvCollision_43FD90(Path_TLV* pTlv)
{
    if (pTlv)
    {
        while (pTlv)
        {
            if (pTlv->field_4_type == TlvTypes::DeathDrop_5)
            {
                if (field_100_health > FP_FromInteger(0))
                {
                    field_1BA_brain_sub_state = 0;
                    field_100_health = FP_FromInteger(0);
                    field_1B8_brain_idx = 13;
                    Event_Broadcast_417220(kEventMudokonDead_15, sActiveHero_507678);
                }
            }
            pTlv = gMap_507BA8.TLV_Get_At_446060(pTlv, field_A8_xpos, field_AC_ypos, field_A8_xpos, field_AC_ypos);
        }
    }
}

void Mudokon::Motion_0_Idle_43CA70()
{
    CheckFloorGone_43C9B0();

    field_E8_LastLineYPos = field_AC_ypos;

    switch (field_FE_next_motion)
    {
        case eMudMotions::Motion_1_WalkLoop_43CC80:
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
            }
            else
            {
                field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
            }
            field_FC_current_motion = eMudMotions::Motion_7_WalkBegin_43CE60;
            field_FE_next_motion = -1;
            break;

        case eMudMotions::Motion_35_SneakLoop_43E0F0:
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(10));
            }
            else
            {
                field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(10));
            }
            field_FC_current_motion = eMudMotions::Motion_40_SneakBegin_43E4E0;
            field_FE_next_motion = -1;
            break;

        case eMudMotions::Motion_29_RunLoop_43DB10:
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
            }
            else
            {
                field_B4_velx = ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4);
            }
            field_FC_current_motion = eMudMotions::Motion_45_StandToRun_43EB00;
            field_FE_next_motion = -1;
            break;

        case eMudMotions::Motion_52_Chant_43D520:
            SND_SEQ_PlaySeq_4775A0(SeqId::eMudokonChant_12, 0, 1);
            field_FC_current_motion = eMudMotions::Motion_52_Chant_43D520;
            field_FE_next_motion = -1;
            break;

        case eMudMotions::Motion_23_CrouchIdle_43E590:
            field_FC_current_motion = eMudMotions::Motion_25_StandToCrouch_43E620;
            field_FE_next_motion = -1;
            break;

        case eMudMotions::Motion_59_CrouchChant_43EC20:
            field_FC_current_motion = eMudMotions::Motion_58_StruggleToCrouchChant_43EC00;
            field_FE_next_motion = -1;
            break;

        case eMudMotions::Motion_16_StandScrubLoop_43D7C0:
            field_FC_current_motion = eMudMotions::Motion_20_IdleToStandScrub_43D8D0;
            field_FE_next_motion = -1;
            break;

        default:
            if (field_FE_next_motion != -1)
            {
                field_FC_current_motion = field_FE_next_motion;
                field_FE_next_motion = -1;
            }
            break;
    }
}

void Mudokon::Motion_1_WalkLoop_43CC80()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);

    if (WallHit_401930(
            field_BC_sprite_scale * FP_FromInteger(50),
            field_B4_velx))
    {
        ToStand_43CA40();
    }
    else
    {
        MoveOnLine_43C7E0();
        if (field_FC_current_motion == eMudMotions::Motion_1_WalkLoop_43CC80)
        {
            switch (field_10_anim.field_92_current_frame)
            {
                case 2:
                    if (field_FE_next_motion == eMudMotions::Motion_0_Idle_43CA70)
                    {
                        field_FC_current_motion = eMudMotions::Motion_9_MidWalkToIdle_43CFA0;
                        field_FE_next_motion = -1;
                    }
                    break;

                case 5:
                    Environment_SFX_42A220(EnvironmentSfx::eWalkingFootstep_1, 0, 0x7FFF, this);
                    if (!field_144_flags.Get(Flags_144::e144_Bit7))
                    {
                        field_144_flags.Set(Flags_144::e144_Bit7);
                        MapFollowMe_401D30(field_144_flags.Get(Flags_144::e144_Bit4_bSnapToGrid));
                    }

                    if (field_FE_next_motion == eMudMotions::Motion_29_RunLoop_43DB10)
                    {
                        field_FC_current_motion = eMudMotions::Motion_28_MidWalkToRun_43DA40;
                        field_FE_next_motion = -1;
                    }
                    else if (field_FE_next_motion == eMudMotions::Motion_35_SneakLoop_43E0F0)
                    {
                        field_FC_current_motion = eMudMotions::Motion_36_MidWalkToSneak_43E240;
                        field_FE_next_motion = -1;
                    }
                    break;

                case 11:
                    if (field_FE_next_motion == eMudMotions::Motion_0_Idle_43CA70)
                    {
                        field_FC_current_motion = eMudMotions::Motion_8_WalkToIdle_43CEF0;
                        field_FE_next_motion = -1;
                    }
                    break;

                case 14:
                    Environment_SFX_42A220(EnvironmentSfx::eWalkingFootstep_1, 0, 0x7FFF, this);
                    if (!field_144_flags.Get(Flags_144::e144_Bit7))
                    {
                        field_144_flags.Set(Flags_144::e144_Bit7);
                        MapFollowMe_401D30(field_144_flags.Get(Flags_144::e144_Bit4_bSnapToGrid));
                    }

                    if (field_FE_next_motion == eMudMotions::Motion_29_RunLoop_43DB10)
                    {
                        field_FC_current_motion = eMudMotions::Motion_27_WalkToRun_43D980;
                        field_FE_next_motion = -1;
                    }
                    else if (field_FE_next_motion == eMudMotions::Motion_35_SneakLoop_43E0F0)
                    {
                        field_FC_current_motion = eMudMotions::Motion_38_WalkToSneak_43E380;
                        field_FE_next_motion = -1;
                    }
                    break;

                default:
                    field_144_flags.Clear(Flags_144::e144_Bit7);
                    break;
            }
        }
    }
}

void Mudokon::Motion_2_StandingTurn_43D050()
{
    CheckFloorGone_43C9B0();

    if (field_10_anim.field_92_current_frame == 0)
    {
        Environment_SFX_42A220(EnvironmentSfx::eGenericMovement_9, 0, 0x7FFF, this);
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eMudMotions::Motion_0_Idle_43CA70;
        field_10_anim.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
    }
}

void Mudokon::Motion_3_Speak_43D440()
{
    CheckFloorGone_43C9B0();

    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eMudMotions::Motion_0_Idle_43CA70;
    }
}

void Mudokon::Motion_4_Speak_43D440()
{
    Motion_3_Speak_43D440();
}

void Mudokon::Motion_5_Speak_43D440()
{
    Motion_3_Speak_43D440();
}

void Mudokon::Motion_6_Speak_43D440()
{
    Motion_3_Speak_43D440();
}

void Mudokon::Motion_7_WalkBegin_43CE60()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);

    if (WallHit_401930(
            field_BC_sprite_scale * FP_FromInteger(50),
            field_B4_velx))
    {
        ToStand_43CA40();
    }
    else
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_FC_current_motion = eMudMotions::Motion_1_WalkLoop_43CC80;
        }
        MoveOnLine_43C7E0();
    }
}

void Mudokon::Motion_8_WalkToIdle_43CEF0()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);

    if (WallHit_401930(
            field_BC_sprite_scale * FP_FromInteger(50),
            field_B4_velx))
    {
        ToStand_43CA40();
    }
    else
    {
        MoveOnLine_43C7E0();

        if (field_10_anim.field_92_current_frame == 0)
        {
            Environment_SFX_42A220(EnvironmentSfx::eWalkingFootstep_1, 0, 0x7FFF, this);
            return;
        }

        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            MapFollowMe_401D30(field_144_flags.Get(Flags_144::e144_Bit4_bSnapToGrid));
            ToStand_43CA40();
        }
    }
}

void Mudokon::Motion_9_MidWalkToIdle_43CFA0()
{
    Motion_8_WalkToIdle_43CEF0();
}

void Mudokon::Motion_10_Unused_43D4D0()
{
    if (sNumCamSwappers_507668 <= 0)
    {
        field_FC_current_motion = field_E4_previous_motion;
        if (field_F8_pLiftPoint)
        {
            field_A8_xpos = FP_FromInteger((field_F4_pLine->field_0_rect.x + field_F4_pLine->field_0_rect.w) / 2);
            field_AC_ypos = FP_FromInteger(field_F4_pLine->field_0_rect.y);
        }
    }
}

void Mudokon::Motion_11_Null_43D350()
{
    // Empty
}

void Mudokon::Motion_12_LiftUse_43D360()
{
    auto pLiftPoint = static_cast<LiftPoint*>(field_194_pLiftPoint);
    if (!pLiftPoint->OnAnyFloor() || pLiftPoint->field_27A_flags.Get(LiftPoint::Flags::eBit7_bIgnoreLiftMover))
    {
        pLiftPoint->Move_435740(FP_FromInteger(0), FP_FromInteger(3), 0);
    }
    else
    {
        pLiftPoint->Move_435740(FP_FromInteger(0), FP_FromInteger(0), 0);
        field_FC_current_motion = eMudMotions::Motion_14_LiftGrabEnd_43D420;
        field_FE_next_motion = eMudMotions::Motion_0_Idle_43CA70;
    }
}

void Mudokon::Motion_13_LiftGrabBegin_43D3F0()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = field_FE_next_motion;
        field_194_pLiftPoint->field_27A_flags.Set(LiftPoint::Flags::eBit8_KeepOnMiddleFloor);
    }
}

void Mudokon::Motion_14_LiftGrabEnd_43D420()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = field_FE_next_motion;
    }
}

void Mudokon::Motion_15_LeverUse_43D4B0()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eMudMotions::Motion_0_Idle_43CA70;
    }
}

void Mudokon::Motion_16_StandScrubLoop_43D7C0()
{
    if (gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos,
            0))
    {
        if (field_10_anim.field_92_current_frame)
        {
            if (field_10_anim.field_92_current_frame == 5)
            {
                SFX_Play_43AD70(SoundEffect::Clean2_86, 0, 0);
            }
        }
        else
        {
            SFX_Play_43AD70(SoundEffect::Clean1_85, 0, 0);
        }
    }

    if (field_FE_next_motion != -1)
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            if (field_FE_next_motion != eMudMotions::Motion_0_Idle_43CA70)
            {
                field_FC_current_motion = field_FE_next_motion;
            }
            else
            {
                field_FC_current_motion = eMudMotions::Motion_21_StandScrubToIdle_43D8F0;
            }
            field_FE_next_motion = -1;
        }
    }
}

void Mudokon::Motion_17_StandScrubLoopToPause_43D860()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eMudMotions::Motion_19_StandScrubPause_43D8A0;
    }
}

void Mudokon::Motion_18_StandScrubPauseToLoop_43D880()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eMudMotions::Motion_16_StandScrubLoop_43D7C0;
    }
}

void Mudokon::Motion_19_StandScrubPause_43D8A0()
{
    if (field_FE_next_motion != -1)
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_FC_current_motion = eMudMotions::Motion_18_StandScrubPauseToLoop_43D880;
            field_FE_next_motion = -1;
        }
    }
}

void Mudokon::Motion_20_IdleToStandScrub_43D8D0()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eMudMotions::Motion_16_StandScrubLoop_43D7C0;
    }
}

void Mudokon::Motion_21_StandScrubToIdle_43D8F0()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eMudMotions::Motion_0_Idle_43CA70;
    }
}

void Mudokon::Motion_22_CrouchScrub_43D910()
{
    if (gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos,
            0))
    {
        if (field_10_anim.field_92_current_frame == 2)
        {
            SFX_Play_43AD70(SoundEffect::Clean1_85, 0, 0);
        }
        else if (field_10_anim.field_92_current_frame == 6)
        {
            SFX_Play_43AD70(SoundEffect::Clean2_86, 0, 0);
        }
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eMudMotions::Motion_23_CrouchIdle_43E590;
    }
}

void Mudokon::Motion_23_CrouchIdle_43E590()
{
    CheckFloorGone_43C9B0();

    if (field_FE_next_motion == eMudMotions::Motion_55_Duck_43EB90)
    {
        field_FC_current_motion = eMudMotions::Motion_54_ToDuck_43EB70;
        field_FE_next_motion = -1;
    }
    else if (field_FE_next_motion == eMudMotions::Motion_0_Idle_43CA70)
    {
        field_FC_current_motion = eMudMotions::Motion_26_CrouchToStand_43E640;
        field_FE_next_motion = -1;
    }
    else if (field_FE_next_motion != -1)
    {
        field_FC_current_motion = field_FE_next_motion;
        field_FE_next_motion = -1;
    }
}

void Mudokon::Motion_24_CrouchTurn_43E5F0()
{
    CheckFloorGone_43C9B0();

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eMudMotions::Motion_23_CrouchIdle_43E590;
        field_10_anim.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
    }
}

void Mudokon::Motion_25_StandToCrouch_43E620()
{
    CheckFloorGone_43C9B0();

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eMudMotions::Motion_23_CrouchIdle_43E590;
    }
}

void Mudokon::Motion_26_CrouchToStand_43E640()
{
    CheckFloorGone_43C9B0();

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eMudMotions::Motion_0_Idle_43CA70;
    }
}

void Mudokon::Motion_27_WalkToRun_43D980()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);

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
        field_FC_current_motion = eMudMotions::Motion_29_RunLoop_43DB10;
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), field_B4_velx))
    {
        ToStand_43CA40();
    }
    else
    {
        MoveOnLine_43C7E0();
    }
}

void Mudokon::Motion_28_MidWalkToRun_43DA40()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);

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
        field_E4_previous_motion = eMudMotions::Motion_29_RunLoop_43DB10;
        field_E6_last_anim_frame = eMudMotions::Motion_8_WalkToIdle_43CEF0;
        field_1BC = 1;
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), field_B4_velx))
    {
        ToStand_43CA40();
    }
    else
    {
        MoveOnLine_43C7E0();
    }
}

void Mudokon::Motion_29_RunLoop_43DB10()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), field_B4_velx))
    {
        ToKnockback_43D6E0();
        return;
    }

    MoveOnLine_43C7E0();

    if (field_FC_current_motion == eMudMotions::Motion_29_RunLoop_43DB10)
    {
        if (field_10_anim.field_92_current_frame == 0 || field_10_anim.field_92_current_frame == 8)
        {
            if (!field_144_flags.Get(Flags_144::e144_Bit7))
            {
                field_144_flags.Set(Flags_144::e144_Bit7);
                MapFollowMe_401D30(field_144_flags.Get(Flags_144::e144_Bit4_bSnapToGrid));
            }

            if (field_FE_next_motion == eMudMotions::Motion_44_RunJumpMid_43E960)
            {
                field_FC_current_motion = eMudMotions::Motion_43_RunJumpBegin_43E870;
                field_FE_next_motion = -1;
            }
        }
        else if (field_10_anim.field_92_current_frame == 4 || field_10_anim.field_92_current_frame == 12)
        {
            Environment_SFX_42A220(EnvironmentSfx::eRunningFootstep_2, 0, 0x7FFF, this);
            if (!field_144_flags.Get(Flags_144::e144_Bit7))
            {
                field_144_flags.Set(Flags_144::e144_Bit7);
                MapFollowMe_401D30(field_144_flags.Get(Flags_144::e144_Bit4_bSnapToGrid));
            }

            switch (field_FE_next_motion)
            {
                case eMudMotions::Motion_1_WalkLoop_43CC80:
                    if (field_10_anim.field_92_current_frame == 4)
                    {
                        field_FC_current_motion = eMudMotions::Motion_30_RunToWalk_43DD50;
                        field_FE_next_motion = -1;
                    }
                    return;

                    // TODO: OG bug - MidRunToWalk will always be set on frame 12. AE doesn't do this.
                    // couldn't see any breaking changes after commenting this out.

                    /*case eMudMotions::Motion_29_RunLoop_43DB10:
                if (field_10_anim.field_92_current_frame == 12)
                {
                    field_FC_current_motion = eMudMotions::Motion_28_MidWalkToRun_43DA40;
                    field_FE_next_state = -1;
                }
                return;*/

                case eMudMotions::Motion_0_Idle_43CA70:
                    field_FC_current_motion = eMudMotions::Motion_32_RunSlideStop_43DEE0;
                    field_FE_next_motion = -1;
                    Environment_SFX_42A220(EnvironmentSfx::eRunSlide_4, 0, 0x7FFF, this);
                    return;

                case eMudMotions::Motion_33_RunSlideTurn_43DF80:
                    field_FC_current_motion = eMudMotions::Motion_33_RunSlideTurn_43DF80;
                    field_FE_next_motion = -1;
                    Environment_SFX_42A220(EnvironmentSfx::eRunSlide_4, 0, 0x7FFF, this);
                    return;

                case eMudMotions::Motion_44_RunJumpMid_43E960:
                    field_FC_current_motion = eMudMotions::Motion_43_RunJumpBegin_43E870;
                    field_FE_next_motion = -1;
                    return;
            }
        }
        else
        {
            field_144_flags.Clear(Flags_144::e144_Bit7);
        }
    }
}

void Mudokon::Motion_30_RunToWalk_43DD50()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
    }
    else
    {
        field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eMudMotions::Motion_1_WalkLoop_43CC80;
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), field_B4_velx))
    {
        ToStand_43CA40();
    }
    else
    {
        MoveOnLine_43C7E0();
    }
}

void Mudokon::Motion_31_MidRunToWalk_43DE10()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
    }
    else
    {
        field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_E4_previous_motion = eMudMotions::Motion_1_WalkLoop_43CC80;
        field_E6_last_anim_frame = 9;
        field_1BC = 1;
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), field_B4_velx))
    {
        ToStand_43CA40();
    }
    else
    {
        MoveOnLine_43C7E0();
    }
}

void Mudokon::Motion_32_RunSlideStop_43DEE0()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), field_B4_velx))
    {
        ToKnockback_43D6E0();
    }
    else
    {
        ReduceXVelocityBy_43C920(FP_FromDouble(0.375));

        if (field_FC_current_motion == eMudMotions::Motion_32_RunSlideStop_43DEE0)
        {
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                Environment_SFX_42A220(EnvironmentSfx::eWalkingFootstep_1, 0, 0x7FFF, this);
                MapFollowMe_401D30(field_144_flags.Get(Flags_144::e144_Bit4_bSnapToGrid));
                ToStand_43CA40();
            }
        }
    }
}

void Mudokon::Motion_33_RunSlideTurn_43DF80()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), field_B4_velx))
    {
        ToKnockback_43D6E0();
    }
    else
    {
        ReduceXVelocityBy_43C920(FP_FromDouble(0.375));

        if (field_FC_current_motion == eMudMotions::Motion_33_RunSlideTurn_43DF80)
        {
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                MapFollowMe_401D30(field_144_flags.Get(Flags_144::e144_Bit4_bSnapToGrid));

                if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    field_B4_velx = ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4);
                    field_FC_current_motion = eMudMotions::Motion_34_RunTurnToRun_43E070;
                }
                else
                {
                    field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
                    field_FC_current_motion = eMudMotions::Motion_34_RunTurnToRun_43E070;
                }
            }
        }
    }
}

void Mudokon::Motion_34_RunTurnToRun_43E070()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), field_B4_velx))
    {
        ToStand_43CA40();
    }
    else
    {
        MoveOnLine_43C7E0();
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_FC_current_motion = eMudMotions::Motion_29_RunLoop_43DB10;
            field_10_anim.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
        }
    }
}

void Mudokon::Motion_35_SneakLoop_43E0F0()
{
    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), field_B4_velx))
    {
        ToStand_43CA40();
    }
    else
    {
        MoveOnLine_43C7E0();
        if (field_FC_current_motion == eMudMotions::Motion_35_SneakLoop_43E0F0)
        {
            switch (field_10_anim.field_92_current_frame)
            {
                case 3:
                    if (!field_FE_next_motion)
                    {
                        field_FC_current_motion = eMudMotions::Motion_41_SneakToIdle_43E530;
                        field_FE_next_motion = -1;
                    }
                    break;

                case 6:
                    Environment_SFX_42A220(EnvironmentSfx::eSneakFootstep_3, 0, 0x7FFF, this);
                    MapFollowMe_401D30(field_144_flags.Get(Flags_144::e144_Bit4_bSnapToGrid));

                    if (field_FE_next_motion == eMudMotions::Motion_1_WalkLoop_43CC80 || field_FE_next_motion == eMudMotions::Motion_29_RunLoop_43DB10)
                    {
                        field_FC_current_motion = eMudMotions::Motion_37_SneakToWalk_43E2E0;
                        field_FE_next_motion = -1;
                    }
                    break;

                case 13:
                    if (!field_FE_next_motion)
                    {
                        field_FC_current_motion = eMudMotions::Motion_42_MidSneakToIdle_43E560;
                        field_FE_next_motion = -1;
                    }
                    break;

                case 16:
                    Environment_SFX_42A220(EnvironmentSfx::eSneakFootstep_3, 0, 0x7FFF, this);
                    MapFollowMe_401D30(field_144_flags.Get(Flags_144::e144_Bit4_bSnapToGrid));

                    if (field_FE_next_motion == eMudMotions::Motion_1_WalkLoop_43CC80 || field_FE_next_motion == eMudMotions::Motion_29_RunLoop_43DB10)
                    {
                        field_FC_current_motion = eMudMotions::Motion_39_MidSneakToWalk_43E430;
                        field_FE_next_motion = -1;
                    }
                    break;
            }
        }
    }
}

void Mudokon::Motion_36_MidWalkToSneak_43E240()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(10));
    }
    else
    {
        field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(10));
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eMudMotions::Motion_35_SneakLoop_43E0F0;
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), field_B4_velx))
    {
        ToStand_43CA40();
    }
    else
    {
        MoveOnLine_43C7E0();
    }
}

void Mudokon::Motion_37_SneakToWalk_43E2E0()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
    }
    else
    {
        field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eMudMotions::Motion_1_WalkLoop_43CC80;
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), field_B4_velx))
    {
        ToStand_43CA40();
    }
    else
    {
        MoveOnLine_43C7E0();
    }
}

void Mudokon::Motion_38_WalkToSneak_43E380()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(10));
    }
    else
    {
        field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(10));
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_E4_previous_motion = eMudMotions::Motion_35_SneakLoop_43E0F0;
        field_E6_last_anim_frame = 10;
        field_1BC = 1;
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), field_B4_velx))
    {
        ToStand_43CA40();
    }
    else
    {
        MoveOnLine_43C7E0();
    }
}

void Mudokon::Motion_39_MidSneakToWalk_43E430()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
    }
    else
    {
        field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_E4_previous_motion = eMudMotions::Motion_1_WalkLoop_43CC80;
        field_E6_last_anim_frame = 9;
        field_1BC = 1;
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), field_B4_velx))
    {
        ToStand_43CA40();
    }
    else
    {
        MoveOnLine_43C7E0();
    }
}

void Mudokon::Motion_40_SneakBegin_43E4E0()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eMudMotions::Motion_35_SneakLoop_43E0F0;
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), field_B4_velx))
    {
        ToStand_43CA40();
    }
    else
    {
        MoveOnLine_43C7E0();
    }
}

void Mudokon::Motion_41_SneakToIdle_43E530()
{
    MoveOnLine_43C7E0();

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        MapFollowMe_401D30(field_144_flags.Get(Flags_144::e144_Bit4_bSnapToGrid));
        ToStand_43CA40();
    }
}

void Mudokon::Motion_42_MidSneakToIdle_43E560()
{
    MoveOnLine_43C7E0();

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        MapFollowMe_401D30(field_144_flags.Get(Flags_144::e144_Bit4_bSnapToGrid));
        ToStand_43CA40();
    }
}

void Mudokon::Motion_43_RunJumpBegin_43E870()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);

    CheckFloorGone_43C9B0();

    field_A8_xpos += field_B4_velx;

    if (field_10_anim.field_92_current_frame == 0)
    {
        Environment_SFX_42A220(EnvironmentSfx::eRunJumpOrLedgeHoist_11, 0, 0x7FFF, this);
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_E8_LastLineYPos = field_AC_ypos;

        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_B4_velx = field_BC_sprite_scale * FP_FromDouble(-7.6);
        }
        else
        {
            field_B4_velx = field_BC_sprite_scale * FP_FromDouble(7.6);
        }

        field_B8_vely = (field_BC_sprite_scale * FP_FromDouble(-9.6));
        field_AC_ypos += field_B8_vely;
        VOnTrapDoorOpen();
        field_FC_current_motion = eMudMotions::Motion_44_RunJumpMid_43E960;
        field_F4_pLine = nullptr;
    }
}

void Mudokon::Motion_44_RunJumpMid_43E960()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);

    if (field_10_anim.field_92_current_frame == 5)
    {
        SFX_Play_43AE60(SoundEffect::PossessEffect_21, 40, 2400, 0);
    }

    PSX_RECT bRect = {};
    VGetBoundingRect(&bRect, 1);

    if ((field_B4_velx > FP_FromInteger(0) && (FP_FromInteger(bRect.x) > field_1AC_pBirdPortal->field_18_xpos)) || ((field_B4_velx < FP_FromInteger(0) && FP_FromInteger(bRect.w) < field_1AC_pBirdPortal->field_18_xpos)))
    {
        field_144_flags.Clear(Flags_144::e144_Bit2);
        field_144_flags.Clear(Flags_144::e144_Bit6_bPersist);

        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        field_B8_vely = FP_FromInteger(0);
        field_B4_velx = FP_FromInteger(0);

        SND_SEQ_Play_477760(SeqId::eSaveTriggerMusic_45, 1, 127, 127);


        auto pMusicMem = ao_new<MusicTrigger>();
        if (pMusicMem)
        {
            pMusicMem->ctor_443A60(MusicTriggerMusicType::eChime_5, TriggeredBy::eTimer_0, 0, 300);
        }

        sRescuedMudokons_5076C0++;

        if (field_1AC_pBirdPortal)
        {
            field_1AC_pBirdPortal->VMudSaved();
        }

        if (field_1B2_rescue_switch_id)
        {
            SwitchStates_Set(field_1B2_rescue_switch_id, 1);
        }
    }

    field_B8_vely += (field_BC_sprite_scale * FP_FromDouble(1.8));
    field_A8_xpos += field_B4_velx;
    field_AC_ypos += field_B8_vely;
}

void Mudokon::Motion_45_StandToRun_43EB00()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eMudMotions::Motion_29_RunLoop_43DB10;
    }

    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), field_B4_velx))
    {
        ToStand_43CA40();
    }
    else
    {
        MoveOnLine_43C7E0();
    }
}

void Mudokon::Motion_46_FallLandDie_43E660()
{
    if (field_10_anim.field_92_current_frame == 4)
    {
        Environment_SFX_42A220(EnvironmentSfx::eKnockback_13, 0, 0x7FFF, this);
    }

    if ((gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarms_1
         || gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarmsReturn_13
         || gMap_507BA8.field_0_current_level == LevelIds::eBoardRoom_12)
        && field_10_anim.field_92_current_frame == 7)
    {
        Environment_SFX_42A220(EnvironmentSfx::eHitGroundSoft_6, 80, -200, this);
    }

    CheckFloorGone_43C9B0();

    if (field_FE_next_motion != -1)
    {
        field_FC_current_motion = field_FE_next_motion;
        field_FE_next_motion = -1;
    }
}

void Mudokon::Motion_47_Knockback_43E730()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);

    if ((gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarms_1
         || gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarmsReturn_13
         || gMap_507BA8.field_0_current_level == LevelIds::eBoardRoom_12)
        && field_10_anim.field_92_current_frame == 7)
    {
        Environment_SFX_42A220(EnvironmentSfx::eHitGroundSoft_6, 80, -200, this);
    }

    if (!field_F4_pLine)
    {
        Motion_51_Fall_43D0D0();

        if (field_FC_current_motion == eMudMotions::Motion_46_FallLandDie_43E660 || field_FC_current_motion == eMudMotions::Motion_50_LandSoft_43E820)
        {
            field_FC_current_motion = eMudMotions::Motion_47_Knockback_43E730;
        }
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_F4_pLine)
        {
            field_FC_current_motion = eMudMotions::Motion_48_KnockbackGetUp_43E7D0;
        }
    }
}

void Mudokon::Motion_48_KnockbackGetUp_43E7D0()
{
    CheckFloorGone_43C9B0();

    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand_43CA40();
    }
}

void Mudokon::Motion_49_WalkOffEdge_43E800()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eMudMotions::Motion_51_Fall_43D0D0;
    }
    Motion_51_Fall_43D0D0();
}

void Mudokon::Motion_50_LandSoft_43E820()
{
    CheckFloorGone_43C9B0();

    if (field_10_anim.field_92_current_frame == 2)
    {
        if (field_E4_previous_motion == eMudMotions::Motion_51_Fall_43D0D0)
        {
            Environment_SFX_42A220(EnvironmentSfx::eLandingSoft_5, 0, 0x7FFF, this);
        }
        else
        {
            Environment_SFX_42A220(EnvironmentSfx::eHitGroundSoft_6, 0, 0x7FFF, this);
        }
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand_43CA40();
    }
}

void Mudokon::Motion_51_Fall_43D0D0()
{
    if (field_B4_velx > FP_FromInteger(0))
    {
        field_B4_velx -= field_BC_sprite_scale * field_11C;
        if (field_B4_velx < FP_FromInteger(0))
        {
            field_B4_velx = FP_FromInteger(0);
        }
    }
    else if (field_B4_velx < FP_FromInteger(0))
    {
        field_B4_velx += field_BC_sprite_scale * field_11C;
        if (field_B4_velx > FP_FromInteger(0))
        {
            field_B4_velx = FP_FromInteger(0);
        }
    }

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (InAirCollision_4019C0(&pLine, &hitX, &hitY, FP_FromDouble(1.8)))
    {
        switch (pLine->field_8_type)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eUnknown_32:
            case eLineTypes::eUnknown_36:
                field_F4_pLine = pLine;
                field_A8_xpos = hitX;
                field_AC_ypos = hitY;
                MapFollowMe_401D30(field_144_flags.Get(Flags_144::e144_Bit4_bSnapToGrid));
                if (field_AC_ypos - field_E8_LastLineYPos > FP_FromInteger(240))
                {
                    field_E8_LastLineYPos += FP_FromInteger(240);
                }

                if (field_AC_ypos - field_E8_LastLineYPos < (field_BC_sprite_scale * FP_FromInteger(180)) && (field_100_health > FP_FromInteger(0) || gAbeInvulnerableCheat_5076E4))
                {
                    field_FC_current_motion = eMudMotions::Motion_50_LandSoft_43E820;
                }
                else
                {
                    field_100_health = FP_FromInteger(0);
                    ToKnockback_43D6E0();
                    field_144_flags.Clear(Flags_144::e144_Bit6_bPersist);
                    field_1B8_brain_idx = 11;
                    field_1C0_timer = gnFrameCount_507670 + 90;
                    field_FC_current_motion = eMudMotions::Motion_46_FallLandDie_43E660;
                }

                field_E4_previous_motion = eMudMotions::Motion_51_Fall_43D0D0;

                PSX_Point xy;
                xy.field_0_x = FP_GetExponent(field_A8_xpos - FP_FromInteger(10));
                xy.field_2_y = FP_GetExponent(field_AC_ypos - FP_FromInteger(10));

                PSX_Point wh;
                wh.field_0_x = FP_GetExponent(field_A8_xpos + FP_FromInteger(10));
                wh.field_2_y = FP_GetExponent(field_AC_ypos + FP_FromInteger(10));

                VOnCollisionWith(
                    xy,
                    wh,
                    ObjListPlatforms_50766C,
                    1,
                    (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_401C10);
                break;

            case eLineTypes::eWallLeft_1:
            case eLineTypes::eWallRight_2:
            case eLineTypes::eBackgroundWallLeft_5:
            case eLineTypes::eBackgroundWallRight_6:
                field_A8_xpos = hitX;
                field_AC_ypos = hitY;
                ToKnockback_43D6E0();
                break;

            default:
                return;
        }
    }
}

void Mudokon::Motion_52_Chant_43D520()
{
    if (!(gnFrameCount_507670 % 4))
    {
        const FP rndX = FP_FromInteger(40 * Math_NextRandom() / 256 - 20);
        const FP rndY = FP_FromInteger(30 * Math_NextRandom() / 256 + 30);

        New_Chant_Particle_4198E0(
            field_A8_xpos + (field_BC_sprite_scale * rndX),
            field_AC_ypos - (field_BC_sprite_scale * rndY),
            field_BC_sprite_scale,
            Layer::eLayer_0);
    }

    if (!SND_SsIsEos_DeInlined_477930(SeqId::eMudokonChant_12))
    {
        SND_SEQ_Play_477760(SeqId::eMudokonChant_12, 1, 50, 50);
    }

    if (field_FE_next_motion == eMudMotions::Motion_0_Idle_43CA70)
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            SND_Seq_Stop_477A60(SeqId::eMudokonChant_12);
            field_FC_current_motion = eMudMotions::Motion_53_ChantEnd_43D640;
            field_FE_next_motion = -1;
        }
    }
}

void Mudokon::Motion_53_ChantEnd_43D640()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eMudMotions::Motion_0_Idle_43CA70;
    }
}

void Mudokon::Motion_54_ToDuck_43EB70()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eMudMotions::Motion_55_Duck_43EB90;
    }
}

void Mudokon::Motion_55_Duck_43EB90()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_FE_next_motion == eMudMotions::Motion_23_CrouchIdle_43E590)
        {
            field_FE_next_motion = -1;
            field_FC_current_motion = eMudMotions::Motion_56_DuckToCrouch_43EBC0;
        }
    }
}

void Mudokon::Motion_56_DuckToCrouch_43EBC0()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eMudMotions::Motion_23_CrouchIdle_43E590;
    }
}

void Mudokon::Motion_57_Struggle_43EBE0()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_FE_next_motion == eMudMotions::Motion_0_Idle_43CA70)
        {
            field_FC_current_motion = eMudMotions::Motion_0_Idle_43CA70;
        }
    }
}

void Mudokon::Motion_58_StruggleToCrouchChant_43EC00()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eMudMotions::Motion_59_CrouchChant_43EC20;
    }
}

void Mudokon::Motion_59_CrouchChant_43EC20()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_FE_next_motion != -1)
        {
            field_FE_next_motion = -1;
            field_FC_current_motion = eMudMotions::Motion_60_CrouchChantToSruggle_43ED50;
        }
    }

    if ((gnFrameCount_507670 % 8) == 0)
    {
        const FP rndX = FP_FromInteger(30 * Math_NextRandom() / 256 - 10);
        const FP rndY = FP_FromInteger(20 * Math_NextRandom() / 256 + 10);

        New_Chant_Particle_4198E0(
            field_A8_xpos + (field_BC_sprite_scale * rndX),
            field_AC_ypos - (field_BC_sprite_scale * rndY),
            field_BC_sprite_scale,
            Layer::eLayer_0);
    }

    if (!SND_SsIsEos_DeInlined_477930(SeqId::eMudokonChant_12))
    {
        if (field_BC_sprite_scale == FP_FromDouble(0.5))
        {
            SND_SEQ_Play_477760(SeqId::eMudokonChant_12, 1, 30, 30);
        }
        else
        {
            SND_SEQ_Play_477760(SeqId::eMudokonChant_12, 1, 50, 50);
        }
    }
}

void Mudokon::Motion_60_CrouchChantToSruggle_43ED50()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eMudMotions::Motion_0_Idle_43CA70;
    }
}

void Mudokon::Motion_61_DuckKnockback_43E6E0()
{
    if (field_10_anim.field_92_current_frame == 1)
    {
        Environment_SFX_42A220(EnvironmentSfx::eGenericMovement_9, 0, 0x7FFF, this);
    }

    CheckFloorGone_43C9B0();

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eMudMotions::Motion_55_Duck_43EB90;
        field_FE_next_motion = -1;
    }
}

void Mudokon::Motion_62_PoisonGasDeath_43ED70()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_144_flags.Clear(Flags_144::e144_Bit6_bPersist);
        field_100_health = FP_FromInteger(0);
        field_1B8_brain_idx = 11;
        field_1C0_timer = gnFrameCount_507670 + 90;
        field_FE_next_motion = -1;
    }
}

s16 Mudokon::Brain_ComingIn_0_441DE0()
{
    if (field_1BA_brain_sub_state == 0)
    {
        field_1C0_timer = gnFrameCount_507670 + 120;
        return 1;
    }

    else if (field_1BA_brain_sub_state == 1)
    {
        if (static_cast<s32>(gnFrameCount_507670) > field_1C0_timer)
        {
            field_FE_next_motion = eMudMotions::Motion_1_WalkLoop_43CC80;
            field_190 = field_18C_how_far_to_walk;
            return 2;
        }
    }
    else if (field_1BA_brain_sub_state == 2)
    {
        if (field_B4_velx < FP_FromInteger(0))
        {
            field_190 += field_B4_velx;
        }
        else
        {
            field_190 -= field_B4_velx;
        }

        if (field_190 < FP_FromInteger(0))
        {
            field_FC_current_motion = eMudMotions::Motion_0_Idle_43CA70;
            field_1B8_brain_idx = 2;
            return 0;
        }
    }
    return field_1BA_brain_sub_state;
}

s16 Mudokon::Brain_ComingOut_1_441E90()
{
    if (field_1BA_brain_sub_state == 0)
    {
        if (field_FC_current_motion == eMudMotions::Motion_0_Idle_43CA70)
        {
            field_FC_current_motion = eMudMotions::Motion_2_StandingTurn_43D050;
            field_FE_next_motion = eMudMotions::Motion_1_WalkLoop_43CC80;
            return 1;
        }
    }
    else if (field_1BA_brain_sub_state == 1)
    {
        if (field_FC_current_motion == eMudMotions::Motion_1_WalkLoop_43CC80)
        {
            field_144_flags.Clear(Flags_144::e144_Bit2);
            field_190 = field_18C_how_far_to_walk;
            return 2;
        }
    }
    else if (field_1BA_brain_sub_state == 2)
    {
        if (field_B4_velx < FP_FromInteger(0))
        {
            field_190 += field_B4_velx;
        }
        else
        {
            field_190 -= field_B4_velx;
        }

        if (field_190 < FP_FromInteger(0))
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
            field_FC_current_motion = eMudMotions::Motion_0_Idle_43CA70;
            return 3;
        }
    }
    return field_1BA_brain_sub_state;
}

s16 Mudokon::Brain_SingSequenceIdle_2_441CA0()
{
    switch (field_1BA_brain_sub_state)
    {
        case 0:
            if (field_144_flags.Get(Flags_144::e144_Bit10_give_ring_without_password))
            {
                field_1B8_brain_idx = field_188;
                return 0;
            }
            else
            {
                field_1C0_timer = gnFrameCount_507670 + 60;
                return 1;
            }
            break;

        case 1:
            if (static_cast<s32>(gnFrameCount_507670) > field_1C0_timer)
            {
                Mudokon_SFX_42A4D0(MudSounds::eHello_3, 0, 300, this);
                field_FE_next_motion = eMudMotions::Motion_4_Speak_43D440;
                return 2;
            }
            break;

        case 2:
            if (!field_198_abe_must_face_mud
                || sActiveHero_507678->field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX) != field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                GameSpeakEvents last_speak = {};
                if (field_128 == pEventSystem_4FF954->field_18_last_event_index)
                {
                    if (pEventSystem_4FF954->field_10_last_event == GameSpeakEvents::eNone_m1)
                    {
                        last_speak = GameSpeakEvents::eNone_m1;
                    }
                    else
                    {
                        last_speak = GameSpeakEvents::eSameAsLast_m2;
                    }
                }
                else
                {
                    last_speak = pEventSystem_4FF954->field_10_last_event;
                    field_128 = pEventSystem_4FF954->field_18_last_event_index;
                }

                if (last_speak == GameSpeakEvents::eHello_9)
                {
                    field_1C0_timer = gnFrameCount_507670 + 20;
                    return 3;
                }
            }
            break;

        case 3:
            if (field_1C0_timer <= static_cast<s32>(gnFrameCount_507670))
            {
                if (field_186_give_password == Choice_short::eYes_1)
                {
                    field_1B8_brain_idx = 4;
                }
                else
                {
                    field_1B8_brain_idx = 3;
                }
                return 0;
            }
            break;

        default:
            return field_1BA_brain_sub_state;
    }
    return field_1BA_brain_sub_state;
}

s16 Mudokon::Brain_SingSequenceSing_3_441510()
{
    switch (field_1BA_brain_sub_state)
    {
        case 0:
            field_19E_code_idx = 0;
            field_13C = GameSpeak::sub_40FA60(field_1A4_code_converted, field_12C);
            return 1;

        case 1:
            switch (Code_LookUp_476050(field_1A4_code_converted, field_19E_code_idx, field_1A8_code_length))
            {
                case GameSpeakEvents::eWhistleHigh_1:
                    field_FE_next_motion = eMudMotions::Motion_4_Speak_43D440;
                    Mudokon_SFX_42A4D0(MudSounds::eWhistleHigh_1, 0, 0, this);
                    break;

                case GameSpeakEvents::eWhistleLow_2:
                    field_FE_next_motion = eMudMotions::Motion_5_Speak_43D440;
                    Mudokon_SFX_42A4D0(MudSounds::eWhistleLow_2, 0, 0, this);
                    break;

                case GameSpeakEvents::eFart_3:
                    field_FE_next_motion = eMudMotions::Motion_6_Speak_43D440;
                    Mudokon_SFX_42A4D0(MudSounds::eFart_7, 0, 300, this);
                    break;

                case GameSpeakEvents::eLaugh_4:
                    field_FE_next_motion = eMudMotions::Motion_3_Speak_43D440;
                    Mudokon_SFX_42A4D0(MudSounds::eLaugh1_8, 0, 300, this);
                    break;

                default:
                    field_FE_next_motion = eMudMotions::Motion_3_Speak_43D440;
                    break;
            }

            field_19E_code_idx++;

            if (field_19E_code_idx >= field_1A8_code_length)
            {
                return 3;
            }
            else
            {
                field_1C0_timer = gnFrameCount_507670 + 30;
                return 2;
            }
            break;

        case 2:
        {
            GameSpeakEvents last_speak = {};
            if (field_128 == pEventSystem_4FF954->field_18_last_event_index)
            {
                if (pEventSystem_4FF954->field_10_last_event == GameSpeakEvents::eNone_m1)
                {
                    last_speak = GameSpeakEvents::eNone_m1;
                }
                else
                {
                    last_speak = GameSpeakEvents::eSameAsLast_m2;
                }
            }
            else
            {
                last_speak = pEventSystem_4FF954->field_10_last_event;
                field_128 = pEventSystem_4FF954->field_18_last_event_index;
            }

            if (last_speak == GameSpeakEvents::eNone_m1 || last_speak == GameSpeakEvents::eSameAsLast_m2)
            {
                if (static_cast<s32>(gnFrameCount_507670) > field_1C0_timer)
                {
                    return 1;
                }
            }
            else
            {
                field_1A0 = 0;
                field_1C0_timer = gnFrameCount_507670 + 30;
                return 5;
            }
            break;
        }

        case 3:
        {
            GameSpeakEvents last_speak = {};
            if (field_128 == pEventSystem_4FF954->field_18_last_event_index)
            {
                if (pEventSystem_4FF954->field_10_last_event == GameSpeakEvents::eNone_m1)
                {
                    last_speak = GameSpeakEvents::eNone_m1;
                }
                else
                {
                    last_speak = GameSpeakEvents::eSameAsLast_m2;
                }
            }
            else
            {
                field_128 = pEventSystem_4FF954->field_18_last_event_index;
                last_speak = pEventSystem_4FF954->field_10_last_event;
            }

            if (last_speak != GameSpeakEvents::eNone_m1 && last_speak != GameSpeakEvents::eSameAsLast_m2)
            {
                field_13E = static_cast<s16>(pEventSystem_4FF954->field_18_last_event_index);
                field_118 = gnFrameCount_507670 + 60;
                return 4;
            }
            break;
        }

        case 4:
        {
            GameSpeakEvents last_speak = {};
            if (field_128 == pEventSystem_4FF954->field_18_last_event_index)
            {
                if (pEventSystem_4FF954->field_10_last_event == GameSpeakEvents::eNone_m1)
                {
                    last_speak = GameSpeakEvents::eNone_m1;
                }
                else
                {
                    last_speak = GameSpeakEvents::eSameAsLast_m2;
                }
            }
            else
            {
                field_128 = pEventSystem_4FF954->field_18_last_event_index;
                last_speak = pEventSystem_4FF954->field_10_last_event;
            }


            if (last_speak != GameSpeakEvents::eNone_m1 && last_speak != GameSpeakEvents::eSameAsLast_m2)
            {
                field_118 = gnFrameCount_507670 + 60;
            }

            if (static_cast<s32>(gnFrameCount_507670) <= field_118)
            {
                if (pEventSystem_4FF954->MatchBuffer_40FAA0(field_12C, field_13C, field_13E) != GameSpeakMatch::eFullMatch_1)
                {
                    if (pEventSystem_4FF954->MatchBuffer_40FAA0(
                            field_12C,
                            field_13C,
                            field_13E)
                        != GameSpeakMatch::eNoMatch_0) // Full or partial
                    {
                        return field_1BA_brain_sub_state;
                    }
                }
            }

            field_1A0 = pEventSystem_4FF954->MatchBuffer_40FAA0(field_12C, field_13C, field_13E) == GameSpeakMatch::eFullMatch_1 || sVoiceCheat_507708;
            field_1C0_timer = gnFrameCount_507670 + 30;
            return 5;
        }

        case 5:
            if (static_cast<s32>(gnFrameCount_507670) > field_1C0_timer)
            {
                if (field_1A0)
                {
                    Mudokon_SFX_42A4D0(MudSounds::eOkay_13, 0, 300, this);
                    field_FE_next_motion = 3;
                }
                else
                {
                    Mudokon_SFX_42A4D0(MudSounds::eRefuse_14, 0, 300, this);
                    field_FE_next_motion = 6;
                }
                return 6;
            }
            break;

        case 6:
            if (field_FC_current_motion == eMudMotions::Motion_0_Idle_43CA70)
            {
                if (field_1A0)
                {
                    field_1B8_brain_idx = field_188;
                }
                else
                {
                    field_1B8_brain_idx = 2;
                }
                return 0;
            }
            break;

        case 8:
            if (static_cast<s32>(gnFrameCount_507670) > field_1C0_timer)
            {
                return 0;
            }
            break;

        default:
            return field_1BA_brain_sub_state;
    }
    return field_1BA_brain_sub_state;
}

s16 Mudokon::Brain_SingSequencePassword_4_441260()
{
    switch (field_1BA_brain_sub_state)
    {
        case 0:
            field_FE_next_motion = eMudMotions::Motion_3_Speak_43D440;
            Mudokon_SFX_42A4D0(MudSounds::ePassword_9, 0, 300, this);
            field_13C = GameSpeak::sub_40FA60(field_1A4_code_converted, field_12C);
            return 1;

        case 1:
            field_1C0_timer = gnFrameCount_507670;
            return 2;

        case 2:
        {
            GameSpeakEvents last_speak = {};
            if (field_128 == pEventSystem_4FF954->field_18_last_event_index)
            {
                if (pEventSystem_4FF954->field_10_last_event == GameSpeakEvents::eNone_m1)
                {
                    last_speak = GameSpeakEvents::eNone_m1;
                }
                else
                {
                    last_speak = GameSpeakEvents::eSameAsLast_m2;
                }
            }
            else
            {
                field_128 = pEventSystem_4FF954->field_18_last_event_index;
                last_speak = pEventSystem_4FF954->field_10_last_event;
            }

            if (last_speak != GameSpeakEvents::eNone_m1 && last_speak != GameSpeakEvents::eSameAsLast_m2)
            {
                field_13E = static_cast<s16>(pEventSystem_4FF954->field_18_last_event_index);
                field_118 = gnFrameCount_507670 + 60;
                return 3;
            }
            break;
        }

        case 3:
        {
            GameSpeakEvents last_speak = {};
            if (field_128 == pEventSystem_4FF954->field_18_last_event_index)
            {
                if (pEventSystem_4FF954->field_10_last_event == GameSpeakEvents::eNone_m1)
                {
                    last_speak = GameSpeakEvents::eNone_m1;
                }
                else
                {
                    last_speak = GameSpeakEvents::eSameAsLast_m2;
                }
            }
            else
            {
                field_128 = pEventSystem_4FF954->field_18_last_event_index;
                last_speak = pEventSystem_4FF954->field_10_last_event;
            }

            if (last_speak != GameSpeakEvents::eNone_m1 && last_speak != GameSpeakEvents::eSameAsLast_m2)
            {
                field_118 = gnFrameCount_507670 + 60;
            }

            if (static_cast<s32>(gnFrameCount_507670) <= field_118)
            {
                if (pEventSystem_4FF954->MatchBuffer_40FAA0(field_12C, field_13C, field_13E) != GameSpeakMatch::eFullMatch_1)
                {
                    if (pEventSystem_4FF954->MatchBuffer_40FAA0(
                            field_12C,
                            field_13C,
                            field_13E)
                        != GameSpeakMatch::eNoMatch_0) // Full or partial
                    {
                        return field_1BA_brain_sub_state;
                    }
                }
            }

            field_1A0 = pEventSystem_4FF954->MatchBuffer_40FAA0(
                            field_12C,
                            field_13C,
                            field_13E)
                         == GameSpeakMatch::eFullMatch_1
                     || sVoiceCheat_507708;

            field_1C0_timer = gnFrameCount_507670 + 30;
            return 4;
        }

        case 4:
            if (static_cast<s32>(gnFrameCount_507670) > field_1C0_timer)
            {
                if (field_1A0)
                {
                    Mudokon_SFX_42A4D0(MudSounds::eOkay_13, 0, 300, this);
                    field_FE_next_motion = eMudMotions::Motion_3_Speak_43D440;
                }
                else
                {
                    if (Math_NextRandom() >= 128u)
                    {
                        Mudokon_SFX_42A4D0(MudSounds::eLaugh1_8, 0, 300, this);
                    }
                    else
                    {
                        Mudokon_SFX_42A4D0(MudSounds::eRefuse_14, 0, 300, this);
                    }
                    field_FE_next_motion = eMudMotions::Motion_6_Speak_43D440;
                }
                return 5;
            }
            break;

        case 5:
            if (field_FC_current_motion == eMudMotions::Motion_0_Idle_43CA70)
            {
                if (field_1A0)
                {
                    field_1B8_brain_idx = field_188;
                }
                else
                {
                    field_1B8_brain_idx = 2;
                }
                return 0;
            }
            break;

        case 6:
            if (static_cast<s32>(gnFrameCount_507670) > field_1C0_timer)
            {
                return 0;
            }
            break;

        default:
            return field_1BA_brain_sub_state;
    }

    return field_1BA_brain_sub_state;
}

s16 Mudokon::Brain_LiftUse_5_43C180()
{
    if (field_1BA_brain_sub_state)
    {
        if (field_1BA_brain_sub_state == 1 && !field_FC_current_motion)
        {
            if (field_184)
            {
                field_1B8_brain_idx = 1;
                return 0;
            }
            else
            {
                return 2;
            }
        }
    }
    else if (field_FC_current_motion == eMudMotions::Motion_0_Idle_43CA70)
    {
        field_FC_current_motion = eMudMotions::Motion_13_LiftGrabBegin_43D3F0;
        field_FE_next_motion = eMudMotions::Motion_12_LiftUse_43D360;
        field_194_pLiftPoint = nullptr;
        for (s32 i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
        {
            BaseAliveGameObject* pObj = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            if (pObj->field_4_typeId == Types::eLiftPoint_51)
            {
                auto pLiftPoint = static_cast<LiftPoint*>(pObj);
                if (field_110_lift_switch_id == pLiftPoint->field_278_lift_point_id)
                {
                    field_194_pLiftPoint = pLiftPoint;
                    pObj->field_C_refCount++;
                    break;
                }
            }
        }
        return 1;
    }
    return field_1BA_brain_sub_state;
}

s16 Mudokon::Brain_LeverUse_6_43C250()
{
    if (field_1BA_brain_sub_state || field_FC_current_motion)
    {
        return field_1BA_brain_sub_state;
    }

    field_FE_next_motion = eMudMotions::Motion_15_LeverUse_43D4B0;

    FP directedGridSize = {};
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        directedGridSize = -ScaleToGridSize_41FA30(field_BC_sprite_scale);
    }
    else
    {
        directedGridSize = ScaleToGridSize_41FA30(field_BC_sprite_scale);
    }

    auto pSwitch = static_cast<Lever*>(FindObjectOfType_418280(Types::eLever_97, field_A8_xpos + directedGridSize, field_AC_ypos - ScaleToGridSize_41FA30(field_BC_sprite_scale)));
    if (pSwitch)
    {
        pSwitch->vPull_481640(field_A8_xpos < pSwitch->field_A8_xpos);
    }

    return 1;
}

s16 Mudokon::Brain_GiveRings_7_43C2F0()
{
    switch (field_1BA_brain_sub_state)
    {
        case 0:
            if (sActiveHero_507678->field_168_ring_pulse_timer <= 0)
            {
                field_FE_next_motion = eMudMotions::Motion_52_Chant_43D520;
                field_1C0_timer = gnFrameCount_507670 + 30;
                return 1;
            }
            return 4;

        case 1:
            if (static_cast<s32>(gnFrameCount_507670) > field_1C0_timer)
            {
                auto pFlicker = ao_new<PossessionFlicker>();
                if (pFlicker)
                {
                    pFlicker->ctor_41A8C0(this, 10, 255, 128, 128);
                }
                field_1C0_timer = gnFrameCount_507670 + 15;
                return 2;
            }
            break;

        case 2:
            if (static_cast<s32>(gnFrameCount_507670) > field_1C0_timer)
            {
                PSX_RECT ourRect = {};
                VGetBoundingRect(&ourRect, 1);

                auto pMudRing = ao_new<AbilityRing>();
                if (pMudRing)
                {
                    pMudRing->ctor_455860(
                        FP_FromInteger((ourRect.w + ourRect.x) / 2),
                        FP_FromInteger((ourRect.h + ourRect.y) / 2),
                        RingTypes::eExplosive_Emit_Effect_2);
                }

                PSX_RECT heroRect = {};
                sActiveHero_507678->VGetBoundingRect(&heroRect, 1);

                auto pAbeRing = ao_new<AbilityRing>();
                if (pAbeRing)
                {
                    pAbeRing->ctor_455860(
                        FP_FromInteger((heroRect.w + heroRect.x) / 2),
                        FP_FromInteger((heroRect.h + heroRect.y) / 2),
                        RingTypes::eExplosive_Give_3);
                    pAbeRing->SetTarget_455EC0(sActiveHero_507678);
                }
                field_1C0_timer = gnFrameCount_507670 + 30;
                return 3;
            }
            break;

        case 3:
            if (static_cast<s32>(gnFrameCount_507670) > field_1C0_timer)
            {
                if (field_1AA_ring_timeout)
                {
                    sActiveHero_507678->field_168_ring_pulse_timer = field_1AA_ring_timeout + gnFrameCount_507670;
                }
                else
                {
                    sActiveHero_507678->field_168_ring_pulse_timer = gnFrameCount_507670 + 200000;
                }

                field_FE_next_motion = eMudMotions::Motion_0_Idle_43CA70;
                sActiveHero_507678->field_16C_bHaveShrykull = 0;
                return 4;
            }
            break;

        case 4:
            if (sActiveHero_507678->field_168_ring_pulse_timer <= 0)
            {
                field_1B8_brain_idx = 2;
                return 0;
            }
            break;

        default:
            return field_1BA_brain_sub_state;
    }
    return field_1BA_brain_sub_state;
}

s16 Mudokon::Brain_StandScrub_8_441F40()
{
    if (FindBirdPortal_440250())
    {
        AddAlerted();
        field_1B8_brain_idx = 12;
        return 0;
    }

    const GameSpeakEvents last_speak = LastGameSpeak_4400B0();
    if (last_speak == GameSpeakEvents::Slig_LookOut_6)
    {
        if (field_1BA_brain_sub_state != 4 && field_1BA_brain_sub_state != 5)
        {
            field_1BA_brain_sub_state = 4;
        }
    }

    switch (field_1BA_brain_sub_state)
    {
        case 0:
            field_FC_current_motion = eMudMotions::Motion_16_StandScrubLoop_43D7C0;
            field_FE_next_motion = -1;
            field_1C0_timer = (Math_NextRandom() % 64) + gnFrameCount_507670 + 35;
            return 1;

        case 1:
            if (last_speak == GameSpeakEvents::eHello_9 && sAlertedMudCount_507B90 == 0 && IAmNearestToAbe_440120())
            {
                AddAlerted();
                field_FE_next_motion = eMudMotions::Motion_21_StandScrubToIdle_43D8F0;
                field_1C0_timer = gnFrameCount_507670 + 15;
                return 3;
            }

            if (static_cast<s32>(gnFrameCount_507670) > field_1C0_timer
                && field_FC_current_motion == eMudMotions::Motion_16_StandScrubLoop_43D7C0)
            {
                field_FE_next_motion = eMudMotions::Motion_17_StandScrubLoopToPause_43D860;
            }

            if (field_FC_current_motion == eMudMotions::Motion_19_StandScrubPause_43D8A0)
            {
                field_1C0_timer = (Math_NextRandom() % 64) + gnFrameCount_507670 + 35;
                return 2;
            }
            return field_1BA_brain_sub_state;

        case 2:
            if (last_speak == GameSpeakEvents::eHello_9 && sAlertedMudCount_507B90 == 0 && IAmNearestToAbe_440120())
            {
                AddAlerted();
                field_FE_next_motion = eMudMotions::Motion_21_StandScrubToIdle_43D8F0;
                field_1C0_timer = gnFrameCount_507670 + 15;
                return 3;
            }

            if (static_cast<s32>(gnFrameCount_507670) > field_1C0_timer)
            {
                field_FE_next_motion = eMudMotions::Motion_18_StandScrubPauseToLoop_43D880;
                field_1C0_timer = (Math_NextRandom() % 64) + gnFrameCount_507670 + 35;
                return 1;
            }
            return field_1BA_brain_sub_state;

        case 3:
            if (static_cast<s32>(gnFrameCount_507670) <= field_1C0_timer || field_FC_current_motion != eMudMotions::Motion_0_Idle_43CA70)
            {
                if (field_FE_next_motion != -1 || field_FC_current_motion != eMudMotions::Motion_16_StandScrubLoop_43D7C0)
                {
                    return field_1BA_brain_sub_state;
                }
                field_FE_next_motion = eMudMotions::Motion_21_StandScrubToIdle_43D8F0;
                return field_1BA_brain_sub_state;
            }
            field_1B8_brain_idx = 10;
            return 0;

        case 4:
            switch (field_FC_current_motion)
            {
                case eMudMotions::Motion_0_Idle_43CA70:
                    field_FE_next_motion = eMudMotions::Motion_23_CrouchIdle_43E590;
                    return field_1BA_brain_sub_state;

                case eMudMotions::Motion_16_StandScrubLoop_43D7C0:
                case eMudMotions::Motion_19_StandScrubPause_43D8A0:
                    field_FE_next_motion = eMudMotions::Motion_0_Idle_43CA70;
                    return field_1BA_brain_sub_state;

                case eMudMotions::Motion_23_CrouchIdle_43E590:
                    field_FE_next_motion = eMudMotions::Motion_55_Duck_43EB90;
                    field_1C0_timer = gnFrameCount_507670 + 60;
                    return 5;

                default:
                    return field_1BA_brain_sub_state;
            }
            break;

        case 5:
            if (static_cast<s32>(gnFrameCount_507670) > field_1C0_timer)
            {
                field_FE_next_motion = eMudMotions::Motion_23_CrouchIdle_43E590;
                return 6;
            }
            return field_1BA_brain_sub_state;

        case 6:
            if (field_FC_current_motion == eMudMotions::Motion_0_Idle_43CA70)
            {
                field_FE_next_motion = eMudMotions::Motion_16_StandScrubLoop_43D7C0;
            }
            else
            {
                if (field_FC_current_motion == eMudMotions::Motion_16_StandScrubLoop_43D7C0)
                {
                    return 0;
                }

                if (field_FC_current_motion == eMudMotions::Motion_23_CrouchIdle_43E590)
                {
                    field_FE_next_motion = eMudMotions::Motion_0_Idle_43CA70;
                }
            }
            return field_1BA_brain_sub_state;

        default:
            return field_1BA_brain_sub_state;
    }
}

s16 Mudokon::Brain_CrouchScrub_9_4422A0()
{
    if (FindBirdPortal_440250())
    {
        AddAlerted();
        field_1B8_brain_idx = 12;
        return 0;
    }

    const GameSpeakEvents last_speak = LastGameSpeak_4400B0();
    if (last_speak == GameSpeakEvents::Slig_LookOut_6)
    {
        field_FE_next_motion = eMudMotions::Motion_55_Duck_43EB90;
        field_1C0_timer = gnFrameCount_507670 + 60;
        return 5;
    }

    switch (field_1BA_brain_sub_state)
    {
        case 0:
            field_FC_current_motion = eMudMotions::Motion_22_CrouchScrub_43D910;
            field_FE_next_motion = -1;
            field_1C0_timer = (Math_NextRandom() % 64) + 15;
            field_114 = (Math_NextRandom() % 64) + 240;
            return 2;

        case 1:
            if (last_speak == GameSpeakEvents::eHello_9 && sAlertedMudCount_507B90 == 0 && IAmNearestToAbe_440120())
            {
                AddAlerted();
                field_1C0_timer = gnFrameCount_507670 + 15;
                return 3;
            }

            if (field_FC_current_motion == eMudMotions::Motion_23_CrouchIdle_43E590 && static_cast<s32>(gnFrameCount_507670) > field_114)
            {
                field_FE_next_motion = eMudMotions::Motion_24_CrouchTurn_43E5F0;
                field_114 = (Math_NextRandom() % 64) + gnFrameCount_507670 + 240;
            }

            if (field_FC_current_motion == eMudMotions::Motion_23_CrouchIdle_43E590 && static_cast<s32>(gnFrameCount_507670) > field_1C0_timer)
            {
                field_FE_next_motion = eMudMotions::Motion_22_CrouchScrub_43D910;
                field_1C0_timer = (Math_NextRandom() % 64) + gnFrameCount_507670 + 35;
            }

            if (field_FC_current_motion != eMudMotions::Motion_22_CrouchScrub_43D910)
            {
                return field_1BA_brain_sub_state;
            }
            return 2;

        case 2:
            if (last_speak != GameSpeakEvents::eHello_9 || sAlertedMudCount_507B90 || !IAmNearestToAbe_440120())
            {
                if (field_FC_current_motion == eMudMotions::Motion_23_CrouchIdle_43E590)
                {
                    if (static_cast<s32>(gnFrameCount_507670) > field_1C0_timer)
                    {
                        field_1C0_timer = (Math_NextRandom() % 64) + gnFrameCount_507670 + 15;
                        return 1;
                    }
                    field_FC_current_motion = eMudMotions::Motion_22_CrouchScrub_43D910;
                }

                if (Event_Is_Event_In_Range_417270(
                        kEventSpeaking_1,
                        field_A8_xpos,
                        field_AC_ypos,
                        field_BC_sprite_scale))
                {
                    field_1C0_timer = (Math_NextRandom() % 64) + gnFrameCount_507670 + 15;
                    return 4;
                }
            }
            else
            {
                AddAlerted();
                field_1C0_timer = gnFrameCount_507670 + 15;
                return 3;
            }
            break;

        case 3:
            if (static_cast<s32>(gnFrameCount_507670) > field_1C0_timer)
            {
                if (field_FC_current_motion == eMudMotions::Motion_23_CrouchIdle_43E590)
                {
                    field_FE_next_motion = eMudMotions::Motion_26_CrouchToStand_43E640;
                }

                if (field_FC_current_motion == eMudMotions::Motion_0_Idle_43CA70)
                {
                    field_1BA_brain_sub_state = 0;
                    field_1B8_brain_idx = 10;
                    return 0;
                }

                return field_1BA_brain_sub_state;
            }
            break;

        case 4:
            if (last_speak != GameSpeakEvents::eHello_9 || sAlertedMudCount_507B90 || !IAmNearestToAbe_440120())
            {
                if (static_cast<s32>(gnFrameCount_507670) > field_1C0_timer)
                {
                    return 1;
                }
            }
            else
            {
                AddAlerted();
                field_1C0_timer = gnFrameCount_507670 + 15;
                return 3;
            }
            break;

        case 5:
            if (static_cast<s32>(gnFrameCount_507670) > field_1C0_timer)
            {
                field_FE_next_motion = eMudMotions::Motion_23_CrouchIdle_43E590;

                field_1C0_timer = (Math_NextRandom() % 64) + gnFrameCount_507670 + 15;
                field_114 = (Math_NextRandom() % 64) + gnFrameCount_507670 + 240;

                if (field_100_health > FP_FromInteger(0))
                {
                    field_100_health = FP_FromInteger(1);
                }
                return 2;
            }
            break;

        case 6:
            if (static_cast<s32>(gnFrameCount_507670) > field_1C0_timer)
            {
                field_FE_next_motion = eMudMotions::Motion_61_DuckKnockback_43E6E0;
                field_1C0_timer = gnFrameCount_507670 + 60;
                return 5;
            }
            break;

        default:
            break;
    }

    return field_1BA_brain_sub_state;
}

s16 Mudokon::Brain_ListeningToAbe_10_440300()
{
    if (FindBirdPortal_440250())
    {
        field_1B8_brain_idx = 12;
        field_FE_next_motion = eMudMotions::Motion_0_Idle_43CA70;
        return 0;
    }

    switch (field_1BA_brain_sub_state)
    {
        case 0:
            field_144_flags.Set(Flags_144::e144_Bit6_bPersist);
            field_FE_next_motion = -1;
            field_FC_current_motion = eMudMotions::Motion_0_Idle_43CA70;
            field_1B0 = -1;
            return 1;

        case 1:
            if (field_FC_current_motion != eMudMotions::Motion_0_Idle_43CA70)
            {
                return field_1BA_brain_sub_state;
            }

            if (!VIsFacingMe(sActiveHero_507678))
            {
                field_FE_next_motion = eMudMotions::Motion_2_StandingTurn_43D050;
                return field_1BA_brain_sub_state;
            }
            Mudokon_SFX_42A4D0(MudSounds::eHello_3, 0, field_124_voice_pitch, this);
            field_FE_next_motion = eMudMotions::Motion_4_Speak_43D440;
            return 6;

        case 2:
            if (field_FC_current_motion == eMudMotions::Motion_0_Idle_43CA70)
            {
                field_FE_next_motion = eMudMotions::Motion_1_WalkLoop_43CC80;
                return field_1BA_brain_sub_state;
            }

            if (field_FC_current_motion == eMudMotions::Motion_1_WalkLoop_43CC80)
            {
                field_FE_next_motion = eMudMotions::Motion_0_Idle_43CA70;
                return 3;
            }
            return field_1BA_brain_sub_state;

        case 3:
        {
            FP gridSizeDirected = {};
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                gridSizeDirected = -ScaleToGridSize_41FA30(field_BC_sprite_scale);
            }
            else
            {
                gridSizeDirected = ScaleToGridSize_41FA30(field_BC_sprite_scale);
            }
            const s16 bHitWall = WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), gridSizeDirected);
            const s16 bEndOfLine = Check_IsOnEndOfLine_4021A0(field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX), 1);

            if (field_FC_current_motion != eMudMotions::Motion_0_Idle_43CA70)
            {
                const s16 oldState = field_1BA_brain_sub_state;
                field_FE_next_motion = eMudMotions::Motion_0_Idle_43CA70;
                field_1B4_idle_time = 0;
                return oldState;
            }

            field_1B4_idle_time++;

            if (field_1B4_idle_time <= 150 || BetweenCameras_418500() != BetweenCamPos::None_0)
            {
                if (!VIsFacingMe(sActiveHero_507678))
                {
                    field_FE_next_motion = eMudMotions::Motion_2_StandingTurn_43D050;
                    return field_1BA_brain_sub_state;
                }

                if (VIsObjNearby(ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromInteger(2), sActiveHero_507678)
                    || bHitWall
                    || bEndOfLine)
                {
                    GameSpeakEvents last_speak = {};
                    if (bHitWall || bEndOfLine)
                    {
                        last_speak = LastGameSpeak_4400B0();
                    }
                    else
                    {
                        if (field_128 == pEventSystem_4FF954->field_18_last_event_index)
                        {
                            if (pEventSystem_4FF954->field_10_last_event == GameSpeakEvents::eNone_m1)
                            {
                                last_speak = GameSpeakEvents::eNone_m1;
                            }
                            else
                            {
                                last_speak = GameSpeakEvents::eSameAsLast_m2;
                            }
                        }
                        else
                        {
                            last_speak = pEventSystem_4FF954->field_10_last_event;
                            field_128 = pEventSystem_4FF954->field_18_last_event_index;
                        }
                    }

                    if (Event_Get_417250(kEventMudokonComfort_16)
                        && gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                            field_B2_lvl_number,
                            field_B0_path_number,
                            field_A8_xpos,
                            field_AC_ypos,
                            1))
                    {
                        field_1B0 = field_1BA_brain_sub_state;
                        field_1C0_timer = gnFrameCount_507670 + Math_RandomRange_450F20(22, 30);
                        return 13;
                    }

                    if (last_speak == GameSpeakEvents::eNone_m1)
                    {
                        return field_1BA_brain_sub_state;
                    }

                    field_1B0 = field_1BA_brain_sub_state;
                    field_1C0_timer = gnFrameCount_507670 + 20;

                    switch (last_speak)
                    {
                        case GameSpeakEvents::eWhistleHigh_1:
                            return 17;

                        case GameSpeakEvents::eWhistleLow_2:
                            return 18;

                        case GameSpeakEvents::eFart_3:
                            field_1C0_timer = gnFrameCount_507670 + 30;
                            return 13;

                        case GameSpeakEvents::eLaugh_4:
                            return 16;

                        case GameSpeakEvents::eHello_9:
                            return 11;

                        case GameSpeakEvents::eFollowMe_10:
                            if (bHitWall)
                            {
                                return 9;
                            }
                            field_1B0 = 2;
                            return 10;

                        case GameSpeakEvents::eAnger_11:
                            return 14;

                        case GameSpeakEvents::eWait_12:
                            if (BetweenCameras_418500() == BetweenCamPos::None_0)
                            {
                                field_1B0 = 6;
                                return 10;
                            }
                            return 9;

                        default:
                            return field_1BA_brain_sub_state;
                    }
                }
                else
                {
                    if (IsAbeSneaking_43D660(this))
                    {
                        field_FE_next_motion = eMudMotions::Motion_35_SneakLoop_43E0F0;
                    }
                    else
                    {
                        field_FE_next_motion = eMudMotions::Motion_1_WalkLoop_43CC80;
                    }
                    return 4;
                }
            }
            else
            {
                if (!field_148_res_array.res[12])
                {
                    field_FE_next_motion = eMudMotions::Motion_23_CrouchIdle_43E590;
                }
                else
                {
                    field_FE_next_motion = eMudMotions::Motion_59_CrouchChant_43EC20;
                }
                return 19;
            }
            break;
        }

        case 4:
        {
            if (field_FC_current_motion == eMudMotions::Motion_0_Idle_43CA70)
            {
                return 3;
            }

            if (field_FC_current_motion != eMudMotions::Motion_1_WalkLoop_43CC80 && field_FC_current_motion != eMudMotions::Motion_35_SneakLoop_43E0F0)
            {
                return field_1BA_brain_sub_state;
            }

            FP gridSizeDirected = {};
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                gridSizeDirected = -ScaleToGridSize_41FA30(field_BC_sprite_scale);
            }
            else
            {
                gridSizeDirected = ScaleToGridSize_41FA30(field_BC_sprite_scale);
            }

            const s16 bHitWall = WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), gridSizeDirected);

            if (Check_IsOnEndOfLine_4021A0(field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX), 1) || bHitWall)
            {
                field_FE_next_motion = eMudMotions::Motion_0_Idle_43CA70;
                return 3;
            }

            if (!VIsFacingMe(sActiveHero_507678))
            {
                field_FE_next_motion = eMudMotions::Motion_0_Idle_43CA70;
                return 3;
            }

            if (VIsObjNearby(ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromInteger(2), sActiveHero_507678))
            {
                field_FE_next_motion = eMudMotions::Motion_0_Idle_43CA70;
                return 3;
            }

            GameSpeakEvents last_speak = {};
            if (field_128 == pEventSystem_4FF954->field_18_last_event_index)
            {
                if (pEventSystem_4FF954->field_10_last_event == GameSpeakEvents::eNone_m1)
                {
                    last_speak = GameSpeakEvents::eNone_m1;
                }
                else
                {
                    last_speak = GameSpeakEvents::eSameAsLast_m2;
                }
            }
            else
            {
                last_speak = pEventSystem_4FF954->field_10_last_event;
                field_128 = pEventSystem_4FF954->field_18_last_event_index;
            }

            if (last_speak == GameSpeakEvents::eWait_12
                && BetweenCameras_418500() == BetweenCamPos::None_0)
            {
                field_FE_next_motion = eMudMotions::Motion_0_Idle_43CA70;
                return 8;
            }

            if (sActiveHero_507678->field_FC_current_motion == eAbeMotions::Motion_35_RunLoop_425060
                || sActiveHero_507678->field_FC_current_motion == eAbeMotions::Motion_25_RollLoop_427BB0)
            {
                field_FE_next_motion = eMudMotions::Motion_29_RunLoop_43DB10;
                return 5;
            }

            if (sActiveHero_507678->field_FC_current_motion == eAbeMotions::Motion_42_SneakLoop_424BB0
                && field_FC_current_motion == eMudMotions::Motion_1_WalkLoop_43CC80)
            {
                field_FE_next_motion = eMudMotions::Motion_35_SneakLoop_43E0F0;
            }

            if (sActiveHero_507678->field_FC_current_motion == eAbeMotions::Motion_1_WalkLoop_423F90 && field_FC_current_motion == eMudMotions::Motion_35_SneakLoop_43E0F0)
            {
                field_FE_next_motion = eMudMotions::Motion_1_WalkLoop_43CC80;
            }

            return field_1BA_brain_sub_state;
        }

        case 5:
        {
            if (field_FC_current_motion == eMudMotions::Motion_0_Idle_43CA70)
            {
                return 3;
            }

            field_FE_next_motion = eMudMotions::Motion_29_RunLoop_43DB10;
            if (field_FC_current_motion != eMudMotions::Motion_29_RunLoop_43DB10)
            {
                return field_1BA_brain_sub_state;
            }

            FP gridSizeDirected = {};
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                gridSizeDirected = -ScaleToGridSize_41FA30(field_BC_sprite_scale);
            }
            else
            {
                gridSizeDirected = ScaleToGridSize_41FA30(field_BC_sprite_scale);
            }

            const s16 bHitWall = WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), gridSizeDirected * FP_FromInteger(3));
            if (Check_IsOnEndOfLine_4021A0(field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX) ? 1 : 0, 4)
                || bHitWall)
            {
                field_FE_next_motion = eMudMotions::Motion_0_Idle_43CA70;
                return 3;
            }

            switch (sActiveHero_507678->field_FC_current_motion)
            {
                case eAbeMotions::Motion_1_WalkLoop_423F90:
                    field_FE_next_motion = eMudMotions::Motion_1_WalkLoop_43CC80;
                    return 4;

                case eAbeMotions::Motion_70_Knockback_428FB0:
                case eAbeMotions::Motion_27_RunSlideStop_425B60:
                case eAbeMotions::Motion_0_Idle_423520:
                    //[[fallthrough]];
                case eAbeMotions::Motion_19_CrouchIdle_4284C0:
                    if (sActiveHero_507678->field_FC_current_motion == eAbeMotions::Motion_70_Knockback_428FB0)
                    {
                        Mudokon_SFX_42A4D0(MudSounds::eLaugh1_8, 0, field_124_voice_pitch, this);
                    }

                    if (VIsObjNearby(ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromInteger(2), sActiveHero_507678))
                    {
                        field_FE_next_motion = eMudMotions::Motion_0_Idle_43CA70;
                        return 3;
                    }
                    else
                    {
                        field_FE_next_motion = eMudMotions::Motion_1_WalkLoop_43CC80;
                        return 4;
                    }
                    break;

                default:
                    if (!VIsFacingMe(sActiveHero_507678))
                    {
                        field_FE_next_motion = eMudMotions::Motion_33_RunSlideTurn_43DF80;
                        return 7;
                    }
                    return field_1BA_brain_sub_state;
            }
            break;
        }

        case 6:
            if (field_FC_current_motion != eMudMotions::Motion_0_Idle_43CA70)
            {
                field_1B4_idle_time = 0;
                return field_1BA_brain_sub_state;
            }

            field_1B4_idle_time++;

            if (field_1B4_idle_time <= 150
                && gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                    field_B2_lvl_number,
                    field_B0_path_number,
                    field_A8_xpos,
                    field_AC_ypos,
                    1)
                && sAlertedMudCount_507B90 <= 1)
            {
                if (!VIsFacingMe(sActiveHero_507678))
                {
                    field_FE_next_motion = eMudMotions::Motion_2_StandingTurn_43D050;
                    return field_1BA_brain_sub_state;
                }

                if (Event_Get_417250(kEventMudokonComfort_16)
                    && gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                        field_B2_lvl_number,
                        field_B0_path_number,
                        field_A8_xpos,
                        field_AC_ypos,
                        1))
                {
                    field_1B0 = field_1BA_brain_sub_state;
                    field_1C0_timer = gnFrameCount_507670 + Math_RandomRange_450F20(22, 30);
                    return 13;
                }
                const GameSpeakEvents speak = LastGameSpeak_4400B0();
                if (speak == GameSpeakEvents::eNone_m1)
                {
                    return field_1BA_brain_sub_state;
                }

                field_1C0_timer = gnFrameCount_507670 + 20;
                field_1B0 = field_1BA_brain_sub_state;

                switch (speak)
                {
                    case GameSpeakEvents::eWhistleHigh_1:
                        return 17;

                    case GameSpeakEvents::eWhistleLow_2:
                        return 18;

                    case GameSpeakEvents::eFart_3:
                        field_1C0_timer = gnFrameCount_507670 + 30;
                        return 13;

                    case GameSpeakEvents::eLaugh_4:
                        return 16;

                    case GameSpeakEvents::eHello_9:
                        return 11;

                    case GameSpeakEvents::eFollowMe_10:
                        field_1B0 = 3;
                        return 10;

                    case GameSpeakEvents::eAnger_11:
                        return 14;

                    case GameSpeakEvents::eWait_12:
                        return 15;

                    default:
                        return field_1BA_brain_sub_state;
                }
            }
            else
            {
                if (field_148_res_array.res[12])
                {
                    field_FE_next_motion = eMudMotions::Motion_59_CrouchChant_43EC20;
                }
                else
                {
                    field_FE_next_motion = eMudMotions::Motion_23_CrouchIdle_43E590;
                }
            }
            return 19;

        case 7:
            if (field_FC_current_motion == eMudMotions::Motion_29_RunLoop_43DB10)
            {
                return 5;
            }
            return field_1BA_brain_sub_state;

        case 8:
            if (field_FC_current_motion == eMudMotions::Motion_0_Idle_43CA70)
            {
                field_1C0_timer = gnFrameCount_507670 + 20;
                field_1B0 = 6;
                return 10;
            }
            return field_1BA_brain_sub_state;

        case 9:
            if (static_cast<s32>(gnFrameCount_507670) > field_1C0_timer)
            {
                Mudokon_SFX_42A4D0(MudSounds::eRefuse_14, 0, field_124_voice_pitch, this);
                field_FE_next_motion = eMudMotions::Motion_6_Speak_43D440;
                return 3;
            }
            return field_1BA_brain_sub_state;

        case 10:
            if (static_cast<s32>(gnFrameCount_507670) > field_1C0_timer)
            {
                Mudokon_SFX_42A4D0(MudSounds::eOkay_13, 0, field_124_voice_pitch, this);
                field_FE_next_motion = eMudMotions::Motion_3_Speak_43D440;
                return field_1B0;
            }
            return field_1BA_brain_sub_state;

        case 11:
            if (static_cast<s32>(gnFrameCount_507670) > field_1C0_timer)
            {
                Mudokon_SFX_42A4D0(MudSounds::eHello_3, 0, field_124_voice_pitch, this);
                field_FE_next_motion = eMudMotions::Motion_4_Speak_43D440;
                return field_1B0;
            }
            return field_1BA_brain_sub_state;

        case 12:
            if (static_cast<s32>(gnFrameCount_507670) > field_1C0_timer)
            {
                Mudokon_SFX_42A4D0(MudSounds::eLaugh1_8, 0, field_124_voice_pitch, this);
                field_FE_next_motion = eMudMotions::Motion_4_Speak_43D440;
                return field_1B0;
            }
            return field_1BA_brain_sub_state;

        case 13:
            if (static_cast<s32>(gnFrameCount_507670) > field_1C0_timer)
            {
                Mudokon_SFX_42A4D0(MudSounds::eLaugh2_11, 0, field_124_voice_pitch, this);
                field_FE_next_motion = eMudMotions::Motion_6_Speak_43D440;
                return field_1B0;
            }
            return field_1BA_brain_sub_state;


        case 14:
            if (static_cast<s32>(gnFrameCount_507670) > field_1C0_timer)
            {
                Mudokon_SFX_42A4D0(MudSounds::eDunno_15, 0, field_124_voice_pitch, this);
                field_FE_next_motion = eMudMotions::Motion_6_Speak_43D440;
                return field_1B0;
            }
            return field_1BA_brain_sub_state;

        case 15:
            if (static_cast<s32>(gnFrameCount_507670) > field_1C0_timer)
            {
                Mudokon_SFX_42A4D0(MudSounds::eAngry_5, 0, field_124_voice_pitch, this);
                field_FE_next_motion = eMudMotions::Motion_6_Speak_43D440;
                return field_1B0;
            }
            return field_1BA_brain_sub_state;

        case 16:
            if (static_cast<s32>(gnFrameCount_507670) > field_1C0_timer)
            {
                Mudokon_SFX_42A4D0(MudSounds::eFart_7, 0, field_124_voice_pitch, this);
                field_FE_next_motion = eMudMotions::Motion_3_Speak_43D440;
                if (sEnableFartGasCheat_507704 == 0)
                {
                    return field_1B0;
                }

                if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    const FP fart_scale = (FP_FromDouble(0.5) * field_BC_sprite_scale);
                    const FP fart_y = field_AC_ypos - (FP_FromInteger(24) * field_BC_sprite_scale);
                    const FP fart_x = (FP_FromInteger(12) * field_BC_sprite_scale) + field_A8_xpos;
                    New_Smoke_Particles_419A80(fart_x, fart_y, fart_scale, 3, 1);
                }
                else
                {
                    const FP fart_scale = (FP_FromDouble(0.5) * field_BC_sprite_scale);
                    const FP fart_y = field_AC_ypos - (FP_FromInteger(24) * field_BC_sprite_scale);
                    const FP fart_x = field_A8_xpos - (FP_FromInteger(12) * field_BC_sprite_scale);
                    New_Smoke_Particles_419A80(fart_x, fart_y, fart_scale, 3, 1);
                }

                return field_1B0;
            }
            break;

        case 17:
            if (static_cast<s32>(gnFrameCount_507670) > field_1C0_timer)
            {
                Mudokon_SFX_42A4D0(MudSounds::eWhistleHigh_1, 0, 0, this);
                field_FE_next_motion = eMudMotions::Motion_4_Speak_43D440;
                return field_1B0;
            }
            break;

        case 18:
            if (static_cast<s32>(gnFrameCount_507670) > field_1C0_timer)
            {
                Mudokon_SFX_42A4D0(MudSounds::eWhistleLow_2, 0, 0, this);
                field_FE_next_motion = eMudMotions::Motion_5_Speak_43D440;
                return field_1B0;
            }
            break;

        case 19:
            if (field_FC_current_motion != eMudMotions::Motion_23_CrouchIdle_43E590 && field_FC_current_motion != eMudMotions::Motion_59_CrouchChant_43EC20)
            {
                return field_1BA_brain_sub_state;
            }

            RemoveAlerted();

            if (field_148_res_array.res[12])
            {
                field_1B8_brain_idx = 14;
            }
            else
            {
                field_1B8_brain_idx = 9;
            }
            return 0;

        default:
            break;
    }
    return field_1BA_brain_sub_state;
}

s16 Mudokon::Brain_ShrivelDeath_11_43C5F0()
{
    if (field_1C0_timer < static_cast<s32>(gnFrameCount_507670) + 80)
    {
        field_C0_r -= 2;
        field_C2_g -= 2;
        field_C4_b -= 2;
        field_BC_sprite_scale = field_BC_sprite_scale - FP_FromDouble(0.008);
    }

    if (static_cast<s32>(gnFrameCount_507670) < field_1C0_timer - 24)
    {
        DeathSmokeEffect(true);
    }

    if (field_BC_sprite_scale < FP_FromInteger(0))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    return 100;
}

s16 Mudokon::Brain_Escape_12_440FD0()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(Options::eDead_Bit3);
        return field_1BA_brain_sub_state;
    }

    BirdPortal* pPortal = field_1AC_pBirdPortal;
    if (!pPortal || pPortal->field_6_flags.Get(BaseGameObject::eDead_Bit3))
    {
        sMudRunningToPortalCount_507B94--;
        if (pPortal)
        {
            pPortal->field_C_refCount--;
        }

        field_144_flags.Set(Flags_144::e144_Bit6_bPersist);
        field_1AC_pBirdPortal = nullptr;
        field_FE_next_motion = eMudMotions::Motion_0_Idle_43CA70;
        field_1B8_brain_idx = 10;
        return 6;
    }

    switch (field_1BA_brain_sub_state)
    {
        case 0:
            if (pPortal->VStateIs6())
            {
                field_1C0_timer = gnFrameCount_507670 + (Math_NextRandom() % 8);
                return 1;
            }
            break;

        case 1:
            if (static_cast<s32>(gnFrameCount_507670) > field_1C0_timer)
            {
                if (FP_Abs(pPortal->field_18_xpos - field_A8_xpos) >= ScaleToGridSize_41FA30(field_BC_sprite_scale))
                {
                    return 2;
                }
                return 4;
            }
            break;

        case 2:
            if (field_FC_current_motion == eMudMotions::Motion_23_CrouchIdle_43E590)
            {
                field_FE_next_motion = eMudMotions::Motion_26_CrouchToStand_43E640;
            }

            if (field_FC_current_motion == eMudMotions::Motion_16_StandScrubLoop_43D7C0)
            {
                field_FE_next_motion = eMudMotions::Motion_21_StandScrubToIdle_43D8F0;
            }

            if (field_FC_current_motion == eMudMotions::Motion_19_StandScrubPause_43D8A0)
            {
                field_FE_next_motion = eMudMotions::Motion_18_StandScrubPauseToLoop_43D880;
            }

            if (field_FC_current_motion == eMudMotions::Motion_0_Idle_43CA70 || field_FC_current_motion == eMudMotions::Motion_1_WalkLoop_43CC80)
            {
                if (FacingTarget_43D6A0(pPortal))
                {
                    field_FE_next_motion = eMudMotions::Motion_29_RunLoop_43DB10;
                }
                else
                {
                    field_FE_next_motion = eMudMotions::Motion_2_StandingTurn_43D050;
                }
            }

            if (field_FC_current_motion == eMudMotions::Motion_29_RunLoop_43DB10)
            {
                if (!FacingTarget_43D6A0(field_1AC_pBirdPortal))
                {
                    field_FE_next_motion = eMudMotions::Motion_33_RunSlideTurn_43DF80;
                }
                else
                {
                    if (IntoBirdPortal_402350(3))
                    {
                        field_1BA_brain_sub_state = 3;
                        field_FE_next_motion = eMudMotions::Motion_44_RunJumpMid_43E960;
                    }
                }
            }
            break;

        case 4:
            if (field_FC_current_motion == eMudMotions::Motion_23_CrouchIdle_43E590)
            {
                field_FE_next_motion = eMudMotions::Motion_26_CrouchToStand_43E640;
            }

            if (field_FC_current_motion == eMudMotions::Motion_16_StandScrubLoop_43D7C0)
            {
                field_FE_next_motion = eMudMotions::Motion_21_StandScrubToIdle_43D8F0;
            }

            if (field_FC_current_motion == eMudMotions::Motion_19_StandScrubPause_43D8A0)
            {
                field_FE_next_motion = eMudMotions::Motion_18_StandScrubPauseToLoop_43D880;
            }

            if (field_FC_current_motion == eMudMotions::Motion_0_Idle_43CA70 || field_FC_current_motion == eMudMotions::Motion_1_WalkLoop_43CC80)
            {
                if ((pPortal->field_12_side == PortalSide::eRight_0) == field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    field_FE_next_motion = eMudMotions::Motion_2_StandingTurn_43D050;
                }
                else
                {
                    field_FE_next_motion = eMudMotions::Motion_29_RunLoop_43DB10;
                }
            }

            if (field_FC_current_motion == eMudMotions::Motion_29_RunLoop_43DB10)
            {
                if (FP_Abs(pPortal->field_18_xpos - field_A8_xpos) <= ScaleToGridSize_41FA30(field_BC_sprite_scale))
                {
                    return field_1BA_brain_sub_state;
                }
                return 2;
            }
            break;

        default:
            return field_1BA_brain_sub_state;
    }

    return field_1BA_brain_sub_state;
}

s16 Mudokon::Brain_FallAndSmackDeath_13_43C700()
{
    if (field_1BA_brain_sub_state)
    {
        if (field_1BA_brain_sub_state == 1)
        {
            if (static_cast<s32>(gnFrameCount_507670) == field_1C0_timer - 6)
            {
                SND_SEQ_Play_477760(SeqId::eHitBottomOfDeathPit_10, 1, 65, 65);
            }

            if (static_cast<s32>(gnFrameCount_507670) > field_1C0_timer)
            {
                Environment_SFX_42A220(EnvironmentSfx::eFallingDeathScreamHitGround_15, 0, 0x7FFF, this);

                auto pShake = ao_new<ScreenShake>();
                if (pShake)
                {
                    pShake->ctor_4624D0(0);
                }
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
            }
        }
        return field_1BA_brain_sub_state;
    }
    else
    {
        Mudokon_SFX_42A4D0(MudSounds::eDeathDropScream_17, 0, 0, this);
        field_1C0_timer = gnFrameCount_507670 + 60;
        return 1;
    }
}

s16 Mudokon::Brain_Chant_14_442710()
{
    if (FindBirdPortal_440250())
    {
        AddAlerted();
        field_1B8_brain_idx = 12;
        return 0;
    }

    const GameSpeakEvents lastSpeak = LastGameSpeak_4400B0();
    if (lastSpeak == GameSpeakEvents::Slig_LookOut_6)
    {
        field_FE_next_motion = eMudMotions::Motion_55_Duck_43EB90;
        field_1C0_timer = gnFrameCount_507670 + 60;
        return 5;
    }

    switch (field_1BA_brain_sub_state)
    {
        case 0:
            field_FC_current_motion = eMudMotions::Motion_59_CrouchChant_43EC20;
            field_FE_next_motion = -1;
            return 1;

        case 1:
            if (field_1B6)
            {
                field_FE_next_motion = eMudMotions::Motion_0_Idle_43CA70;
                return 3;
            }

            if (lastSpeak == GameSpeakEvents::eHello_9 && sAlertedMudCount_507B90 == 0 && IAmNearestToAbe_440120())
            {
                AddAlerted();
                field_1C0_timer = gnFrameCount_507670 + 15;
                return 2;
            }
            break;

        case 2:
            if (static_cast<s32>(gnFrameCount_507670) > field_1C0_timer && field_FC_current_motion == eMudMotions::Motion_0_Idle_43CA70)
            {
                field_1BA_brain_sub_state = 0;
                field_1B8_brain_idx = 10;
                return field_1BA_brain_sub_state;
            }
            break;

        case 3:
            if (!field_148_res_array.res[11])
            {
                field_148_res_array.res[11] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kAbewaspAOResID, 1, 0);
            }

            if (field_FC_current_motion == eMudMotions::Motion_57_Struggle_43EBE0)
            {
                field_1B6 = 0;
                field_1C0_timer = gnFrameCount_507670 + 32;
                return 4;
            }
            field_FE_next_motion = eMudMotions::Motion_57_Struggle_43EBE0;
            break;

        case 4:
            if (static_cast<s32>(gnFrameCount_507670) > field_1C0_timer)
            {
                if (field_1B6 == 0)
                {
                    field_FE_next_motion = eMudMotions::Motion_0_Idle_43CA70;
                    return 5;
                }
                field_1B6 = 0;
                field_1C0_timer = gnFrameCount_507670 + 32;
            }
            break;

        case 5:
            if (field_FC_current_motion == eMudMotions::Motion_0_Idle_43CA70)
            {
                field_FE_next_motion = eMudMotions::Motion_59_CrouchChant_43EC20;
                return 1;
            }
            break;

        default:
            return field_1BA_brain_sub_state;
    }
    return field_1BA_brain_sub_state;
}

s16 Mudokon::Brain_Choke_15_43C5D0()
{
    if (field_FC_current_motion != eMudMotions::Motion_62_PoisonGasDeath_43ED70)
    {
        field_FE_next_motion = eMudMotions::Motion_62_PoisonGasDeath_43ED70;
    }
    return 0;
}

} // namespace AO
