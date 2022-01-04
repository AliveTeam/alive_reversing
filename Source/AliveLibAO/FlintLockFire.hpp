#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
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
    EXPORT FlintLockFire* ctor_41AA90(Path_FlintLockFire* pTlv, s32 tlvInfo);

    EXPORT BaseGameObject* dtor_41AE20();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT FlintLockFire* Vdtor_41B500(s32 flags);

    virtual void VStopAudio() override;

    EXPORT void VStopAudio_41B0C0();

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_41B0B0();

    virtual void VUpdate() override;

    EXPORT void VUpdate_41AEE0();

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_41B0F0(PrimHeader** ppOt);

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
