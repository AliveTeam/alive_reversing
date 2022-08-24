#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"
#include "../AliveLibAE/Path.hpp"
#include "SwitchStates.hpp"

namespace AO {

enum class FootSwitchTriggerBy : s16
{
    eAbe_0 = 0,
    eAnyone_1 = 1,
};

enum class SwitchOp : s16;

struct Path_FootSwitch final : public Path_TLV
{
    s16 mSwitchId;
    Scale_short mScale;
    SwitchOp mAction;
    FootSwitchTriggerBy mTriggeredBy;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_FootSwitch, 0x20);

class FootSwitch final : public BaseAnimatedWithPhysicsGameObject
{
public:
    FootSwitch(relive::Path_FootSwitch* pTlv, s32 tlvInfo);
    ~FootSwitch();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    BaseAliveGameObject* WhoIsStoodOnMe();

    s32 mTlvInfo = 0;
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
