#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/Primitives.hpp"
#include "../relive_lib/BaseGameObject.hpp"

class ScreenShake final : public BaseGameObject
{
public:
    ScreenShake(bool enableShakeEvent, bool softerShakes);
    ~ScreenShake();
    
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

private:
    Prim_ScreenOffset mScreenOffset[2] = {};
    s16 mShakeNumber = 0;
    bool mEnableShakeEvent = false;
    bool mSofterShakes = false;
};
ALIVE_ASSERT_SIZEOF(ScreenShake, 0x4C);
