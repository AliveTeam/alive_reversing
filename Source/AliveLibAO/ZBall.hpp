#pragma once

#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"

namespace relive
{
    struct Path_ZBall;
}

namespace AO {

class ZBall final : public BaseAnimatedWithPhysicsGameObject
{
public:
    ZBall(relive::Path_ZBall* pTlv, const Guid& tlvId);

    virtual void VUpdate() override;

    Guid mTlvInfo;
    s16 mFrameAbove12 = 0;
    s16 mSoundPitch = 0;
};

} // namespace AO
