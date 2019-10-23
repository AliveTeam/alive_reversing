#pragma once

#include "BaseAliveGameObject.hpp"
#include "FunctionFwd.hpp"
#include "BitField.hpp"
#include "Input.hpp"

#define ABE_STATES_ENUM(ENTRY) \
    ENTRY(State_0_Idle_44EEB0) \
    ENTRY(State_1_WalkLoop_44FBA0) \
    ENTRY(State_2_StandingTurn_451830) \
    ENTRY(State_3_Fall_459B60) \
    ENTRY(State_4_WalkEndLeftFoot_44FFC0) \
    ENTRY(State_5_WalkEndRightFoot_00450080) \
    ENTRY(State_6_WalkBegin_44FEE0) \
    ENTRY(State_7_Speak_45B140) \
    ENTRY(State_8_Speak_45B160) \
    ENTRY(State_9_SpeakMovement_45B180) \
    ENTRY(State_10_Fart_45B1A0) \
    ENTRY(State_11_Speak_45B0A0) \
    ENTRY(State_12_Null_4569C0) \
    ENTRY(State_13_HoistBegin_452B20) \
    ENTRY(State_14_HoistIdle_452440) \
    ENTRY(State_15_HoistLand_452BA0) \
    ENTRY(State_16_LandSoft_45A360) \
    ENTRY(State_17_CrouchIdle_456BC0) \
    ENTRY(State_18_CrouchToStand_454600) \
    ENTRY(State_19_StandToCrouch_453DC0) \
    ENTRY(State_20_CrouchGameSpeak_454550) \
    ENTRY(jState_21_ToCrouchGameSpeak_4545E0) \
    ENTRY(State_22_RollBegin_4539A0) \
    ENTRY(State_23_RollLoop_453A90) \
    ENTRY(State_24_453D00) \
    ENTRY(State_25_RunSlideStop_451330) \
    ENTRY(State_26_RunTurn_451500) \
    ENTRY(State_27_HopBegin_4521C0) \
    ENTRY(State_28_HopMid_451C50) \
    ENTRY(State_29_HopLand_4523D0) \
    ENTRY(State_30_RunJumpBegin_4532E0) \
    ENTRY(State_31_RunJumpMid_452C10) \
    ENTRY(State_32_RunJumpLand_453460) \
    ENTRY(State_33_RunLoop_4508E0) \
    ENTRY(State_34_DunnoBegin_44ECF0) \
    ENTRY(State_35_DunnoEnd_44ED10) \
    ENTRY(State_36_Null_45BC50) \
    ENTRY(State_37_CrouchTurn_454390) \
    ENTRY(jState_38_RunToRoll_453A70) \
    ENTRY(State_39_StandingToRun_450D40) \
    ENTRY(State_40_SneakLoop_450550) \
    ENTRY(State_41_WalkToSneak_450250) \
    ENTRY(State_42_SneakToWalk_4503D0) \
    ENTRY(State_43_ToWalkToSneak_450380) \
    ENTRY(State_44_ToSneakToWalk_450500) \
    ENTRY(State_45_SneakBegin_4507A0) \
    ENTRY(State_46_SneakEnd_450870) \
    ENTRY(jState_47_SneakEnd_4508C0) \
    ENTRY(State_48_WalkToRun_4500A0) \
    ENTRY(State_49_ToWalkToRun_450200) \
    ENTRY(State_50_RunToWalk1_450E20) \
    ENTRY(State_51_RunToWalk2_450F50) \
    ENTRY(State_52_RunTurn_ToRun_451710) \
    ENTRY(State_53_RunTurn_ToWalk_451800) \
    ENTRY(State_54_RunJumpLandRun_4538F0) \
    ENTRY(State_55_RunJumpLandWalk_453970) \
    ENTRY(State_56_FallAndCrunchDeath_4591F0) \
    ENTRY(State_57_Dead_4589A0) \
    ENTRY(State_58_DeadPre_4593E0) \
    ENTRY(State_59_Null_459450) \
    ENTRY(State_60_4A3200) \
    ENTRY(State_61_TurnToRun_456530) \
    ENTRY(State_62_Punch_454750) \
    ENTRY(State_63_Sorry_454670) \
    ENTRY(State_64_AfterSorry_454730) \
    ENTRY(State_65_LedgeAscend_End_4548E0) \
    ENTRY(State_66_LedgeDescend_454970) \
    ENTRY(State_67_LedgeHang_454E20) \
    ENTRY(State_68_ToOffScreenHoist_454B80) \
    ENTRY(State_69_LedgeHangWobble_454EF0) \
    ENTRY(State_70_RingRopePullHang_455AF0) \
    ENTRY(State_71_Knockback_455090) \
    ENTRY(State_72_KnockbackGetUp_455340) \
    ENTRY(State_73_PushWall_4553A0) \
    ENTRY(State_74_Rolling_KnockedBack_455290) \
    ENTRY(State_75_Jump_Into_Well_45C7B0) \
    ENTRY(State_76_ToInsideOfAWellLocal_45CA40) \
    ENTRY(State_77_ToWellShotOut_45D130) \
    ENTRY(State_78_WellBegin_45C810) \
    ENTRY(State_79_Inside_Of_A_Well_Local_45CA60) \
    ENTRY(State_80_WellShotOut_45D150) \
    ENTRY(jState_81_WellBegin_45C7F0) \
    ENTRY(State_82_Inside_Of_A_Well_Express_45CC80) \
    ENTRY(State_83_Shoot_Out_Of_A_Well_45CF70) \
    ENTRY(State_84_FallLandDie_45A420) \
    ENTRY(jState_85_Fall_455070) \
    ENTRY(State_86_HandstoneBegin_45BD00) \
    ENTRY(State_87_HandstoneEnd_45C4F0) \
    ENTRY(State_88_GrenadeMachineUse_45C510) \
    ENTRY(State_89_BrewMachineBegin_4584C0) \
    ENTRY(State_90_BrewMachineEnd_4585B0) \
    ENTRY(State_91_FallingFromGrab_4557B0) \
    ENTRY(State_92_ForceDown_From_Hoist_455800) \
    ENTRY(State_93_FallLedgeBegin_455970) \
    ENTRY(State_94_RunOffEdge_4559A0) \
    ENTRY(State_95_SneakOffEdge_4559C0) \
    ENTRY(State_96_HopToFall_4559E0) \
    ENTRY(State_97_RunJumpFall_455A80) \
    ENTRY(State_98_RollOffEdge_455AA0) \
    ENTRY(State_99_LeverUse_455AC0) \
    ENTRY(State_100_Slap_Bomb_455B60) \
    ENTRY(State_101_KnockForward_455420) \
    ENTRY(State_102_ToRollingKnockedBack_455310) \
    ENTRY(jState_103_KnockbackGetUp_455380) \
    ENTRY(State_104_RockThrowStandingHold_455DF0) \
    ENTRY(State_105_RockThrowStandingThrow_456460) \
    ENTRY(State_106_RockThrowStandingEnd_455F20) \
    ENTRY(State_107_RockThrowCrouchingHold_454410) \
    ENTRY(State_108_RockThrowCrouchingThrow_454500) \
    ENTRY(State_109_Shot_Rolling_455550) \
    ENTRY(State_110_Shot_455670) \
    ENTRY(State_111_GrabRock_4564A0) \
    ENTRY(State_112_Chant_45B1C0) \
    ENTRY(State_113_ChantEnd_45BBE0) \
    ENTRY(State_114_DoorEnter_459470) \
    ENTRY(State_115_DoorExit_459A40) \
    ENTRY(State_116_MineCarEnter_458780) \
    ENTRY(State_117_In_MineCar_4587C0) \
    ENTRY(State_118_MineCarExit_458890) \
    ENTRY(State_119_To_Shrykull_45A990) \
    ENTRY(State_120_EndShrykull_45AB00) \
    ENTRY(State_121_LiftGrabBegin_45A600) \
    ENTRY(State_122_LiftGrabEnd_45A670) \
    ENTRY(State_123_LiftGrabIdle_45A6A0) \
    ENTRY(State_124_LiftUseUp_45A780) \
    ENTRY(State_125_LiftUseDown_45A7B0) \
    ENTRY(State_126_TurnWheelBegin_456700) \
    ENTRY(State_127_TurnWheelLoop_456750) \
    ENTRY(State_128_TurnWheelEnd_4569A0) \
    ENTRY(State_129_PoisonGasDeath_4565C0)

