#pragma once

#include "BaseAliveGameObject.hpp"
#include "FunctionFwd.hpp"

struct Abe_1BC_20_sub_object
{
    int field_0;
    char field_4;
    char field_5;
    __int16 field_6;
    char field_8;
    char field_9;
    char field_A;
    char field_B;
    __int16 field_C;
    __int16 field_E;
    __int16 field_10;
    __int16 field_12_mood;
    int field_14;
    __int16 field_18;
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
    
    virtual void VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;
    virtual void VScreenChanged() override;
    virtual int GetSaveState_4DC110(BYTE* pSaveBuffer) override;
    virtual __int16 Vsub_408730(int arg0) override;
    virtual __int16 Vnull_4087F0(int a2a) override;
    virtual int Vsub_408FD0(__int16 a2) override;
    virtual int Vnull_4081F0() override;

    // Virtual impls
    EXPORT void vdtor_44B350(signed int flags);
    EXPORT void Update_449DC0();
    EXPORT void vRender_44B580(int** pOrderingTable);
    EXPORT void vScreenChanged_44D240();
    EXPORT int vGetSaveState_457110(BYTE* pSaveBuffer);
    EXPORT bool vsub_Kill_44BB50(BaseGameObject *otherObj);
    EXPORT __int16 vsub_44B5D0(int a2a);
    EXPORT int vsub_44E970(__int16 a2);
    EXPORT BaseGameObject* vsub_45A570();

    // Non virtuals
    EXPORT void Knockback_44E700(__int16 a2, __int16 a3);
    EXPORT int sub_44B7B0();
    EXPORT void Load_Basic_Resources_44D460();
    EXPORT void Free_Resources_44D420();
    EXPORT BOOL sub_449D30();
    EXPORT void sub_45AA90();

    EXPORT BYTE ** StateToAnimResource_44AAB0(signed int state);

