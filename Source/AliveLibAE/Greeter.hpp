#pragma once

#include "BaseAliveGameObject.hpp"
#include "Path.hpp"
#include "FunctionFwd.hpp"

struct Path_Greeter : public Path_TLV
{
    __int16 field_10_scale;
    __int16 field_12_motion_detector_speed;
    __int16 field_14_direction;
    // pad
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Greeter, 0x18);

enum class GreeterStates : __int16
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
    Types field_0_type;
    __int16 field_2_r;
    __int16 field_4_g;
    __int16 field_6_b;
    __int16 field_8_path_number;
    LevelIds field_A_lvl_number;
    FP field_C_xpos;
    FP field_10_ypos;
    FP field_14_velx;
    FP field_18_vely;
    FP field_1C_sprite_scale;
    __int16 field_20_current_frame;
    __int16 field_22_frame_change_counter;
    char field_24_bAnimRender;
    char field_25_bDrawable;
    char field_26;
    char field_27;
    int field_28_tlvInfo;
    int field_2C;
    int field_30_last_turn_time;
    int field_34_timer;
    __int16 field_38;
    __int16 field_3A;
    __int16 field_3C;
    __int16 field_3E;
    FP  field_40_speed;
    GreeterStates field_44_state;
    __int16 field_46_targetOnLeft;
    __int16 field_48_targetOnRight;
    __int16 field_4A;
    FP field_4C_motion_laser_xpos;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Greeter_State, 0x50);

class PathLine;

enum class GreeterSpeak : __int16
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
    EXPORT Greeter* ctor_4465B0(Path_Greeter* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;
    virtual signed __int16 VTakeDamage_408730(BaseGameObject* pFrom) override;
    virtual int VGetSaveState(BYTE* pSaveBuffer) override;

    EXPORT static int CC CreateFromSaveState_446040(const BYTE* pBuffer);


private:
    EXPORT int vGetSaveState_446400(Greeter_State* pState);


    EXPORT Greeter* vdtor_4468B0(signed int flags);
    EXPORT void vScreenChanged_447DD0();
    EXPORT void dtor_4468E0();
    EXPORT void BlowUp_447E50();
    EXPORT void ChangeDirection_447BD0();
    EXPORT void BounceBackFromShot_447B10();
    EXPORT void HandleRollingAlong_447860();
    EXPORT signed __int16 vTakeDamage_447C20(BaseGameObject* pFrom);
    EXPORT void vOnThrowableHit_447DB0(BaseGameObject* pFrom);
    EXPORT void ZapTarget_447320(FP xpos, FP ypos, BaseAliveGameObject* pTarget);
    EXPORT void RandomishSpeak_447A70(GreeterSpeak effect);
    EXPORT BOOL ZapIsNotBlocked_447240(BaseAliveGameObject* pUs, BaseAliveGameObject* pThem);
    EXPORT BaseAliveGameObject* GetMudToZap_447690();
    EXPORT void vUpdate_4469B0();
private:
    int field_118_tlvInfo;
    int field_11C_motionDetectorId;
    int field_120;
    int field_124_last_turn_time;
    int field_128_timer;
    __int16 field_12C;
    __int16 field_12E;
    __int16 field_130;
    __int16 field_132;
    FP field_134_speed;
    Path_TLV* field_138_pTlv;
public:
    GreeterStates field_13C_state;
    __int16 field_13E_targetOnLeft;
    __int16 field_140_targetOnRight;
private:
    __int16 field_142;
};
ALIVE_ASSERT_SIZEOF(Greeter, 0x144);
