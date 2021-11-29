#pragma once

#include "BaseAliveGameObject.hpp"
#include "../AliveLibCommon/FunctionFwd.hpp"
#include "../AliveLibCommon/BitField.hpp"
#include "Input.hpp"
#include "Path.hpp"


#define ABE_MOTIONS_ENUM(ENTRY)                      \
    ENTRY(Motion_0_Idle_44EEB0)                      \
    ENTRY(Motion_1_WalkLoop_44FBA0)                  \
    ENTRY(Motion_2_StandingTurn_451830)              \
    ENTRY(Motion_3_Fall_459B60)                      \
    ENTRY(Motion_4_WalkToIdle_44FFC0)                \
    ENTRY(Motion_5_MidWalkToIdle_450080)             \
    ENTRY(Motion_6_WalkBegin_44FEE0)                 \
    ENTRY(Motion_7_Speak_45B140)                     \
    ENTRY(Motion_8_Speak_45B160)                     \
    ENTRY(Motion_9_Speak_45B180)                     \
    ENTRY(Motion_10_Fart_45B1A0)                     \
    ENTRY(Motion_11_Speak_45B0A0)                    \
    ENTRY(Motion_12_Null_4569C0)                     \
    ENTRY(Motion_13_HoistBegin_452B20)               \
    ENTRY(Motion_14_HoistIdle_452440)                \
    ENTRY(Motion_15_HoistLand_452BA0)                \
    ENTRY(Motion_16_LandSoft_45A360)                 \
    ENTRY(Motion_17_CrouchIdle_456BC0)               \
    ENTRY(Motion_18_CrouchToStand_454600)            \
    ENTRY(Motion_19_StandToCrouch_453DC0)            \
    ENTRY(Motion_20_CrouchSpeak_454550)              \
    ENTRY(jMotion_21_ToCrouchSpeak_4545E0)           \
    ENTRY(Motion_22_RollBegin_4539A0)                \
    ENTRY(Motion_23_RollLoop_453A90)                 \
    ENTRY(Motion_24_453D00)                          \
    ENTRY(Motion_25_RunSlideStop_451330)             \
    ENTRY(Motion_26_RunTurn_451500)                  \
    ENTRY(Motion_27_HopBegin_4521C0)                 \
    ENTRY(Motion_28_HopMid_451C50)                   \
    ENTRY(Motion_29_HopLand_4523D0)                  \
    ENTRY(Motion_30_RunJumpBegin_4532E0)             \
    ENTRY(Motion_31_RunJumpMid_452C10)               \
    ENTRY(Motion_32_RunJumpLand_453460)              \
    ENTRY(Motion_33_RunLoop_4508E0)                  \
    ENTRY(Motion_34_DunnoBegin_44ECF0)               \
    ENTRY(Motion_35_DunnoEnd_44ED10)                 \
    ENTRY(Motion_36_Null_45BC50)                     \
    ENTRY(Motion_37_CrouchTurn_454390)               \
    ENTRY(jMotion_38_RunToRoll_453A70)               \
    ENTRY(Motion_39_StandingToRun_450D40)            \
    ENTRY(Motion_40_SneakLoop_450550)                \
    ENTRY(Motion_41_WalkToSneak_450250)              \
    ENTRY(Motion_42_SneakToWalk_4503D0)              \
    ENTRY(Motion_43_MidWalkToSneak_450380)           \
    ENTRY(Motion_44_MidSneakToWalk_450500)           \
    ENTRY(Motion_45_SneakBegin_4507A0)               \
    ENTRY(Motion_46_SneakToIdle_450870)              \
    ENTRY(jMotion_47_MidSneakToIdle_4508C0)          \
    ENTRY(Motion_48_WalkToRun_4500A0)                \
    ENTRY(Motion_49_MidWalkToRun_450200)             \
    ENTRY(Motion_50_RunToWalk_450E20)                \
    ENTRY(Motion_51_MidRunToWalk_450F50)             \
    ENTRY(Motion_52_RunTurnToRun_451710)             \
    ENTRY(Motion_53_RunTurnToWalk_451800)            \
    ENTRY(Motion_54_RunJumpLandRun_4538F0)           \
    ENTRY(Motion_55_RunJumpLandWalk_453970)          \
    ENTRY(Motion_56_DeathDropFall_4591F0)            \
    ENTRY(Motion_57_Dead_4589A0)                     \
    ENTRY(Motion_58_DeadPre_4593E0)                  \
    ENTRY(Motion_59_Null_459450)                     \
    ENTRY(Motion_60_Unused_4A3200)                          \
    ENTRY(Motion_61_TurnToRun_456530)                \
    ENTRY(Motion_62_Punch_454750)                    \
    ENTRY(Motion_63_Sorry_454670)                    \
    ENTRY(Motion_64_AfterSorry_454730)               \
    ENTRY(Motion_65_LedgeAscend_4548E0)              \
    ENTRY(Motion_66_LedgeDescend_454970)             \
    ENTRY(Motion_67_LedgeHang_454E20)                \
    ENTRY(Motion_68_ToOffScreenHoist_454B80)         \
    ENTRY(Motion_69_LedgeHangWobble_454EF0)          \
    ENTRY(Motion_70_RingRopePullHang_455AF0)         \
    ENTRY(Motion_71_Knockback_455090)                \
    ENTRY(Motion_72_KnockbackGetUp_455340)           \
    ENTRY(Motion_73_PushWall_4553A0)                 \
    ENTRY(Motion_74_RollingKnockback_455290)         \
    ENTRY(Motion_75_JumpIntoWell_45C7B0)             \
    ENTRY(Motion_76_ToInsideOfAWellLocal_45CA40)     \
    ENTRY(Motion_77_ToWellShotOut_45D130)            \
    ENTRY(Motion_78_WellBegin_45C810)                \
    ENTRY(Motion_79_InsideWellLocal_45CA60)          \
    ENTRY(Motion_80_WellShotOut_45D150)              \
    ENTRY(jMotion_81_WellBegin_45C7F0)               \
    ENTRY(Motion_82_InsideWellExpress_45CC80)        \
    ENTRY(Motion_83_WellExpressShotOut_45CF70)       \
    ENTRY(Motion_84_FallLandDie_45A420)              \
    ENTRY(jMotion_85_Fall_455070)                    \
    ENTRY(Motion_86_HandstoneBegin_45BD00)           \
    ENTRY(Motion_87_HandstoneEnd_45C4F0)             \
    ENTRY(Motion_88_GrenadeMachineUse_45C510)        \
    ENTRY(Motion_89_BrewMachineBegin_4584C0)         \
    ENTRY(Motion_90_BrewMachineEnd_4585B0)           \
    ENTRY(Motion_91_FallingFromGrab_4557B0)          \
    ENTRY(Motion_92_ForceDownFromHoist_455800)       \
    ENTRY(Motion_93_WalkOffEdge_455970)              \
    ENTRY(Motion_94_RunOffEdge_4559A0)               \
    ENTRY(Motion_95_SneakOffEdge_4559C0)             \
    ENTRY(Motion_96_HopToFall_4559E0)                \
    ENTRY(Motion_97_RunJumpToFall_455A80)            \
    ENTRY(Motion_98_RollOffEdge_455AA0)              \
    ENTRY(Motion_99_LeverUse_455AC0)                 \
    ENTRY(Motion_100_SlapBomb_455B60)                \
    ENTRY(Motion_101_KnockForward_455420)            \
    ENTRY(Motion_102_RollingKnockForward_455310)     \
    ENTRY(jMotion_103_KnockForwardGetUp_455380)      \
    ENTRY(Motion_104_RockThrowStandingHold_455DF0)   \
    ENTRY(Motion_105_RockThrowStandingThrow_456460)  \
    ENTRY(Motion_106_RockThrowStandingEnd_455F20)    \
    ENTRY(Motion_107_RockThrowCrouchingHold_454410)  \
    ENTRY(Motion_108_RockThrowCrouchingThrow_454500) \
    ENTRY(Motion_109_ZShotRolling_455550)            \
    ENTRY(Motion_110_ZShot_455670)                   \
    ENTRY(Motion_111_PickupItem_4564A0)              \
    ENTRY(Motion_112_Chant_45B1C0)                   \
    ENTRY(Motion_113_ChantEnd_45BBE0)                \
    ENTRY(Motion_114_DoorEnter_459470)               \
    ENTRY(Motion_115_DoorExit_459A40)                \
    ENTRY(Motion_116_MineCarEnter_458780)            \
    ENTRY(Motion_117_InMineCar_4587C0)               \
    ENTRY(Motion_118_MineCarExit_458890)             \
    ENTRY(Motion_119_ToShrykull_45A990)              \
    ENTRY(Motion_120_EndShrykull_45AB00)             \
    ENTRY(Motion_121_LiftGrabBegin_45A600)           \
    ENTRY(Motion_122_LiftGrabEnd_45A670)             \
    ENTRY(Motion_123_LiftGrabIdle_45A6A0)            \
    ENTRY(Motion_124_LiftUseUp_45A780)               \
    ENTRY(Motion_125_LiftUseDown_45A7B0)             \
    ENTRY(Motion_126_TurnWheelBegin_456700)          \
    ENTRY(Motion_127_TurnWheelLoop_456750)           \
    ENTRY(Motion_128_TurnWheelEnd_4569A0)            \
    ENTRY(Motion_129_PoisonGasDeath_4565C0)