#define MAKE_ENUM(VAR) VAR,
enum eAbeStates : int
{
    ABE_STATES_ENUM(MAKE_ENUM)
};

class PullRingRope;

extern const TintEntry sTintTable_Abe_554D20[15];

extern const InputCommands sInputKey_Right_5550D0;
extern const InputCommands sInputKey_Left_5550D4;
extern const InputCommands sInputKey_Up_5550D8;
extern const InputCommands sInputKey_Down_5550DC;
extern const InputCommands sInputKey_Hop_5550E0;
extern const InputCommands sInputKey_DoAction_5550E4;
extern const InputCommands sInputKey_Run_5550E8;
extern const InputCommands sInputKey_Sneak_5550EC;
extern const InputCommands sInputKey_FartRoll_5550F0;
extern const InputCommands sInputKey_ThrowItem_5550F4;
extern const InputCommands sInputKey_GameSpeak2_5550F8;
extern const InputCommands sInputKey_GameSpeak4_5550FC;
extern const InputCommands sInputKey_GameSpeak3_555100;
extern const InputCommands sInputKey_GameSpeak1_555104;
extern const InputCommands sInputKey_GameSpeak6_555108;
extern const InputCommands sInputKey_GameSpeak5_55510C;
extern const InputCommands sInputKey_GameSpeak8_555110;
extern const InputCommands sInputKey_GameSpeak7_555114;

