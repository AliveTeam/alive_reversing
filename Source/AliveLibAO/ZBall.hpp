#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

struct Path_ZBall final : public Path_TLV
{
    enum class StartPos : s16
    {
        eCenter_0 = 0,
        eOut_1 = 1,
        eIn_2 = 2
    };

    enum class Speed : s16
    {
        eNormal_0 = 0,
        eFast_1 = 1,
        eSlow_2 = 2
    };

    StartPos mStartPos;
    Scale_short mScale;
    Speed mSpeed;
    s16 field_1E_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ZBall, 0x20);

class ZBall final : public BaseAnimatedWithPhysicsGameObject
{
public:
    ZBall(relive::Path_ZBall* pTlv, const Guid& tlvId);

    virtual void VUpdate() override;

    Guid mTlvInfo;
    s16 mFrameAbove12 = 0;
    s16 mSoundPitch = 0;
};
ALIVE_ASSERT_SIZEOF(ZBall, 0xEC);

} // namespace AO
