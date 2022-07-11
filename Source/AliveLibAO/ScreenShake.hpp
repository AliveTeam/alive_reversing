#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "../relive_lib/Primitives.hpp"

namespace AO {

class ScreenShake final : public BaseGameObject
{
public:
    ScreenShake(bool enableShakeEvent);
    ~ScreenShake();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

    Prim_ScreenOffset field_10_screenOffset[2] = {};
    s16 field_30_shakeNumber = 0;
    s16 field_32_enableShakeEvent = 0;
};
ALIVE_ASSERT_SIZEOF(ScreenShake, 0x34);

} // namespace AO
