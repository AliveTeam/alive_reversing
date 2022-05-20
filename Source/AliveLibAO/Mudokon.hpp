#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Map.hpp"
#include "../AliveLibAE/Path.hpp"
#include "SwitchStates.hpp"

void Mud_ForceLink();

namespace AO {

#define MUD_STATES_ENUM(ENTRY)                   \
    ENTRY(Motion_0_Idle_43CA70)                   \
    ENTRY(Motion_1_WalkLoop_43CC80)               \
    ENTRY(Motion_2_StandingTurn_43D050)           \
    ENTRY(Motion_3_Speak_43D440)                  \
    ENTRY(Motion_4_Speak_43D440)                  \
    ENTRY(Motion_5_Speak_43D440)                  \
    ENTRY(Motion_6_Speak_43D440)                  \
    ENTRY(Motion_7_WalkBegin_43CE60)              \
    ENTRY(Motion_8_WalkToIdle_43CEF0)             \
    ENTRY(Motion_9_MidWalkToIdle_43CFA0)          \
    ENTRY(Motion_10_Unused_43D4D0)                \
    ENTRY(Motion_11_Null_43D350)                  \
    ENTRY(Motion_12_LiftUse_43D360)               \
    ENTRY(Motion_13_LiftGrabBegin_43D3F0)         \
    ENTRY(Motion_14_LiftGrabEnd_43D420)           \
    ENTRY(Motion_15_LeverUse_43D4B0)              \
    ENTRY(Motion_16_StandScrubLoop_43D7C0)        \
    ENTRY(Motion_17_StandScrubLoopToPause_43D860) \
    ENTRY(Motion_18_StandScrubPauseToLoop_43D880) \
    ENTRY(Motion_19_StandScrubPause_43D8A0)       \
    ENTRY(Motion_20_IdleToStandScrub_43D8D0)      \
    ENTRY(Motion_21_StandScrubToIdle_43D8F0)      \
    ENTRY(Motion_22_CrouchScrub_43D910)           \
    ENTRY(Motion_23_CrouchIdle_43E590)            \
    ENTRY(Motion_24_CrouchTurn_43E5F0)            \
    ENTRY(Motion_25_StandToCrouch_43E620)         \
    ENTRY(Motion_26_CrouchToStand_43E640)         \
    ENTRY(Motion_27_WalkToRun_43D980)             \
    ENTRY(Motion_28_MidWalkToRun_43DA40)          \
    ENTRY(Motion_29_RunLoop_43DB10)               \
    ENTRY(Motion_30_RunToWalk_43DD50)             \
    ENTRY(Motion_31_MidRunToWalk_43DE10)          \
    ENTRY(Motion_32_RunSlideStop_43DEE0)          \
    ENTRY(Motion_33_RunSlideTurn_43DF80)          \
    ENTRY(Motion_34_RunTurnToRun_43E070)          \
    ENTRY(Motion_35_SneakLoop_43E0F0)             \
    ENTRY(Motion_36_MidWalkToSneak_43E240)           \
    ENTRY(Motion_37_SneakToWalk_43E2E0)           \
    ENTRY(Motion_38_WalkToSneak_43E380)        \
    ENTRY(Motion_39_MidSneakToWalk_43E430)        \
    ENTRY(Motion_40_SneakBegin_43E4E0)            \
    ENTRY(Motion_41_SneakToIdle_43E530)           \
    ENTRY(Motion_42_MidSneakToIdle_43E560)        \
    ENTRY(Motion_43_RunJumpBegin_43E870)             \
    ENTRY(Motion_44_RunJumpMid_43E960)               \
    ENTRY(Motion_45_StandToRun_43EB00)         \
    ENTRY(Motion_46_FallLandDie_43E660)           \
    ENTRY(Motion_47_Knockback_43E730)             \
    ENTRY(Motion_48_KnockbackGetUp_43E7D0)        \
    ENTRY(Motion_49_WalkOffEdge_43E800)            \
    ENTRY(Motion_50_LandSoft_43E820)              \
    ENTRY(Motion_51_Fall_43D0D0)                  \
    ENTRY(Motion_52_Chant_43D520)                 \
    ENTRY(Motion_53_ChantEnd_43D640)              \
    ENTRY(Motion_54_ToDuck_43EB70)                \
    ENTRY(Motion_55_Duck_43EB90)                  \
    ENTRY(Motion_56_DuckToCrouch_43EBC0)          \
    ENTRY(Motion_57_Struggle_43EBE0)              \
    ENTRY(Motion_58_StruggleToCrouchChant_43EC00) \
    ENTRY(Motion_59_CrouchChant_43EC20)           \
    ENTRY(Motion_60_CrouchChantToSruggle_43ED50)  \
    ENTRY(Motion_61_DuckKnockback_43E6E0)         \
    ENTRY(Motion_62_PoisonGasDeath_43ED70)

#define MAKE_ENUM(VAR) VAR,
enum eMudMotions : s32
{
    MUD_STATES_ENUM(MAKE_ENUM)
};

struct Path_Mudokon final : public Path_TLV
{
    Scale_short field_18_scale;
    enum class MudJobs : s16
    {
        eStandScrub_0 = 0,
        eSitScrub_1 = 1,
        eSitChant_2 = 2,
    };
    MudJobs field_1A_job;
    XDirection_short field_1C_direction;
    s16 field_1E_voice_pitch;
    s16 field_20_rescue_switch_id;
    Choice_short field_22_deaf;
    s16 field_24_disabled_resources;
    s16 field_26_persist;
};
ALIVE_ASSERT_SIZEOF(Path_Mudokon, 0x28);

struct Path_RingMudokon final : public Path_TLV
{
    XDirection_short field_18_direction;
    enum class MustFaceMud : s16
    {
        eYes_0 = 0,
        eNo_1 = 1
    };
    MustFaceMud field_1A_abe_must_face_mud;
    Scale_short field_1C_scale;
    Choice_short field_1E_give_password;
    s16 field_20_code1;
    s16 field_22_code2;
    SwitchOp field_24_action;
    s16 field_26_ring_timeout;
    Choice_short field_28_give_ring_without_password;
    s16 field_2A_pad;
};
ALIVE_ASSERT_SIZEOF(Path_RingMudokon, 0x2C);

struct Path_LiftMudokon final : public Path_TLV
{
    s16 field_18_how_far_to_walk;
    s16 field_1A_lift_switch_id;
    enum class Direction : s16
    {
        eRight_0 = 0,
        eLeft_1 = 1
    };
    Direction field_1C_direction;
    Choice_short field_1E_give_password;
    Scale_short field_20_scale;
    s16 field_22_code1;
    s16 field_24_code2;
    s16 field_26_pad;
};
ALIVE_ASSERT_SIZEOF(Path_LiftMudokon, 0x28);

class LiftPoint;
class BirdPortal;

struct Mudokon_Resources final
{
    u8** res[15];
};

enum class GameSpeakEvents : s16;

class Mudokon final : public BaseAliveGameObject
{
public:
    EXPORT Mudokon* ctor_43EED0(Path_TLV* pTlv, s32 tlvInfo);