enum class MudSounds : __int8
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
    eDeathDrop_15 = 15,
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

enum class Mud_Emotion : __int16;

struct Abe_1BC_20_sub_object
{
    int field_0_gnFrame;
    int field_4_regen_health_timer;
    FP field_8;
    int field_C;
    unsigned __int16 field_10;
    Mud_Emotion field_12_mood;
    int field_14;
    MudSounds field_18_say;
    __int16 field_1A;
};
ALIVE_ASSERT_SIZEOF(Abe_1BC_20_sub_object, 0x1C);

EXPORT void CC Abe_SFX_457EC0(MudSounds idx, __int16 volume, int pitch, BaseAliveGameObject *pHero);

struct Abe_SaveState
{
    Types field_0_id;
    __int16 field_2_pad;
    FP field_4_xpos;
    FP field_8_ypos;
    FP field_c_velx;
    FP field_10_vely;
    WORD field_14_path_number;
    LevelIds field_16_lvl_number;
    FP field_18_sprite_scale;
    WORD field_1C_scale;
    WORD field_1e_r;
    WORD field_20_g;
    WORD field_22_b;
    WORD bAnimFlipX;
    WORD current_motion;
    WORD anim_current_frame;
    WORD anim_frame_change_counter;
    char anim_render_layer;
    char bAnimRender;
    char bDrawable;
    char field_2F;
    FP field_30_health;
    WORD field_34_animation_num;
    WORD next_motion;
    WORD last_line_ypos;
    short field_3a_collision_line_id;
    DWORD platform_obj_id;
    WORD bElectrocuted;
    WORD word42;
    char field_44_is_abe_controlled;
    char field_45;
    __int16 field_46;
    FP dword48;
    DWORD dword4C;
    DWORD dword50;
    DWORD dword54;
    DWORD dword58;
    DWORD dword5C;
    Mud_Emotion mood;
    MudSounds say;
    DWORD auto_say_timer;
    DWORD ring_pulse_timer;
    char field_6c_rock_bone_count;
    char bShrivel;
    char bHaveShrykull;
    char bHaveInvisiblity;
    WORD prev_held;
    WORD released_buttons;
    WORD word74;
    __int16 field_76;
    DWORD dword78;
    DWORD fade_obj_id;
    DWORD circular_fade_id;
    DWORD orb_whirl_wind_id;
    DWORD possesed_object_id;
    DWORD throwabe_obj_id;
    DWORD pull_ring_rope_id;
    DWORD slapable_or_pickup_id;
    DWORD wheel_id;
    DWORD invisible_timer;
    WORD wordA0;
    WORD wordA2;
    char byteA4;
    char field_A5;
    __int16 field_A6;
    DWORD hand_stone_type;
    WORD fmv_id;
    WORD cam_id_1;
    WORD cam_id_2;
    WORD cam_id_3;
    WORD wordB4;
    WORD wordB6;
    WORD wordB8;
    WORD wordBA;
    WORD wordBC;
    WORD wordBE;
    WORD bHaveEvilFart;
    LevelIds to_level;
    WORD to_path;
    WORD to_camera;
    WORD door_id;
    char field_ca_throw_direction;
    char field_CB;
    WORD wordCC;
    __int16 field_CE;
    DWORD bird_portal_id;
    enum Flags_D4
    {
        eD4_Bit1 = 0x1,
        eD4_Bit2 = 0x2,
        eD4_Bit3 = 0x4,
        eD4_Bit4 = 0x8,
        eD4_Bit5 = 0x10,
        eD4_Bit6 = 0x20,
        eD4_Bit7 = 0x40,
        eD4_Bit8 = 0x80,
        eD4_Bit9 = 0x100,
        eD4_Bit10 = 0x200,
        eD4_Bit11 = 0x400,
        eD4_Bit12 = 0x800,
        eD4_eBit13 = 0x1000,
        eD4_eBit14 = 0x2000,
        eD4_eBit15 = 0x4000,
        eD4_eBit16 = 0x8000,
    };
    BitField16<Flags_D4> wordD4;

