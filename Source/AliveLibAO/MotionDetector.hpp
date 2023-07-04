#pragma once


#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../relive_lib/Primitives.hpp"

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
    virtual void VRender(BasePrimitive** ppOt) override;

    void SetDontComeBack(bool bDontComeBack);

    Guid mTlvId;
    enum class States : s16
    {
        eMoveRight_0 = 0,
        eWaitThenMoveLeft_1 = 1,
        eMoveLeft_2 = 2,
        eWaitThenMoveRight_3 = 3
    };
    States mState = States::eMoveRight_0;
    s32 mPauseTimer = 0;
    u16 mDisableSwitchId = 0;
    s16 mAlarmSwitchId = 0;
    s16 mAlarmDuration = 0;
    bool mDontComeBack = false;
    FP mTopLeftX = {};
    FP mTopLeftY = {};
    FP mBottomRightX = {};
    FP mBottomRightY = {};
    Guid mLaserId;
    Poly_G3 mPrims[2] = {};
    FP mSpeed = {};
    bool mObjectInLaser = false;
};

} // namespace AO
