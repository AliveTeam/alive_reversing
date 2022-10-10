#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"
#include "Path.hpp"

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