    enum Flags_D6
    {
        eD6_Bit1 = 0x1
    };
    BitField16<Flags_D6> wordD6;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Abe_SaveState, 216);

class Bullet;

class Abe : public BaseAliveGameObject
{
public:
    EXPORT Abe* ctor_44AD10(int frameTableOffset, int a3, int a4, int a5);
    EXPORT void dtor_44B380();

    EXPORT static signed int CC CreateFromSaveState_44D4F0(const BYTE* a1);
    
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;
    virtual void VScreenChanged() override;
    virtual int VGetSaveState(BYTE* pSaveBuffer) override;
    virtual __int16 VTakeDamage_408730(BaseGameObject* pFrom) override;
    virtual void VOn_TLV_Collision_4087F0(Path_TLV* pTlv) override;
    virtual BirdPortal* VIntoBirdPortal_408FD0(__int16 gridBlocks) override;
    virtual void VOnTrapDoorOpen() override;

    // Virtual impls
    EXPORT BaseGameObject* vdtor_44B350(signed int flags);
    EXPORT void Update_449DC0();
    EXPORT void vRender_44B580(int** pOrderingTable);
    EXPORT void vScreenChanged_44D240();
    EXPORT int vGetSaveState_457110(BYTE* pSaveBuffer);
    EXPORT __int16 vTakeDamage_44BB50(BaseGameObject* pFrom);
    EXPORT void vOn_TLV_Collision_44B5D0(Path_TLV* pTlv);
    EXPORT BirdPortal* vIntoBirdPortal_44E970(__int16 gridBlocks);
    EXPORT void vOnTrapDoorOpen_45A570();

    // Non virtuals
    EXPORT void ToKnockback_44E700(__int16 a2, __int16 a3);
    EXPORT BaseAliveGameObject* FindObjectToPosses_44B7B0();
    EXPORT void Load_Basic_Resources_44D460();
    EXPORT void Free_Resources_44D420();
    EXPORT BOOL IsStanding_449D30();
    EXPORT void Free_Shrykull_Resources_45AA90();

    EXPORT BYTE ** StateToAnimResource_44AAB0(short state);

