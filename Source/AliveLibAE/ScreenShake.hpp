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
    Prim_ScreenOffset field_20_screenOffset[2] = {};
    s16 field_40_shakeNumber = 0;
    s16 field_42_enableShakeEvent = 0;
    s16 field_44_softerShakes = 0;
};
ALIVE_ASSERT_SIZEOF(ScreenShake, 0x4C);
