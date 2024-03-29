#pragma once

#include "BaseGameObject.hpp"
#include "Path.hpp"

enum class WheelSyncerOutputRequirement : s16
{
    eAllOn_0 = 0,
    e1OnAnd2Off_1 = 1,
    e1Or2On_2 = 2,
    e1OnOr2Off_3 = 3
};

struct Path_WheelSyncer final : public Path_TLV
{
    s16 field_10_input_switch_id1;
    s16 field_12_input_switch_id2;
    s16 field_14_output_switch_id;
    WheelSyncerOutputRequirement field_16_output_requirement;
    s16 field_18_input_switch_id3;
    s16 field_1A_input_switch_id4;
    s16 field_1C_input_switch_id5;
    s16 field_1E_input_switch_id6;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_WheelSyncer, 0x20);

class WheelSyncer final : public BaseGameObject
{
public:
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    EXPORT WheelSyncer* ctor_466090(Path_WheelSyncer* pTlv, u32 tlvInfo);

private:
    EXPORT void vUpdate_4661D0();
    EXPORT WheelSyncer* vdtor_466120(s32 flags);
    EXPORT void dtor_466150();
    EXPORT void vScreenChanged_466310();

private:
    s16 field_20_input_switch_id1;
    s16 field_22_input_switch_id2;
    s16 field_24_trigger_id;
    s16 field_26_input_switch_id3;
    s16 field_28_input_switch_id4;
    s16 field_2A_input_switch_id5;
    s16 field_2C_input_switch_id6;
    WheelSyncerOutputRequirement field_2E_output_requirement;
    u32 field_30_tlvInfo;
};
ALIVE_ASSERT_SIZEOF(WheelSyncer, 0x34);
