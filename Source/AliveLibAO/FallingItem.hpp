#pragma once

#include "../AliveLibCommon/Function.hpp"
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
    s16 mSwitchId;
    Scale_short mScale;
    s16 mFallInterval;
    s16 mMaxFallingItems;
    Choice_short mResetSwitchIdAfterUse;
    s16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_FallingItem, 0x24);

class FallingItem final : public BaseAliveGameObject
{
public:
    FallingItem(relive::Path_FallingItem* pTlv, s32 tlvInfo);
    ~FallingItem();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;

    void DamageHitItems();

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
    s32 field_10C_tlvInfo = 0;
    State field_110_state = State::eWaitForIdEnable_0;
    u16 field_112_switch_id = 0;
    s16 mMaxFallingItems = 0;
    s16 mRemainingFallingItems = 0;
    s16 mFallInterval = 0;
    s32 field_11C_delay_timer = 0;
    relive::reliveChoice mResetSwitchIdAfterUse = relive::reliveChoice::eNo;
    s16 field_122_do_sound_in_state_falling = 0;
    FP field_124_yPosStart = {};
    FP field_128_xpos = {};
    FP field_12C_ypos = {};
    s32 field_130_sound_channels = 0;
    s32 field_134_created_gnFrame = 0;
};
ALIVE_ASSERT_SIZEOF(FallingItem, 0x138);

} // namespace AO
