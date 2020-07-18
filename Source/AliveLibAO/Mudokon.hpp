#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"

void Mud_ForceLink();

START_NS_AO

#define MUD_STATES_ENUM(ENTRY) \
    ENTRY(State_0_Idle_43CA70) \
    ENTRY(State_1_WalkLoop_43CC80) \
    ENTRY(State_2_StandingTurn_43D050) \
    ENTRY(State_3_Speak_43D440) \
    ENTRY(State_4_Speak_43D440) \
    ENTRY(State_5_Speak_43D440) \
    ENTRY(State_6_Speak_43D440) \
    ENTRY(State_7_WalkBegin_43CE60) \
    ENTRY(State_8_WalkToIdle_43CEF0) \
    ENTRY(State_9_MidWalkToIdle_43CFA0) \
    ENTRY(State_10_Unused_43D4D0) \
    ENTRY(State_11_Null_43D350) \
    ENTRY(State_12_LiftUse_43D360) \
    ENTRY(State_13_LiftGrabBegin_43D3F0) \
    ENTRY(State_14_LiftGrabEnd_43D420) \
    ENTRY(State_15_LeverUse_43D4B0) \
    ENTRY(State_16_StandScrubLoop_43D7C0) \
    ENTRY(State_17_StandScrubLoopToPause_43D860) \
    ENTRY(State_18_StandScrubPauseToLoop_43D880) \
    ENTRY(State_19_StandScrubPause_43D8A0) \
    ENTRY(State_20_IdleToStandScrub_43D8D0) \
    ENTRY(State_21_StandScrubToIdle_43D8F0) \
    ENTRY(State_22_CrouchScrub_43D910) \
    ENTRY(State_23_CrouchIdle_43E590) \
    ENTRY(State_24_CrouchTurn_43E5F0) \
    ENTRY(State_25_StandToCrouch_43E620) \
    ENTRY(State_26_CrouchToStand_43E640) \
    ENTRY(State_27_RunToWalk_43D980) \
    ENTRY(State_28_MidRunToWalk_43DA40) \
    ENTRY(State_29_RunLoop_43DB10) \
    ENTRY(State_30_RunToWalk_43DD50) \
    ENTRY(State_31_MidRunToWalk_43DE10) \
    ENTRY(State_32_RunSlideStop_43DEE0) \
    ENTRY(State_33_RunSlideTurn_43DF80) \
    ENTRY(State_34_RunTurnToRun_43E070) \
    ENTRY(State_35_SneakLoop_43E0F0) \
    ENTRY(State_36_WalkToSneak_43E240) \
    ENTRY(State_37_SneakToWalk_43E2E0) \
    ENTRY(State_38_MidWalkToSneak_43E380) \
    ENTRY(State_39_MidSneakToWalk_43E430) \
    ENTRY(State_40_SneakBegin_43E4E0) \
    ENTRY(State_41_SneakToIdle_43E530) \
    ENTRY(State_42_MidSneakToIdle_43E560) \
    ENTRY(State_43_JumpBegin_43E870) \
    ENTRY(State_44_JumpMid_43E960) \
    ENTRY(State_45_ToRunToPortal_43EB00) \
    ENTRY(State_46_FallLandDie_43E660) \
    ENTRY(State_47_Knockback_43E730) \
    ENTRY(State_48_KnockbackGetUp_43E7D0) \
    ENTRY(State_49_FallOfEdge_43E800) \
    ENTRY(State_50_LandSoft_43E820) \
    ENTRY(State_51_Fall_43D0D0) \
    ENTRY(State_52_Chant_43D520) \
    ENTRY(State_53_ChantEnd_43D640) \
    ENTRY(State_54_ToDuck_43EB70) \
    ENTRY(State_55_Duck_43EB90) \
    ENTRY(State_56_DuckToCrouch_43EBC0) \
    ENTRY(State_57_Struggle_43EBE0) \
    ENTRY(State_58_StruggleToCrouchChant_43EC00) \
    ENTRY(State_59_CrouchChant_43EC20) \
    ENTRY(State_60_CrouchChantToSruggle_43ED50) \
    ENTRY(State_61_DuckKnockback_43E6E0) \
    ENTRY(State_62_Choke_43ED70)

#define MAKE_ENUM(VAR) VAR,
enum eMudStates : int
{
    MUD_STATES_ENUM(MAKE_ENUM)
};


class Mudokon : public BaseAliveGameObject
{
public:
    EXPORT void VUpdate_Real_43F560();

    EXPORT void VUpdate_43F560();

    // States
    EXPORT void State_0_Idle_43CA70();
    EXPORT void State_1_WalkLoop_43CC80();
    EXPORT void State_2_StandingTurn_43D050();
    EXPORT void State_3_Speak_43D440();

