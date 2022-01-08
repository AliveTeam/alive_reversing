#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

struct Path_TimedMine final : public Path_TLV
{
    s16 field_18_switch_id;
    s16 field_1A_state;
    Scale_short field_1C_scale;
    s16 field_1E_ticks_before_explode;
    s16 field_20_disable_resources;
    s16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF(Path_TimedMine, 0x24);

enum TimedMine_Flags_1B8
{
    eStickToLiftPoint_0 = 0x1
};

class TimedMine final : public BaseAliveGameObject
{
public:
    EXPORT TimedMine* ctor_4083F0(Path_TimedMine* pTlv, s32 tlvInfo);
    EXPORT BaseGameObject* dtor_408690();
    EXPORT void StickToLiftPoint_408CA0();

    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VScreenChanged() override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VUpdate() override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;
    virtual void VOnPickUpOrSlapped() override;

private:
    EXPORT void VOnThrowableHit_408B10(BaseGameObject* pFrom);
    EXPORT void vOnPickUpOrSlapped_408A80();
    EXPORT void VUpdate_408760();
    EXPORT s16 VTakeDamage_408B90(BaseGameObject* pFrom);
    EXPORT void VScreenChanged_408DD0();
    EXPORT TimedMine* Vdtor_408E10(s32 flags);
    EXPORT void VRender_408960(PrimHeader** ppOt);

public:
    s16 field_10C_armed;
    u16 field_10E_explode_timeout;
    s32 field_110_tlvInfo;
    s32 field_114_timer;
    Animation field_118_anim;
    s32 field_1B0_gnFrame_2;
    s32 field_1B4_detonation_timer;
    BitField16<TimedMine_Flags_1B8> field_1B8_flags;
    s8 field_1BA_padding;
    s8 field_1BB_padding;
};
ALIVE_ASSERT_SIZEOF(TimedMine, 0x1BC);

} // namespace AO
