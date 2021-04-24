#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"
#include "Path.hpp"

namespace AO {

enum class FootSwitchTriggerBy : s16
{
    eOnlyAbe_0 = 0,
    eAnyone_1 = 1,
};

enum class SwitchOp : s16;

struct Path_FootSwitch : public Path_TLV
{
    s16 field_18_id;
    Scale_short field_1A_scale;
    SwitchOp field_1C_action;
    FootSwitchTriggerBy field_1E_trigger_by;
};
ALIVE_ASSERT_SIZEOF(Path_FootSwitch, 0x20);

class FootSwitch : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT BaseAliveGameObject* WhoIsStoodOnMe_488A60();

    virtual void VUpdate() override;

    EXPORT void VUpdate_4888E0();

    EXPORT FootSwitch* ctor_4887F0(Path_FootSwitch* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT FootSwitch* vdtor_488C10(s32 flags);

    EXPORT BaseGameObject* dtor_4889E0();

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_4889D0();

    int field_D4[4];
    int field_E4_tlvInfo;
    enum class States : s16
    {
        eWaitForStepOnMe_0 = 0,
        eWaitForGetOffMe_1 = 1,
    };
    States field_E8_state;
    s16 field_EA_id;
    SwitchOp field_EC_action;
    FootSwitchTriggerBy field_EE_trigger_by;
    BaseAliveGameObject* field_F0_pStoodOnMe;
};
ALIVE_ASSERT_SIZEOF(FootSwitch, 0xF4);


}
