#pragma once

#include "BaseAliveGameObject.hpp"
#include "Path.hpp"
#include "FunctionFwd.hpp"

struct Path_Greeter final : public Path_TLV
{
    Scale_short field_10_scale;
    s16 field_12_motion_detector_speed;
    XDirection_short field_14_direction;
    s16 field_16_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Greeter, 0x18);

enum class GreeterStates : s16
{
    eState_0_Patrol,
    eState_1_PatrolTurn,
    eState_2_Speak,
    eState_3_ChaseSpeak,
    eState_4_Chase,
    eState_5_Knockback,
    eState_6_ToChase,
    eState_7_Fall
};

struct Greeter_State
{
    AETypes field_0_type;
    s16 field_2_r;
    s16 field_4_g;
    s16 field_6_b;
    s16 field_8_path_number;
    LevelIds field_A_lvl_number;
    FP field_C_xpos;
    FP field_10_ypos;
    FP field_14_velx;
    FP field_18_vely;
    FP field_1C_sprite_scale;
    s16 field_20_current_frame;
    s16 field_22_frame_change_counter;
    s8 field_24_bAnimRender;
    s8 field_25_bDrawable;
    s8 field_26;
    s8 field_27;
    s32 field_28_tlvInfo;
    s32 field_2C;
    s32 field_30_last_turn_time;
    s32 field_34_timer;
    s16 field_38;
    s16 field_3A;
    s16 field_3C;
    s16 field_3E;
    FP field_40_speed;
    GreeterStates field_44_state;
    s16 field_46_targetOnLeft;
    s16 field_48_targetOnRight;
    s16 field_4A;
    FP field_4C_motion_laser_xpos;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Greeter_State, 0x50);

class PathLine;

enum class GreeterSpeak : s16
{
    Hi_0 = 0,
    HereBoy_1 = 1,
    GetHim_2 = 2,
    Laugh_3 = 3,
    What_9 = 9,
    Randomized_1000 = 1000,
};

class Greeter : public BaseAliveGameObject
{
public:
    EXPORT Greeter* ctor_4465B0(Path_Greeter* pTlv, s32 tlvInfo);

    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;
    virtual s16 VTakeDamage_408730(BaseGameObject* pFrom) override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

    EXPORT static s32 CC CreateFromSaveState_446040(const u8* pBuffer);


private:
    EXPORT s32 vGetSaveState_446400(Greeter_State* pState);


    EXPORT Greeter* vdtor_4468B0(s32 flags);
    EXPORT void vScreenChanged_447DD0();
    EXPORT void dtor_4468E0();
    EXPORT void BlowUp_447E50();
    EXPORT void ChangeDirection_447BD0();
    EXPORT void BounceBackFromShot_447B10();
    EXPORT void HandleRollingAlong_447860();
    EXPORT s16 vTakeDamage_447C20(BaseGameObject* pFrom);
    EXPORT void vOnThrowableHit_447DB0(BaseGameObject* pFrom);
    EXPORT void ZapTarget_447320(FP xpos, FP ypos, BaseAliveGameObject* pTarget);
    EXPORT void RandomishSpeak_447A70(GreeterSpeak effect);
    EXPORT BOOL ZapIsNotBlocked_447240(BaseAliveGameObject* pUs, BaseAliveGameObject* pThem);
    EXPORT BaseAliveGameObject* GetMudToZap_447690();
    EXPORT void vUpdate_4469B0();

private:
    s32 field_118_tlvInfo;
    s32 field_11C_motionDetectorId;
    s32 field_120;
    s32 field_124_last_turn_time;
    s32 field_128_timer;
    s16 field_12C;
    s16 field_12E;
    s16 field_130;
    s16 field_132;
    FP field_134_speed;
    Path_TLV* field_138_pTlv;

public:
    GreeterStates field_13C_state;
    s16 field_13E_targetOnLeft;
    s16 field_140_targetOnRight;

private:
    s16 field_142;
};
ALIVE_ASSERT_SIZEOF(Greeter, 0x144);
