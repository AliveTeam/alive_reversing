#pragma once


#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../AliveLibCommon/Primitives_common.hpp"

namespace relive
{
    struct Path_MotionDetector;
}

namespace AO {

class MotionDetectorLaser final : public BaseAnimatedWithPhysicsGameObject
{
public:
    MotionDetectorLaser()
        : BaseAnimatedWithPhysicsGameObject(0)
    {
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::MotionDetector_Laser));
    }

    virtual void VScreenChanged() override
    {
        // Empty
    }
};

class MotionDetector final : public BaseAnimatedWithPhysicsGameObject
{
public:
    MotionDetector(relive::Path_MotionDetector* pTlv, const Guid& tlvId);
    ~MotionDetector();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

    void SetDontComeBack(bool bDontComeBack);

    Guid field_E4_tlvInfo;
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
    bool field_F6_bDontComeBack = false;
    FP field_F8_top_left_x = {};
    FP field_FC_top_left_y = {};
    FP field_100_bottom_right_x = {};
    FP field_104_bottom_right_y = {};
    Guid field_F8_laser_id;
    Poly_G3 field_10C_prims[2] = {};
    Prim_SetTPage field_13C_tPage[2] = {};
    FP field_15C_speed = {};
    s16 field_160_bObjectInLaser = 0;
};

} // namespace AO
