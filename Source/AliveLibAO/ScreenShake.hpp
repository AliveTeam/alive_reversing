#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "../relive_lib/Primitives.hpp"

namespace AO {

class ScreenShake final : public BaseGameObject
{
public:
    ScreenShake(bool enableShakeEvent);
    ~ScreenShake();

    virtual void VUpdate() override;
    virtual void VRender(BasePrimitive** ppOt) override;

private:
    Prim_ScreenOffset mScreenOffset[2] = {};
    s16 mShakeNumber = 0;
    bool mEnableShakeEvent = false;
};

} // namespace AO
