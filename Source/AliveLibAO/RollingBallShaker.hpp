#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "Primitives_common.hpp"

namespace AO {

class RollingBallShaker final : public ::BaseGameObject
{
public:
    RollingBallShaker();
    ~RollingBallShaker();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

    Prim_ScreenOffset mPrimScreenOffset[2] = {};
    s16 mShakeTableIdx = 0;
    bool mStopShaking = false;
};

} // namespace AO
