#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/Layer.hpp"

struct Path_MotionDetector final : public Path_TLV
{
    Scale_short mScale;
    s16 mDeviceX;
    s16 mDeviceY;
    s16 mSpeedx256;
    enum class InitialMoveDirection : s16
    {
        eRight_0 = 0,
        eLeft_1 = 1
    };
    InitialMoveDirection mInitialMoveDirection;
    Choice_short mDrawFlare;
    s16 mDisableSwitchId;
    s16 mAlarmSwitchId;
    s16 mAlarmDuration;
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
    MotionDetector(relive::Path_MotionDetector* pTlv, const Guid& tlvId, BaseAnimatedWithPhysicsGameObject* pOwner);
    ~MotionDetector();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;

private:
    s16 IsInLaser(BaseAliveGameObject* pWho, BaseGameObject* pOwner);

private:
    Guid field_F4_tlvInfo;

public:
    Guid field_F8_laser_id;

private:
    Guid field_FC_owner_id;

    enum class States : s16
    {
        eMoveRight_0 = 0,
        eWaitThenMoveLeft_1 = 1,
        eMoveLeft_2 = 2,
        eWaitThenMoveRight_3 = 3
    };
    States field_100_state = States::eMoveRight_0;

    s32 field_104_timer = 0;
    s16 field_108_disable_switch_id = 0;
    s16 field_10A_alarm_switch_id = 0;
    s16 field_10C_alarm_duration = 0;
    s16 field_10E_bUnknown = 0;
    s16 field_110_bDontComeBack = 0;
    FP field_114_x1_fp = {};
    FP field_118_x2_fp = {};
    FP field_11C_y1_fp = {};
    FP field_120_y2_fp = {};
    Poly_F3 field_124_prims[2] = {};
    Prim_SetTPage field_154_tPage[2] = {};
    FP field_174_speed = {};
    s16 field_178_bObjectInLaser = 0;
};
ALIVE_ASSERT_SIZEOF(MotionDetector, 0x17C);
