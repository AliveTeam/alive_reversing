#pragma once

#include "BaseAliveGameObject.hpp"
#include "Path.hpp"
#include "../AliveLibCommon/FunctionFwd.hpp"

struct FallingItem_Data final
{
    s32 field_0;
    s32 field_4;
    s16 field_8;
    s16 field_A;
};
ALIVE_ASSERT_SIZEOF(FallingItem_Data, 0xC);

struct Path_FallingItem final : public Path_TLV
{
    s16 field_10_switch_id;
    Scale_short field_12_scale;
    s16 field_14_fall_interval;
    s16 field_16_max_falling_items;
    Choice_short field_18_reset_switch_id_after_use;
    s16 field_20_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_FallingItem, 0x1C);

class FallingItem final : public BaseAliveGameObject
{
public:
    FallingItem(Path_FallingItem* pTlv, s32 tlvInfo);

    FallingItem(s32 xpos, s32 ypos, s32 scale, s32 id, s32 fallInterval, s32 numItems, s32 bResetIdAfterUse);

    ~FallingItem();

    virtual void VUpdate() override;

    virtual void VScreenChanged() override;

    virtual void VOnThrowableHit(BaseGameObject*) override
    {
        // FallingItem__vnullsub_75
    }

private:
    EXPORT void vScreenChanged_428180();

    EXPORT void vUpdate_427780();

    EXPORT void DamageHitItems_427F40();

private:
    s32 field_118_tlvInfo;
    enum class State : s16
    {
        eWaitForIdEnable_0 = 0,
        eGoWaitForDelay_1 = 1,
        eWaitForFallDelay_2 = 2,
        eFalling_3 = 3,
        eSmashed_4 = 4
    };
    State field_11C_state;
    s16 field_11E_switch_id;
    s16 field_120_max_falling_items;
    s16 field_122_remaining_falling_items;
    s16 field_124_fall_interval;
    //s16 field_126_pad;
    s32 field_128_fall_interval_timer;
    Choice_short field_12C_reset_switch_id_after_use;
    s16 field_12E_do_sound_in_state_falling;
    FP field_130_yPosStart;
    s16 field_134_bHitDrillOrMineCar;
    //s16 field_136_pad;
    FP field_138_xpos;
    FP field_13C_ypos;
    s32 field_140_sound_channels;
    s32 field_144_created_gnFrame;
};
ALIVE_ASSERT_SIZEOF(FallingItem, 0x148);
