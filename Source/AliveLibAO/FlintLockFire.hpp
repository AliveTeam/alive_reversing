#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

struct Path_FlintLockFire final : public Path_TLV
{
    Scale_short field_18_scale;
    s16 field_1A_switch_id;
};
ALIVE_ASSERT_SIZEOF(Path_FlintLockFire, 0x1C);

class FlintLockFire final : public BaseAnimatedWithPhysicsGameObject
{
public:
    FlintLockFire(Path_FlintLockFire* pTlv, s32 tlvInfo);
    ~FlintLockFire();

    virtual void VStopAudio() override;
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

    s32 field_D4_padding[4];
    enum class States : s16
    {
        eDisabled_0 = 0,
        eActivating_1 = 1,
        eActivated_2 = 2
    };
    States field_E4_state;
    s16 field_E6_switch_id;
    s32 field_E8_tlvInfo;
    s32 field_EC_fire_sound;
    Animation field_F0_anim;
    Animation field_188_anim;
    Animation field_220_anim;
};
ALIVE_ASSERT_SIZEOF(FlintLockFire, 0x2B8);

} // namespace AO
