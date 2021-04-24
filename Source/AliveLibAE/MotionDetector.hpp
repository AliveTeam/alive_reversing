#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "FunctionFwd.hpp"
#include "Layer.hpp"

struct Path_MotionDetector : public Path_TLV
{
    Scale_short field_10_scale;
    s16 field_12_device_x;
    s16 field_14_device_y;
    s16 field_16_speed_x256;
    enum class InitialMoveDirection : s16
    {
        eRight_0 = 0,
        eLeft_1 = 1
    };
    InitialMoveDirection field_18_initial_move_direction;
    Choice_short field_1A_draw_flare;
    s16 field_1C_disable_id;
    s16 field_1E_alarm_id;
    s16 field_20_alarm_duration;
    s16 field_22_padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_MotionDetector, 0x24);

class MotionDetectorLaser : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT MotionDetectorLaser* ctor_468290(FP xpos, FP ypos, FP scale, Layer layer);

    virtual BaseGameObject* VDestructor(s32 flags) override;

private:
    EXPORT MotionDetectorLaser* vdtor_468360(s32 flags);
};
ALIVE_ASSERT_SIZEOF(MotionDetectorLaser, 0xF4);


class MotionDetector : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT MotionDetector* ctor_4683B0(Path_MotionDetector* pTlv, int tlvInfo, BaseAnimatedWithPhysicsGameObject* pOwner);
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
private:
    EXPORT MotionDetector* vdtor_468850(s32 flags);
    EXPORT void dtor_468880();
    EXPORT void vScreenChanged_469460();
    EXPORT void vRender_469120(PrimHeader** ppOt);
    EXPORT s16 IsInLaser_468980(BaseAliveGameObject* pWho, BaseGameObject* pOwner);
    EXPORT void vUpdate_468A90();

private:
    int field_F4_tlvInfo;
public:
    int field_F8_laser_id;
private:
    int field_FC_owner_id;

    enum class States : s16
    {
        eMoveRight_0 = 0,
        eWaitThenMoveLeft_1 = 1,
        eMoveLeft_2 = 2,
        eWaitThenMoveRight_3 = 3
    };
    States field_100_state;

    s16 field_102_pad;
    int field_104_timer;
    s16 field_108_disable_id;
    s16 field_10A_alarm_id;
    s16 field_10C_alarm_ticks;
    s16 field_10E_bUnknown;
    s16 field_110_bDontComeBack;
    s16 field_112_pad;
    FP field_114_x1_fp;
    FP field_118_x2_fp;
    FP field_11C_y1_fp;
    FP field_120_y2_fp;
    Poly_F3 field_124_prims[2];
    Prim_SetTPage field_154_tPage[2];
    FP field_174_speed;
    s16 field_178_bObjectInLaser;
    s16 field_17A_pad;
};
ALIVE_ASSERT_SIZEOF(MotionDetector, 0x17C);