    // States
    EXPORT void State_0_Idle_44EEB0();
    EXPORT void State_1_WalkLoop_44FBA0();
    EXPORT void State_2_StandingTurn_451830();
    EXPORT void State_3_Fall_459B60();
    EXPORT void State_4_WalkEndLeftFoot_44FFC0();
    EXPORT void State_5_WalkEndRightFoot_00450080();
    EXPORT void State_6_WalkBegin_44FEE0();
    EXPORT void State_7_Speak_45B140();
    EXPORT void State_8_Speak_45B160();
    EXPORT void State_9_SpeakMovement_45B180();
    EXPORT void State_10_Fart_45B1A0();
    EXPORT void State_11_Speak_45B0A0();
    void State_12_Null_4569C0();
    EXPORT void State_13_HoistBegin_452B20();
    EXPORT void State_14_HoistIdle_452440();
    EXPORT void State_15_HoistLand_452BA0();
    EXPORT void State_16_LandSoft_45A360();
    EXPORT void State_17_CrouchIdle_456BC0();
    EXPORT void State_18_CrouchToStand_454600();
    EXPORT void State_19_StandToCrouch_453DC0();
    EXPORT void State_20_CrouchGameSpeak_454550();
    EXPORT void jState_21_ToCrouchGameSpeak_4545E0();
    EXPORT void State_22_RollBegin_4539A0();
    EXPORT void State_23_RollLoop_453A90();
    EXPORT void State_24_453D00();
    EXPORT void State_25_RunSlideStop_451330();
    EXPORT void State_26_RunTurn_451500();
    EXPORT void State_27_HopBegin_4521C0();
    EXPORT void State_28_HopMid_451C50();
    EXPORT void State_29_HopLand_4523D0();
    EXPORT void State_30_RunJumpBegin_4532E0();
    EXPORT void State_31_RunJumpMid_452C10();
    EXPORT void State_32_RunJumpLand_453460();
    EXPORT void State_33_RunLoop_4508E0();
    EXPORT void State_34_DunnoBegin_44ECF0();
    EXPORT void State_35_DunnoEnd_44ED10();
    void State_36_Null_45BC50();
    EXPORT void State_37_CrouchTurn_454390();
    EXPORT void jState_38_RunToRoll_453A70();
    EXPORT void State_39_StandingToRun_450D40();
    EXPORT void State_40_SneakLoop_450550();
    EXPORT void State_41_WalkToSneak_450250();
    EXPORT void State_42_SneakToWalk_4503D0();
    EXPORT void State_43_ToWalkToSneak_450380();
    EXPORT void State_44_ToSneakToWalk_450500();
    EXPORT void State_45_SneakBegin_4507A0();
    EXPORT void State_46_SneakEnd_450870();
    EXPORT void jState_47_SneakEnd_4508C0();
    EXPORT void State_48_WalkToRun_4500A0();
    EXPORT void State_49_ToWalkToRun_450200();
    EXPORT void State_50_RunToWalk1_450E20();
    EXPORT void State_51_RunToWalk2_450F50();
    EXPORT void State_52_RunTurn_ToRun_451710();
    EXPORT void State_53_RunTurn_ToWalk_451800();
    EXPORT void State_54_RunJumpLandRun_4538F0();
    EXPORT void State_55_RunJumpLandWalk_453970();
    EXPORT void State_56_FallAndCrunchDeath_4591F0();
    EXPORT void State_57_Dead_4589A0();
    EXPORT void State_58_DeadPre_4593E0();
    void State_59_Null_459450();
    EXPORT void State_60_4A3200();
    EXPORT void State_61_TurnToRun_456530();
    EXPORT void State_62_Punch_454750();
    EXPORT void State_63_Sorry_454670();
    EXPORT void State_64_AfterSorry_454730();
    EXPORT void State_65_LedgeAscend_End_4548E0();
    EXPORT void State_66_LedgeDescend_454970();
    EXPORT void State_67_LedgeHang_454E20();
    EXPORT void State_68_ToOffScreenHoist_454B80();
    EXPORT void State_69_LedgeHangWobble_454EF0();
    EXPORT void State_70_RingRopePullHang_455AF0();
    EXPORT void State_71_Knockback_455090();
    EXPORT void State_72_KnockbackGetUp_455340();
    EXPORT void State_73_PushWall_4553A0();
    EXPORT void State_74_Rolling_KnockedBack_455290();
    EXPORT void State_75_Jump_Into_Well_45C7B0();
    EXPORT void State_76_ToInsideOfAWellLocal_45CA40();
    EXPORT void State_77_ToWellShotOut_45D130();
    EXPORT void State_78_WellBegin_45C810();
    EXPORT void State_79_Inside_Of_A_Well_Local_45CA60();
    EXPORT void State_80_WellShotOut_45D150();
    EXPORT void jState_81_WellBegin_45C7F0();
    EXPORT void State_82_Inside_Of_A_Well_Express_45CC80();
    EXPORT void State_83_Shoot_Out_Of_A_Well_45CF70();
    EXPORT void State_84_FallLandDie_45A420();
    EXPORT void jState_85_Fall_455070();
    EXPORT void State_86_HandstoneBegin_45BD00();
    EXPORT void State_87_HandstoneEnd_45C4F0();
    EXPORT void State_88_GrenadeMachineUse_45C510();
    EXPORT void State_89_BrewMachineBegin_4584C0();
    EXPORT void State_90_BrewMachineEnd_4585B0();
    EXPORT void State_91_FallingFromGrab_4557B0();
    EXPORT void State_92_ForceDown_From_Hoist_455800();
    EXPORT void State_93_FallLedgeBegin_455970();
    EXPORT void State_94_RunOffEdge_4559A0();
    EXPORT void State_95_SneakOffEdge_4559C0();
    EXPORT void State_96_HopToFall_4559E0();
    EXPORT void State_97_RunJumpFall_455A80();
    EXPORT void State_98_RollOffEdge_455AA0();
    EXPORT void State_99_LeverUse_455AC0();
    EXPORT void State_100_Slap_Bomb_455B60();
    EXPORT void State_101_KnockForward_455420();
    EXPORT void State_102_ToRollingKnockedBack_455310();
    EXPORT void jState_103_KnockbackGetUp_455380();
    EXPORT void State_104_RockThrowStandingHold_455DF0();
    EXPORT void State_105_RockThrowStandingThrow_456460();
    EXPORT void State_106_RockThrowStandingEnd_455F20();
    EXPORT void State_107_RockThrowCrouchingHold_454410();
    EXPORT void State_108_RockThrowCrouchingThrow_454500();
    EXPORT void State_109_Shot_Rolling_455550();
    EXPORT void State_110_Shot_455670();
    EXPORT void State_111_GrabRock_4564A0();
    EXPORT void State_112_Chant_45B1C0();
    EXPORT void State_113_ChantEnd_45BBE0();
    EXPORT void State_114_DoorEnter_459470();
    EXPORT void State_115_DoorExit_459A40();
    EXPORT void State_116_MineCarEnter_458780();
    EXPORT void State_117_In_MineCar_4587C0();
    EXPORT void State_118_MineCarExit_458890();
    EXPORT void State_119_To_Shrykull_45A990();
    EXPORT void State_120_EndShrykull_45AB00();
    EXPORT void State_121_LiftGrabBegin_45A600();
    EXPORT void State_122_LiftGrabEnd_45A670();
    EXPORT void State_123_LiftGrabIdle_45A6A0();
    EXPORT void State_124_LiftUseUp_45A780();
    EXPORT void State_125_LiftUseDown_45A7B0();
    EXPORT void State_126_TurnWheelBegin_456700();
    EXPORT void State_127_TurnWheelLoop_456750();
    EXPORT void State_128_TurnWheelEnd_4569A0();
    EXPORT void State_129_PoisonGasDeath_4565C0();

