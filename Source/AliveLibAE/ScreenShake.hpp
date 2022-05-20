#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "Primitives.hpp"
#include "BaseGameObject.hpp"

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

    s16 field_46_padding;
    s32 field_48_padding;
};
ALIVE_ASSERT_SIZEOF(ScreenShake, 0x4C);
