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

struct Greeter_State
{
    __int16 field_0_type;
    __int16 field_2_r;
    __int16 field_4_g;
    __int16 field_6_b;
    __int16 field_8_path_number;
    __int16 field_A_lvl_number;
    int field_C_xpos;
    int field_10_ypos;
    int field_14_velx;
    int field_18_vely;
    int field_1C_sprite_scale;
    __int16 field_20_current_frame;
    __int16 field_22_frame_change_counter;
    char field_24;
    char field_25;
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
    int field_40_speed;
    __int16 field_44_state;
    __int16 field_46;
    __int16 field_48;
    __int16 field_4A;
    int field_4C;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Greeter_State, 0x50);

struct PathLine;

class Greeter : public BaseAliveGameObject
{
public:
    EXPORT Greeter* ctor_4465B0(Path_Greeter* pTlv, int tlvInfo);

private:
    EXPORT Greeter* vdtor_4468B0(signed int flags);

    EXPORT void vScreenChanged_447DD0();

    EXPORT void dtor_4468E0();

    EXPORT void BlowUp_447E50();

    EXPORT void ChangeDirection_447BD0();

    EXPORT void BounceBackFromShot_447B10();

    EXPORT void HandleRollingAlong_447860();

    EXPORT signed __int16 vTakeDamage_447C20(BaseGameObject* pFrom);
    
    EXPORT void vsub_447DB0(BaseGameObject* pFrom);

    EXPORT void ZapTarget_447320(FP xpos, FP ypos, BaseAliveGameObject* pTarget);

    EXPORT void RandomishSpeak_447A70(__int16 effect);

    EXPORT BOOL ZapIsNotBlocked_447240(BaseAliveGameObject* pUs, BaseAliveGameObject* pThem);

    EXPORT BaseAliveGameObject* GetMudToZap_447690();

private:
    __int16 field_116;
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
    __int16 field_13C_state;
    __int16 field_13E;
    __int16 field_140;
    __int16 field_142;
};
ALIVE_ASSERT_SIZEOF(Greeter, 0x144);
