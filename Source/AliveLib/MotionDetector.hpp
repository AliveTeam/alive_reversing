#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "FunctionFwd.hpp"

struct Path_MotionDetector : public Path_TLV
{
    __int16 field_10_scale;
    __int16 field_12_device_x;
    __int16 field_14_device_y;
    __int16 field_16_speed_x256;
    __int16 field_18_start_on;
    __int16 field_1A_draw_flare;
    __int16 field_1C_disable_id;
    __int16 field_1E_alarm_id;
    __int16 field_20_alarm_ticks;
    __int16 field_22;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_MotionDetector, 0x24);

class MotionDetectorLaser : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT MotionDetectorLaser* ctor_468290(FP xpos, FP ypos, FP scale, __int16 layer);

    virtual BaseGameObject* VDestructor(signed int flags) override;

private:
    EXPORT MotionDetectorLaser* vdtor_468360(signed int flags);
};
ALIVE_ASSERT_SIZEOF(MotionDetectorLaser, 0xF4);


class MotionDetector : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT MotionDetector* ctor_4683B0(Path_MotionDetector* pTlv, int tlvInfo, BaseAnimatedWithPhysicsGameObject* pOwner);
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;
    virtual void VScreenChanged() override;
private:
    EXPORT MotionDetector* vdtor_468850(signed int flags);
    EXPORT void dtor_468880();
    EXPORT void vScreenChanged_469460();
    EXPORT void vRender_469120(int** pOt);
    EXPORT signed __int16 IsInLaser_468980(BaseAliveGameObject* pWho, BaseGameObject* pOwner);
    EXPORT void vUpdate_468A90();

private:
    int field_F4_tlvInfo;
public:
    int field_F8_laser_id;
private:
    int field_FC_owner_id;

    enum class States : __int16
    {
        eState_0_Go_Right = 0,
        eState_1_Wait_Then_Go_Left = 1,
        eState_2_Go_Left = 2,
        eState_3_Wait_Then_Go_Right = 3
    };

    States field_100_state;

    __int16 field_102_pad;
    int field_104_timer;
    __int16 field_108_disable_id;
    __int16 field_10A_alarm_id;
    __int16 field_10C_alarm_ticks;
    __int16 field_10E_bUnknown;
    __int16 field_110_bDontComeBack;
    __int16 field_112_pad;
    FP field_114_x1_fp;
    FP field_118_x2_fp;
    FP field_11C_y1_fp;
    FP field_120_y2_fp;
    Poly_F3 field_124_prims[2];
    Prim_SetTPage field_154_tPage[2];
    FP field_174_speed;
    __int16 field_178_bObjectInLaser;
    __int16 field_17A_pad;
};
ALIVE_ASSERT_SIZEOF(MotionDetector, 0x17C);
