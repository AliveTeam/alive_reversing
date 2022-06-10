#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

struct Path_TimedMine final : public Path_TLV
{
    s16 field_18_switch_id;
    s16 field_1A_state;
    Scale_short field_1C_scale;
    s16 field_1E_ticks_before_explosion;
    s16 field_20_disable_resources;
    s16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_TimedMine, 0x24);

enum TimedMine_Flags_1B8
{
    eStickToLiftPoint_0 = 0x1
};

class TimedMine final : public BaseAliveGameObject
{
public:
    TimedMine(Path_TimedMine* pTlv, s32 tlvInfo);
    ~TimedMine();
    
    virtual void VScreenChanged() override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VUpdate() override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;
    virtual void VOnPickUpOrSlapped() override;

    void StickToLiftPoint();
public:
    s16 field_10C_armed;
    u16 field_10E_ticks_before_explosion;
    s32 field_110_tlvInfo;
    s32 field_114_timer;
    Animation field_118_anim;
    s32 field_1B0_gnFrame_2;
    s32 field_1B4_detonation_timer;
    BitField16<TimedMine_Flags_1B8> field_1B8_flags;
};
ALIVE_ASSERT_SIZEOF(TimedMine, 0x1BC);

} // namespace AO