    EXPORT BaseGameObject* dtor_43F6A0();

    void KillLiftPoint_194();

    

    EXPORT Mudokon* Vdtor_440230(s32 flags);

    virtual void VUpdate() override;

    virtual void VOnTrapDoorOpen() override;

    EXPORT void VOnTrapDoorOpen_43C9F0();

    virtual void VOn_TLV_Collision(Path_TLV* pTlv) override;

    EXPORT void VOnTlvCollision_43FD90(Path_TLV* pTlv);

    EXPORT void VUpdate_43F560();

    EXPORT virtual void VUpdateResBlock_43EDB0();

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_43FFC0();

    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;

    EXPORT s16 VTakeDamage_43F830(BaseGameObject* pFrom);

    s16 DoSmashDamage();

    void KillBirdPortal();

    EXPORT u8** GetResBlockForMotion_43EDE0(s16 motion);

    EXPORT void DoPathTrans_43FE00();

    EXPORT void ToStand_43CA40();

    EXPORT void CheckFloorGone_43C9B0();

    EXPORT static s16 CC IsAbeSneaking_43D660(Mudokon* pMud);

    EXPORT void ToKnockback_43D6E0();

    EXPORT void ReduceXVelocityBy_43C920(FP amount);

    EXPORT void MoveOnLine_43C7E0();

