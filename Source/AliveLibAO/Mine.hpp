#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

struct Path_Mine final : public Path_TLV
{
    s16 field_18_num_patterns;
    s16 field_1A_pattern;
    Scale_short field_1C_scale;
    s16 field_1E_disabled_resources;
    Choice_short field_20_persists_offscreen;
    s16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF(Path_Mine, 0x24);

class Mine final : public BaseAliveGameObject
{
public:
    Mine(Path_Mine* pTlv, s32 tlvInfo);
    ~Mine();

    virtual void VScreenChanged() override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;
    virtual void VOnPickUpOrSlapped() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VUpdate() override;

    s16 IsColliding();

    u16 field_10C_detonating;
    s16 field_10E_disabled_resources;
    s32 field_110_tlv;
    s32 field_114_gnframe;
    Animation field_118_animation;
    s32 field_1B0_flags;
};
ALIVE_ASSERT_SIZEOF(Mine, 0x1B4);

} // namespace AO
