#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Map.hpp"
#include "../AliveLibAE/Path.hpp"
#include "../AliveLibCommon/AnimResources.hpp"

namespace AO {

struct FallingItem_Data final
{
    AnimId field_0_falling_animId;
    AnimId field_4_waiting_animId;
    s16 field_8_maxW;
    s16 field_A_maxH;
};
ALIVE_ASSERT_SIZEOF(FallingItem_Data, 0xC);

struct Path_FallingItem final : public Path_TLV
{
    s16 field_18_switch_id;
    Scale_short field_1A_scale;
    s16 field_1C_fall_interval;
    s16 field_1E_max_falling_items;
    Choice_short field_20_reset_switch_id_after_use;
    s16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF(Path_FallingItem, 0x24);

class FallingItem final : public BaseAliveGameObject
{
public:
    EXPORT void DamageHitItems_41A6D0();

    FallingItem(Path_FallingItem* pTlv, s32 tlvInfo);
    ~FallingItem();

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_41A7C0();

    virtual void VUpdate() override;

    EXPORT void VUpdate_41A120();

    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;

private:
    enum class State : s16
    {
        eWaitForIdEnable_0 = 0,
        eGoWaitForDelay_1 = 1,
        eWaitForFallDelay_2 = 2,
        eFalling_3 = 3,
        eSmashed_4 = 4
    };

public:
    s32 field_10C_tlvInfo;
    State field_110_state;
    u16 field_112_switch_id;
    s16 field_114_max_falling_items;
    s16 field_116_remaining_falling_items;
    s16 field_118_fall_interval;
    s16 field_11A_padding;
    s32 field_11C_delay_timer;
    Choice_short field_120_reset_switch_id_after_use;
    s16 field_122_do_sound_in_state_falling;
    FP field_124_yPosStart;
    FP field_128_xpos;
    FP field_12C_ypos;
    s32 field_130_sound_channels;
    s32 field_134_created_gnFrame;
};
ALIVE_ASSERT_SIZEOF(FallingItem, 0x138);

} // namespace AO
