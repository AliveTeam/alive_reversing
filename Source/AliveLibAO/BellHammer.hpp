#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "SwitchStates.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

enum class BellHammerStates : u16
{
    eWaitForActivation_0 = 0,
    eSmashingBell_1 = 1
};

class BellHammer final : public BaseAnimatedWithPhysicsGameObject
{
public:
    BellHammer(relive::Path_BellHammer* pTlv, const Guid& tlvId);
    ~BellHammer();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    static void OnResLoaded(BellHammer* pThis);

    BellHammerStates mState = BellHammerStates::eWaitForActivation_0;
    s16 mSwitchId = 0;
    Guid mTlvInfo;
    s32 mPendingResourceCount = 0;
    bool mSpawnElum = false;
};
ALIVE_ASSERT_SIZEOF(BellHammer, 0xF4);


} // namespace AO
