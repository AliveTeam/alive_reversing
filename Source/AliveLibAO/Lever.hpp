#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "ScopedSeq.hpp"
#include "../AliveLibAE/Path.hpp"
#include "SwitchStates.hpp"

namespace AO {

enum class LeverState : s16
{
    eWaiting_0 = 0,
    ePulled_1 = 1,
    eFinished_2 = 2
};

enum class LeverSoundType : s16
{
    eNone = 0,
    eWell_1 = 1,
    eSwitchBellHammer_2 = 2,
    eDoor_3 = 3,
    eElectricWall_4 = 4,
    eSecurityOrb_5 = 5
};

enum class LeverSoundDirection : s16
{
    eLeftAndRight_0 = 0,
    eLeft_1 = 1,
    eRight_2 = 2,
};

enum class SwitchOp : s16;

struct Path_Lever final : public Path_TLV
{
    s16 mSwitchId;
    SwitchOp mAction;
    Scale_short mScale;
    LeverSoundType mOnSound;
    LeverSoundType mOffSound;
    LeverSoundDirection mSoundDirection;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Lever, 0x24);


class Lever final : public BaseAnimatedWithPhysicsGameObject
{
public:
    Lever(relive::Path_Lever* pTlv, const Guid& tlvId);
    ~Lever();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    // New virtual
    virtual s32 VPull(s16 bLeftDirection);

    s16 mSwitchId = 0;
    LeverState mState = LeverState::eWaiting_0;
    Guid mTlvInfo;
    s16 field_F0_bPulledFromLeft = 0;
    relive::reliveSwitchOp mAction = relive::reliveSwitchOp::eSetTrue;
    relive::Path_Lever::LeverSoundType mOnSound = relive::Path_Lever::LeverSoundType::eNone;
    relive::Path_Lever::LeverSoundType mOffSound = relive::Path_Lever::LeverSoundType::eNone;
    relive::Path_Lever::LeverSoundDirection mSoundDirection = relive::Path_Lever::LeverSoundDirection::eLeftAndRight;
};
ALIVE_ASSERT_SIZEOF(Lever, 0xFC);

} // namespace AO
