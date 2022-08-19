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
    FootSwitch(Path_FootSwitch* pTlv, s32 tlvInfo);
    ~FootSwitch();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    BaseAliveGameObject* WhoIsStoodOnMe();

    s32 field_E4_tlvInfo = 0;
    enum class States : s16
    {
        eWaitForStepOnMe_0 = 0,
        eWaitForGetOffMe_1 = 1,
    };
    States field_E8_state = States::eWaitForStepOnMe_0;
    s16 field_EA_switch_id = 0;
    SwitchOp field_EC_action = SwitchOp::eSetTrue_0;
    FootSwitchTriggerBy field_EE_trigger_by = FootSwitchTriggerBy::eAbe_0;
    BaseAliveGameObject* field_F0_pStoodOnMe = nullptr;
};
ALIVE_ASSERT_SIZEOF(FootSwitch, 0xF4);


} // namespace AO