    EXPORT void jState_81_WellBegin_4017F8();

    // End states
    
    EXPORT void FleechDeath_459350();


    EXPORT void ToDie_4588D0();
    EXPORT void ToIdle_44E6B0();
    EXPORT void PickUpThrowabe_Or_PressBomb_454090(FP fpX, int fpY, int bStandToCrouch);
    EXPORT void Get_Shrykull_Resources_45AA20();
    EXPORT __int16 ToLeftRightMovement_44E340();
    EXPORT void TryHoist_44ED30();
    EXPORT static void CC Create_Fart_421D20();
    EXPORT __int16 TryEnterMineCar_4569E0();
    EXPORT int NearDoorIsOpen_44EE10();
    EXPORT __int16 HandleDoAction_455BD0();
    EXPORT void PushWall_44E890();
    EXPORT void sub_44E9A0();
    EXPORT __int16 CrouchingGameSpeak_453E10();
    EXPORT BOOL Is_Celling_Above_44E8D0();
    EXPORT void MoveWithVelocity_450FA0(FP velocityX);
    EXPORT __int16 RunTryEnterDoor_451220();
    EXPORT __int16 RunTryEnterWell_451060();
    EXPORT void ToDieFinal_458910();
    EXPORT short DoGameSpeak_45AB70(int input);
    EXPORT __int16 CantBeDamaged_44BAB0();
    EXPORT void FallOnBombs_44EC10();
    EXPORT __int16 ForceDownIfHoisting_44BA30();
    EXPORT void BulletDamage_44C980(Bullet* pObj);
    EXPORT void GiveControlBackToMe_44BA10();
    EXPORT PullRingRope* GetPullRope_44D120();
    EXPORT void IntoPortalStates_451990();
    EXPORT void Calc_Well_Velocity_45C530(short x1, short y1, short x2, short y2);
    EXPORT void FollowLift_45A500();

