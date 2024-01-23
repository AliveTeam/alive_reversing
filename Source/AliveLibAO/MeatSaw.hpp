#pragma once

#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"

namespace relive
{
    struct Path_MeatSaw;
}

namespace AO {

enum class MeatSawStates : s16
{
    eIdle_0 = 0,
    eGoingDown_1 = 1,
    eGoingUp_2 = 2
};

class MeatSaw final : public BaseAnimatedWithPhysicsGameObject
{
public:
    MeatSaw(relive::Path_MeatSaw* pTlv, const Guid& tlvId);
    ~MeatSaw();


    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(OrderingTable& ot) override;

private:
    void LoadAnimations();
    void GrindUpObjects();

    MeatSawStates mState = MeatSawStates::eIdle_0;
    u16 mYTravelDistance = 0;
    s16 mCurrentSpeed = 0;
    s16 mSwitchSpeed = 0;
    s16 mAutomaticSpeed = 0;
    u16 mSwitchId = 0;
    s32 field_F0_switch_value = 0;
    s32 field_F2_switch_value = 0;
    s16 mRenderYOffset = 0;
    s16 mSwitchMinTimeOff = 0;
    s16 mSwitchMaxTimeOff = 0;
    s16 mAutomaticMinTimeOff = 0;
    s16 mAutomaticMaxTimeOff = 0;
    Guid mTlvId;
    s32 mIdleTimer = 0;
    s32 mSfxTimer = 0;
    s32 mFrameCountForSfx = 0;
    Animation mMotorAnim;
    bool mResetOffscreen = false;
    bool mUsesSwitchId = false;
    bool mAutomaticMeatSawIsDown = false;
};


} // namespace AO
