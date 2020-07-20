#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"

void Scrab_ForceLink();

START_NS_AO

#define SCRAB_STATES_ENUM(ENTRY) \
    ENTRY(State_0_Empty_45E3D0) \
    ENTRY(State_1_Stand_45E620) \
    ENTRY(State_2_Walk_45E730) \
    ENTRY(State_3_Run_45EAB0) \
    ENTRY(State_4_Turn_45EF30) \
    ENTRY(State_5_RunToStand_45ED90) \
    ENTRY(State_6_HopBegin_45F3C0) \
    ENTRY(State_7_HopMidair_45F1A0) \
    ENTRY(State_8_Land_45F500) \
    ENTRY(State_9_JumpToFall_45EFD0) \
    ENTRY(State_10_StandToWalk_45E670) \
    ENTRY(State_11_StandToRun_45E9F0) \
    ENTRY(State_12_WalkToStand_45E930) \
    ENTRY(State_13_RunJumpBegin_45F5D0) \
    ENTRY(State_14_RunJumpEnd_45F850) \
    ENTRY(State_15_ToFall_45F180) \
    ENTRY(State_16_Stamp_45F920) \
    ENTRY(State_17_Empty_45F9C0) \
    ENTRY(State_18_GetEaten_45FF70) \
    ENTRY(State_19_Unused_45F9D0) \
    ENTRY(State_20_HowlBegin_45FA60) \
    ENTRY(State_21_HowlEnd_45FAF0) \
    ENTRY(State_22_Shriek_45FB00) \
    ENTRY(State_23_ScrabBattleAnim_45FBA0) \
    ENTRY(State_24_FeedToGulp_45FC30) \
    ENTRY(State_25_ToFeed_45FCE0) \
    ENTRY(State_26_Feed_45FDA0) \
    ENTRY(State_27_AttackLunge_45FDF0) \
    ENTRY(State_28_LegKick_45FF60) \
    ENTRY(State_29_DeathBegin_45FFA0)

#define MAKE_ENUM(VAR) VAR,
enum eScrabStates : int
{
    SCRAB_STATES_ENUM(MAKE_ENUM)
};

class Scrab : public BaseAliveGameObject
{
public:
    EXPORT void VUpdate_Real_45B360();

    EXPORT void VUpdate_45B360();

    // States
    EXPORT void State_0_Empty_45E3D0();
    EXPORT void State_1_Stand_45E620();
    EXPORT void State_2_Walk_45E730();
    EXPORT void State_3_Run_45EAB0();
    EXPORT void State_4_Turn_45EF30();
    EXPORT void State_5_RunToStand_45ED90();
    EXPORT void State_6_HopBegin_45F3C0();
    EXPORT void State_7_HopMidair_45F1A0();
    EXPORT void State_8_Land_45F500();
    EXPORT void State_9_JumpToFall_45EFD0();
    EXPORT void State_10_StandToWalk_45E670();
    EXPORT void State_11_StandToRun_45E9F0();
    EXPORT void State_12_WalkToStand_45E930();
    EXPORT void State_13_RunJumpBegin_45F5D0();
    EXPORT void State_14_RunJumpEnd_45F850();
    EXPORT void State_15_ToFall_45F180();
    EXPORT void State_16_Stamp_45F920();
    EXPORT void State_17_Empty_45F9C0();
    EXPORT void State_18_GetEaten_45FF70();
    EXPORT void State_19_Unused_45F9D0();
    EXPORT void State_20_HowlBegin_45FA60();
    EXPORT void State_21_HowlEnd_45FAF0();
    EXPORT void State_22_Shriek_45FB00();
    EXPORT void State_23_ScrabBattleAnim_45FBA0();
    EXPORT void State_24_FeedToGulp_45FC30();
    EXPORT void State_25_ToFeed_45FCE0();
    EXPORT void State_26_Feed_45FDA0();
    EXPORT void State_27_AttackLunge_45FDF0();
    EXPORT void State_28_LegKick_45FF60();
    EXPORT void State_29_DeathBegin_45FFA0();

    int field_10C_fn;
    __int16 field_110;
    __int16 field_112;
    __int16 field_114;
    __int16 field_116;
    int field_118;
    BaseGameObject* field_11C;
    BaseAliveGameObject* field_120;
    int field_124;
    int field_128;
    int field_12C;
    __int16 field_130;
    __int16 field_132;
    int field_134;
    int field_138;
    int field_13C;
    int field_140;
    __int16 field_144;
    __int16 field_146;
    __int16 field_148;
    __int16 field_14A;
    int field_14C;
    int field_150_resources[14];
    __int16 field_188;
    __int16 field_18A;
};
ALIVE_ASSERT_SIZEOF(Scrab, 0x18C);

END_NS_AO