    EXPORT s16 FindBirdPortal_440250();

    EXPORT s16 FacingTarget_43D6A0(BirdPortal* pTarget);

    EXPORT GameSpeakEvents LastGameSpeak_4400B0();

    void AddAlerted();

    void RemoveAlerted();

    EXPORT s16 IAmNearestToAbe_440120();

    // Motions
    EXPORT void Motion_0_Idle_43CA70();
    EXPORT void Motion_1_WalkLoop_43CC80();
    EXPORT void Motion_2_StandingTurn_43D050();
    EXPORT void Motion_3_Speak_43D440();

    // Not exported as same func as 3
    void Motion_4_Speak_43D440();
    void Motion_5_Speak_43D440();
    void Motion_6_Speak_43D440();

    EXPORT void Motion_7_WalkBegin_43CE60();
    EXPORT void Motion_8_WalkToIdle_43CEF0();
    EXPORT void Motion_9_MidWalkToIdle_43CFA0();
    EXPORT void Motion_10_Unused_43D4D0();
    EXPORT void Motion_11_Null_43D350();
    EXPORT void Motion_12_LiftUse_43D360();
    EXPORT void Motion_13_LiftGrabBegin_43D3F0();
    EXPORT void Motion_14_LiftGrabEnd_43D420();
    EXPORT void Motion_15_LeverUse_43D4B0();
    EXPORT void Motion_16_StandScrubLoop_43D7C0();
    EXPORT void Motion_17_StandScrubLoopToPause_43D860();
    EXPORT void Motion_18_StandScrubPauseToLoop_43D880();
    EXPORT void Motion_19_StandScrubPause_43D8A0();
    EXPORT void Motion_20_IdleToStandScrub_43D8D0();
    EXPORT void Motion_21_StandScrubToIdle_43D8F0();
    EXPORT void Motion_22_CrouchScrub_43D910();
    EXPORT void Motion_23_CrouchIdle_43E590();
    EXPORT void Motion_24_CrouchTurn_43E5F0();
    EXPORT void Motion_25_StandToCrouch_43E620();
    EXPORT void Motion_26_CrouchToStand_43E640();
    EXPORT void Motion_27_WalkToRun_43D980();
    EXPORT void Motion_28_MidWalkToRun_43DA40();
    EXPORT void Motion_29_RunLoop_43DB10();
    EXPORT void Motion_30_RunToWalk_43DD50();
    EXPORT void Motion_31_MidRunToWalk_43DE10();
    EXPORT void Motion_32_RunSlideStop_43DEE0();
    EXPORT void Motion_33_RunSlideTurn_43DF80();
    EXPORT void Motion_34_RunTurnToRun_43E070();
	EXPORT void Motion_35_SneakLoop_43E0F0();
	EXPORT void Motion_36_MidWalkToSneak_43E240();
	EXPORT void Motion_37_SneakToWalk_43E2E0();
	EXPORT void Motion_38_WalkToSneak_43E380();
	EXPORT void Motion_39_MidSneakToWalk_43E430();
	EXPORT void Motion_40_SneakBegin_43E4E0();
	EXPORT void Motion_41_SneakToIdle_43E530();
    EXPORT void Motion_42_MidSneakToIdle_43E560();
    EXPORT void Motion_43_RunJumpBegin_43E870();
    EXPORT void Motion_44_RunJumpMid_43E960();
    EXPORT void Motion_45_StandToRun_43EB00();
    EXPORT void Motion_46_FallLandDie_43E660();
    EXPORT void Motion_47_Knockback_43E730();
    EXPORT void Motion_48_KnockbackGetUp_43E7D0();
    EXPORT void Motion_49_WalkOffEdge_43E800();
    EXPORT void Motion_50_LandSoft_43E820();
    EXPORT void Motion_51_Fall_43D0D0();
    EXPORT void Motion_52_Chant_43D520();
    EXPORT void Motion_53_ChantEnd_43D640();
    EXPORT void Motion_54_ToDuck_43EB70();
    EXPORT void Motion_55_Duck_43EB90();
    EXPORT void Motion_56_DuckToCrouch_43EBC0();
    EXPORT void Motion_57_Struggle_43EBE0();
    EXPORT void Motion_58_StruggleToCrouchChant_43EC00();
    EXPORT void Motion_59_CrouchChant_43EC20();
    EXPORT void Motion_60_CrouchChantToSruggle_43ED50();
    EXPORT void Motion_61_DuckKnockback_43E6E0();
    EXPORT void Motion_62_PoisonGasDeath_43ED70();