#define MAKE_ENUM(VAR) VAR,
enum eAbeMotions : s32
{
    ABE_MOTIONS_ENUM(MAKE_ENUM)
};

class PullRingRope;

extern const TintEntry sTintTable_Abe_554D20[15];

extern const InputCommands::Enum sInputKey_Right_5550D0;
extern const InputCommands::Enum sInputKey_Left_5550D4;
extern const InputCommands::Enum sInputKey_Up_5550D8;
extern const InputCommands::Enum sInputKey_Down_5550DC;
extern const InputCommands::Enum sInputKey_Hop_5550E0;
extern const InputCommands::Enum sInputKey_DoAction_5550E4;
extern const InputCommands::Enum sInputKey_Run_5550E8;
extern const InputCommands::Enum sInputKey_Sneak_5550EC;
extern const InputCommands::Enum sInputKey_FartRoll_5550F0;
extern const InputCommands::Enum sInputKey_ThrowItem_5550F4;
extern const InputCommands::Enum sInputKey_GameSpeak2_5550F8;
extern const InputCommands::Enum sInputKey_GameSpeak4_5550FC;
extern const InputCommands::Enum sInputKey_GameSpeak3_555100;
extern const InputCommands::Enum sInputKey_GameSpeak1_555104;
extern const InputCommands::Enum sInputKey_GameSpeak6_555108;
extern const InputCommands::Enum sInputKey_GameSpeak5_55510C;
extern const InputCommands::Enum sInputKey_GameSpeak8_555110;
extern const InputCommands::Enum sInputKey_GameSpeak7_555114;

