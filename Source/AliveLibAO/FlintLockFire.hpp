#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"

namespace AO {

class FlintLockFire final : public BaseAnimatedWithPhysicsGameObject
{
public:
    FlintLockFire(relive::Path_FlintLockFire* pTlv, const Guid& tlvId);
    ~FlintLockFire();

    void LoadAnimations();

    virtual void VStopAudio() override;
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

    enum class States : s16
    {
        eDisabled_0 = 0,
        eActivating_1 = 1,
        eActivated_2 = 2
    };
    States field_E4_state = States::eDisabled_0;
    s16 field_E6_switch_id = 0;
    Guid field_E8_tlvInfo;
    s32 field_EC_fire_sound = 0;
    Animation field_F0_anim;
    Animation field_188_anim;
    Animation field_220_anim;
};
ALIVE_ASSERT_SIZEOF(FlintLockFire, 0x2B8);

} // namespace AO
