#include "stdafx_ao.h"
#include "Function.hpp"
#include "Mudokon.hpp"

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


void Mudokon::VUpdate_Real_43F560()
{
    NOT_IMPLEMENTED();
}

void Mudokon::VUpdate_43F560()
{
    const __int16 oldMotion = field_FC_current_motion;

    VUpdate_Real_43F560();

    if (oldMotion != field_FC_current_motion)
    {
        LOG_INFO("oldMotion = " << oldMotion << " newMotion = " << field_FC_current_motion);
    }
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