struct Path_ContinuePoint final : public Path_TLV
{
    enum class Scale : s16
    {
        eNone_0 = 0,
        eHalf_1 = 1,
        eFull_2 = 2,
    };
    Scale field_10_scale;
    s16 field_12_save_file_id;
};


struct Path_ResetSwitchRange final : public Path_TLV
{
    s16 field_10_set_switches;
    s16 field_12_start_id;
    s16 field_14_end_id;
    s16 field_16_skip_id;
    s16 field_18_free_path_res;
    s16 field_1A_path_to_free_id;
    s16 field_1C_bEnabled;
    s16 field_1E_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ResetSwitchRange, 0x20);

enum class MudSounds : s16
{
    eNone = -1,
    eEmpty_0 = 0,
    eEmpty_1 = 1,
    eEmpty_2 = 2,
    eHelloNeutral_3 = 3,
    eFollowMe_4 = 4,
    eAnger_5 = 5,
    eWait_6 = 6,
    eFart_7 = 7,
    eGiggle_8 = 8,
    eHurt2_9 = 9,
    eLaugh_10 = 10,
    eGoodbye_11 = 11,
    eOkay_12 = 12,
    eNuhUh_13 = 13,
    eOops_14 = 14,
    eDeathDropScream_15 = 15,
    eHurt1_16 = 16,
    eAllOYa_17 = 17,
    eHiAngry_18 = 18,
    eHiHappy_19 = 19,
    eHiSad_20 = 20,
    eNoAngry_21 = 21,
    eNoSad_22 = 22,
    eFartPuh_23 = 23,
    eSick_24 = 24,
    eWork_25 = 25,
    eStopIt_26 = 26,
    eSorry_27 = 27,
    eSadUgh_28 = 28,
};

enum class EnvironmentSfx : s8
{
    eSlideStop_0 = 0,
    eWalkingFootstep_1 = 1,
    eRunningFootstep_2 = 2,
    eSneakFootstep_3 = 3,
    eRunSlide_4 = 4,
    eLandingSoft_5 = 5,
    eHitGroundSoft_6 = 6,
    eDeathNoise_7 = 7,
    eRollingNoise_8 = 8,
    eGenericMovement_9 = 9,
    eExhaustingHoistNoise_10 = 10, // empty in AE?
    eRunJumpOrLedgeHoist_11 = 11,
    eAllOYa_12 = 12,
    eKnockback_13 = 13,
    eElumHitWall_14 = 14,                // TODO: better name
    eFallingDeathScreamHitGround_15 = 15 // TODO: better name
};

enum class ChantStates : u16
{
    eIdleChanting_0 = 0,
    ePossessVictim_1 = 1,
    ePossessedVictim_2 = 2,
    eWaitForUnpossessing_3 = 3,
    eUnpossessing_4 = 4,
    ePadding_5 = 5,
    eChantingForBirdPortal_6 = 6,
};

