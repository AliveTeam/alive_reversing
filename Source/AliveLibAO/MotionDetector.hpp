#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "../relive_lib/Primitives.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

struct Path_MotionDetector final : public Path_TLV
{
    Scale_short field_18_scale;
    s16 field_1A_device_x;
    s16 field_1C_device_y;
    s16 field_1E_speed_x256;
    enum class InitialMoveDirection : s16
    {
        eRight_0 = 0,
        eLeft_1 = 1
    };
    InitialMoveDirection field_20_initial_move_direction;
    Choice_short field_22_draw_flare;
    s16 field_24_disable_switch_id;
    s16 field_26_alarm_switch_id;
    s16 field_28_alarm_duration;
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
    MotionDetector(Path_MotionDetector* pTlv, s32 tlvInfo);
    ~MotionDetector();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

    void SetDontComeBack(s16 bDontComeBack);

    s32 field_E4_tlvInfo = 0;
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
