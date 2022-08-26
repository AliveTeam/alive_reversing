#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "../relive_lib/Primitives.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

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
    s16 field_2A_padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_MotionDetector, 0x2C);

class MotionDetectorLaser final : public BaseAnimatedWithPhysicsGameObject
{
public:
    virtual void VScreenChanged() override
    {
        // Empty
    }

    


    s32 field_D4[4];
};
ALIVE_ASSERT_SIZEOF(MotionDetectorLaser, 0xE4);

class MotionDetector final : public BaseAnimatedWithPhysicsGameObject
{
public:
    MotionDetector(relive::Path_MotionDetector* pTlv, const TLVUniqueId& tlvId);
    ~MotionDetector();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

    void SetDontComeBack(s16 bDontComeBack);

    TLVUniqueId field_E4_tlvInfo = {};
    enum class States : s16
    {
        eMoveRight_0 = 0,
        eWaitThenMoveLeft_1 = 1,
        eMoveLeft_2 = 2,
        eWaitThenMoveRight_3 = 3
    };
    States field_E8_state = States::eMoveRight_0;
    s32 field_EC_timer = 0;
    u16 field_F0_disable_switch_id = 0;
    s16 field_F2_alarm_switch_id = 0;
    s16 field_F4_alarm_duration = 0;
    s16 field_F6_bDontComeBack = 0;
    FP field_F8_top_left_x = {};
    FP field_FC_top_left_y = {};
    FP field_100_bottom_right_x = {};
    FP field_104_bottom_right_y = {};
    MotionDetectorLaser* field_108_pLaser = nullptr;
    Poly_F3 field_10C_prims[2] = {};
    Prim_SetTPage field_13C_tPage[2] = {};
    FP field_15C_speed = {};
    s16 field_160_bObjectInLaser = 0;
};
ALIVE_ASSERT_SIZEOF(MotionDetector, 0x164);

} // namespace AO
