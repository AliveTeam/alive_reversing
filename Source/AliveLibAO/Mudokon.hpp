#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Map.hpp"

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

struct Path_Mud : public Path_TLV
{
    __int16 field_18_scale;
    __int16 field_1A_job;
    __int16 field_1C_direction;
    __int16 field_1E_voice_adjust;
    __int16 field_20_rescue_id;
    __int16 field_22_deaf;
    __int16 field_24_disabled_resources;
    __int16 field_26_persist;
};
ALIVE_ASSERT_SIZEOF(Path_Mud, 0x28);

struct Path_Ring_Mud : public Path_TLV
{
    __int16 field_18_facing;
    __int16 field_1A_abe_must_be_same_direction;
    __int16 field_1C_scale;
    __int16 field_1E_silent;
    __int16 field_20_code1;
    __int16 field_22_code2;
    __int16 field_24_action;
    __int16 field_26_ring_timeout;
    __int16 field_28_instant_powerup;
    __int16 field_2A_pad;
};
ALIVE_ASSERT_SIZEOF(Path_Ring_Mud, 0x2C);

struct Path_Lift_Mud : public Path_TLV
{
    __int16 field_18_how_far_to_walk;
    __int16 field_1A_lift_id;
    __int16 field_1C_direction;
    __int16 field_1E_silent;
    __int16 field_20_scale;
    __int16 field_22_code1;
    __int16 field_24_code2;
    __int16 field_26_pad;
};
ALIVE_ASSERT_SIZEOF(Path_Lift_Mud, 0x28);

class LiftPoint;
class BirdPortal;

struct Mudokon_Resources
{
    BYTE** res[15];
};

enum class GameSpeakEvents : __int16;

class Mudokon : public BaseAliveGameObject
{
public:
    EXPORT Mudokon* ctor_43EED0(Path_TLV* pTlv, int tlvInfo);

    EXPORT BaseGameObject* dtor_43F6A0();

    void KillLiftPoint_194();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT Mudokon* Vdtor_440230(signed int flags);

    virtual void VUpdate() override;

    virtual void VOnTrapDoorOpen() override;

    EXPORT void VOnTrapDoorOpen_43C9F0();

    virtual void VOn_TLV_Collision(Path_TLV* pTlv) override;

    EXPORT void VOnTlvCollision_43FD90(Path_TLV* pTlv);

    EXPORT void VUpdate_43F560();

    EXPORT virtual void VUpdateResBlock_43EDB0();

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_43FFC0();

    virtual __int16 VTakeDamage(BaseGameObject* pFrom) override;

    EXPORT __int16 VTakeDamage_43F830(BaseGameObject* pFrom);

    __int16 DoSmashDamage();

    void KillBirdPortal();

    EXPORT BYTE** GetResBlockForMotion_43EDE0(__int16 motion);

    EXPORT void DoPathTrans_43FE00();

    EXPORT void ToIdle_43CA40();

    EXPORT void CheckFloorGone_43C9B0();

    EXPORT static __int16 CC IsAbeSneaking_43D660(Mudokon* pMud);

    EXPORT void ToKnockBack_43D6E0();

    EXPORT void SlowOnX_43C920(FP amount);

    EXPORT void MoveOnLine_43C7E0();

    EXPORT __int16 FindBirdPortal_440250();

    EXPORT __int16 FacingTarget_43D6A0(BirdPortal* pTarget);

    EXPORT GameSpeakEvents LastGameSpeak_4400B0();

    void AddAlerted();

    EXPORT __int16 IAmNearestToAbe_440120();

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

    // Brain states
    EXPORT short Brain_ComingIn_0_441DE0();
    EXPORT short Brain_ComingOut_1_441E90();
    EXPORT short Brain_SingSequenceIdle_2_441CA0();
    EXPORT short Brain_SingSequenceSing_3_441510();
    EXPORT short Brain_SingSequencePassword_4_441260();
    EXPORT short Brain_LiftUse_5_43C180();
    EXPORT short Brain_LeverUse_6_43C250();
    EXPORT short Brain_GiveRings_7_43C2F0();
    EXPORT short Brain_StandScrub_8_441F40();
    EXPORT short Brain_CrouchScrub_9_4422A0();
    EXPORT short Brain_ListeningToAbe_10_440300();
    EXPORT short Brain_ShrivelDeath_11_43C5F0();
    EXPORT short Brain_Escape_12_440FD0();
    EXPORT short Brain_FallAndSmackDeath_13_43C700();
    EXPORT short Brain_Chant_14_442710();
    EXPORT short Brain_Choke_15_43C5D0();

    int field_10C;
    short field_110;
    short field_112;
    int field_114;
    int field_118;
    FP field_11C;
    int field_120;
    __int16 field_124;
    __int16 field_126_input;
    int field_128;
    int field_12C;
    int field_130;
    int field_134;
    int field_138;
    __int16 field_13C;
    __int16 field_13E;
    int field_140;

    enum Flags_144
    {
        e144_Bit1 = 0x1,
        e144_Bit2 = 0x2,
        e144_Bit3 = 0x4,
        e144_Bit4_bSnapToGrid = 0x8,
        e144_Bit5 = 0x10,
        e144_Bit6_bPersist = 0x20,
        e144_Bit7 = 0x40,
        e144_Bit8 = 0x80,
        e144_Bit9 = 0x100,
        e144_Bit10 = 0x200,
        e144_Bit11_bDeaf = 0x400,
        e144_Bit12 = 0x800,
        e144_eBit13 = 0x1000,
        e144_eBit14 = 0x2000,
        e144_eBit15 = 0x4000,
        e144_eBit16 = 0x8000,
    };
    BitField16<Flags_144> field_144_flags;

    __int16 field_146;
    Mudokon_Resources field_148_res_array;
    __int16 field_184;
    __int16 field_186;
    __int16 field_188;
    __int16 field_18A;
    FP field_18C;
    FP field_190;
    LiftPoint* field_194_pLiftPoint;
    __int16 field_198;
    __int16 field_19A;
    __int16 field_19C;
    __int16 field_19E;
    __int16 field_1A0;
    __int16 field_1A2;
    int field_1A4;
    __int16 field_1A8;
    unsigned __int16 field_1AA;
    BirdPortal* field_1AC_pBirdPortal;
    __int16 field_1B0;
    __int16 field_1B2_switch_id;
    __int16 field_1B4;
    __int16 field_1B6;
    __int16 field_1B8_brain_idx;
    __int16 field_1BA_sub_state;
    __int16 field_1BC;
    __int16 field_1BE;
    int field_1C0_timer;
    __int16 field_1C4_bDoPathTrans;
    __int16 field_1C6;
};
ALIVE_ASSERT_SIZEOF(Mudokon, 0x1C8);

END_NS_AO

