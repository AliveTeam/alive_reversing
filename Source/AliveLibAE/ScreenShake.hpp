#pragma once

#include "../relive_lib/Primitives.hpp"
#include "../relive_lib/GameObjects/BaseGameObject.hpp"

class ScreenShake final : public BaseGameObject
{
public:
    ScreenShake(bool enableShakeEvent, bool softerShakes);
    ~ScreenShake();

    virtual void VUpdate() override;
    virtual void VRender(OrderingTable& ot) override;

private:
    Prim_ScreenOffset mScreenOffset = {};
    s16 mShakeNumber = 0;
    bool mEnableShakeEvent = false;
    bool mSofterShakes = false;
};
