#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

struct Path_Mine final : public Path_TLV
{
    s16 field_18_num_patterns; // unused
    s16 field_1A_pattern; // unused
    Scale_short mScale;
    s16 mDisabledResources;
    Choice_short mPersistOffscreen;
    s16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Mine, 0x24);

class Mine final : public BaseAliveGameObject
{
public:
    Mine(relive::Path_Mine* pTlv, const Guid& tlvId);
    ~Mine();

    virtual void VScreenChanged() override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;
    virtual void VOnPickUpOrSlapped() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VUpdate() override;

    s16 IsColliding();

    u16 field_10C_detonating = 0;
    s16 field_10E_disabled_resources = 0;
    Guid field_110_tlv;
    s32 field_114_gnframe = 0;
    Animation field_118_animation;
    s32 field_1B0_flags = 0;
};
ALIVE_ASSERT_SIZEOF(Mine, 0x1B4);

} // namespace AO