enum class StoneStates : u16
{
    eHandstoneBegin_0 = 0,
    eGetHandstoneType_1 = 1,
    eHandstoneMovieDone_2 = 2,
    eHandstoneEnd_3 = 3,
    eWaitForInput_4 = 4,
    eCamChangeTransition_5 = 5,
    eSetActiveCamToAbe_6 = 6,
    eCircularFadeExit_7 = 7,
};

enum class DoorStates : u16
{
    eAbeComesIn_0 = 0,
    ePadding_1 = 1,
    eWaitABit_2 = 2,
    eToState4_3 = 3,
    eSetNewActiveCamera_4 = 4,
    eSetNewAbePosition_5 = 5,
    eAbeComesOut_6 = 6
};

enum class PortalSubStates : s16
{
    eJumpingInsidePortal_0 = 0,
    eSetNewActiveCamera_1 = 1,
    eHopOutOfPortal_2 = 2,
    ePadding_3 = 3,
    eSetNewAbePosition_4 = 4
};

enum class WorkWheelStates : u16
{
    eTurningWheel_0 = 0,
    eCheckForNoLongerTurningWheel_1 = 1,
    eMapChanging_2 = 2,
};

union AllInternalStates
{
    ChantStates chant;
    StoneStates stone;
    DoorStates door;
    WorkWheelStates wheel;
    u16 raw;
};

enum class Mud_Emotion : s16;

struct Abe_1BC_20_sub_object final
{
    s32 field_0_abe_timer;
    s32 field_4_regen_health_timer;
    FP field_8_x_vel_slow_by;
    s32 field_C_unused;
    u16 field_10_resource_index;
    Mud_Emotion field_12_mood;
    s32 field_14_rolling_motion_timer;
    MudSounds field_18_say;
    s16 field_1A_padding;
};
ALIVE_ASSERT_SIZEOF(Abe_1BC_20_sub_object, 0x1C);

EXPORT void CC Mudokon_SFX_457EC0(MudSounds idx, s16 volume, s32 pitch, BaseAliveGameObject* pHero);

struct Abe_SaveState final
{
    AETypes field_0_id;
    s16 field_2_padding;
    FP field_4_xpos;
    FP field_8_ypos;
    FP field_c_velx;
    FP field_10_vely;
    u16 field_14_path_number;
    LevelIds field_16_lvl_number;
    FP field_18_sprite_scale;
    u16 field_1C_scale;
    u16 field_1e_r;
    u16 field_20_g;
    u16 field_22_b;
    u16 bAnimFlipX;
    u16 current_motion;
    u16 anim_current_frame;
    u16 anim_frame_change_counter;
    s8 anim_render_layer;
    s8 bAnimRender;
    s8 bDrawable;
    s8 field_2F_padding;
    FP field_30_health;
    u16 field_34_animation_num;
    u16 next_motion;
    u16 last_line_ypos;
    s16 field_3a_collision_line_id;
    u32 platform_obj_id;
    u16 bElectrocuted;
    u16 field_42_bInvisible;
    s8 field_44_is_abe_controlled;
    s8 field_45_padding;
    s16 field_46_padding;
    FP field_48_x_vel_slow_by;
    u32 field_4C_unused;
    u32 field_50_state;
    u32 field_54_timer;
    u32 field_58_abe_timer;
    u32 field_5C_regen_health_timer;
    Mud_Emotion mood;
    MudSounds say;
    u32 auto_say_timer;
    u32 ring_pulse_timer;
    s8 field_6c_rock_bone_count;
    s8 bShrivel;
    s8 bHaveShrykull;
    s8 bHaveInvisiblity;
    u16 prev_held;
    u16 released_buttons;
    u16 field_74_knockdown_motion;
    s16 field_76_padding;
    u32 field_78_rolling_motion_timer;
    u32 fade_obj_id;
    u32 circular_fade_id;
    u32 orb_whirl_wind_id;
    u32 possesed_object_id;
    u32 throwabe_obj_id;
    u32 pull_ring_rope_id;
    u32 slappable_or_pickup_id;
    u32 wheel_id;
    u32 invisible_timer;
    u16 field_A0_unused;
    u16 field_A2_invisibility_id;
    s8 field_A4_cam_idx;
    s8 field_A5_padding;
    s16 field_A6_padding;
    TlvTypes32 hand_stone_type;
    u16 fmv_id;
    u16 cam_id_1;
    u16 cam_id_2;
    u16 cam_id_3;
    u16 field_B4_unused;
    u16 field_B6_unused;
    u16 field_B8_unused;
    u16 field_BA_unused;
    u16 field_BC_unused;
    u16 field_BE_unused;
    u16 bHaveEvilFart;
    LevelIds to_level;
    u16 to_path;
    u16 to_camera;
    u16 door_id;
    s8 field_ca_throw_direction;
    s8 field_CB_padding;
    u16 field_CC_portal_sub_state;
    s16 field_CE_padding;
    u32 bird_portal_id;
    enum Flags_D4
    {
        eD4_Bit1_lift_point_dead_while_using_lift = 0x1,
        eD4_Bit2_return_to_previous_motion = 0x2,
        eD4_Bit3_WalkToRun = 0x4,
        eD4_Bit4_unused = 0x8,
        eD4_Bit5_prevent_chanting = 0x10,
        eD4_Bit6_land_softly = 0x20,
        eD4_Bit7_unused = 0x40,
        eD4_Bit8_laugh_at_chant_end = 0x80,
        eD4_Bit9_unused = 0x100,
        eD4_Bit10_play_ledge_grab_sounds = 0x200,
        eD4_Bit11_unused = 0x400,
        eD4_Bit12_have_healing = 0x800,
        eD4_eBit13_teleporting = 0x1000,
        eD4_eBit14_is_mudanchee_vault_ender = 0x2000,
        eD4_eBit15_is_mudomo_vault_ender = 0x4000,
        eD4_eBit16_shadow_enabled = 0x8000,
    };
    BitField16<Flags_D4> field_D4_flags;

