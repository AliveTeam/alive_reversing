#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "FunctionFwd.hpp"

enum class SwitchOp : s16;

enum class FootSwitchTriggerBy : s16
{
    eOnlyAbe_0 = 0,
    eAnyone_1 = 1,
};

struct Path_FootSwitch final : public Path_TLV
{
    s16 field_10_id;
    Scale_short field_12_scale;
    SwitchOp field_14_action;
    FootSwitchTriggerBy field_16_trigger_by;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_FootSwitch, 0x18);

struct FootSwitch_Data
{
    s32 field_0_frameTableOffset;
    s32 field_4_frameTableOffset;
    s16 field_8_maxH;
    s16 field_A_frameTableOffset;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(FootSwitch_Data, 0xC);


class FootSwitch : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT FootSwitch* ctor_4DE090(Path_FootSwitch* pTlv, s32 tlvInfo);

    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    EXPORT FootSwitch* vdtor_4DE240(s32 flags);
    EXPORT void dtor_4DE670();
    EXPORT void vScreenChanged_4DE650();
    EXPORT void vUpdate_4DE270();
    EXPORT BaseAliveGameObject* WhoIsStoodOnMe_4DE700();

private:
    s32 field_F4_tlvInfo;
    enum class States : s16
    {
        eWaitForStepOnMe_0 = 0,
        eWaitForGetOffMe_1 = 1,
    };
    States field_F8_state;
    s16 field_FA_id;
    SwitchOp field_FC_action;
    FootSwitchTriggerBy field_FE_trigger_by;
    s32 field_100_obj_id;
    s16 field_104_bUnknown;
    s16 field_106_bFindStander;
};
ALIVE_ASSERT_SIZEOF(FootSwitch, 0x108);