    // States
    EXPORT void State_Idle_44EEB0();
    EXPORT void State_WalkLoop_44FBA0();
    EXPORT void State_StandingTurn_451830();
    EXPORT void State_Fall_459B60();
    EXPORT void State_WalkEndLeftFoot_44FFC0();
    EXPORT void State_WalkEndRightFoot_00450080();
    EXPORT void State_WalkBegin_44FEE0();
    EXPORT void State_45B140();
    EXPORT void State_45B160();
    EXPORT void State_45B180();
    EXPORT void State_Fart_45B1A0();
    EXPORT void State_Speak_45B0A0();
    void State_Null_4569C0();
    EXPORT void State_HoistBegin_452B20();
    EXPORT void State_HoistIdle_452440();
    EXPORT void State_HoistLand_452BA0();
    EXPORT void State_LandSoft_45A360();
    EXPORT void State_CrouchIdle_456BC0();
    EXPORT void State_CrouchToStand_454600();
    EXPORT void State_StandToCrouch_453DC0();
    EXPORT void State_454550();
    EXPORT void jState_4545E0();
    EXPORT void State_RollBegin_4539A0();
    EXPORT void State_RollLoop_453A90();
    EXPORT void State_453D00();
    EXPORT void State_RunSlideStop_451330();
    EXPORT void State_RunTurn_451500();
    EXPORT void State_HopBegin_4521C0();
    EXPORT void State_HopMid_451C50();
    EXPORT void State_HopLand_4523D0();
    EXPORT void State_RunJumpBegin_4532E0();
    EXPORT void State_RunJumpMid_452C10();
    EXPORT void State_RunJumpLand_453460();
    EXPORT void State_RunLoop_4508E0();
    EXPORT void State_DunnoBegin_44ECF0();
    EXPORT void State_DunnoEnd_44ED10();
    void State_Null_45BC50();
    EXPORT void State_CrouchTurn_454390();
    EXPORT void jState_RollBegin_453A70();
    EXPORT void State_StandingToRun_450D40();
    EXPORT void State_SneakLoop_450550();
    EXPORT void State_450250();
    EXPORT void State_4503D0();
    EXPORT void State_450380();
    EXPORT void State_450500();
    EXPORT void State_SneakBegin_4507A0();
    EXPORT void State_SneakEnd_450870();
    EXPORT void jState_SneakEnd_4508C0();
    EXPORT void State_4500A0();
    EXPORT void State_450200();
    EXPORT void State_RunToWalk1_450E20();
    EXPORT void State_RunToWalk2_450F50();
    EXPORT void State_451710();
    EXPORT void State_451800();
    EXPORT void State_RunJumpLandRun_4538F0();
    EXPORT void State_RunJumpLandWalk_453970();
    EXPORT void State_4591F0();
    EXPORT void State_Dead_4589A0();
    EXPORT void State_DeadPre_4593E0();
    void State_Null_459450();
    EXPORT void State_4A3200();
    EXPORT void State_TurnToRun_456530();
    EXPORT void State_Punch_454750();
    EXPORT void State_Sorry_454670();
    EXPORT void State_454730();
    EXPORT void State_LedgeAscend_4548E0();
    EXPORT void State_LedgeDescend_454970();
    EXPORT void State_LedgeHang_454E20();
    EXPORT void State_454B80();
    EXPORT void State_LedgeHangWobble_454EF0();
    EXPORT void State_RingRopePull_455AF0();
    EXPORT void State_Knockback_455090();
    EXPORT void State_KnockbackGetUp_455340();
    EXPORT void State_PushWall_4553A0();
    EXPORT void State_455290();
    EXPORT void State_45C7B0();
    EXPORT void State_45CA40();
    EXPORT void State_45D130();
    EXPORT void State_WellBegin_45C810();
    EXPORT void State_WellInside_45CA60();
    EXPORT void State_WellShotOut_45D150();
    EXPORT void jState_WellBegin_45C7F0();
    EXPORT void State_45CC80();
    EXPORT void State_45CF70();
    EXPORT void State_FallLandDie_45A420();
    EXPORT void jState_Fall_455070();
    EXPORT void State_HandstoneBegin_45BD00();
    EXPORT void State_HandstoneEnd_45C4F0();
    EXPORT void State_GrenadeMachineUse_45C510();
    EXPORT void State_BrewMachineBegin_4584C0();
    EXPORT void State_BrewMachineEnd_4585B0();
    EXPORT void State_RingRopePullEnd_4557B0();
    EXPORT void State_455800();
    EXPORT void State_FallLedgeBegin_455970();
    EXPORT void jState_FallLedgeBegin_4559A0();
    EXPORT void jState_FallLedgeBegin_4559C0();
    EXPORT void State_4559E0();
    EXPORT void jState_FallLedgeBegin_455A80();
    EXPORT void jState_FallLedgeBegin_455AA0();
    EXPORT void State_LeverUse_455AC0();
    EXPORT void State_455B60();
    EXPORT void State_KnockForward_455420();
    EXPORT void State_455310();
    EXPORT void jState_KnockbackGetUp_455380();
    EXPORT void State_RockThrowStandingHold_455DF0();
    EXPORT void State_RockThrowStandingThrow_456460();
    EXPORT void State_RockThrowStandingEnd_455F20();
    EXPORT void State_RockThrowCrouchingHold_454410();
    EXPORT void State_RockThrowCrouchingThrow_454500();
    EXPORT void State_455550();
    EXPORT void State_455670();
    EXPORT void State_GrabRock_4564A0();
    EXPORT void State_Chant_45B1C0();
    EXPORT void State_ChantEnd_45BBE0();
    EXPORT void State_DoorEnter_459470();
    EXPORT void State_DoorExit_459A40();
    EXPORT void State_MineCarEnter_458780();
    EXPORT void State_4587C0();
    EXPORT void State_MineCarExit_458890();
    EXPORT void State_45A990();
    EXPORT void State_45AB00();
    EXPORT void State_LiftGrabBegin_45A600();
    EXPORT void State_LiftGrabEnd_45A670();
    EXPORT void State_LiftGrabIdle_45A6A0();
    EXPORT void State_LiftUseUp_45A780();
    EXPORT void State_LiftUseDown_45A7B0();
    EXPORT void State_TurnWheelBegin_456700();
    EXPORT void State_TurnWheelLoop_456750();
    EXPORT void State_TurnWheelEnd_4569A0();
    EXPORT void State_PoisonGasDeath_4565C0();

public:
    __int16 field_116;
    int field_118;
    __int16 field_11C;
    __int16 field_11E;
    unsigned __int16 field_120_state;
    __int16 field_122;
    int field_124_gnFrame;
    Abe_1BC_20_sub_object field_128;
    unsigned int field_144;
    int field_148;
    int field_14C;
    int field_150;
    int field_154;
    int field_158;
    int field_15C;
    int field_160;
    int field_164;
    unsigned int field_168;
    __int16 field_16C;
    __int16 field_16E;
    unsigned int field_170;
    __int16 field_174;
    __int16 field_176;
    int field_178;
    int field_17C;
    int field_180;
    int field_184;
    int field_188;
    int field_18C;
    int field_190;
    int field_194;
    __int16 field_198_has_evil_fart;
    __int16 field_19A;
    int field_19C;
    __int16 field_1A0_door_id;
    char field_1A2_rock_or_bone_count;
    char field_1A3_throw_direction;
    int field_1A4;
    int field_1A8;
    __int16 field_1AC_flags;
    __int16 field_1AE;
    int field_1B0_save_num;
    int field_1B4;
    int field_1B8;
};
ALIVE_ASSERT_SIZEOF(Abe, 0x1BC);

ALIVE_VAR_EXTERN(Abe *, sActiveHero_5C1B68);
ALIVE_VAR_EXTERN(Abe *, sControlledCharacter_5C1B8C);

extern int sAbeFrameOffsetTable_554B18[130];
extern const char * sAbeStateNames[];