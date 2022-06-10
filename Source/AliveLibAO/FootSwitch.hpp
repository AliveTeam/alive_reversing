#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

enum class FootSwitchTriggerBy : s16
{
    eAbe_0 = 0,
    eAnyone_1 = 1,
};

enum class SwitchOp : s16;

struct Path_FootSwitch final : public Path_TLV
{
    s16 field_18_switch_id;
    Scale_short field_1A_scale;
    SwitchOp field_1C_action;
    FootSwitchTriggerBy field_1E_trigger_by;
};
ALIVE_ASSERT_SIZEOF(Path_FootSwitch, 0x20);

class FootSwitch final : public BaseAnimatedWithPhysicsGameObject
{
public:
    FootSwitch(Path_FootSwitch* pTlv, s32 tlvInfo);
    ~FootSwitch();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    BaseAliveGameObject* WhoIsStoodOnMe();

    s32 field_E4_tlvInfo;
    enum class States : s16
    {
        eWaitForStepOnMe_0 = 0,
        eWaitForGetOffMe_1 = 1,
    };
    States field_E8_state;
    s16 field_EA_switch_id;
    SwitchOp field_EC_action;
    FootSwitchTriggerBy field_EE_trigger_by;
    BaseAliveGameObject* field_F0_pStoodOnMe;
};
ALIVE_ASSERT_SIZEOF(FootSwitch, 0xF4);


} // namespace AO
