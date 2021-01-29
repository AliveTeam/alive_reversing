#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

struct Path_FlintLockFire : public Path_TLV
{
    __int16 field_18_scale;
    __int16 field_1A_id;
};
ALIVE_ASSERT_SIZEOF(Path_FlintLockFire, 0x1C);

class FlintLockFire : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT FlintLockFire* ctor_41AA90(Path_FlintLockFire* pTlv, int tlvInfo);

    EXPORT BaseGameObject* dtor_41AE20();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT FlintLockFire* Vdtor_41B500(signed int flags);

    virtual void VStopAudio() override;

    EXPORT void VStopAudio_41B0C0();

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_41B0B0();

    virtual void VUpdate() override;

    EXPORT void VUpdate_41AEE0();

    virtual void VRender(int** pOrderingTable) override;

    EXPORT void VRender_41B0F0(int** ppOt);

    int field_D4_padding[4];
    enum class States : __int16
    {
        eDisabled_0 = 0,
        eActivating_1 = 1,
        eActivated_2 = 2
    };
    States field_E4_state;
    __int16 field_E6_switch_id;
    int field_E8_tlvInfo;
    int field_EC_fire_sound;
    Animation field_F0_anim;
    Animation field_188_anim;
    Animation field_220_anim;
};
ALIVE_ASSERT_SIZEOF(FlintLockFire, 0x2B8);

}

