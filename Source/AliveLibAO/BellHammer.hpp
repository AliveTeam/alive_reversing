#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "SwitchStates.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

struct Path_BellHammer final : public Path_TLV
{
    s16 mSwitchId;
    SwitchOp mAction;
    Scale_short mScale;
    XDirection_short mDirection;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_BellHammer, 0x20);

enum class BellHammerStates : u16
{
    eWaitForActivation_0 = 0,
    eSmashingBell_1 = 1
};

class BellHammer final : public BaseAnimatedWithPhysicsGameObject
{
public:
    BellHammer(relive::Path_BellHammer* pTlv, s32 tlvInfo);
    ~BellHammer();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    static void OnResLoaded(BellHammer* pThis);

    BellHammerStates mState = BellHammerStates::eWaitForActivation_0;
    s16 mSwitchId = 0;
    s32 mTlvInfo = 0;
    s32 mPendingResourceCount = 0;
    bool mSpawnElum = false;
};
ALIVE_ASSERT_SIZEOF(BellHammer, 0xF4);


} // namespace AO
