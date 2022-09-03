#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "../relive_lib/Primitives.hpp"

namespace AO {

class ScreenShake final : public ::BaseGameObject
{
public:
    ScreenShake(bool enableShakeEvent);
    ~ScreenShake();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

    Prim_ScreenOffset mScreenOffset[2] = {};
    s16 mShakeNumber = 0;
    bool mEnableShakeEvent = false;
};
ALIVE_ASSERT_SIZEOF(ScreenShake, 0x34);

} // namespace AO
