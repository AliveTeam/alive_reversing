#pragma once

#include "BaseAliveGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

struct FallingItem_Data final
{
    s32 field_0;
    s32 field_4;
    s16 field_8;
    s16 field_A;
};
ALIVE_ASSERT_SIZEOF(FallingItem_Data, 0xC);

class FallingItem final : public BaseAliveGameObject
{
public:
    FallingItem(relive::Path_FallingItem* pTlv, const Guid& tlvId);
    FallingItem(s32 xpos, s32 ypos, s32 scale, s32 id, s32 fallInterval, s32 numItems, s32 bResetIdAfterUse);
    ~FallingItem();

    void LoadAnimations();
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VOnThrowableHit(BaseGameObject*) override
    {
        // FallingItem__vnullsub_75
    }

private:
    void DamageHitItems();

private:
    Guid field_118_tlvInfo;
    enum class State : s16
    {
        eWaitForIdEnable_0 = 0,
        eGoWaitForDelay_1 = 1,
        eWaitForFallDelay_2 = 2,
        eFalling_3 = 3,
        eSmashed_4 = 4
    };
    State field_11C_state = State::eWaitForIdEnable_0;
    s16 field_11E_switch_id = 0;
    s16 field_120_max_falling_items = 0;
    s16 field_122_remaining_falling_items = 0;
    s16 field_124_fall_interval = 0;
    s32 field_128_fall_interval_timer = 0;
    relive::reliveChoice field_12C_reset_switch_id_after_use = relive::reliveChoice::eNo;
    s16 field_12E_do_sound_in_state_falling = 0;
    FP field_130_yPosStart = {};
    s16 field_134_bHitDrillOrMineCar = 0;
    FP field_138_xpos = {};
    FP field_13C_ypos = {};
    s32 field_140_sound_channels = 0;
    s32 field_144_created_gnFrame = 0;
};
ALIVE_ASSERT_SIZEOF(FallingItem, 0x148);