    enum Flags_D6
    {
        eD6_Bit1_shadow_at_bottom = 0x1
    };
    BitField16<Flags_D6> field_D6_flags;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Abe_SaveState, 216);

class Bullet;

class Abe final : public BaseAliveGameObject
{
public:
    EXPORT Abe* ctor_44AD10(s32 frameTableOffset, s32 r, s32 g, s32 b);
    EXPORT void dtor_44B380();

    EXPORT static s32 CC CreateFromSaveState_44D4F0(const u8* pData);

    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;
    virtual s16 VTakeDamage_408730(BaseGameObject* pFrom) override;
    virtual void VOn_TLV_Collision_4087F0(Path_TLV* pTlv) override;
    virtual BirdPortal* VIntoBirdPortal_408FD0(s16 gridBlocks) override;
    virtual void VOnTrapDoorOpen() override;

    void DoRunJump();

    // Virtual impls
    EXPORT BaseGameObject* vdtor_44B350(s32 flags);
    EXPORT void Update_449DC0();
    EXPORT void vRender_44B580(PrimHeader** ppOt);
    EXPORT void vScreenChanged_44D240();
    EXPORT s32 vGetSaveState_457110(u8* pSaveBuffer);
    EXPORT s16 vTakeDamage_44BB50(BaseGameObject* pFrom);
    EXPORT void vOn_TLV_Collision_44B5D0(Path_TLV* pTlv);
    EXPORT BirdPortal* vIntoBirdPortal_44E970(s16 gridBlocks);
    EXPORT void vOnTrapDoorOpen_45A570();

    // Non virtuals
    EXPORT void ToKnockback_44E700(s16 bKnockbackSound, s16 bDelayedAnger);
    EXPORT BaseAliveGameObject* FindObjectToPossess_44B7B0();
    EXPORT void Load_Basic_Resources_44D460();
    EXPORT void Free_Resources_44D420();
    EXPORT Bool32 IsStanding_449D30();
    EXPORT void Free_Shrykull_Resources_45AA90();

    EXPORT u8** MotionToAnimResource_44AAB0(s16 motion);

