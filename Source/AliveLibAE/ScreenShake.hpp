#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Primitives.hpp"
#include "../relive_lib/BaseGameObject.hpp"

class ScreenShake final : public BaseGameObject
{
public:
    ScreenShake(bool enableShakeEvent, bool softerShakes);
    ~ScreenShake();
    
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

private:
    Prim_ScreenOffset field_20_screenOffset[2];
    s16 field_40_shakeNumber;
    s16 field_42_enableShakeEvent;
    s16 field_44_softerShakes;
};
ALIVE_ASSERT_SIZEOF(ScreenShake, 0x4C);
