#pragma once

#include "BaseGameObject.hpp"
#include "Path.hpp"

enum class WheelSyncerAction : s16
{
    eAllOn_0 = 0,
    eOff_1 = 1,
    eToggle_2 = 2,
    eOn_3 = 3
};

struct Path_WheelSyncer final : public Path_TLV
{
    s16 field_10_id1;
    s16 field_12_id2;
    s16 field_14_trigger_id;
    WheelSyncerAction field_16_action;
    s16 field_18_id3;
    s16 field_1A_id4;
    s16 field_1C_id5;
    s16 field_1E_id6;
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
    s16 field_20_id1;
    s16 field_22_id2;
    s16 field_24_trigger_id;
    s16 field_26_id3;
    s16 field_28_id4;
    s16 field_2A_id5;
    s16 field_2C_id6;
    WheelSyncerAction field_2E_action;
    u32 field_30_tlvInfo;
};
ALIVE_ASSERT_SIZEOF(WheelSyncer, 0x34);