    // Motions
    EXPORT void Motion_0_Idle_44EEB0();
    EXPORT void Motion_1_WalkLoop_44FBA0();
    EXPORT void Motion_2_StandingTurn_451830();
    EXPORT void Motion_3_Fall_459B60();
    EXPORT void Motion_4_WalkToIdle_44FFC0();
    EXPORT void Motion_5_MidWalkToIdle_450080();
    EXPORT void Motion_6_WalkBegin_44FEE0();
    EXPORT void Motion_7_Speak_45B140();
    EXPORT void Motion_8_Speak_45B160();
    EXPORT void Motion_9_Speak_45B180();
    EXPORT void Motion_10_Fart_45B1A0();
    EXPORT void Motion_11_ToSpeak_45B0A0();
    void Motion_12_Null_4569C0();
    EXPORT void Motion_13_HoistBegin_452B20();
    EXPORT void Motion_14_HoistIdle_452440();
    EXPORT void Motion_15_HoistLand_452BA0();
    EXPORT void Motion_16_LandSoft_45A360();
    EXPORT void Motion_17_CrouchIdle_456BC0();
    EXPORT void Motion_18_CrouchToStand_454600();
    EXPORT void Motion_19_StandToCrouch_453DC0();
    EXPORT void Motion_20_CrouchSpeak_454550();
    EXPORT void jMotion_21_ToCrouchSpeak_4545E0();
    EXPORT void Motion_22_RollBegin_4539A0();
    EXPORT void Motion_23_RollLoop_453A90();
    EXPORT void Motion_24_453D00();
    EXPORT void Motion_25_RunSlideStop_451330();
    EXPORT void Motion_26_RunTurn_451500();
    EXPORT void Motion_27_HopBegin_4521C0();
    EXPORT void Motion_28_HopMid_451C50();
    EXPORT void Motion_29_HopLand_4523D0();
    EXPORT void Motion_30_RunJumpBegin_4532E0();
    EXPORT void Motion_31_RunJumpMid_452C10();
    EXPORT void Motion_32_RunJumpLand_453460();
    EXPORT void Motion_33_RunLoop_4508E0();
    EXPORT void Motion_34_DunnoBegin_44ECF0();
    EXPORT void Motion_35_DunnoEnd_44ED10();
    void Motion_36_Null_45BC50();
    EXPORT void Motion_37_CrouchTurn_454390();
    EXPORT void jMotion_38_RunToRoll_453A70();
    EXPORT void Motion_39_StandingToRun_450D40();
    EXPORT void Motion_40_SneakLoop_450550();
    EXPORT void Motion_41_WalkToSneak_450250();
    EXPORT void Motion_42_SneakToWalk_4503D0();
    EXPORT void Motion_43_MidWalkToSneak_450380();
    EXPORT void Motion_44_MidSneakToWalk_450500();
    EXPORT void Motion_45_SneakBegin_4507A0();
    EXPORT void Motion_46_SneakToIdle_450870();
    EXPORT void jMotion_47_MidSneakToIdle_4508C0();
    EXPORT void Motion_48_WalkToRun_4500A0();
    EXPORT void Motion_49_MidWalkToRun_450200();
    EXPORT void Motion_50_RunToWalk_450E20();
    EXPORT void Motion_51_MidRunToWalk_450F50();
    EXPORT void Motion_52_RunTurnToRun_451710();
    EXPORT void Motion_53_RunTurnToWalk_451800();
    EXPORT void Motion_54_RunJumpLandRun_4538F0();
    EXPORT void Motion_55_RunJumpLandWalk_453970();
    EXPORT void Motion_56_DeathDropFall_4591F0();
    EXPORT void Motion_57_Dead_4589A0();
    EXPORT void Motion_58_DeadPre_4593E0();
    void Motion_59_Null_459450();
    EXPORT void Motion_60_Unused_4A3200();
    EXPORT void Motion_61_TurnToRun_456530();
    EXPORT void Motion_62_Punch_454750();
    EXPORT void Motion_63_Sorry_454670();
    EXPORT void Motion_64_AfterSorry_454730();
    EXPORT void Motion_65_LedgeAscend_4548E0();
    EXPORT void Motion_66_LedgeDescend_454970();
    EXPORT void Motion_67_LedgeHang_454E20();
    EXPORT void Motion_68_ToOffScreenHoist_454B80();
    EXPORT void Motion_69_LedgeHangWobble_454EF0();
    EXPORT void Motion_70_RingRopePullHang_455AF0();
    EXPORT void Motion_71_Knockback_455090();
    EXPORT void Motion_72_KnockbackGetUp_455340();
    EXPORT void Motion_73_PushWall_4553A0();
    EXPORT void Motion_74_RollingKnockback_455290();
    EXPORT void Motion_75_JumpIntoWell_45C7B0();
    EXPORT void Motion_76_ToInsideOfAWellLocal_45CA40();
    EXPORT void Motion_77_ToWellShotOut_45D130();
    EXPORT void Motion_78_WellBegin_45C810();
    EXPORT void Motion_79_InsideWellLocal_45CA60();
    EXPORT void Motion_80_WellShotOut_45D150();
    EXPORT void jMotion_81_WellBegin_45C7F0();
    EXPORT void Motion_82_InsideWellExpress_45CC80();
    EXPORT void Motion_83_WellExpressShotOut_45CF70();
    EXPORT void Motion_84_FallLandDie_45A420();
    EXPORT void jMotion_85_Fall_455070();
    EXPORT void Motion_86_HandstoneBegin_45BD00();
    EXPORT void Motion_87_HandstoneEnd_45C4F0();
    EXPORT void Motion_88_GrenadeMachineUse_45C510();
    EXPORT void Motion_89_BrewMachineBegin_4584C0();
    EXPORT void Motion_90_BrewMachineEnd_4585B0();
    EXPORT void Motion_91_FallingFromGrab_4557B0();
    EXPORT void Motion_92_ForceDownFromHoist_455800();
    EXPORT void Motion_93_WalkOffEdge_455970();
    EXPORT void Motion_94_RunOffEdge_4559A0();
    EXPORT void Motion_95_SneakOffEdge_4559C0();
    EXPORT void Motion_96_HopToFall_4559E0();
    EXPORT void Motion_97_RunJumpToFall_455A80();
    EXPORT void Motion_98_RollOffEdge_455AA0();
    EXPORT void Motion_99_LeverUse_455AC0();
    EXPORT void Motion_100_SlapBomb_455B60();
    EXPORT void Motion_101_KnockForward_455420();
    EXPORT void Motion_102_RollingKnockForward_455310();
    EXPORT void jMotion_103_KnockForwardGetUp_455380();
    EXPORT void Motion_104_RockThrowStandingHold_455DF0();
    EXPORT void Motion_105_RockThrowStandingThrow_456460();
    EXPORT void Motion_106_RockThrowStandingEnd_455F20();
    EXPORT void Motion_107_RockThrowCrouchingHold_454410();
    EXPORT void Motion_108_RockThrowCrouchingThrow_454500();
    EXPORT void Motion_109_ZShotRolling_455550();
    EXPORT void Motion_110_ZShot_455670();
    EXPORT void Motion_111_PickupItem_4564A0();
    EXPORT void Motion_112_Chant_45B1C0();
    EXPORT void Motion_113_ChantEnd_45BBE0();
    EXPORT void Motion_114_DoorEnter_459470();
    EXPORT void Motion_115_DoorExit_459A40();
    EXPORT void Motion_116_MineCarEnter_458780();
    EXPORT void Motion_117_InMineCar_4587C0();
    EXPORT void Motion_118_MineCarExit_458890();
    EXPORT void Motion_119_ToShrykull_45A990();
    EXPORT void Motion_120_EndShrykull_45AB00();
    EXPORT void Motion_121_LiftGrabBegin_45A600();
    EXPORT void Motion_122_LiftGrabEnd_45A670();
    EXPORT void Motion_123_LiftGrabIdle_45A6A0();
    EXPORT void Motion_124_LiftUseUp_45A780();
    EXPORT void Motion_125_LiftUseDown_45A7B0();
    EXPORT void Motion_126_TurnWheelBegin_456700();
    EXPORT void Motion_127_TurnWheelLoop_456750();
    EXPORT void Motion_128_TurnWheelEnd_4569A0();
    EXPORT void Motion_129_PoisonGasDeath_4565C0();

