#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "Path.hpp"

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

    void LoadAnimations();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    BellHammerStates mState = BellHammerStates::eWaitForActivation_0;
    s16 mSwitchId = 0;
    Guid mTlvInfo;
    bool mSpawnElum = false;
};


} // namespace AO
