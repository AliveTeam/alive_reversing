#pragma once

#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"

namespace relive
{
    struct Path_LightEffect;
}

namespace AO {

class DoorLight final : public BaseAnimatedWithPhysicsGameObject
{
public:
    DoorLight(relive::Path_LightEffect* pTlv, const Guid& tlvId);
    ~DoorLight();
    
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

public:
    Guid mTlvId;
    s16 mWidth = 0;
    s16 mHeight = 0;
    bool mHasSwitchId = false;
    s32 mSwitchState = 0;
    s16 mSwitchId = 0;
};

} // namespace AO