    EXPORT void jMotion_81_WellBegin_4017F8();

    // End motions

    EXPORT void FleechDeath_459350();

    EXPORT void ToDie_4588D0();
    EXPORT void ToIdle_44E6B0();
    EXPORT void PickUpThrowabe_Or_PressBomb_454090(FP fpX, s32 fpY, s32 bStandToCrouch);
    EXPORT void Get_Shrykull_Resources_45AA20();
    EXPORT s16 ToLeftRightMovement_44E340();
    EXPORT void TryHoist_44ED30();
    EXPORT static void CC Create_Fart_421D20();
    EXPORT s16 TryEnterMineCar_4569E0();
    EXPORT s32 NearDoorIsOpen_44EE10();
    EXPORT s16 HandleDoAction_455BD0();
    EXPORT void PushWall_44E890();
    EXPORT void MoveForward_44E9A0();
    EXPORT s16 CrouchingGameSpeak_453E10();
    EXPORT Bool32 Is_Celling_Above_44E8D0();
    EXPORT void MoveWithVelocity_450FA0(FP velocityX);
    EXPORT s16 RunTryEnterDoor_451220();
    EXPORT s16 RunTryEnterWell_451060();
    EXPORT void ToDieFinal_458910();
    EXPORT s16 DoGameSpeak_45AB70(s32 input);
    EXPORT s16 CantBeDamaged_44BAB0();
    EXPORT void FallOnBombs_44EC10();
    EXPORT s16 ForceDownIfHoisting_44BA30();
    EXPORT void BulletDamage_44C980(Bullet* pObj);
    EXPORT void GiveControlBackToMe_44BA10();
    EXPORT PullRingRope* GetPullRope_44D120();
    EXPORT void IntoPortalStates_451990();
    EXPORT void Calc_Well_Velocity_45C530(s16 xPosSource, s16 yPosSource, s16 XPosDest, s16 yPosDest);
    EXPORT void FollowLift_45A500();

