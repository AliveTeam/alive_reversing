#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "Layer.hpp"

struct Path_MotionDetector final : public Path_TLV
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
    s16 field_1C_disable_switch_id;
    s16 field_1E_alarm_switch_id;
    s16 field_20_alarm_duration;
    s16 field_22_padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_MotionDetector, 0x24);

class MotionDetectorLaser final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    MotionDetectorLaser(FP xpos, FP ypos, FP scale, Layer layer);

};
ALIVE_ASSERT_SIZEOF(MotionDetectorLaser, 0xF4);


class MotionDetector final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    MotionDetector(Path_MotionDetector* pTlv, s32 tlvInfo, BaseAnimatedWithPhysicsGameObject* pOwner);
    ~MotionDetector();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;

private:
    void vScreenChanged_469460();
    void vRender_469120(PrimHeader** ppOt);
    s16 IsInLaser_468980(BaseAliveGameObject* pWho, BaseGameObject* pOwner);
    void vUpdate_468A90();

private:
    s32 field_F4_tlvInfo;

public:
    s32 field_F8_laser_id;

private:
    s32 field_FC_owner_id;

    enum class States : s16
    {
        eMoveRight_0 = 0,
        eWaitThenMoveLeft_1 = 1,
        eMoveLeft_2 = 2,
        eWaitThenMoveRight_3 = 3
    };
    States field_100_state;

    s16 field_102_pad;
    s32 field_104_timer;
    s16 field_108_disable_switch_id;
    s16 field_10A_alarm_switch_id;
    s16 field_10C_alarm_duration;
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
