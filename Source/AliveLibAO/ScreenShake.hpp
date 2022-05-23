#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Primitives.hpp"

namespace AO {

class ScreenShake final : public BaseGameObject
{
public:
    ScreenShake(bool enableShakeEvent);
    ~ScreenShake();

    virtual void VUpdate() override;

    EXPORT void VUpdate_4625B0();

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_4625E0(PrimHeader** ppOt);


    Prim_ScreenOffset field_10_screenOffset[2];
    s16 field_30_shakeNumber;
    s16 field_32_enableShakeEvent;
};
ALIVE_ASSERT_SIZEOF(ScreenShake, 0x34);

} // namespace AO
