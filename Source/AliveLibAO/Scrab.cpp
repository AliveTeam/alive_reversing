#include "stdafx_ao.h"
#include "Function.hpp"
#include "Scrab.hpp"

void Scrab_ForceLink() {}

START_NS_AO

using TScrabStateFunction = decltype(&Scrab::State_0_Empty_45E3D0);

const TScrabStateFunction sScrabMotionTable_4CF690[] =
{
    &Scrab::State_0_Empty_45E3D0,
    &Scrab::State_1_Stand_45E620,
    &Scrab::State_2_Walk_45E730,
    &Scrab::State_3_Run_45EAB0,
    &Scrab::State_4_Turn_45EF30,
    &Scrab::State_5_RunToStand_45ED90,
    &Scrab::State_6_HopBegin_45F3C0,
    &Scrab::State_7_HopMidair_45F1A0,
    &Scrab::State_8_Land_45F500,
    &Scrab::State_9_JumpToFall_45EFD0,
    &Scrab::State_10_StandToWalk_45E670,
    &Scrab::State_11_StandToRun_45E9F0,
    &Scrab::State_12_WalkToStand_45E930,
    &Scrab::State_13_RunJumpBegin_45F5D0,
    &Scrab::State_14_RunJumpEnd_45F850,
    &Scrab::State_15_ToFall_45F180,
    &Scrab::State_16_Stamp_45F920,
    &Scrab::State_17_Empty_45F9C0,
    &Scrab::State_18_GetEaten_45FF70,
    &Scrab::State_19_Unused_45F9D0,
    &Scrab::State_20_HowlBegin_45FA60,
    &Scrab::State_21_HowlEnd_45FAF0,
    &Scrab::State_22_Shriek_45FB00,
    &Scrab::State_23_ScrabBattleAnim_45FBA0,
    &Scrab::State_24_FeedToGulp_45FC30,
    &Scrab::State_25_ToFeed_45FCE0,
    &Scrab::State_26_Feed_45FDA0,
    &Scrab::State_27_AttackLunge_45FDF0,
    &Scrab::State_28_LegKick_45FF60,
    &Scrab::State_29_DeathBegin_45FFA0,
};

void Scrab::VUpdate_Real_45B360()
{
    NOT_IMPLEMENTED();
}

void Scrab::VUpdate_45B360()
{
    const __int16 oldMotion = field_FC_current_motion;

    VUpdate_Real_45B360();

    if (oldMotion != field_FC_current_motion)
    {
        LOG_INFO("oldMotion = " << oldMotion << " newMotion = " << field_FC_current_motion);
    }
}

void Scrab::State_0_Empty_45E3D0()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_1_Stand_45E620()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_2_Walk_45E730()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_3_Run_45EAB0()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_4_Turn_45EF30()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_5_RunToStand_45ED90()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_6_HopBegin_45F3C0()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_7_HopMidair_45F1A0()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_8_Land_45F500()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_9_JumpToFall_45EFD0()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_10_StandToWalk_45E670()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_11_StandToRun_45E9F0()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_12_WalkToStand_45E930()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_13_RunJumpBegin_45F5D0()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_14_RunJumpEnd_45F850()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_15_ToFall_45F180()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_16_Stamp_45F920()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_17_Empty_45F9C0()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_18_GetEaten_45FF70()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_19_Unused_45F9D0()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_20_HowlBegin_45FA60()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_21_HowlEnd_45FAF0()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_22_Shriek_45FB00()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_23_ScrabBattleAnim_45FBA0()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_24_FeedToGulp_45FC30()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_25_ToFeed_45FCE0()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_26_Feed_45FDA0()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_27_AttackLunge_45FDF0()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_28_LegKick_45FF60()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_29_DeathBegin_45FFA0()
{
    NOT_IMPLEMENTED();
}


END_NS_AO

