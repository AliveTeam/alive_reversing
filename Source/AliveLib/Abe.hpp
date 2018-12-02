#pragma once

#include "BaseAliveGameObject.hpp"
#include "FunctionFwd.hpp"
#include "BitField.hpp"

struct Abe_1BC_20_sub_object
{
    int field_0_gnFrame;
    int field_4;
    FP field_8;
    int field_C;
    unsigned __int16 field_10;
    __int16 field_12_mood;
    int field_14;
    __int16 field_18_say;
    __int16 field_1A;
};
ALIVE_ASSERT_SIZEOF(Abe_1BC_20_sub_object, 0x1C);

EXPORT void CC Abe_SFX_457EC0(unsigned __int8 idx, __int16 volume, int pitch, class Abe *pHero);

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
    virtual int GetSaveState_4DC110(BYTE* pSaveBuffer) override;
    virtual __int16 VTakeDamage_408730(BaseAliveGameObject* pFrom) override;
    virtual __int16 VOn_TLV_Collision_4087F0(Path_TLV* pTlv) override;
    virtual BaseGameObject* Vsub_408FD0(__int16 a2) override;
    virtual int Vnull_4081F0() override;

    // Virtual impls
    EXPORT BaseGameObject* vdtor_44B350(signed int flags);
    EXPORT void Update_449DC0();
    EXPORT void vRender_44B580(int** pOrderingTable);
    EXPORT void vScreenChanged_44D240();
    EXPORT int vGetSaveState_457110(BYTE* pSaveBuffer);
    EXPORT __int16 vTakeDamage_44BB50(BaseAliveGameObject* pFrom);
    EXPORT __int16 vOn_TLV_Collision_44B5D0(Path_TLV* pTlv);
    EXPORT BaseGameObject* vsub_44E970(__int16 a2);
    EXPORT BaseGameObject* vsub_45A570();

    // Non virtuals
    EXPORT void ToKnockback_44E700(__int16 a2, __int16 a3);
    EXPORT int sub_44B7B0();
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
    EXPORT void State_7_45B140();
    EXPORT void State_8_45B160();
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
    EXPORT void State_20_454550();
    EXPORT void jState_21_4545E0();
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
    EXPORT void jState_38_RollBegin_453A70();
    EXPORT void State_39_StandingToRun_450D40();
    EXPORT void State_40_SneakLoop_450550();
    EXPORT void State_41_450250();
    EXPORT void State_42_4503D0();
    EXPORT void State_43_450380();
    EXPORT void State_44_450500();
    EXPORT void State_45_SneakBegin_4507A0();
    EXPORT void State_46_SneakEnd_450870();
    EXPORT void jState_47_SneakEnd_4508C0();
    EXPORT void State_48_4500A0();
    EXPORT void State_49_450200();
    EXPORT void State_50_RunToWalk1_450E20();
    EXPORT void State_51_RunToWalk2_450F50();
    EXPORT void State_52_RunTurn_ToRun_451710();
    EXPORT void State_53_RunTurn_ToWalk_451800();
    EXPORT void State_54_RunJumpLandRun_4538F0();
    EXPORT void State_55_RunJumpLandWalk_453970();
    EXPORT void State_56_4591F0();
    EXPORT void State_57_Dead_4589A0();
    EXPORT void State_58_DeadPre_4593E0();
    void State_59_Null_459450();
    EXPORT void State_60_4A3200();
    EXPORT void State_61_TurnToRun_456530();
    EXPORT void State_62_Punch_454750();
    EXPORT void State_63_Sorry_454670();
    EXPORT void State_64_454730();
    EXPORT void State_65_LedgeAscend_4548E0();
    EXPORT void State_66_LedgeDescend_454970();
    EXPORT void State_67_LedgeHang_454E20();
    EXPORT void State_68_454B80();
    EXPORT void State_69_LedgeHangWobble_454EF0();
    EXPORT void State_70_RingRopePull_455AF0();
    EXPORT void State_71_Knockback_455090();
    EXPORT void State_72_KnockbackGetUp_455340();
    EXPORT void State_73_PushWall_4553A0();
    EXPORT void State_74_455290();
    EXPORT void State_75_Jump_Into_Well_45C7B0();
    EXPORT void State_76_45CA40();
    EXPORT void State_77_45D130();
    EXPORT void State_78_WellBegin_45C810();
    EXPORT void State_79_WellInside_45CA60();
    EXPORT void State_80_WellShotOut_45D150();
    EXPORT void jState_81_WellBegin_45C7F0();
    EXPORT void State_82_45CC80();
    EXPORT void State_83_45CF70();
    EXPORT void State_84_FallLandDie_45A420();
    EXPORT void jState_85_Fall_455070();
    EXPORT void State_86_HandstoneBegin_45BD00();
    EXPORT void State_87_HandstoneEnd_45C4F0();
    EXPORT void State_88_GrenadeMachineUse_45C510();
    EXPORT void State_89_BrewMachineBegin_4584C0();
    EXPORT void State_90_BrewMachineEnd_4585B0();
    EXPORT void State_91_RingRopePullEnd_4557B0();
    EXPORT void State_92_ForceDown_From_Hoist_455800();
    EXPORT void State_93_FallLedgeBegin_455970();
    EXPORT void jState_94_FallLedgeBegin_4559A0();
    EXPORT void jState_95_FallLedgeBegin_4559C0();
    EXPORT void State_96_4559E0();
    EXPORT void jState_97_FallLedgeBegin_455A80();
    EXPORT void jState_98_FallLedgeBegin_455AA0();
    EXPORT void State_99_LeverUse_455AC0();
    EXPORT void State_100_455B60();
    EXPORT void State_101_KnockForward_455420();
    EXPORT void State_102_455310();
    EXPORT void jState_103_KnockbackGetUp_455380();
    EXPORT void State_104_RockThrowStandingHold_455DF0();
    EXPORT void State_105_RockThrowStandingThrow_456460();
    EXPORT void State_106_RockThrowStandingEnd_455F20();
    EXPORT void State_107_RockThrowCrouchingHold_454410();
    EXPORT void State_108_RockThrowCrouchingThrow_454500();
    EXPORT void State_109_455550();
    EXPORT void State_110_455670();
    EXPORT void State_111_GrabRock_4564A0();
    EXPORT void State_112_Chant_45B1C0();
    EXPORT void State_113_ChantEnd_45BBE0();
    EXPORT void State_114_DoorEnter_459470();
    EXPORT void State_115_DoorExit_459A40();
    EXPORT void State_116_MineCarEnter_458780();
    EXPORT void State_117_4587C0();
    EXPORT void State_118_MineCarExit_458890();
    EXPORT void State_119_45A990();
    EXPORT void State_120_45AB00();
    EXPORT void State_121_LiftGrabBegin_45A600();
    EXPORT void State_122_LiftGrabEnd_45A670();
    EXPORT void State_123_LiftGrabIdle_45A6A0();
    EXPORT void State_124_LiftUseUp_45A780();
    EXPORT void State_125_LiftUseDown_45A7B0();
    EXPORT void State_126_TurnWheelBegin_456700();
    EXPORT void State_127_TurnWheelLoop_456750();
    EXPORT void State_128_TurnWheelEnd_4569A0();
    EXPORT void State_129_PoisonGasDeath_4565C0();

    // End states

    EXPORT void ToDie_4588D0();
    EXPORT void ToIdle_44E6B0();
    EXPORT __int16 sub_454090(FP fpX, int fpY, int a4);
    EXPORT void Get_Shrykull_Resources_45AA20();
    EXPORT __int16 ToLeftRightMovement_44E340();
    EXPORT void TryHoist_44ED30();
    EXPORT static void CC Create_Fart_421D20();
    EXPORT __int16 TryEnterMineCar_4569E0();
    EXPORT int sub_44EE10();
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
    EXPORT __int16 sub_44EC10();
    EXPORT __int16 ForceDownIfHoisting_44BA30();
    EXPORT __int16 sub_44C980(BaseAliveGameObject* pObj);