    EXPORT s16 MoveLiftUpOrDown_45A7E0(FP yVelocity);


    EXPORT s16 GetEvilFart_4585F0(s16 bDontLoad);

    EXPORT void ChangeChantState_45BB90(s16 bLaughAtChantEnd);

    EXPORT void SetAsDead_459430();

    EXPORT void ExitShrykull_45A9D0(s16 bResetRingTimer);

public:
    s32 field_118_prev_held;
    s32 field_11C_released_buttons;
    AllInternalStates field_120_state;
    s16 field_122_knockdown_motion;
    s32 field_124_timer;
    Abe_1BC_20_sub_object field_128;
    s32 field_144_auto_say_timer;
    s32 field_148_fade_obj_id;
    s32 field_14C_circular_fade_id;
    s32 field_150_OrbWhirlWind_id;
    s32 field_154_possessed_object_id;
    s32 field_158_throwable_id;
    s32 field_15C_pull_rope_id;
    s32 field_160_slappable_or_pick_item_id;
    s32 field_164_wheel_id;
    s32 field_168_ring_pulse_timer;
    s16 field_16C_bHaveShrykull;
    s16 field_16E_bHaveInvisiblity;
    s32 field_170_invisible_timer;
    s16 field_174_unused;
    s16 field_176_invisibility_id;
    s32 field_178_invisible_effect_id;
    s8 field_17C_cam_idx;
    s8 field_17D_padding;
    s8 field_17E_padding;
    s8 field_17F_padding;
    TlvTypes32 field_180_hand_stone_type;
    s16 field_184_fmv_id;
    s16 field_186_to_camera_id[3];
    s16 field_18C_unused;
    s16 field_18E_unused;
    s16 field_190_unused;
    s16 field_192_unused;
    s16 field_194_unused;
    s16 field_196_unused;
    s16 field_198_has_evil_fart;
    LevelIds field_19A_to_level;
    s16 field_19C_to_path;
    s16 field_19E_to_camera;
    s16 field_1A0_door_id;
    s8 field_1A2_throwable_count;
    s8 field_1A3_throw_direction;
    PortalSubStates field_1A4_portal_sub_state;
    s16 field_1A6_padding;
    s32 field_1A8_portal_id;

    enum Flags_1AC
    {
        e1AC_Bit1_lift_point_dead_while_using_lift = 0x1,
        e1AC_Bit2_return_to_previous_motion = 0x2,
        e1AC_Bit3_WalkToRun = 0x4,
        e1AC_Bit4_unused = 0x8,
        e1AC_Bit5_shrivel = 0x10,
        e1AC_Bit6_prevent_chanting = 0x20,
        e1AC_Bit7_land_softly = 0x40,
        e1AC_Bit8_unused = 0x80,
        e1AC_Bit9_laugh_at_chant_end = 0x100,
        e1AC_Bit10_padding = 0x200,
        e1AC_Bit11_padding = 0x400,
        e1AC_Bit12_unused = 0x800,
        e1AC_eBit13_play_ledge_grab_sounds = 0x1000,
        e1AC_eBit14_unused = 0x2000,
        e1AC_eBit15_have_healing = 0x4000,
        e1AC_eBit16_is_mudanchee_vault_ender = 0x8000,
    };
    BitField16<Flags_1AC> field_1AC_flags;

    enum Flags_1AE
    {
        e1AE_Bit1_is_mudomo_vault_ender = 0x1,
        e1AE_Bit2_do_quicksave = 0x2,
    };
    BitField16<Flags_1AE> field_1AE_flags;

    s16 field_1B0_save_num;

    s32 field_1B4_padding;
    s32 field_1B8_padding;
};
ALIVE_ASSERT_SIZEOF(Abe, 0x1BC);

ALIVE_VAR_EXTERN(Abe*, sActiveHero_5C1B68);
ALIVE_VAR_EXTERN(BaseAliveGameObject*, sControlledCharacter_5C1B8C);

ALIVE_VAR_EXTERN(s16, gAbeBulletProof_5C1BDA);

extern const char_type* const sAbeMotionNames[130];

EXPORT s32 CC Environment_SFX_457A40(EnvironmentSfx sfxId, s32 volume, s32 pitchMin, BaseAliveGameObject* pAliveObj);

EXPORT s32 CC Animation_OnFrame_Abe_455F80(void* pPtr, s16* pData);

EXPORT s32 CC XGrid_Index_To_XPos_4498F0(FP scale, s32 xGridIndex);
