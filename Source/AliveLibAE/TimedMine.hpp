#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseGameObject.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"
#include "Factory.hpp"
#include "Animation.hpp"

struct Path_TimedMine final : public Path_TLV
{
    s16 field_10_switch_id;
    s16 field_12_state;
    Scale_short field_14_scale;
    s16 field_16_ticks_before_explosion;
    s32 field_18_disabled_resources;
};
ALIVE_ASSERT_SIZEOF(Path_TimedMine, 0x1C);

enum TimedMine_Flags_1C4
{
    eStickToLiftPoint_0 = 0x1, // TODO: This is never set.
    ePadding_1 = 0x2,
};

class TimedMine final : public BaseAliveGameObject
{
public:
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    
    virtual void VScreenChanged() override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;
    virtual void VOnPickUpOrSlapped() override;
    TimedMine(Path_TimedMine* pPath, TlvItemInfoUnion tlv);
    ~TimedMine();

private:
    void InitBlinkAnimation_4108E0(Animation* pAnimation);
    void StickToLiftPoint_411100();

public:
    u16 field_118_armed;
    u16 field_11A_ticks_before_explosion;
    u32 field_11C_tlv;
    u32 field_120_gnframe;
    Animation field_124_animation;
    u32 field_1BC_gnFrame_2;
    u32 field_1C0_detonation_timer;
    BitField32<TimedMine_Flags_1C4> field_1C4_flags;
};
ALIVE_ASSERT_SIZEOF(TimedMine, 0x1C8);