public:
    __int16 field_116;
    int field_118_prev_held;
    int field_11C;
    unsigned __int16 field_120_state;
    __int16 field_122;
    int field_124_gnFrame;
    Abe_1BC_20_sub_object field_128;
    int field_144;
    int field_148;
    int field_14C;
    int field_150_OrbWhirlWind_id;
    int field_154_possesed_object_id;
    int field_158_throwable_id;
    int field_15C_pull_rope_id;
    int field_160;
    int field_164_wheel_id;
    int field_168;
    __int16 field_16C;
    __int16 field_16E;
    int field_170;
    __int16 field_174;
    __int16 field_176;
    int field_178_invisible_effect_id;
    char field_17C;
    char field_17D;
    char field_17E;
    char field_17F;
    int field_180;
    __int16 field_184;
    __int16 field_186;
    __int16 field_188;
    __int16 field_18A;
    __int16 field_18C;
    __int16 field_18E;
    __int16 field_190;
    __int16 field_192;
    __int16 field_194;
    __int16 field_196;
    __int16 field_198_has_evil_fart;
    __int16 field_19A;
    __int16 field_19C;
    __int16 field_19E;
    __int16 field_1A0_door_id;
    char field_1A2_rock_or_bone_count;
    char field_1A3_throw_direction;
    __int16 field_1A4;
    __int16 field_1A6;
    int field_1A8;

    enum Flags_1AC
    {
        e1AC_Bit1 = 0x1,
        e1AC_Bit2 = 0x2,
        e1AC_Bit3 = 0x4,
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
        e1AC_eBit15 = 0x4000,
        e1AC_eBit16 = 0x8000,
        e1AC_eBit17 = 0x10000,
        e1AC_eBit18 = 0x20000,
        e1AC_eBit19 = 0x40000,
        e1AC_eBit20 = 0x80000,
        e1AC_eBit21 = 0x100000,
        e1AC_eBit22 = 0x200000,
        e1AC_eBit23 = 0x400000,
        e1AC_eBit24 = 0x800000,
        e1AC_eBit25 = 0x1000000,
    };
    BitField16<Flags_1AC> field_1AC_flags;
    __int16 field_1AE;
    int field_1B0_save_num;
    int field_1B4;
    int field_1B8;
};
ALIVE_ASSERT_SIZEOF(Abe, 0x1BC);

ALIVE_VAR_EXTERN(Abe *, sActiveHero_5C1B68);
ALIVE_VAR_EXTERN(Abe *, sControlledCharacter_5C1B8C);

ALIVE_VAR_EXTERN(short, word_5C1BDA);

extern int sAbeFrameOffsetTable_554B18[130];
extern const char* const sAbeStateNames[130];