    EXPORT short MoveLiftUpOrDown_45A7E0(FP yVelocity);


    EXPORT __int16 GetEvilFart_4585F0(__int16 bDontLoad);

    EXPORT void sub_45BB90(__int16 a2);

    EXPORT void sub_459430();

    EXPORT void ExitShrykull_45A9D0(__int16 bResetRingTimer);

public:
    int field_118_prev_held;
    int field_11C_released_buttons;
    unsigned __int16 field_120_state;
    __int16 field_122;
    int field_124_gnFrame;
    Abe_1BC_20_sub_object field_128;
    int field_144_auto_say_timer;
    int field_148_fade_obj_id;
    int field_14C_circular_fade_id;
    int field_150_OrbWhirlWind_id;
    int field_154_possesed_object_id;
    int field_158_throwable_id;
    int field_15C_pull_rope_id;
    int field_160_slapable_or_pick_item_id;
    int field_164_wheel_id;
    int field_168_ring_pulse_timer;
    __int16 field_16C_bHaveShrykull;
    __int16 field_16E_bHaveInvisiblity;
    int field_170_invisible_timer;
    __int16 field_174;
    __int16 field_176;
    int field_178_invisible_effect_id;
    char field_17C_cam_idx;
    char field_17D;
    char field_17E;
    char field_17F;
    int field_180_hand_stone_type;
    __int16 field_184_fmv_id;
    __int16 field_186_to_camera_id[3];
    __int16 field_18C_not_used;
    __int16 field_18E;
    __int16 field_190;
    __int16 field_192;
    __int16 field_194;
    __int16 field_196;
    __int16 field_198_has_evil_fart;
    LevelIds field_19A_to_level;
    __int16 field_19C_to_path;
    __int16 field_19E_to_camera;
    __int16 field_1A0_door_id;
    char field_1A2_rock_or_bone_count;
    char field_1A3_throw_direction;
    __int16 field_1A4_portal_sub_state;
    __int16 field_1A6;
    int field_1A8_portal_id;

    enum Flags_1AC
    {
        e1AC_Bit1 = 0x1,
        e1AC_Bit2 = 0x2,
        e1AC_Bit3_Fall_To_Well = 0x4,
        e1AC_Bit4 = 0x8,
        e1AC_Bit5_bShrivel = 0x10,
        e1AC_Bit6 = 0x20,
        e1AC_Bit7 = 0x40,
        e1AC_Bit8 = 0x80,
        e1AC_Bit9_bLaughAtChantEnd = 0x100,
        e1AC_Bit10 = 0x200,
        e1AC_Bit11 = 0x400,
        e1AC_Bit12 = 0x800,
        e1AC_eBit13 = 0x1000,
        e1AC_eBit14 = 0x2000,
        e1AC_eBit15_bHaveHealing = 0x4000,
        e1AC_eBit16_bIsMudancheeVault_Ender = 0x8000,
    };
    BitField16<Flags_1AC> field_1AC_flags;

    enum Flags_1AE
    {
        e1AE_Bit1_bIsMudancheeVault_Ender = 0x1,
        e1AE_Bit2_bDoQuickSave = 0x2,
    };
    BitField16<Flags_1AE> field_1AE;

    __int16 field_1B0_save_num;
    // pad __int16

    int field_1B4;
    int field_1B8;
};
ALIVE_ASSERT_SIZEOF(Abe, 0x1BC);

ALIVE_VAR_EXTERN(Abe *, sActiveHero_5C1B68);
ALIVE_VAR_EXTERN(BaseAliveGameObject*, sControlledCharacter_5C1B8C);

ALIVE_VAR_EXTERN(short, gAbeBulletProof_5C1BDA);

extern const int sAbeFrameOffsetTable_554B18[130];
extern const char* const sAbeStateNames[130];

EXPORT int CC Abe_SFX_2_457A40(char sfxId, int volume, int pitchMin, BaseAliveGameObject* pAliveObj);

EXPORT int CC Animation_OnFrame_Abe_455F80(void* pPtr, signed __int16* pData);

EXPORT int CC GridXMidPos_4498F0(FP scale, int unknown);
