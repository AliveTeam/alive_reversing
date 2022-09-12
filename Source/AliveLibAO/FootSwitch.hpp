#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"
#include "../AliveLibAE/Path.hpp"
#include "SwitchStates.hpp"

namespace AO {

class FootSwitch final : public BaseAnimatedWithPhysicsGameObject
{
public:
    FootSwitch(relive::Path_FootSwitch* pTlv, const Guid& tlvId);
    ~FootSwitch();

    void LoadAnimations();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    BaseAliveGameObject* WhoIsStoodOnMe();

    Guid mTlvInfo;
    enum class States : s16
    {
        eWaitForStepOnMe_0 = 0,
        eWaitForGetOffMe_1 = 1,
    };
    States mState = States::eWaitForStepOnMe_0;
    s16 mSwitchId = 0;
    relive::reliveSwitchOp mAction = relive::reliveSwitchOp::eSetTrue;
    relive::Path_FootSwitch::FootSwitchTriggerBy mTriggeredBy = relive::Path_FootSwitch::FootSwitchTriggerBy::eAbe;
    BaseAliveGameObject* mStoodOnMe = nullptr;
};
ALIVE_ASSERT_SIZEOF(FootSwitch, 0xF4);


} // namespace AO