    // Brains
    EXPORT s16 Brain_ComingIn_0_441DE0();
    EXPORT s16 Brain_ComingOut_1_441E90();
    EXPORT s16 Brain_SingSequenceIdle_2_441CA0();
    EXPORT s16 Brain_SingSequenceSing_3_441510();
    EXPORT s16 Brain_SingSequencePassword_4_441260();
    EXPORT s16 Brain_LiftUse_5_43C180();
    EXPORT s16 Brain_LeverUse_6_43C250();
    EXPORT s16 Brain_GiveRings_7_43C2F0();
    EXPORT s16 Brain_StandScrub_8_441F40();
    EXPORT s16 Brain_CrouchScrub_9_4422A0();
    EXPORT s16 Brain_ListeningToAbe_10_440300();
    EXPORT s16 Brain_ShrivelDeath_11_43C5F0();
    EXPORT s16 Brain_Escape_12_440FD0();
    EXPORT s16 Brain_FallAndSmackDeath_13_43C700();
    EXPORT s16 Brain_Chant_14_442710();
    EXPORT s16 Brain_Choke_15_43C5D0();

    s32 field_10C;
    s16 field_110_lift_switch_id;
    s16 field_112;
    s32 field_114;
    s32 field_118;
    FP field_11C;
    s32 field_120_unused;
    s16 field_124_voice_pitch;
    s16 field_126_input;
    s32 field_128;
    u8 field_12C[16];
    s16 field_13C;
    s16 field_13E;
    s32 field_140;

    enum Flags_144
    {
        e144_Bit1 = 0x1,
        e144_Bit2 = 0x2,
        e144_Bit3 = 0x4,
        e144_Bit4_bSnapToGrid = 0x8,
        e144_Bit5_unused = 0x10,
        e144_Bit6_bPersist = 0x20,
        e144_Bit7 = 0x40,
        e144_Bit8 = 0x80,
        e144_Bit9 = 0x100,
        e144_Bit10_give_ring_without_password = 0x200,
        e144_Bit11_bDeaf = 0x400,
        e144_Bit12 = 0x800,
        e144_eBit13 = 0x1000,
        e144_eBit14 = 0x2000,
        e144_eBit15 = 0x4000,
        e144_eBit16 = 0x8000,
    };
    BitField16<Flags_144> field_144_flags;

    s16 field_146;
    Mudokon_Resources field_148_res_array;
    s16 field_184;
    Choice_short field_186_give_password;
    s16 field_188;
    s16 field_18A;
    FP field_18C_how_far_to_walk;
    FP field_190;
    LiftPoint* field_194_pLiftPoint;
    s16 field_198_abe_must_face_mud;
    s16 field_19A;
    s16 field_19C;
    s16 field_19E_code_idx;
    s16 field_1A0;
    s16 field_1A2;
    s32 field_1A4_code_converted;
    s16 field_1A8_code_length;
    u16 field_1AA_ring_timeout;
    BirdPortal* field_1AC_pBirdPortal;
    s16 field_1B0;
    s16 field_1B2_rescue_switch_id;
    s16 field_1B4_idle_time;
    s16 field_1B6;
    s16 field_1B8_brain_idx;
    s16 field_1BA_brain_sub_state;
    s16 field_1BC;
    s16 field_1BE;
    s32 field_1C0_timer;
    s16 field_1C4_bDoPathTrans;
    s16 field_1C6;
};
ALIVE_ASSERT_SIZEOF(Mudokon, 0x1C8);

} // namespace AO