    // Not exported as same func as 3
    void State_4_Speak_43D440();
    void State_5_Speak_43D440();
    void State_6_Speak_43D440();

    EXPORT void State_7_WalkBegin_43CE60();
    EXPORT void State_8_WalkToIdle_43CEF0();
    EXPORT void State_9_MidWalkToIdle_43CFA0();
    EXPORT void State_10_Unused_43D4D0();
    EXPORT void State_11_Null_43D350();
    EXPORT void State_12_LiftUse_43D360();
    EXPORT void State_13_LiftGrabBegin_43D3F0();
    EXPORT void State_14_LiftGrabEnd_43D420();
    EXPORT void State_15_LeverUse_43D4B0();
    EXPORT void State_16_StandScrubLoop_43D7C0();
    EXPORT void State_17_StandScrubLoopToPause_43D860();
    EXPORT void State_18_StandScrubPauseToLoop_43D880();
    EXPORT void State_19_StandScrubPause_43D8A0();
    EXPORT void State_20_IdleToStandScrub_43D8D0();
    EXPORT void State_21_StandScrubToIdle_43D8F0();
    EXPORT void State_22_CrouchScrub_43D910();
    EXPORT void State_23_CrouchIdle_43E590();
    EXPORT void State_24_CrouchTurn_43E5F0();
    EXPORT void State_25_StandToCrouch_43E620();
    EXPORT void State_26_CrouchToStand_43E640();
    EXPORT void State_27_RunToWalk_43D980();
    EXPORT void State_28_MidRunToWalk_43DA40();
    EXPORT void State_29_RunLoop_43DB10();
    EXPORT void State_30_RunToWalk_43DD50();
    EXPORT void State_31_MidRunToWalk_43DE10();
    EXPORT void State_32_RunSlideStop_43DEE0();
    EXPORT void State_33_RunSlideTurn_43DF80();
    EXPORT void State_34_RunTurnToRun_43E070();
    EXPORT void State_35_SneakLoop_43E0F0();
    EXPORT void State_36_WalkToSneak_43E240();
    EXPORT void State_37_SneakToWalk_43E2E0();
    EXPORT void State_38_MidWalkToSneak_43E380();
    EXPORT void State_39_MidSneakToWalk_43E430();
    EXPORT void State_40_SneakBegin_43E4E0();
    EXPORT void State_41_SneakToIdle_43E530();
    EXPORT void State_42_MidSneakToIdle_43E560();
    EXPORT void State_43_JumpBegin_43E870();
    EXPORT void State_44_JumpMid_43E960();
    EXPORT void State_45_ToRunToPortal_43EB00();
    EXPORT void State_46_FallLandDie_43E660();
    EXPORT void State_47_Knockback_43E730();
    EXPORT void State_48_KnockbackGetUp_43E7D0();
    EXPORT void State_49_FallOfEdge_43E800();
    EXPORT void State_50_LandSoft_43E820();
    EXPORT void State_51_Fall_43D0D0();
    EXPORT void State_52_Chant_43D520();
    EXPORT void State_53_ChantEnd_43D640();
    EXPORT void State_54_ToDuck_43EB70();
    EXPORT void State_55_Duck_43EB90();
    EXPORT void State_56_DuckToCrouch_43EBC0();
    EXPORT void State_57_Struggle_43EBE0();
    EXPORT void State_58_StruggleToCrouchChant_43EC00();
    EXPORT void State_59_CrouchChant_43EC20();
    EXPORT void State_60_CrouchChantToSruggle_43ED50();
    EXPORT void State_61_DuckKnockback_43E6E0();
    EXPORT void State_62_Choke_43ED70();

    int field_10C;
    int field_110;
    int field_114;
    int field_118;
    int field_11C;
    int field_120;
    __int16 field_124;
    __int16 field_126;
    int field_128;
    int field_12C;
    int field_130;
    int field_134;
    int field_138;
    __int16 field_13C;
    __int16 field_13E;
    int field_140;
    __int16 field_144;
    __int16 field_146;
    int field_148[15];
    __int16 field_184;
    __int16 field_186;
    __int16 field_188;
    __int16 field_18A;
    int field_18C;
    int field_190;
    int field_194;
    __int16 field_198;
    __int16 field_19A;
    __int16 field_19C;
    __int16 field_19E;
    __int16 field_1A0;
    __int16 field_1A2;
    int field_1A4;
    __int16 field_1A8;
    __int16 field_1AA;
    int field_1AC;
    __int16 field_1B0;
    __int16 field_1B2;
    __int16 field_1B4;
    __int16 field_1B6;
    __int16 field_1B8;
    __int16 field_1BA;
    __int16 field_1BC;
    __int16 field_1BE;
    int field_1C0;
    __int16 field_1C4;
    __int16 field_1C6;
};
ALIVE_ASSERT_SIZEOF(Mudokon, 0x1C8);

END_NS_AO

